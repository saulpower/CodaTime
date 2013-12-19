//
//  BasicGJChronology.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BasicGJChronology_h
#define CodaTime_BasicGJChronology_h

#include "CodaTimeMacros.h"

#include "chrono/BasicChronology.h"
#include "DateTimeConstants.h"

#include <vector>

using namespace std;

CODATIME_BEGIN

/**
 * Abstract Chronology for implementing chronologies based on Gregorian/Julian formulae.
 * Most of the utility methods required by subclasses are package-private,
 * reflecting the intention that they be defined in the same package.
 * <p>
 * BasicGJChronology is thread-safe and immutable, and all subclasses must
 * be as well.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @author Guy Allard
 * @since 1.2, refactored from CommonGJChronology
 */
class BasicGJChronology : public BasicChronology {
    
private:
    
    /** Serialization lock */
    static const long long serialVersionUID = 538276888268L;
    
    // These arrays are NOT public. We trust ourselves not to alter the array.
    // They use zero-based array indexes so the that valid range of months is
    // automatically checked.
    static const vector<int> MIN_DAYS_PER_MONTH_ARRAY;
    static const vector<int> MAX_DAYS_PER_MONTH_ARRAY;
    static vector<int64_t> MIN_TOTAL_MILLIS_BY_MONTH_ARRAY;
    static vector<int64_t> MAX_TOTAL_MILLIS_BY_MONTH_ARRAY;
    static const int64_t FEB_29 = (31L + 29 - 1) * DateTimeConstants::MILLIS_PER_DAY;
    
    struct StaticBlock {
        StaticBlock() {
            MIN_TOTAL_MILLIS_BY_MONTH_ARRAY.resize(12);
            MAX_TOTAL_MILLIS_BY_MONTH_ARRAY.resize(12);
            
            int64_t minSum = 0;
            int64_t maxSum = 0;
            for (int i = 0; i < 11; i++) {
                int64_t millis = MIN_DAYS_PER_MONTH_ARRAY[i]
                * (int64_t)DateTimeConstants::MILLIS_PER_DAY;
                minSum += millis;
                MIN_TOTAL_MILLIS_BY_MONTH_ARRAY[i + 1] = minSum;
                
                millis = MAX_DAYS_PER_MONTH_ARRAY[i]
                * (int64_t)DateTimeConstants::MILLIS_PER_DAY;
                maxSum += millis;
                MAX_TOTAL_MILLIS_BY_MONTH_ARRAY[i + 1] = maxSum;
            }
        }
    };
    
    static StaticBlock staticBlock;
    
protected:
    
    /**
     * Constructor.
     */
    BasicGJChronology(Chronology *base, Object *param, int minDaysInFirstWeek) : BasicChronology(base, param, minDaysInFirstWeek) {
    }
    
    //-----------------------------------------------------------------------
    int getMonthOfYear(int64_t millis, int year) {
        // Perform a binary search to get the month. To make it go even faster,
        // compare using ints instead of uint64_ts. The number of milliseconds per
        // year exceeds the limit of a 32-bit int's capacity, so divide by
        // 1024. No precision is lost (except time of day) since the number of
        // milliseconds per day contains 1024 as a factor. After the division,
        // the instant isn't measured in milliseconds, but in units of
        // (128/125)seconds.
        
        int i = (int)((millis - getYearMillis(year)) >> 10);
        
        // There are 86400000 milliseconds per day, but divided by 1024 is
        // 84375. There are 84375 (128/125)seconds per day.
        
        return
        (isLeapYear(year))
        ? ((i < 182 * 84375)
           ? ((i < 91 * 84375)
              ? ((i < 31 * 84375) ? 1 : (i < 60 * 84375) ? 2 : 3)
              : ((i < 121 * 84375) ? 4 : (i < 152 * 84375) ? 5 : 6))
           : ((i < 274 * 84375)
              ? ((i < 213 * 84375) ? 7 : (i < 244 * 84375) ? 8 : 9)
              : ((i < 305 * 84375) ? 10 : (i < 335 * 84375) ? 11 : 12)))
        : ((i < 181 * 84375)
           ? ((i < 90 * 84375)
              ? ((i < 31 * 84375) ? 1 : (i < 59 * 84375) ? 2 : 3)
              : ((i < 120 * 84375) ? 4 : (i < 151 * 84375) ? 5 : 6))
           : ((i < 273 * 84375)
              ? ((i < 212 * 84375) ? 7 : (i < 243 * 84375) ? 8 : 9)
              : ((i < 304 * 84375) ? 10 : (i < 334 * 84375) ? 11 : 12)));
    }
    
