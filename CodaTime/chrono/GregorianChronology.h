//
//  GregorianChronology.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_GregorianChronology_h
#define CodaTime_GregorianChronology_h

#include "CodaTimeMacros.h"

#include "chrono/BasicGJChronology.h"
#include "DateTimeConstants.h"
#include "DateTimeZone.h"
#include "Exceptions.h"

#include <map>
#include <vector>

using namespace std;

CODATIME_BEGIN

/**
 * Implements a pure proleptic Gregorian calendar system, which defines every
 * fourth year as leap, unless the year is divisible by 100 and not by 400.
 * This improves upon the Julian calendar leap year rule.
 * <p>
 * Although the Gregorian calendar did not exist before 1582 CE, this
 * chronology assumes it did, thus it is proleptic. This implementation also
 * fixes the start of the year at January 1, and defines the year zero.
 * <p>
 * GregorianChronology is thread-safe and immutable.
 *
 * @see <a href="http://en.wikipedia.org/wiki/Gregorian_calendar">Wikipedia</a>
 * @see JulianChronology
 * @see GJChronology
 *
 * @author Guy Allard
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
class GregorianChronology : public BasicGJChronology {
    
private:
    
    /** Serialization lock */
    static const long long serialVersionUID = -861407383323710522L;
    
    static const int64_t MILLIS_PER_YEAR =
    (int64_t) (365.2425 * DateTimeConstants::MILLIS_PER_DAY);
    
    static const int64_t MILLIS_PER_MONTH =
    (int64_t) (365.2425 * DateTimeConstants::MILLIS_PER_DAY / 12);
    
    static const int DAYS_0000_TO_1970 = 719527;
    
    /** The lowest year that can be fully supported. */
    static const int MIN_YEAR = -292275054;
    
    /** The highest year that can be fully supported. */
    static const int MAX_YEAR = 292278993;
    
    /** Singleton instance of a UTC GregorianChronology */
    static GregorianChronology *INSTANCE_UTC;
    
    /** Cache of zone to chronology arrays */
    static map<DateTimeZone*, vector<GregorianChronology*>> cCache;
    
    struct StaticBlock {
        StaticBlock() {
            INSTANCE_UTC = getInstance(DateTimeZone::UTC);
        }
    };
    
    static StaticBlock staticBlock;
    
    // Constructors and instance variables
    //-----------------------------------------------------------------------
    
    /**
     * Restricted constructor
     */
    GregorianChronology(Chronology *base, Object *param, int minDaysInFirstWeek) : BasicGJChronology(base, param, minDaysInFirstWeek) {
    }
    
    /**
     * Serialization singleton
     */
    Object *readResolve() {
        Chronology *base = getBase();
        int minDays = getMinimumDaysInFirstWeek();
        minDays = (minDays == 0 ? 4 : minDays);  // handle rename of BaseGJChronology
        return (base == NULL) ?
        getInstance(DateTimeZone::UTC, minDays) :
        getInstance(base->getZone(), minDays);
    }
    
    bool isLeapYear(int year) {
        return ((year & 3) == 0) && ((year % 100) != 0 || (year % 400) == 0);
    }
    
    int64_t calculateFirstDayOfYearMillis(int year) {
        // Initial value is just temporary.
        int leapYears = year / 100;
        if (year < 0) {
            // Add 3 before shifting right since /4 and >>2 behave differently
            // on negative numbers. When the expression is written as
            // (year / 4) - (year / 100) + (year / 400),
            // it works for both positive and negative values, except this optimization
            // eliminates two divisions.
            leapYears = ((year + 3) >> 2) - leapYears + ((leapYears + 3) >> 2) - 1;
        } else {
            leapYears = (year >> 2) - leapYears + (leapYears >> 2);
            if (isLeapYear(year)) {
                leapYears--;
            }
        }
        
        return (year * 365L + (leapYears - DAYS_0000_TO_1970)) * DateTimeConstants::MILLIS_PER_DAY;
    }
    
    int getMinYear() {
        return MIN_YEAR;
    }
    
    int getMaxYear() {
        return MAX_YEAR;
    }
    
    int64_t getAverageMillisPerYear() {
        return MILLIS_PER_YEAR;
    }
    
    int64_t getAverageMillisPerYearDividedByTwo() {
        return MILLIS_PER_YEAR / 2;
    }
    
    int64_t getAverageMillisPerMonth() {
        return MILLIS_PER_MONTH;
    }
    
    int64_t getApproxMillisAtEpochDividedByTwo() {
        return (1970L * MILLIS_PER_YEAR) / 2;
    }
    
