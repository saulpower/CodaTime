//
//  BasicChronology.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BasicChronology_h
#define CodaTime_BasicChronology_h

#include "CodaTimeMacros.h"

#include "chrono/AssembledChronology.h"
#include "DateTimeConstants.h"
#include "field/MillisDurationField.h"
#include "field/PreciseDateTimeField.h"
#include "field/PreciseDurationField.h"
#include "field/ZeroIsMaxDateTimeField.h"

#include <vector>

using namespace std;

CODATIME_BEGIN

/**
 * virtual implementation for calendar systems that use a typical
 * day/month/year/leapYear model.
 * Most of the utility methods required by subclasses are package-private,
 * reflecting the intention that they be defined in the same package.
 * <p>
 * BasicChronology is thread-safe and immutable, and all subclasses must
 * be as well.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @author Guy Allard
 * @since 1.2, renamed from BaseGJChronology
 */
class BasicChronology : public AssembledChronology {
    
    friend class BasicYearDateTimeField;
    
private:
    
    /** Serialization lock */
    static const long long serialVersionUID = 8283225332206808863L;
    
    static const DurationField *cMillisField;
    static const DurationField *cSecondsField;
    static const DurationField *cMinutesField;
    static const DurationField *cHoursField;
    static const DurationField *cHalfdaysField;
    static const DurationField *cDaysField;
    static const DurationField *cWeeksField;
    
    static const DateTimeField *cMillisOfSecondField;
    static const DateTimeField *cMillisOfDayField;
    static const DateTimeField *cSecondOfMinuteField;
    static const DateTimeField *cSecondOfDayField;
    static const DateTimeField *cMinuteOfHourField;
    static const DateTimeField *cMinuteOfDayField;
    static const DateTimeField *cHourOfDayField;
    static const DateTimeField *cHourOfHalfdayField;
    static const DateTimeField *cClockhourOfDayField;
    static const DateTimeField *cClockhourOfHalfdayField;
    static const DateTimeField *cHalfdayOfDayField;
    
    class HalfdayField : public PreciseDateTimeField {
        
    private:
        
        static const long long serialVersionUID = 581601443656929254L;
        
    public:
        
        HalfdayField() : PreciseDateTimeField(DateTimeFieldType::halfdayOfDay(), cHalfdaysField, cDaysField) {
        }
        
        string getAsText(int fieldValue, Locale *locale) const;
        
        int64_t set(int64_t millis, string text, Locale *locale);
        
        int getMaximumTextLength(Locale *locale);
    };
    
    class YearInfo {
        
    public:
        int iYear;
        int64_t iFirstDayMillis;
        
        YearInfo(int year, int64_t firstDayMillis) {
            iYear = year;
            iFirstDayMillis = firstDayMillis;
        }
    };
    
    struct StaticBlock {
        StaticBlock() {
            cMillisField = MillisDurationField::INSTANCE;
            cSecondsField = new PreciseDurationField
            (DurationFieldType::seconds(), DateTimeConstants::MILLIS_PER_SECOND);
            cMinutesField = new PreciseDurationField
            (DurationFieldType::minutes(), DateTimeConstants::MILLIS_PER_MINUTE);
            cHoursField = new PreciseDurationField
            (DurationFieldType::hours(), DateTimeConstants::MILLIS_PER_HOUR);
            cHalfdaysField = new PreciseDurationField
            (DurationFieldType::halfdays(), DateTimeConstants::MILLIS_PER_DAY / 2);
            cDaysField = new PreciseDurationField
            (DurationFieldType::days(), DateTimeConstants::MILLIS_PER_DAY);
            cWeeksField = new PreciseDurationField
            (DurationFieldType::weeks(), DateTimeConstants::MILLIS_PER_WEEK);
            
            cMillisOfSecondField = new PreciseDateTimeField
            (DateTimeFieldType::millisOfSecond(), cMillisField, cSecondsField);
            
            cMillisOfDayField = new PreciseDateTimeField
            (DateTimeFieldType::millisOfDay(), cMillisField, cDaysField);
            
            cSecondOfMinuteField = new PreciseDateTimeField
            (DateTimeFieldType::secondOfMinute(), cSecondsField, cMinutesField);
            
            cSecondOfDayField = new PreciseDateTimeField
            (DateTimeFieldType::secondOfDay(), cSecondsField, cDaysField);
            
            cMinuteOfHourField = new PreciseDateTimeField
            (DateTimeFieldType::minuteOfHour(), cMinutesField, cHoursField);
            
            cMinuteOfDayField = new PreciseDateTimeField
            (DateTimeFieldType::minuteOfDay(), cMinutesField, cDaysField);
            
            cHourOfDayField = new PreciseDateTimeField
            (DateTimeFieldType::hourOfDay(), cHoursField, cDaysField);
            
            cHourOfHalfdayField = new PreciseDateTimeField
            (DateTimeFieldType::hourOfHalfday(), cHoursField, cHalfdaysField);
            
            cClockhourOfDayField = new ZeroIsMaxDateTimeField
            (cHourOfDayField, DateTimeFieldType::clockhourOfDay());
            
            cClockhourOfHalfdayField = new ZeroIsMaxDateTimeField
            (cHourOfHalfdayField, DateTimeFieldType::clockhourOfHalfday());
            
            cHalfdayOfDayField = new HalfdayField();
        }
    };
    