    //-----------------------------------------------------------------------
    /**
     * Gets the number of days in the specified month and year.
     *
     * @param year  the year
     * @param month  the month
     * @return the number of days
     */
    int getDaysInYearMonth(int year, int month) {
        if (isLeapYear(year)) {
            return MAX_DAYS_PER_MONTH_ARRAY[month - 1];
        } else {
            return MIN_DAYS_PER_MONTH_ARRAY[month - 1];
        }
    }
    
    //-----------------------------------------------------------------------
    int getDaysInMonthMax(int month) {
        return MAX_DAYS_PER_MONTH_ARRAY[month - 1];
    }
    
    //-----------------------------------------------------------------------
    int getDaysInMonthMaxForSet(int64_t instant, int value) {
        return ((value > 28 || value < 1) ? getDaysInMonthMax((int) instant) : 28);
    }
    
    //-----------------------------------------------------------------------
    int64_t getTotalMillisByYearMonth(int year, int month) {
        if (isLeapYear(year)) {
            return MAX_TOTAL_MILLIS_BY_MONTH_ARRAY[month - 1];
        } else {
            return MIN_TOTAL_MILLIS_BY_MONTH_ARRAY[month - 1];
        }
    }
    
    //-----------------------------------------------------------------------
    int64_t getYearDifference(int64_t minuendInstant, int64_t subtrahendInstant) {
        int minuendYear = getYear(minuendInstant);
        int subtrahendYear = getYear(subtrahendInstant);
        
        // Inlined remainder method to avoid duplicate calls to get.
        int64_t minuendRem = minuendInstant - getYearMillis(minuendYear);
        int64_t subtrahendRem = subtrahendInstant - getYearMillis(subtrahendYear);
        
        // Balance leap year differences on remainders.
        if (subtrahendRem >= FEB_29) {
            if (isLeapYear(subtrahendYear)) {
                if (!isLeapYear(minuendYear)) {
                    subtrahendRem -= DateTimeConstants::MILLIS_PER_DAY;
                }
            } else if (minuendRem >= FEB_29 && isLeapYear(minuendYear)) {
                minuendRem -= DateTimeConstants::MILLIS_PER_DAY;
            }
        }
        
        int difference = minuendYear - subtrahendYear;
        if (minuendRem < subtrahendRem) {
            difference--;
        }
        return difference;
    }
    
    //-----------------------------------------------------------------------
    int64_t setYear(int64_t instant, int year) {
        int thisYear = getYear(instant);
        int dayOfYear = getDayOfYear(instant, thisYear);
        int millisOfDay = getMillisOfDay(instant);
        
        if (dayOfYear > (31 + 28)) { // after Feb 28
            if (isLeapYear(thisYear)) {
                // Current date is Feb 29 or later.
                if (!isLeapYear(year)) {
                    // Moving to a non-leap year, Feb 29 does not exist.
                    dayOfYear--;
                }
            } else {
                // Current date is Mar 01 or later.
                if (isLeapYear(year)) {
                    // Moving to a leap year, account for Feb 29.
                    dayOfYear++;
                }
            }
        }
        
        instant = getYearMonthDayMillis(year, 1, dayOfYear);
        instant += millisOfDay;
        
        return instant;
    }
    
};

const vector<int> BasicGJChronology::MIN_DAYS_PER_MONTH_ARRAY = {
    31,28,31,30,31,30,31,31,30,31,30,31
};

const vector<int> BasicGJChronology::MAX_DAYS_PER_MONTH_ARRAY = {
    31,29,31,30,31,30,31,31,30,31,30,31
};

CODATIME_END

#endif