public:
    
    /**
     * Gets an instance of the GregorianChronology.
     * The time zone of the returned instance is UTC.
     *
     * @return a singleton UTC instance of the chronology
     */
    static GregorianChronology *getInstanceUTC() {
        return INSTANCE_UTC;
    }
    
    /**
     * Gets an instance of the GregorianChronology in the default time zone.
     *
     * @return a chronology in the default time zone
     */
    static GregorianChronology *getInstance() {
        return getInstance(DateTimeZone::getDefault(), 4);
    }
    
    /**
     * Gets an instance of the GregorianChronology in the given time zone.
     *
     * @param zone  the time zone to get the chronology in, NULL is default
     * @return a chronology in the specified time zone
     */
    static GregorianChronology *getInstance(DateTimeZone *zone) {
        return getInstance(zone, 4);
    }
    
    /**
     * Gets an instance of the GregorianChronology in the given time zone.
     *
     * @param zone  the time zone to get the chronology in, NULL is default
     * @param minDaysInFirstWeek  minimum number of days in first week of the year; default is 4
     * @return a chronology in the specified time zone
     */
    static GregorianChronology *getInstance(DateTimeZone *zone, int minDaysInFirstWeek) {
        if (zone == NULL) {
            zone = DateTimeZone::getDefault();
        }
        GregorianChronology *chrono = NULL;
        //TODO: synchronized
//        synchronized (cCache) {
            vector<GregorianChronology*> &chronos = cCache.get(zone);
            if (chronos.empty()) {
                chronos.resize(7);
                cCache[zone] = chronos;
            }
            try {
                chrono = chronos[minDaysInFirstWeek - 1];
            } catch (ArrayIndexOutOfBoundsException e) {
                throw IllegalArgumentException("Invalid min days in first week: " + to_string(minDaysInFirstWeek));
            }
            if (chrono == NULL) {
                if (zone == DateTimeZone::UTC) {
                    chrono = new GregorianChronology(NULL, NULL, minDaysInFirstWeek);
                } else {
                    chrono = getInstance(DateTimeZone::UTC, minDaysInFirstWeek);
                    chrono = new GregorianChronology(ZonedChronology::getInstance(chrono, zone), NULL, minDaysInFirstWeek);
                }
                chronos[minDaysInFirstWeek - 1] = chrono;
            }
//        }
        return chrono;
    }
    
    // Conversion
    //-----------------------------------------------------------------------
    /**
     * Gets the Chronology in the UTC time zone.
     *
     * @return the chronology in UTC
     */
    Chronology *withUTC() {
        return INSTANCE_UTC;
    }
    
    /**
     * Gets the Chronology in a specific time zone.
     *
     * @param zone  the zone to get the chronology in, NULL is default
     * @return the chronology
     */
    Chronology *withZone(DateTimeZone *zone) {
        if (zone == NULL) {
            zone = DateTimeZone::getDefault();
        }
        if (zone == getZone()) {
            return this;
        }
        return getInstance(zone);
    }
    
protected:
    
    void assemble(Fields *fields) {
        if (getBase() == NULL) {
            BasicChronology::assemble(fields);
        }
    }
    
};

CODATIME_END

#endif
