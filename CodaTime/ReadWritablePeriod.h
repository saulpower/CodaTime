//
//  ReadWritablePeriod.h
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_ReadWritablePeriod_h
#define CodaTime_ReadWritablePeriod_h

#include "CodaTimeMacros.h"

#include "ReadablePeriod.h"

CODATIME_BEGIN

class DurationFieldType;
class ReadableInterval;
class ReadablePeriod;

/**
 * Defines a duration of time that can be queried and modified using datetime fields.
 * <p>
 * The implementation of this interface will be mutable.
 * It may provide more advanced methods than those in the interface.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class ReadWritablePeriod : public virtual ReadablePeriod {
    
public:
    
    /**
     * Clears the period, setting all values back to zero.
     */
    virtual void clear() = 0;
    
    /**
     * Sets the value of one of the fields by index.
     *
     * @param index  the field index
     * @param value  the new value for the field
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    virtual void setValue(int index, int value) = 0;
    
    /**
     * Sets the value of one of the fields.
     * <p>
     * The field type specified must be one of those that is supported by the period.
     *
     * @param field  a DurationFieldType instance that is supported by this period
     * @param value  the new value for the field
     * @throws IllegalArgumentException if the field is null or not supported
     */
    virtual void set(DurationFieldType *field, int value) = 0;
    
    /**
     * Sets all the fields in one go from another ReadablePeriod.
     *
     * @param period  the period to set, null means zero length period
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    virtual void setPeriod(ReadablePeriod *period) = 0;
    
    /**
     * Sets all the fields in one go.
     *
     * @param years  amount of years in this period, which must be zero if unsupported
     * @param months  amount of months in this period, which must be zero if unsupported
     * @param weeks  amount of weeks in this period, which must be zero if unsupported
     * @param days  amount of days in this period, which must be zero if unsupported
     * @param hours  amount of hours in this period, which must be zero if unsupported
     * @param minutes  amount of minutes in this period, which must be zero if unsupported
     * @param seconds  amount of seconds in this period, which must be zero if unsupported
     * @param millis  amount of milliseconds in this period, which must be zero if unsupported
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    virtual void setPeriod(int years, int months, int weeks, int days,
                   int hours, int minutes, int seconds, int millis) = 0;
    
    /**
     * Sets all the fields in one go from an interval dividing the
     * fields using the period type.
     *
     * @param interval  the interval to set, null means zero length
     */
    virtual void setPeriod(ReadableInterval *interval) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Adds to the value of one of the fields.
     * <p>
     * The field type specified must be one of those that is supported by the period.
     *
     * @param field  a DurationFieldType instance that is supported by this period
     * @param value  the value to add to the field
     * @throws IllegalArgumentException if the field is null or not supported
     */
    virtual void add(DurationFieldType *field, int value) = 0;
    
    /**
     * Adds a period to this one by adding each field in turn.
     *
     * @param period  the period to add, null means add nothing
     * @throws IllegalArgumentException if the period being added contains a field
     * not supported by this period
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void add(ReadablePeriod *period) = 0;
    
    /**
     * Adds to each field of this period.
     *
     * @param years  amount of years to add to this period, which must be zero if unsupported
     * @param months  amount of months to add to this period, which must be zero if unsupported
     * @param weeks  amount of weeks to add to this period, which must be zero if unsupported
     * @param days  amount of days to add to this period, which must be zero if unsupported
     * @param hours  amount of hours to add to this period, which must be zero if unsupported
     * @param minutes  amount of minutes to add to this period, which must be zero if unsupported
     * @param seconds  amount of seconds to add to this period, which must be zero if unsupported
     * @param millis  amount of milliseconds to add to this period, which must be zero if unsupported
     * @throws IllegalArgumentException if the period being added contains a field
     * not supported by this period
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void add(int years, int months, int weeks, int days,
             int hours, int minutes, int seconds, int millis) = 0;
    
    /**
     * Adds an interval to this one by dividing the interval into
     * fields and then adding each field in turn.
     *
     * @param interval  the interval to add, null means add nothing
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void add(ReadableInterval *interval) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of years of the period.
     *
     * @param years  the number of years
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    virtual void setYears(int years) = 0;
    
    /**
     * Adds the specified years to the number of years in the period.
     *
     * @param years  the number of years
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void addYears(int years) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of months of the period.
     *
     * @param months  the number of months
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    virtual void setMonths(int months) = 0;
    
    /**
     * Adds the specified months to the number of months in the period.
     *
     * @param months  the number of months
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void addMonths(int months) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of weeks of the period.
     *
     * @param weeks  the number of weeks
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    virtual void setWeeks(int weeks) = 0;
    
    /**
     * Adds the specified weeks to the number of weeks in the period.
     *
     * @param weeks  the number of weeks
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void addWeeks(int weeks) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of days of the period.
     *
     * @param days  the number of days
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    virtual void setDays(int days) = 0;
    
    /**
     * Adds the specified days to the number of days in the period.
     *
     * @param days  the number of days
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void addDays(int days) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of hours of the period.
     *
     * @param hours  the number of hours
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    virtual void setHours(int hours) = 0;
    
    /**
     * Adds the specified hours to the number of hours in the period.
     *
     * @param hours  the number of hours
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void addHours(int hours) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of minutes of the period.
     *
     * @param minutes  the number of minutes
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    virtual void setMinutes(int minutes) = 0;
    
    /**
     * Adds the specified minutes to the number of minutes in the period.
     *
     * @param minutes  the number of minutes
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void addMinutes(int minutes) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of seconds of the period.
     *
     * @param seconds  the number of seconds
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    virtual void setSeconds(int seconds) = 0;
    
    /**
     * Adds the specified seconds to the number of seconds in the period.
     *
     * @param seconds  the number of seconds
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void addSeconds(int seconds) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of millis of the period.
     *
     * @param millis  the number of millis
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    virtual void setMillis(int millis) = 0;
    
    /**
     * Adds the specified millis to the number of millis in the period.
     *
     * @param millis  the number of millis
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    virtual void addMillis(int millis) = 0;
    
};

CODATIME_END

#endif
