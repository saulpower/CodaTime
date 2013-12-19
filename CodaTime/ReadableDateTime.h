//
//  ReadableDateTime.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_ReadableDateTime_h
#define CodaTime_ReadableDateTime_h

#include "CodaTimeMacros.h"

#include "Object.h"
#include "ReadableInstant.h"

#include <string>

CODATIME_BEGIN

class DateTime;
class MutableDateTime;
class Locale;

/**
 * Defines an instant in time that can be queried using datetime fields.
 * <p>
 * The implementation of this interface may be mutable or immutable.
 * This interface only gives access to retrieve data, never to change it.
 * <p>
 * Methods in your application should be defined using <code>ReadableDateTime</code>
 * as a parameter if the method only wants to read the datetime, and not perform
 * any advanced manipulations.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
class ReadableDateTime : public virtual ReadableInstant, public virtual Object {
    
public:
    
    /**
     * Get the day of week field value.
     * <p>
     * The values for the day of week are defined in {@link DateTimeConstants}.
     *
     * @return the day of week
     */
    virtual int getDayOfWeek() = 0;
    
    /**
     * Get the day of month field value.
     *
     * @return the day of month
     */
    virtual int getDayOfMonth() = 0;
    
    /**
     * Get the day of year field value.
     *
     * @return the day of year
     */
    virtual int getDayOfYear() = 0;
    
    /**
     * Get the week of weekyear field value.
     * <p>
     * This field is associated with the "weekyear" via {@link #getWeekyear()}.
     * In the standard ISO8601 week algorithm, the first week of the year
     * is that in which at least 4 days are in the year. As a result of this
     * definition, day 1 of the first week may be in the previous year.
     *
     * @return the week of a week based year
     */
    virtual int getWeekOfWeekyear() = 0;
    
    /**
     * Get the weekyear field value.
     * <p>
     * The weekyear is the year that matches with the weekOfWeekyear field.
     * In the standard ISO8601 week algorithm, the first week of the year
     * is that in which at least 4 days are in the year. As a result of this
     * definition, day 1 of the first week may be in the previous year.
     * The weekyear allows you to query the effective year for that day.
     *
     * @return the year of a week based year
     */
    virtual int getWeekyear() = 0;
    
    /**
     * Get the month of year field value.
     *
     * @return the month of year
     */
    virtual int getMonthOfYear() = 0;
    
    /**
     * Get the year field value.
     *
     * @return the year
     */
    virtual int getYear() = 0;
    
    /**
     * Get the year of era field value.
     *
     * @return the year of era
     */
    virtual int getYearOfEra() = 0;
    
    /**
     * Get the year of century field value.
     *
     * @return the year of century
     */
    virtual int getYearOfCentury() = 0;
    
    /**
     * Get the year of era field value.
     *
     * @return the year of era
     */
    virtual int getCenturyOfEra() = 0;
    
    /**
     * Get the era field value.
     *
     * @return the era
     */
    virtual int getEra() = 0;
    
    // Time field access methods
    //-----------------------------------------------------------
    
    /**
     * Get the millis of second field value.
     *
     * @return the millis of second
     */
    virtual int getMillisOfSecond() = 0;
    
    /**
     * Get the millis of day field value.
     *
     * @return the millis of day
     */
    virtual int getMillisOfDay() = 0;
    
    /**
     * Get the second of minute field value.
     *
     * @return the second of minute
     */
    virtual int getSecondOfMinute() = 0;
    
    /**
     * Get the second of day field value.
     *
     * @return the second of day
     */
    virtual int getSecondOfDay() = 0;
    
    /**
     * Get the minute of hour field value.
     *
     * @return the minute of hour
     */
    virtual int getMinuteOfHour() = 0;
    
    /**
     * Get the minute of day field value.
     *
     * @return the minute of day
     */
    virtual int getMinuteOfDay() = 0;
    
    /**
     * Get the hour of day field value.
     *
     * @return the hour of day
     */
    virtual int getHourOfDay() = 0;
    
    /**
     * Get this object as a DateTime.
     * <p>
     * If the implementation of the interface is a DateTime, it is returned directly.
     *
     * @return a DateTime using the same millis
     */
    virtual DateTime *toDateTime() = 0;
    
    /**
     * Get this object as a MutableDateTime, always returning a new instance.
     *
     * @return a MutableDateTime using the same millis
     */
    virtual MutableDateTime *toMutableDateTime() = 0;
    
    /**
     * Output the instant using the specified format pattern.
     *
     * @param pattern  pattern specification
     * @throws IllegalArgumentException  if pattern is invalid
     * @see  org.joda.time.format.DateTimeFormat
     */
    virtual string toString(string pattern) = 0;
    
    /**
     * Output the instant using the specified format pattern.
     *
     * @param pattern  pattern specification
     * @param locale  Locale to use, or null for default
     * @throws IllegalArgumentException  if pattern is invalid
     * @see  org.joda.time.format.DateTimeFormat
     */
    virtual string toString(string pattern, Locale *locale) = 0;
    
};

CODATIME_END

#endif
