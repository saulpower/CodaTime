//
//  Chronology.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_Chronology_h
#define CodaTime_Chronology_h

#include "CodaTimeMacros.h"

#include "Object.h"

#include <vector>
#include <string>

using namespace std;

CODATIME_BEGIN

class DateTimeZone;
class ReadablePartial;
class ReadablePeriod;
class DateTimeField;
class DurationField;

/**
 * Chronology provides access to the individual date time fields for a
 * chronological calendar system.
 * <p>
 * Various chronologies are supported by subclasses including ISO
 * and GregorianJulian. To construct a Chronology you should use the
 * factory methods on the chronology subclass in the chrono package.
 * <p>
 * For example, to obtain the current time in the coptic calendar system:
 * <pre>
 * DateTime dt = new DateTime(CopticChronology.getInstance());
 * </pre>
 * <p>
 * The provided chronology implementations are:
 * <ul>
 * <li>ISO - Based on the ISO8601 standard and suitable for use after about 1600
 * <li>GJ - Historically accurate calendar with Julian followed by Gregorian
 * <li>Gregorian - The Gregorian calendar system used for all time (proleptic)
 * <li>Julian - The Julian calendar system used for all time (proleptic)
 * <li>Buddhist - The Buddhist calendar system which is an offset in years from GJ
 * <li>Coptic - The Coptic calendar system which defines 30 day months
 * <li>Ethiopic - The Ethiopic calendar system which defines 30 day months
 * </ul>
 * Hopefully future releases will contain more chronologies.
 * <p>
 * This class defines a number of fields with names from the ISO8601 standard.
 * It does not 'strongly' define these fields however, thus implementations
 * are free to interpret the field names as they wish.
 * For example, a week could be defined as 10 days and a month as 40 days in a
 * special WeirdChronology implementation. Clearly the GJ and ISO
 * implementations provided use the field names as you would expect.
 *
 * @see org.joda.time.chrono.ISOChronology
 * @see org.joda.time.chrono.GJChronology
 * @see org.joda.time.chrono.GregorianChronology
 * @see org.joda.time.chrono.JulianChronology
 * @see org.joda.time.chrono.CopticChronology
 * @see org.joda.time.chrono.BuddhistChronology
 * @see org.joda.time.chrono.EthiopicChronology
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
class Chronology : public virtual Object {
    
public:
    
    /**
     * Returns the DateTimeZone that this Chronology operates in, or null if
     * unspecified.
     *
     * @return the DateTimeZone, null if unspecified
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
     * @param zone to use, or default if null
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
     * @throws IllegalArgumentException if the values are invalid
     */
    virtual int64_t getDateTimeMillis(int year, int monthOfYear, int dayOfMonth, int millisOfDay) = 0;
    
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
     * @throws IllegalArgumentException if the values are invalid
     */
    virtual int64_t getDateTimeMillis(int year, int monthOfYear, int dayOfMonth,
                                           int hourOfDay, int minuteOfHour,
                                           int secondOfMinute, int millisOfSecond) = 0;
    
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
     * @throws IllegalArgumentException if the values are invalid
     */
    virtual int64_t getDateTimeMillis(int64_t instant,
                                           int hourOfDay, int minuteOfHour,
                                           int secondOfMinute, int millisOfSecond) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Validates whether the values are valid for the fields of a partial instant.
     *
     * @param partial  the partial instant to validate
     * @param values  the values to validate, not null, match fields in partial
     * @throws IllegalArgumentException if the instant is invalid
     */
    virtual void validate(ReadablePartial *partial, vector<int> values) = 0;
    
    /**
     * Gets the values of a partial from an instant.
     *
     * @param partial  the partial instant to use
     * @param instant  the instant to query
     * @return the values of this partial extracted from the instant
     */
    virtual vector<int> get(ReadablePartial *partial, int64_t instant) = 0;
    
    /**
     * Sets the partial into the instant.
     *
     * @param partial  the partial instant to use
     * @param instant  the instant to update
     * @return the updated instant
     */
    virtual int64_t set(ReadablePartial *partial, int64_t instant) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the values of a period from an interval.
     *
     * @param period  the period instant to use
     * @param startInstant  the start instant of an interval to query
     * @param endInstant  the start instant of an interval to query
     * @return the values of the period extracted from the interval
     */
    virtual vector<int> get(ReadablePeriod *period, int64_t startInstant, int64_t endInstant) = 0;
    
    /**
     * Gets the values of a period from an interval.
     *
     * @param period  the period instant to use
     * @param duration  the duration to query
     * @return the values of the period extracted from the duration
     */
    virtual vector<int> get(ReadablePeriod *period, int64_t duration) = 0;
    
    /**
     * Adds the period to the instant, specifying the number of times to add.
     *
     * @param period  the period to add, null means add nothing
     * @param instant  the instant to add to
     * @param scalar  the number of times to add
     * @return the updated instant
     */
    virtual int64_t add(ReadablePeriod *period, int64_t instant, int scalar) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Adds the duration to the instant, specifying the number of times to add.
     *
     * @param instant  the instant to add to
     * @param duration  the duration to add
     * @param scalar  the number of times to add
     * @return the updated instant
     */
    virtual int64_t add(int64_t instant, int64_t duration, int scalar) = 0;
    
    // Millis
    //-----------------------------------------------------------------------
    /**
     * Get the millis duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *millis() = 0;
    
    /**
     * Get the millis of second field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *millisOfSecond() = 0;
    
    /**
     * Get the millis of day field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *millisOfDay() = 0;
    
    // Second
    //-----------------------------------------------------------------------
    /**
     * Get the seconds duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *seconds() = 0;
    
    /**
     * Get the second of minute field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *secondOfMinute() = 0;
    
    /**
     * Get the second of day field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *secondOfDay() = 0;
    
    // Minute
    //-----------------------------------------------------------------------
    /**
     * Get the minutes duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *minutes() = 0;
    
    /**
     * Get the minute of hour field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *minuteOfHour() = 0;
    
    /**
     * Get the minute of day field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *minuteOfDay() = 0;
    
    // Hour
    //-----------------------------------------------------------------------
    /**
     * Get the hours duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *hours() = 0;
    
    /**
     * Get the hour of day (0-23) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *hourOfDay() = 0;
    
    /**
     * Get the hour of day (offset to 1-24) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *clockhourOfDay() = 0;
    
    // Halfday
    //-----------------------------------------------------------------------
    /**
     * Get the halfdays duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *halfdays() = 0;
    
    /**
     * Get the hour of am/pm (0-11) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *hourOfHalfday() = 0;
    
    /**
     * Get the hour of am/pm (offset to 1-12) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *clockhourOfHalfday() = 0;
    
    /**
     * Get the AM(0) PM(1) field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *halfdayOfDay() = 0;
    
    // Day
    //-----------------------------------------------------------------------
    /**
     * Get the days duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *days() = 0;
    
    /**
     * Get the day of week field for this chronology.
     *
     * <p>DayOfWeek values are defined in {@link DateTimeConstants}.
     * They use the ISO definitions, where 1 is Monday and 7 is Sunday.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *dayOfWeek() = 0;
    
    /**
     * Get the day of month field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *dayOfMonth() = 0;
    
    /**
     * Get the day of year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *dayOfYear() = 0;
    
    // Week
    //-----------------------------------------------------------------------
    /**
     * Get the weeks duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *weeks() = 0;
    
    /**
     * Get the week of a week based year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *weekOfWeekyear() = 0;
    
    // Weekyear
    //-----------------------------------------------------------------------
    /**
     * Get the weekyears duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *weekyears() = 0;
    
    /**
     * Get the year of a week based year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *weekyear() = 0;
    
    /**
     * Get the year of a week based year in a century field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual  const DateTimeField *weekyearOfCentury() = 0;
    
    // Month
    //-----------------------------------------------------------------------
    /**
     * Get the months duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *months() = 0;
    
    /**
     * Get the month of year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *monthOfYear() = 0;
    
    // Year
    //-----------------------------------------------------------------------
    /**
     * Get the years duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *years() = 0;
    
    /**
     * Get the year field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *year() = 0;
    
    /**
     * Get the year of era field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *yearOfEra() = 0;
    
    /**
     * Get the year of century field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *yearOfCentury() = 0;
    
    // Century
    //-----------------------------------------------------------------------
    /**
     * Get the centuries duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *centuries() = 0;
    
    /**
     * Get the century of era field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *centuryOfEra() = 0;
    
    // Era
    //-----------------------------------------------------------------------
    /**
     * Get the eras duration field for this chronology.
     *
     * @return DurationField or UnsupportedDurationField if unsupported
     */
    virtual const DurationField *eras() = 0;
    
    /**
     * Get the era field for this chronology.
     *
     * @return DateTimeField or UnsupportedDateTimeField if unsupported
     */
    virtual const DateTimeField *era() = 0;
    
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
