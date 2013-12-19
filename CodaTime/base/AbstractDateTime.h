//
//  AbstractDateTime.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_AbstractDateTime_h
#define CodaTime_AbstractDateTime_h

#include "CodaTimeMacros.h"

#include "base/AbstractInstant.h"
#include "ReadableDateTime.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Locale;

/**
 * AbstractDateTime provides the common behaviour for datetime classes.
 * <p>
 * This class should generally not be used directly by API users.
 * The {@link ReadableDateTime} interface should be used when different
 * kinds of date/time objects are to be referenced.
 * <p>
 * Whenever you want to implement <code>ReadableDateTime</code> you should
 * extend this class.
 * <p>
 * AbstractDateTime subclasses may be mutable and not thread-safe.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class AbstractDateTime : public AbstractInstant, public virtual ReadableDateTime {
    
protected:
    
    /**
     * Constructor.
     */
    AbstractDateTime() : AbstractInstant() {
    }
    
public:
    
    virtual Chronology *getChronology() const = 0;
    
    virtual int64_t getMillis() const = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Get the value of one of the fields of a datetime.
     * <p>
     * This method uses the chronology of the datetime to obtain the value.
     * It is essentially a generic way of calling one of the get methods.
     *
     * @param type  a field type, usually obtained from DateTimeFieldType
     * @return the value of that field
     * @throws IllegalArgumentException if the field type is NULL
     */
    int get(const DateTimeFieldType *type) const;
    
    //-----------------------------------------------------------------------
    /**
     * Get the era field value.
     *
     * @return the era
     */
    int getEra();
    
    /**
     * Get the year of era field value.
     *
     * @return the year of era
     */
    int getCenturyOfEra();
    
    /**
     * Get the year of era field value.
     *
     * @return the year of era
     */
    int getYearOfEra();
    
    /**
     * Get the year of century field value.
     *
     * @return the year of century
     */
    int getYearOfCentury();
    
    /**
     * Get the year field value.
     *
     * @return the year
     */
    int getYear();
    
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
    int getWeekyear();
    
    /**
     * Get the month of year field value.
     *
     * @return the month of year
     */
    int getMonthOfYear();
    
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
    int getWeekOfWeekyear();
    
    /**
     * Get the day of year field value.
     *
     * @return the day of year
     */
    int getDayOfYear();
    
    /**
     * Get the day of month field value.
     * <p>
     * The values for the day of month are defined in {@link org.joda.time.DateTimeConstants}.
     *
     * @return the day of month
     */
    int getDayOfMonth();
    
    /**
     * Get the day of week field value.
     * <p>
     * The values for the day of week are defined in {@link org.joda.time.DateTimeConstants}.
     *
     * @return the day of week
     */
    int getDayOfWeek();
    
    //-----------------------------------------------------------------------
    /**
     * Get the hour of day field value.
     *
     * @return the hour of day
     */
    int getHourOfDay();
    
    /**
     * Get the minute of day field value.
     *
     * @return the minute of day
     */
    int getMinuteOfDay();
    
    /**
     * Get the minute of hour field value.
     *
     * @return the minute of hour
     */
    int getMinuteOfHour();
    
    /**
     * Get the second of day field value.
     *
     * @return the second of day
     */
    int getSecondOfDay();
    
    /**
     * Get the second of minute field value.
     *
     * @return the second of minute
     */
    int getSecondOfMinute();
    
    /**
     * Get the millis of day field value.
     *
     * @return the millis of day
     */
    int getMillisOfDay();
    
    /**
     * Get the millis of second field value.
     *
     * @return the millis of second
     */
    int getMillisOfSecond();
    
    //-----------------------------------------------------------------------
    /**
     * Output the instant using the specified format pattern.
     *
     * @param pattern  the pattern specification, NULL means use <code>toString</code>
     * @see  org.joda.time.format.DateTimeFormat
     */
    string toString(string pattern);
    
    /**
     * Output the instant using the specified format pattern.
     *
     * @param pattern  the pattern specification, NULL means use <code>toString</code>
     * @param locale  Locale to use, NULL means default
     * @see  org.joda.time.format.DateTimeFormat
     */
    string toString(string pattern, Locale *locale);
    
};

CODATIME_END

#endif