    static StaticBlock staticBlock;
    
    static const int CACHE_SIZE = 1 << 10;
    static const int CACHE_MASK = CACHE_SIZE - 1;
    
    vector<YearInfo*> iYearInfoCache;
    
    int iMinDaysInFirstWeek;
    
public:
    
    DateTimeZone *getZone() const;
    
    int64_t getDateTimeMillis(int year, int monthOfYear, int dayOfMonth, int millisOfDay);
    
    int64_t getDateTimeMillis(int year, int monthOfYear, int dayOfMonth,
                              int hourOfDay, int minuteOfHour, int secondOfMinute, int millisOfSecond);
    
    int getMinimumDaysInFirstWeek() const { return iMinDaysInFirstWeek; }
    
    //-----------------------------------------------------------------------
    /**
     * Checks if this chronology instance equals another.
     *
     * @param obj  the object to compare to
     * @return true if equal
     * @since 1.6
     */
    bool equals(const Object *obj) const;
    
    /**
     * A suitable hash code for the chronology.
     *
     * @return the hash code
     * @since 1.6
     */
    int hashCode();
    
    // Output
    //-----------------------------------------------------------------------
    /**
     * Gets a debugging tostring.
     *
     * @return a debugging string
     */
    string toString();
    
protected:
    
    BasicChronology(Chronology *base, Object *param, int minDaysInFirstWeek);
    
    void assemble(Fields *fields);
    
    // Although accessed by multiple threads, this method doesn't need to be synchronized.
    YearInfo *getYearInfo(int year);
    
    //-----------------------------------------------------------------------
    /**
     * Get the number of days in the year.
     *
     * @return 366
     */
    int getDaysInYearMax() { return 366; }
    
    /**
     * Get the number of days in the year.
     *
     * @param year  the year to use
     * @return 366 if a leap year, otherwise 365
     */
    int getDaysInYear(int year) { return isLeapYear(year) ? 366 : 365; }
    
    /**
     * Get the number of weeks in the year.
     *
     * @param year  the year to use
     * @return number of weeks in the year
     */
    int getWeeksInYear(int year);
    
    /**
     * Get the millis for the first week of a year.
     *
     * @param year  the year to use
     * @return millis
     */
    int64_t getFirstWeekOfYearMillis(int year);
    
    /**
     * Get the milliseconds for the start of a year.
     *
     * @param year The year to use.
     * @return millis from 1970-01-01T00:00:00Z
     */
    int64_t getYearMillis(int year);
    
    /**
     * Get the milliseconds for the start of a month.
     *
     * @param year The year to use.
     * @param month The month to use
     * @return millis from 1970-01-01T00:00:00Z
     */
    int64_t getYearMonthMillis(int year, int month);
    
    /**
     * Get the milliseconds for a particular date.
     *
     * @param year The year to use.
     * @param month The month to use
     * @param dayOfMonth The day of the month to use
     * @return millis from 1970-01-01T00:00:00Z
     */
    int64_t getYearMonthDayMillis(int year, int month, int dayOfMonth);
    
    /**
     * @param instant millis from 1970-01-01T00:00:00Z
     */
    int getYear(int64_t instant);
    
    /**
     * @param millis from 1970-01-01T00:00:00Z
     */
    int getMonthOfYear(int64_t millis) { return getMonthOfYear(millis, getYear(millis)); }
    
    /**
     * @param millis from 1970-01-01T00:00:00Z
     * @param year precalculated year of millis
     */
    virtual int getMonthOfYear(int64_t millis, int year) = 0;
    
    /**
     * @param millis from 1970-01-01T00:00:00Z
     */
    int getDayOfMonth(int64_t millis);
    
    /**
     * @param millis from 1970-01-01T00:00:00Z
     * @param year precalculated year of millis
     */
    int getDayOfMonth(int64_t millis, int year);
    
    /**
     * @param millis from 1970-01-01T00:00:00Z
     * @param year precalculated year of millis
     * @param month precalculated month of millis
     */
    int getDayOfMonth(int64_t millis, int year, int month);
    
    /**
     * @param instant millis from 1970-01-01T00:00:00Z
     */
    int getDayOfYear(int64_t instant) { return getDayOfYear(instant, getYear(instant)); }
    
    /**
     * @param instant millis from 1970-01-01T00:00:00Z
     * @param year precalculated year of millis
     */
    int getDayOfYear(int64_t instant, int year);
    
    /**
     * @param instant millis from 1970-01-01T00:00:00Z
     */
    int getWeekyear(int64_t instant);
    
