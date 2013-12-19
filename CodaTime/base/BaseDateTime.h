//
//  BaseDateTime.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BaseDateTime_h
#define CodaTime_BaseDateTime_h

#include "CodaTimeMacros.h"

#include "base/AbstractDateTime.h"
#include "chrono/ISOChronology.h"
#include "Chronology.h"
#include "DateTimeUtils.h"
#include "ReadableDateTime.h"

CODATIME_BEGIN

/**
 * BaseDateTime is an abstract implementation of ReadableDateTime that stores
 * data in <code>long</code> and <code>Chronology</code> fields.
 * <p>
 * This class should generally not be used directly by API users.
 * The {@link ReadableDateTime} interface should be used when different
 * kinds of date/time objects are to be referenced.
 * <p>
 * BaseDateTime subclasses may be mutable and not thread-safe.
 *
 * @author Stephen Colebourne
 * @author Kandarp Shah
 * @author Brian S O'Neill
 * @since 1.0
 */
class BaseDateTime : public AbstractDateTime, virtual public ReadableDateTime {
    
private:
    
    /** Serialization lock */
    static const long long serialVersionUID = -6728882245981L;
    
    /** The millis from 1970-01-01T00:00:00Z */
    int64_t iMillis;
    /** The chronology to use */
    Chronology *iChronology;
    
public:
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance set to the current system millisecond time
     * using <code>ISOChronology</code> in the default time zone.
     */
    BaseDateTime() : BaseDateTime(DateTimeUtils::currentTimeMillis(), ISOChronology::getInstance()) {
    }
    
    /**
     * Constructs an instance set to the current system millisecond time
     * using <code>ISOChronology</code> in the specified time zone.
     * <p>
     * If the specified time zone is null, the default zone is used.
     *
     * @param zone  the time zone, null means default zone
     */
    BaseDateTime(DateTimeZone *zone) : BaseDateTime(DateTimeUtils::currentTimeMillis(), ISOChronology::getInstance(zone)) {
    }
    
    /**
     * Constructs an instance set to the current system millisecond time
     * using the specified chronology.
     * <p>
     * If the chronology is null, <code>ISOChronology</code>
     * in the default time zone is used.
     *
     * @param chronology  the chronology, null means ISOChronology in default zone
     */
    BaseDateTime(Chronology *chronology) : BaseDateTime(DateTimeUtils::currentTimeMillis(), chronology) {
    }
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance set to the milliseconds from 1970-01-01T00:00:00Z
     * using <code>ISOChronology</code> in the default time zone.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z
     */
    BaseDateTime(int64_t instant) : BaseDateTime(instant, ISOChronology::getInstance()) {
    }
    
    /**
     * Constructs an instance set to the milliseconds from 1970-01-01T00:00:00Z
     * using <code>ISOChronology</code> in the specified time zone.
     * <p>
     * If the specified time zone is null, the default zone is used.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z
     * @param zone  the time zone, null means default zone
     */
    BaseDateTime(int64_t instant, DateTimeZone *zone) : BaseDateTime(instant, ISOChronology::getInstance(zone)) {
    }
    
    /**
     * Constructs an instance set to the milliseconds from 1970-01-01T00:00:00Z
     * using the specified chronology.
     * <p>
     * If the chronology is null, <code>ISOChronology</code>
     * in the default time zone is used.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z
     * @param chronology  the chronology, null means ISOChronology in default zone
     */
    BaseDateTime(int64_t instant, Chronology *chronology) : AbstractDateTime() {
        iChronology = checkChronology(chronology);
        iMillis = checkInstant(instant, iChronology);
    }
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance from an Object that represents a datetime,
     * forcing the time zone to that specified.
     * <p>
     * If the object contains no chronology, <code>ISOChronology</code> is used.
     * If the specified time zone is null, the default zone is used.
     * <p>
     * The recognised object types are defined in
     * {@link org.joda.time.convert.ConverterManager ConverterManager} and
     * include ReadableInstant, String, Calendar and Date.
     *
     * @param instant  the datetime object
     * @param zone  the time zone
     * @throws IllegalArgumentException if the instant is invalid
     */
    BaseDateTime(Object *instant, DateTimeZone *zone) : AbstractDateTime() {
        InstantConverter *converter = ConverterManager::getInstance()->getInstantConverter(instant);
        Chronology *chrono = checkChronology(converter->getChronology(instant, zone));
        iChronology = chrono;
        iMillis = checkInstant(converter->getInstantMillis(instant, chrono), chrono);
    }
    
