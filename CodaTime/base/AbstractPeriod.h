//
//  AbstractPeriod.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__AbstractPeriod__
#define __CodaTime__AbstractPeriod__

#include "CodaTimeMacros.h"

#include "ReadablePeriod.h"

#include <string>
#include <vector>

using namespace std;

CODATIME_BEGIN

class Period;
class Object;
class PeriodFormatter;
class DurationFieldType;
class MutablePeriod;

/**
 * AbstractPeriod provides the common behaviour for period classes.
 * <p>
 * This class should generally not be used directly by API users. The
 * {@link ReadablePeriod} interface should be used when different
 * kinds of periods are to be referenced.
 * <p>
 * AbstractPeriod subclasses may be mutable and not thread-safe.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class AbstractPeriod : public virtual ReadablePeriod {
    
protected:
    
    /**
     * Constructor.
     */
    AbstractPeriod() {
    }
    
public:
    
    /**
     * Gets the period type that defines which fields are included in the period.
     *
     * @return the period type
     */
    virtual const PeriodType *getPeriodType() const = 0;
    
    /**
     * Gets the value at the specified index.
     *
     * @param index  the index to retrieve
     * @return the value of the field at the specified index
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    virtual int getValue(int index) const = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the number of fields that this period supports.
     *
     * @return the number of fields supported
     * @since 2.0 (previously on BasePeriod)
     */
    int size() const;
    
    /**
     * Gets the field type at the specified index.
     *
     * @param index  the index to retrieve
     * @return the field at the specified index
     * @throws IndexOutOfBoundsException if the index is invalid
     * @since 2.0 (previously on BasePeriod)
     */
    const DurationFieldType *getFieldType(int index) const;
    
    /**
     * Gets an array of the field types that this period supports.
     * <p>
     * The fields are returned largest to smallest, for example Hours, Minutes, Seconds.
     *
     * @return the fields supported in an array that may be altered, largest to smallest
     */
    vector<const DurationFieldType*> getFieldTypes();
    
    /**
     * Gets an array of the value of each of the fields that this period supports.
     * <p>
     * The fields are returned largest to smallest, for example Hours, Minutes, Seconds.
     * Each value corresponds to the same array index as <code>getFields()</code>
     *
     * @return the current values of each field in an array that may be altered, largest to smallest
     */
    vector<int> getValues();
    
    //-----------------------------------------------------------------------
    /**
     * Gets the value of one of the fields.
     * <p>
     * If the field type specified is not supported by the period then zero
     * is returned.
     *
     * @param type  the field type to query, null returns zero
     * @return the value of that field, zero if field not supported
     */
    int get(const DurationFieldType *type);
    
    /**
     * Checks whether the field specified is supported by this period.
     *
     * @param type  the type to check, may be null which returns false
     * @return true if the field is supported
     */
    bool isSupported(const DurationFieldType *type);
    
    /**
     * Gets the index of the field in this period.
     *
     * @param type  the type to check, may be null which returns -1
     * @return the index of -1 if not supported
     */
    int indexOf(const DurationFieldType *type);
    
    //-----------------------------------------------------------------------
    /**
     * Get this period as an immutable <code>Period</code> object.
     *
     * @return a Period using the same field set and values
     */
    Period *toPeriod();
    
    /**
     * Get this object as a <code>MutablePeriod</code>.
     * <p>
     * This will always return a new <code>MutablePeriod</code> with the same fields.
     *
     * @return a MutablePeriod using the same field set and values
     */
    MutablePeriod *toMutablePeriod();
    
    //-----------------------------------------------------------------------
    /**
     * Compares this object with the specified object for equality based
     * on the value of each field. All ReadablePeriod instances are accepted.
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
     * {@link org.joda.time.Duration Duration}s, an operation that emphasises
     * that the result may differ according to the date you choose.
     *
     * @param period  a readable period to check against
     * @return true if all the field values are equal, false if
     *  not or the period is null or of an incorrect type
     */
    bool equals(const Object *period) const;
    
    /**
     * Gets a hash code for the period as defined by ReadablePeriod.
     *
     * @return a hash code
     */
    int hashCode();
    
    //-----------------------------------------------------------------------
    /**
     * Gets the value as a string in the ISO8601 duration format.
     * <p>
     * For example, "PT6H3M7S" represents 6 hours, 3 minutes, 7 seconds.
     * <p>
     * For more control over the output, see
     * {@link org.joda.time.format.PeriodFormatterBuilder PeriodFormatterBuilder}.
     *
     * @return the value as an ISO8601 string
     */
    string toString();
    
    //-----------------------------------------------------------------------
    /**
     * Uses the specified formatter to convert this period to a String.
     *
     * @param formatter  the formatter to use, null means use <code>toString()</code>.
     * @return the formatted string
     * @since 1.5
     */
    string toString(PeriodFormatter *formatter);
    
};

CODATIME_END

#endif /* defined(__CodaTime__AbstractPeriod__) */