    /**
     * @param instant millis from 1970-01-01T00:00:00Z
     */
    int getWeekOfWeekyear(int64_t instant) { return getWeekOfWeekyear(instant, getYear(instant)); }
    
    /**
     * @param instant millis from 1970-01-01T00:00:00Z
     * @param year precalculated year of millis
     */
    int getWeekOfWeekyear(int64_t instant, int year);
    
    /**
     * @param instant millis from 1970-01-01T00:00:00Z
     */
    int getDayOfWeek(int64_t instant);
    
    /**
     * @param instant millis from 1970-01-01T00:00:00Z
     */
    int getMillisOfDay(int64_t instant);
    
    /**
     * Gets the maximum number of days in any month.
     *
     * @return 31
     */
    int getDaysInMonthMax() { return 31; }
    
    /**
     * Gets the maximum number of days in the month specified by the instant.
     *
     * @param instant  millis from 1970-01-01T00:00:00Z
     * @return the maximum number of days in the month
     */
    int getDaysInMonthMax(int64_t instant);
    
    /**
     * Gets the maximum number of days in the month specified by the instant.
     * The value represents what the user is trying to set, and can be
     * used to optimise this method.
     *
     * @param instant  millis from 1970-01-01T00:00:00Z
     * @param value  the value being set
     * @return the maximum number of days in the month
     */
    int getDaysInMonthMaxForSet(int64_t instant, int value) { return getDaysInMonthMax(instant); }
    
    //-----------------------------------------------------------------------
    /**
     * Gets the milliseconds for a date at midnight.
     *
     * @param year  the year
     * @param monthOfYear  the month
     * @param dayOfMonth  the day
     * @return the milliseconds
     */
    int64_t getDateMidnightMillis(int year, int monthOfYear, int dayOfMonth);
    
    /**
     * Gets the difference between the two instants in years.
     *
     * @param minuendInstant  the first instant
     * @param subtrahendInstant  the second instant
     * @return the difference
     */
    virtual int64_t getYearDifference(int64_t minuendInstant, int64_t subtrahendInstant) = 0;
    
    /**
     * Is the specified year a leap year?
     *
     * @param year  the year to test
     * @return true if leap
     */
    virtual bool isLeapYear(int year) = 0;
    
    /**
     * Gets the number of days in the specified month and year.
     *
     * @param year  the year
     * @param month  the month
     * @return the number of days
     */
    virtual int getDaysInYearMonth(int year, int month) = 0;
    
    /**
     * Gets the maximum days in the specified month.
     *
     * @param month  the month
     * @return the max days
     */
    virtual int getDaysInMonthMax(int month) = 0;
    
    /**
     * Gets the total number of millis elapsed in this year at the start
     * of the specified month, such as zero for month 1.
     *
     * @param year  the year
     * @param month  the month
     * @return the elapsed millis at the start of the month
     */
    virtual int64_t getTotalMillisByYearMonth(int year, int month) = 0;
    
    /**
     * Gets the millisecond value of the first day of the year.
     *
     * @return the milliseconds for the first of the year
     */
    virtual int64_t calculateFirstDayOfYearMillis(int year) = 0;
    
    /**
     * Gets the minimum supported year.
     *
     * @return the year
     */
    virtual int getMinYear() = 0;
    
    /**
     * Gets the maximum supported year.
     *
     * @return the year
     */
    virtual int getMaxYear() = 0;
    
    /**
     * Gets the maximum month for the specified year.
     * This implementation calls getMaxMonth().
     *
     * @param year  the year
     * @return the maximum month value
     */
    int getMaxMonth(int year) { return getMaxMonth(); }
    
    /**
     * Gets the maximum number of months.
     *
     * @return 12
     */
    int getMaxMonth() {
        return 12;
    }
    
    /**
     * Gets an average value for the milliseconds per year.
     *
     * @return the millis per year
     */
    virtual int64_t getAverageMillisPerYear() = 0;
    
    /**
     * Gets an average value for the milliseconds per year, divided by two.
     *
     * @return the millis per year divided by two
     */
    virtual int64_t getAverageMillisPerYearDividedByTwo() = 0;
    
    /**
     * Gets an average value for the milliseconds per month.
     *
     * @return the millis per month
     */
    virtual int64_t getAverageMillisPerMonth() = 0;
    
    /**
     * Returns a constant representing the approximate number of milliseconds
     * elapsed from year 0 of this chronology, divided by two. This constant
     * <em>must</em> be defined as:
     * <pre>
     *    (yearAtEpoch * averageMillisPerYear + millisOfYearAtEpoch) / 2
     * </pre>
     * where epoch is 1970-01-01 (Gregorian).
     */
    virtual int64_t getApproxMillisAtEpochDividedByTwo() = 0;
    
    /**
     * Sets the year from an instant and year.
     *
     * @param instant  millis from 1970-01-01T00:00:00Z
     * @param year  the year to set
     * @return the updated millis
     */
    virtual int64_t setYear(int64_t instant, int year) = 0;

};

CODATIME_END

#endif