    /**
     * Constructs an instance from an Object that represents a datetime,
     * using the specified chronology.
     * <p>
     * If the chronology is null, ISO in the default time zone is used.
     * <p>
     * The recognised object types are defined in
     * {@link org.joda.time.convert.ConverterManager ConverterManager} and
     * include ReadableInstant, String, Calendar and Date.
     *
     * @param instant  the datetime object
     * @param chronology  the chronology
     * @throws IllegalArgumentException if the instant is invalid
     */
    BaseDateTime(Object *instant, Chronology *chronology) : AbstractDateTime() {
        InstantConverter *converter = ConverterManager::getInstance()->getInstantConverter(instant);
        iChronology = checkChronology(converter->getChronology(instant, chronology));
        iMillis = checkInstant(converter->getInstantMillis(instant, chronology), iChronology);
    }
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance from datetime field values
     * using <code>ISOChronology</code> in the default time zone.
     *
     * @param year  the year
     * @param monthOfYear  the month of the year
     * @param dayOfMonth  the day of the month
     * @param hourOfDay  the hour of the day
     * @param minuteOfHour  the minute of the hour
     * @param secondOfMinute  the second of the minute
     * @param millisOfSecond  the millisecond of the second
     */
    BaseDateTime(
                 int year,
                 int monthOfYear,
                 int dayOfMonth,
                 int hourOfDay,
                 int minuteOfHour,
                 int secondOfMinute,
                 int millisOfSecond) : BaseDateTime(year, monthOfYear, dayOfMonth, hourOfDay,
                                                    minuteOfHour, secondOfMinute, millisOfSecond, ISOChronology::getInstance()) {
    }
    
    /**
     * Constructs an instance from datetime field values
     * using <code>ISOChronology</code> in the specified time zone.
     * <p>
     * If the specified time zone is null, the default zone is used.
     *
     * @param year  the year
     * @param monthOfYear  the month of the year
     * @param dayOfMonth  the day of the month
     * @param hourOfDay  the hour of the day
     * @param minuteOfHour  the minute of the hour
     * @param secondOfMinute  the second of the minute
     * @param millisOfSecond  the millisecond of the second
     * @param zone  the time zone, null means default time zone
     */
    BaseDateTime(
                 int year,
                 int monthOfYear,
                 int dayOfMonth,
                 int hourOfDay,
                 int minuteOfHour,
                 int secondOfMinute,
                 int millisOfSecond,
                 DateTimeZone *zone) : BaseDateTime(year, monthOfYear, dayOfMonth, hourOfDay,
                                                   minuteOfHour, secondOfMinute, millisOfSecond, ISOChronology::getInstance(zone)) {
    }
    
    /**
     * Constructs an instance from datetime field values
     * using the specified chronology.
     * <p>
     * If the chronology is null, <code>ISOChronology</code>
     * in the default time zone is used.
     *
     * @param year  the year
     * @param monthOfYear  the month of the year
     * @param dayOfMonth  the day of the month
     * @param hourOfDay  the hour of the day
     * @param minuteOfHour  the minute of the hour
     * @param secondOfMinute  the second of the minute
     * @param millisOfSecond  the millisecond of the second
     * @param chronology  the chronology, null means ISOChronology in default zone
     */
    BaseDateTime(
                 int year,
                 int monthOfYear,
                 int dayOfMonth,
                 int hourOfDay,
                 int minuteOfHour,
                 int secondOfMinute,
                 int millisOfSecond,
                 Chronology *chronology) : AbstractDateTime() {
        iChronology = checkChronology(chronology);
        int64_t instant = iChronology->getDateTimeMillis(year, monthOfYear, dayOfMonth,
                                                          hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond);
        iMillis = checkInstant(instant, iChronology);
    }
    
    //-----------------------------------------------------------------------
    /**
     * Checks the specified chronology before storing it, potentially altering it.
     * This method must not access any instance variables.
     * <p>
     * This implementation converts nulls to ISOChronology in the default zone.
     *
     * @param chronology  the chronology to use, may be null
     * @return the chronology to store in this datetime, not null
     */
    Chronology *checkChronology(Chronology *chronology) {
        return DateTimeUtils::getChronology(chronology);
    }
    
    /**
     * Checks the specified instant before storing it, potentially altering it.
     * This method must not access any instance variables.
     * <p>
     * This implementation simply returns the instant.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to round
     * @param chronology  the chronology to use, not null
     * @return the instant to store in this datetime
     */
    int64_t checkInstant(int64_t instant, Chronology *chronology) {
        return instant;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Gets the milliseconds of the datetime instant from the Java epoch
     * of 1970-01-01T00:00:00Z.
     *
     * @return the number of milliseconds since 1970-01-01T00:00:00Z
     */
    int64_t getMillis() const {
        return iMillis;
    }
    
    /**
     * Gets the chronology of the datetime.
     *
     * @return the Chronology that the datetime is using
     */
    Chronology *getChronology() const {
        return iChronology;
    }
    
protected:
    
    //-----------------------------------------------------------------------
    /**
     * Sets the milliseconds of the datetime.
     * <p>
     * All changes to the millisecond field occurs via this method.
     * Override and block this method to make a subclass immutable.
     *
     * @param instant  the milliseconds since 1970-01-01T00:00:00Z to set the datetime to
     */
    void setMillis(int64_t instant) {
        iMillis = checkInstant(instant, iChronology);
    }
    
    /**
     * Sets the chronology of the datetime.
     * <p>
     * All changes to the chronology field occurs via this method.
     * Override and block this method to make a subclass immutable.
     *
     * @param chronology  the chronology to set
     */
    void setChronology(Chronology *chronology) {
        iChronology = checkChronology(chronology);
    }
    
};

CODATIME_END

#endif
