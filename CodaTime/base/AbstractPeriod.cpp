//
//  AbstractPeriod.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "AbstractPeriod.h"
#include "DurationFieldType.h"
#include "format/ISOPeriodFormat.h"
#include "MutablePeriod.h"
#include "Object.h"
#include "Period.h"
#include "PeriodType.h"

CODATIME_BEGIN

int AbstractPeriod::size() const {
    return getPeriodType()->size();
}

/**
 * Gets the field type at the specified index.
 *
 * @param index  the index to retrieve
 * @return the field at the specified index
 * @throws IndexOutOfBoundsException if the index is invalid
 * @since 2.0 (previously on BasePeriod)
 */
const DurationFieldType *AbstractPeriod::getFieldType(int index) const {
    return getPeriodType()->getFieldType(index);
}

/**
 * Gets an array of the field types that this period supports.
 * <p>
 * The fields are returned largest to smallest, for example Hours, Minutes, Seconds.
 *
 * @return the fields supported in an array that may be altered, largest to smallest
 */
vector<const DurationFieldType*> AbstractPeriod::getFieldTypes() {
    vector<const DurationFieldType*> result = vector<const DurationFieldType*>(size());
    for (int i = 0; i < result.size(); i++) {
        result[i] = getFieldType(i);
    }
    return result;
}

/**
 * Gets an array of the value of each of the fields that this period supports.
 * <p>
 * The fields are returned largest to smallest, for example Hours, Minutes, Seconds.
 * Each value corresponds to the same array index as <code>getFields()</code>
 *
 * @return the current values of each field in an array that may be altered, largest to smallest
 */
vector<int> AbstractPeriod::getValues() {
    vector<int> result = vector<int>(size());
    for (int i = 0; i < result.size(); i++) {
        result[i] = getValue(i);
    }
    return result;
}

//-----------------------------------------------------------------------
/**
 * Gets the value of one of the fields.
 * <p>
 * If the field type specified is not supported by the period then zero
 * is returned.
 *
 * @param type  the field type to query, NULL returns zero
 * @return the value of that field, zero if field not supported
 */
int AbstractPeriod::get(const DurationFieldType *type) {
    int index = indexOf(type);
    if (index == -1) {
        return 0;
    }
    return getValue(index);
}

/**
 * Checks whether the field specified is supported by this period.
 *
 * @param type  the type to check, may be NULL which returns false
 * @return true if the field is supported
 */
bool AbstractPeriod::isSupported(const DurationFieldType *type) {
    return getPeriodType()->isSupported(type);
}

/**
 * Gets the index of the field in this period.
 *
 * @param type  the type to check, may be NULL which returns -1
 * @return the index of -1 if not supported
 */
int AbstractPeriod::indexOf(const DurationFieldType *type) {
    return getPeriodType()->indexOf(type);
}

//-----------------------------------------------------------------------
/**
 * Get this period as an immutable <code>Period</code> object.
 *
 * @return a Period using the same field set and values
 */
Period *AbstractPeriod::toPeriod() {
    return new Period(this);
}

/**
 * Get this object as a <code>MutablePeriod</code>.
 * <p>
 * This will always return a new <code>MutablePeriod</code> with the same fields.
 *
 * @return a MutablePeriod using the same field set and values
 */
MutablePeriod *AbstractPeriod::toMutablePeriod() {
    return new MutablePeriod(this);
}

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
 *  not or the period is NULL or of an incorrect type
 */
bool AbstractPeriod::equals(const Object *period) const {
//    if (this == period) {
//        return true;
//    }
    
    const ReadablePeriod *other = dynamic_cast<const ReadablePeriod*>(period);
    
    if (other == 0) {
        return false;
    }
    if (size() != other->size()) {
        return false;
    }
    for (int i = 0, isize = size(); i < isize; i++) {
        if (getValue(i) != other->getValue(i) || getFieldType(i) != other->getFieldType(i)) {
            return false;
        }
    }
    return true;
}

/**
 * Gets a hash code for the period as defined by ReadablePeriod.
 *
 * @return a hash code
 */
int AbstractPeriod::hashCode() {
    int total = 17;
    for (int i = 0, isize = size(); i < isize; i++) {
        total = 27 * total + getValue(i);
        total = 27 * total + getFieldType(i)->hashCode();
    }
    return total;
}

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
string AbstractPeriod::toString() {
    return ISOPeriodFormat::standard()->print(this);
}

//-----------------------------------------------------------------------
/**
 * Uses the specified formatter to convert this period to a String.
 *
 * @param formatter  the formatter to use, NULL means use <code>toString()</code>.
 * @return the formatted string
 * @since 1.5
 */
string AbstractPeriod::toString(PeriodFormatter *formatter) {
    if (formatter == NULL) {
        return toString();
    }
    return formatter->print(this);
}

CODATIME_END