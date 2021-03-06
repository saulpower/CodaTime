//
//  ReadablePeriod.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_ReadablePeriod_h
#define CodaTime_ReadablePeriod_h

#include "CodaTimeMacros.h"

#include "Object.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Period;
class PeriodType;
class DurationFieldType;

/**
 * Defines a time period specified in terms of individual duration fields
 * such as years and days.
 * <p>
 * The implementation of this interface may be mutable or immutable. This
 * interface only gives access to retrieve data, never to change it.
 * <p>
 * Periods are split up into multiple fields, for example days and seconds.
 * Implementations are not required to evenly distribute the values across the fields.
 * The value for each field may be positive or negative.
 * <p>
 * When a time period is added to an instant, the effect is to add each field in turn.
 * For example, a time period could be defined as 3 months, 2 days and -1 hours.
 * In most circumstances this would be the same as 3 months, 1 day, and 23 hours.
 * However, when adding across a daylight savings boundary, a day may be 23 or 25 hours long.
 * Thus, the time period is always added field by field to the datetime.
 * <p>
 * Periods are independent of chronology, and can only be treated as durations
 * when paired with a time via an interval.
 *
 * @see ReadableDuration
 * @see ReadableInterval
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class ReadablePeriod : public virtual Object {
    
public:
    
    /**
     * Gets the period type that defines which fields are included in the period.
     *
     * @return the period type
     */
    virtual const PeriodType *getPeriodType() const = 0;
    
    /**
     * Gets the number of fields that this period supports.
     *
     * @return the number of fields supported
     */
    virtual int size() const = 0;
    
    /**
     * Gets the field type at the specified index.
     *
     * @param index  the index to retrieve
     * @return the field at the specified index
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    virtual const DurationFieldType *getFieldType(int index) const = 0;
    
    /**
     * Gets the value at the specified index.
     *
     * @param index  the index to retrieve
     * @return the value of the field at the specified index
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    virtual int getValue(int index) const = 0;
    
    /**
     * Gets the value of one of the fields.
     * <p>
     * If the field type specified is not supported by the period then zero
     * is returned.
     *
     * @param field  the field type to query, null returns zero
     * @return the value of that field, zero if field not supported
     */
    virtual int get(const DurationFieldType *field) = 0;
    
    /**
     * Checks whether the field type specified is supported by this period.
     *
     * @param field  the field to check, may be null which returns false
     * @return true if the field is supported
     */
    virtual bool isSupported(const DurationFieldType *field) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Get this period as an immutable <code>Period</code> object.
     * <p>
     * This will either typecast this instance, or create a new <code>Period</code>.
     *
     * @return a Duration using the same field set and values
     */
    virtual Period *toPeriod() = 0;
    
    /**
     * Get this object as a <code>MutablePeriod</code>.
     * <p>
     * This will always return a new <code>MutablePeriod</code> with the same fields.
     *
     * @return a MutablePeriod using the same field set and values
     */
//    virtual MutablePeriod *toMutablePeriod() = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Compares this object with the specified object for equality based
     * on the value and type of each supported field.
     * All ReadablePeriod instances are accepted.
     * <p>
     * Note that a period of 1 day is not equal to a period of 24 hours,
     * nor is 1 hour equal to 60 minutes. Only periods with the same amount
     * in each field are equal.
     * <p>
     * This is because periods represent an abstracted definition of a time
     * period (eg. a day may not actually be 24 hours, it might be 23 or 25
     * at daylight savings boundary).
     * <p>
     * To compare the actual duration of two periods, convert both to
     * {@link Duration}s, an operation that emphasises that the result may
     * differ according to the date you choose.
     *
     * @param readablePeriod  a readable period to check against
     * @return true if all the field values and types are equal, false if
     *  not or the period is null or of an incorrect type
     */
    virtual bool equals(const Object *readablePeriod) const = 0;
    
    /**
     * Gets a hash code for the period that is compatible with the equals method.
     * The hashcode is calculated as follows:
     * <pre>
     *  int total = 17;
     *  for (int i = 0; i < fields.length; i++) {
     *      total = 27 * total + getValue(i);
     *      total = 27 * total + getFieldType(i).hashCode();
     *  }
     *  return total;
     * </pre>
     *
     * @return a hash code
     */
    virtual int hashCode() = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the value as a String in the style of the ISO8601 duration format.
     * Technically, the output can breach the ISO specification as weeks may be included.
     * <p>
     * For example, "PT6H3M5S" represents 6 hours, 3 minutes, 5 seconds.
     *
     * @return the value as an ISO8601 style string
     */
    virtual string toString() = 0;
    
};

CODATIME_END

#endif
