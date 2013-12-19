//
//  BaseChronology.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BaseChronology_h
#define CodaTime_BaseChronology_h

#include "CodaTimeMacros.h"

#include "Chronology.h"

#include <vector>
#include <string>

using namespace std;

CODATIME_BEGIN

class DateTimeField;
class DateTimeZone;
class DurationField;
class ReadablePeriod;
class ReadablePartial;

/**
 * BaseChronology provides a skeleton implementation for chronology
 * classes. Many utility methods are defined, but all fields are unsupported.
 * <p>
 * BaseChronology is thread-safe and immutable, and all subclasses must be
 * as well.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class BaseChronology : public Chronology {
    
private:
    
    /** Serialization version. */
    static const long long serialVersionUID = -7310865996721419676L;
    
protected:
    
    /**
     * Restricted constructor.
     */
    BaseChronology() {}
    
public:
    
    /**
     * Returns the DateTimeZone that this Chronology operates in, or NULL if
     * unspecified.
     *
     * @return DateTimeZone NULL if unspecified
     */
    virtual DateTimeZone *getZone() const = 0;
    
    /**
     * Returns an instance of this Chronology that operates in the UTC time
     * zone. Chronologies that do not operate in a time zone or are already
     * UTC must return themself.
     *
     * @return a version of this chronology that ignores time zones
     */
    virtual Chronology *withUTC() = 0;
    
    /**
     * Returns an instance of this Chronology that operates in any time zone.
     *
     * @return a version of this chronology with a specific time zone
     * @param zone to use, or default if NULL
     * @see org.joda.time.chrono.ZonedChronology
     */
    virtual Chronology *withZone(DateTimeZone *zone) = 0;
    
    /**
     * Returns a datetime millisecond instant, formed from the given year,
     * month, day, and millisecond values. The set of given values must refer
     * to a valid datetime, or else an IllegalArgumentException is thrown.
     * <p>
     * The default implementation calls upon separate DateTimeFields to
     * determine the result. Subclasses are encouraged to provide a more
     * efficient implementation.
     *
     * @param year year to use
     * @param monthOfYear month to use
     * @param dayOfMonth day of month to use
     * @param millisOfDay millisecond to use
     * @return millisecond instant from 1970-01-01T00:00:00Z
     */
    int64_t getDateTimeMillis(int year, int monthOfYear, int dayOfMonth,
                                  int millisOfDay);
    
    /**
     * Returns a datetime millisecond instant, formed from the given year,
     * month, day, hour, minute, second, and millisecond values. The set of
     * given values must refer to a valid datetime, or else an
     * IllegalArgumentException is thrown.
     * <p>
     * The default implementation calls upon separate DateTimeFields to
     * determine the result. Subclasses are encouraged to provide a more
     * efficient implementation.
     *
     * @param year year to use
     * @param monthOfYear month to use
     * @param dayOfMonth day of month to use
     * @param hourOfDay hour to use
     * @param minuteOfHour minute to use
     * @param secondOfMinute second to use
     * @param millisOfSecond millisecond to use
     * @return millisecond instant from 1970-01-01T00:00:00Z
     */
    int64_t getDateTimeMillis(int year, int monthOfYear, int dayOfMonth,
                                  int hourOfDay, int minuteOfHour,
                                  int secondOfMinute, int millisOfSecond);
    
    /**
     * Returns a datetime millisecond instant, from from the given instant,
     * hour, minute, second, and millisecond values. The set of given values
     * must refer to a valid datetime, or else an IllegalArgumentException is
     * thrown.
     * <p>
     * The default implementation calls upon separate DateTimeFields to
     * determine the result. Subclasses are encouraged to provide a more
     * efficient implementation.
     *
     * @param instant instant to start from
     * @param hourOfDay hour to use
     * @param minuteOfHour minute to use
     * @param secondOfMinute second to use
     * @param millisOfSecond millisecond to use
     * @return millisecond instant from 1970-01-01T00:00:00Z
     */
    int64_t getDateTimeMillis(int64_t instant,
                                  int hourOfDay, int minuteOfHour,
                                  int secondOfMinute, int millisOfSecond);
    
    //-----------------------------------------------------------------------
    /**
     * Validates whether the fields stored in a partial instant are valid.
     * <p>
     * This implementation uses {@link DateTimeField#getMinimumValue(ReadablePartial, vector<int>)}
     * and {@link DateTimeField#getMaximumValue(ReadablePartial, vector<int>)}.
     *
     * @param partial  the partial instant to validate
     * @param values  the values to validate, not NULL unless the partial is empty
     * @throws IllegalArgumentException if the instant is invalid
     */
    void validate(ReadablePartial *partial, vector<int> values);
    
    /**
     * Gets the values of a partial from an instant.
     *
     * @param partial  the partial instant to use
     * @param instant  the instant to query
     * @return the values of the partial extracted from the instant
     */
    vector<int> get(ReadablePartial *partial, int64_t instant);
    
    /**
     * Sets the partial into the instant.
     *
     * @param partial  the partial instant to use
     * @param instant  the instant to update
     * @return the updated instant
     */
    int64_t set(ReadablePartial *partial, int64_t instant);
    
    //-----------------------------------------------------------------------
    /**
     * Gets the values of a period from an interval.
     *
     * @param period  the period instant to use
     * @param startInstant  the start instant of an interval to query
     * @param endInstant  the start instant of an interval to query
     * @return the values of the period extracted from the interval
     */
    vector<int> get(ReadablePeriod *period, int64_t startInstant, int64_t endInstant);
    
    /**
     * Gets the values of a period from an interval.
     *
     * @param period  the period instant to use
     * @param duration  the duration to query
     * @return the values of the period extracted from the duration
     */
    vector<int> get(ReadablePeriod *period, int64_t duration);
    
    /**
     * Adds the period to the instant, specifying the number of times to add.
     *
     * @param period  the period to add, NULL means add nothing
     * @param instant  the instant to add to
     * @param scalar  the number of times to add
     * @return the updated instant
     */
    int64_t add(ReadablePeriod *period, int64_t instant, int scalar);
    
    //-----------------------------------------------------------------------
    /**
     * Adds the duration to the instant, specifying the number of times to add.
     *
     * @param instant  the instant to add to
     * @param duration  the duration to add
     * @param scalar  the number of times to add
     * @return the updated instant
     */
    int64_t add(int64_t instant, int64_t duration, int scalar);
    
    // Millis
    //-----------------------------------------------------------------------
    /**
     * Get the millis duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *millis();
    
    /**
     * Get the millis of second field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *millisOfSecond();
    
    /**
     * Get the millis of day field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *millisOfDay();
    
    // Second
    //-----------------------------------------------------------------------
    /**
     * Get the seconds duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *seconds();
    
    /**
     * Get the second of minute field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *secondOfMinute();
    
    /**
     * Get the second of day field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *secondOfDay();
    
    // Minute
    //-----------------------------------------------------------------------
    /**
     * Get the minutes duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *minutes();
    
    /**
     * Get the minute of hour field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *minuteOfHour();
    
    /**
     * Get the minute of day field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *minuteOfDay();
    
    // Hour
    //-----------------------------------------------------------------------
    /**
     * Get the hours duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *hours();
    
    /**
     * Get the hour of day (0-23) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *hourOfDay();
    
    /**
     * Get the hour of day (offset to 1-24) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *clockhourOfDay();
    
    // Halfday
    //-----------------------------------------------------------------------
    /**
     * Get the halfdays duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *halfdays();
    
    /**
     * Get the hour of am/pm (0-11) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *hourOfHalfday();
    
    /**
     * Get the hour of am/pm (offset to 1-12) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *clockhourOfHalfday();
    
    /**
     * Get the AM(0) PM(1) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *halfdayOfDay();
    
    // Day
    //-----------------------------------------------------------------------
    /**
     * Get the days duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *days();
    
    /**
     * Get the day of week field for this chronology.
     *
     * <p>DayOfWeek values are defined in
     * {@link org.joda.time.DateTimeConstants DateTimeConstants}.
     * They use the ISO definitions, where 1 is Monday and 7 is Sunday.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *dayOfWeek();
    
    /**
     * Get the day of month field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *dayOfMonth();
    
    /**
     * Get the day of year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *dayOfYear();
    
    // Week
    //-----------------------------------------------------------------------
    /**
     * Get the weeks duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *weeks();
    
    /**
     * Get the week of a week based year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *weekOfWeekyear();
    
    // Weekyear
    //-----------------------------------------------------------------------
    /**
     * Get the weekyears duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *weekyears();
    
    /**
     * Get the year of a week based year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *weekyear();
    
    /**
     * Get the year of a week based year in a century field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *weekyearOfCentury();
    
    // Month
    //-----------------------------------------------------------------------
    /**
     * Get the months duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *months();
    
    /**
     * Get the month of year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *monthOfYear();
    
    // Year
    //-----------------------------------------------------------------------
    /**
     * Get the years duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *years();
    
    /**
     * Get the year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *year();
    
    /**
     * Get the year of era field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *yearOfEra();
    
    /**
     * Get the year of century field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *yearOfCentury();
    
    // Century
    //-----------------------------------------------------------------------
    /**
     * Get the centuries duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *centuries();
    
    /**
     * Get the century of era field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *centuryOfEra();
    
    // Era
    //-----------------------------------------------------------------------
    /**
     * Get the eras duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    const DurationField *eras();
    
    /**
     * Get the era field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    const DateTimeField *era();
    
    //-----------------------------------------------------------------------
    /**
     * Gets a debugging toString.
     * 
     * @return a debugging string
     */
    virtual string toString() = 0;
    
};

CODATIME_END

#endif
