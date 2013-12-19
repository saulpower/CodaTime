//
//  AbstractPartial.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_AbstractPartial_h
#define CodaTime_AbstractPartial_h

#include "CodaTimeMacros.h"

#include "ReadablePartial.h"
#include "Comparable.h"
#include "DateTime.h"
#include "DateTimeFieldType.h"
#include "DurationFieldType.h"
#include "Exceptions.h"

#include "field/FieldUtils.h"
#include "format/DateTimeFormatter.h"

#include <vector>
#include <string>

using namespace std;

CODATIME_BEGIN

class ReadableInstant;

/**
 * AbstractPartial provides a standard base implementation of most methods
 * in the ReadablePartial interface.
 * <p>
 * Calculations on are performed using a {@link Chronology}.
 * This Chronology *is set to be in the UTC time zone for all calculations.
 * <p>
 * The methods on this class use {@link ReadablePartial#size()},
 * {@link AbstractPartial#getField(int, Chronology)} and
 * {@link ReadablePartial#getValue(int)} to calculate their results.
 * Subclasses may have a better implementation.
 * <p>
 * AbstractPartial allows subclasses may be mutable and not thread-safe.
 *
 * @author Stephen Colebourne
 * @since 1.0
 */
class AbstractPartial : public virtual ReadablePartial, public Comparable<ReadablePartial> {
    
protected:
    
    //-----------------------------------------------------------------------
    /**
     * Constructor.
     */
    AbstractPartial() : ReadablePartial() {
    }
    
    //-----------------------------------------------------------------------
    /**
     * Gets the field for a specific index in the Chronology *specified.
     * <p>
     * This method must not use any instance variables.
     *
     * @param index  the index to retrieve
     * @param chrono  the Chronology *to use
     * @return the field
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    virtual const DateTimeField *getField(int index, Chronology *chrono) const = 0;
    
    /**
     * Gets the index of the specified field, throwing an exception if the
     * field is unsupported.
     *
     * @param type  the type to check, not NULL
     * @return the index of the field
     * @throws IllegalArgumentException if the field is NULL or not supported
     */
    int indexOfSupported(const DateTimeFieldType *type) const {
        int index = indexOf(type);
        if (index == -1) {
            string err("Field '");
            err.append(type->toString());
            err.append("' is not supported");
            throw IllegalArgumentException(err);
        }
        return index;
    }
    
    /**
     * Gets the index of the first fields to have the specified duration,
     * or -1 if the field is unsupported.
     *
     * @param type  the type to check, may be NULL which returns -1
     * @return the index of the field, -1 if unsupported
     */
    int indexOf(const DurationFieldType *type) const {
        for (int i = 0, isize = size(); i < isize; i++) {
            if (getFieldType(i)->getDurationType() == type) {
                return i;
            }
        }
        return -1;
    }
    
    /**
     * Gets the index of the first fields to have the specified duration,
     * throwing an exception if the field is unsupported.
     *
     * @param type  the type to check, not NULL
     * @return the index of the field
     * @throws IllegalArgumentException if the field is NULL or not supported
     */
    int indexOfSupported(DurationFieldType *type) {
        int index = indexOf(type);
        if (index == -1) {
            string err("Field '");
            err.append(type->toString());
            err.append("' is not supported");
            throw IllegalArgumentException(err);
        }
        return index;
    }
    
public:
    
    virtual int size() const = 0;
    
    virtual int getValue(int index) const = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the field type at the specifed index.
     *
     * @param index  the index
     * @return the field type
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    const DateTimeFieldType *getFieldType(int index) const {
        return getField(index, getChronology())->getType();
    }
    
    /**
     * Gets an array of the field types that this partial supports.
     * <p>
     * The fields are returned largest to smallest, for example Hour, Minute, Second.
     *
     * @return the fields supported in an array that may be altered, largest to smallest
     */
    vector<const DateTimeFieldType*> getFieldTypes() {
        vector<const DateTimeFieldType*> result = vector<const DateTimeFieldType*>(size());
        for (int i = 0; i < result.size(); i++) {
            result[i] = getFieldType(i);
        }
        return result;
    }
    
    /**
     * Gets the field at the specifed index.
     *
     * @param index  the index
     * @return the field
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    const DateTimeField *getField(int index) const {
        return getField(index, getChronology());
    }
    
    /**
     * Gets an array of the fields that this partial supports.
     * <p>
     * The fields are returned largest to smallest, for example Hour, Minute, Second.
     *
     * @return the fields supported in an array that may be altered, largest to smallest
     */
    vector<const DateTimeField*> getFields() {
        vector<const DateTimeField*> result = vector<const DateTimeField*>(size());
        for (int i = 0; i < result.size(); i++) {
            result[i] = getField(i);
        }
        return result;
    }
    
    /**
     * Gets an array of the value of each of the fields that this partial supports.
     * <p>
     * The fields are returned largest to smallest, for example Hour, Minute, Second.
     * Each value corresponds to the same array index as <code>getFields()</code>
     *
     * @return the current values of each field in an array that may be altered, largest to smallest
     */
    vector<int> getValues() {
        vector<int> result = vector<int>(size());
        for (int i = 0; i < result.size(); i++) {
            result[i] = getValue(i);
        }
        return result;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Get the value of one of the fields of a datetime.
     * <p>
     * The field specified must be one of those that is supported by the partial.
     *
     * @param type  a DateTimeFieldType *instance that is supported by this partial
     * @return the value of that field
     * @throws IllegalArgumentException if the field is NULL or not supported
     */
    int get(const DateTimeFieldType *type) const {
        return getValue(indexOfSupported(type));
    }
    
    /**
     * Checks whether the field specified is supported by this partial.
     *
     * @param type  the type to check, may be NULL which returns false
     * @return true if the field is supported
     */
    bool isSupported(const DateTimeFieldType *type) const {
        return (indexOf(type) != -1);
    }
    
    /**
     * Gets the index of the specified field, or -1 if the field is unsupported.
     *
     * @param type  the type to check, may be NULL which returns -1
     * @return the index of the field, -1 if unsupported
     */
    int indexOf(const DateTimeFieldType *type) const {
        for (int i = 0, isize = size(); i < isize; i++) {
            if (getFieldType(i) == type) {
                return i;
            }
        }
        return -1;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Resolves this partial against another complete instant to create a new
     * full instant. The combination is performed using the Chronology *of the
     * specified instant.
     * <p>
     * For example, if this partial represents a time, then the result of this
     * method will be the datetime from the specified base instant plus the
     * time from this partial.
     *
     * @param baseInstant  the instant that provides the missing fields, NULL means now
     * @return the combined datetime
     */
    DateTime *toDateTime(ReadableInstant *baseInstant) {
        Chronology *chrono = DateTimeUtils::getInstantChronology(baseInstant);
        int64_t instantMillis = DateTimeUtils::getInstantMillis(baseInstant);
        int64_t resolved = chrono->set(this, instantMillis);
        return new DateTime(resolved, chrono);
    }
    
    //-----------------------------------------------------------------------
    /**
     * Compares this ReadablePartial with another returning true if the chronology,
     * field types and values are equal.
     *
     * @param partial  an object to check against
     * @return true if fields and values are equal
     */
    bool equals(const Object *partial) const {
        if (this == partial) {
            return true;
        }
        const ReadablePartial *other = dynamic_cast<const ReadablePartial*>(partial);
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
        return FieldUtils::equals(getChronology(), other->getChronology());
    }
    
    /**
     * Gets a hash code for the ReadablePartial that is compatible with the
     * equals method.
     *
     * @return a suitable hash code
     */
    int hashCode() {
        int total = 157;
        for (int i = 0, isize = size(); i < isize; i++) {
            total = 23 * total + getValue(i);
            total = 23 * total + getFieldType(i)->hashCode();
        }
        total += getChronology()->hashCode();
        return total;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Compares this partial with another returning an integer
     * indicating the order.
     * <p>
     * The fields are compared in order, from largest to smallest.
     * The first field that is non-equal is used to determine the result.
     * <p>
     * The specified object must be a partial instance whose field types
     * match those of this partial.
     * <p>
     * NOTE: Prior to v2.0, the {@code Comparable} interface was only implemented
     * in this class and not in the {@code ReadablePartial} interface.
     *
     * @param other  an object to check against
     * @return negative if this is less, zero if equal, positive if greater
     * @throws ClassCastException if the partial is the wrong class
     *  or if it has field types that don't match
     * @throws NULLPointerException if the partial is NULL
     * @since 1.1
     */
    int compareTo(const ReadablePartial *other) const {
        if (this == other) {
            return 0;
        }
        if (size() != other->size()) {
            throw ClassCastException("ReadablePartial objects must have matching field types");
        }
        for (int i = 0, isize = size(); i < isize; i++) {
            if (getFieldType(i) != other->getFieldType(i)) {
                throw ClassCastException("ReadablePartial objects must have matching field types");
            }
        }
        // fields are ordered largest first
        for (int i = 0, isize = size(); i < isize; i++) {
            if (getValue(i) > other->getValue(i)) {
                return 1;
            }
            if (getValue(i) < other->getValue(i)) {
                return -1;
            }
        }
        return 0;
    }
    
    /**
     * Is this partial later than the specified partial.
     * <p>
     * The fields are compared in order, from largest to smallest.
     * The first field that is non-equal is used to determine the result.
     * <p>
     * You may not pass NULL into this method. This is because you need
     * a time zone to accurately determine the current date.
     *
     * @param partial  a partial to check against, must not be NULL
     * @return true if this date is after the date passed in
     * @throws IllegalArgumentException if the specified partial is NULL
     * @throws ClassCastException if the partial has field types that don't match
     * @since 1.1
     */
    bool isAfter(ReadablePartial *partial) {
        if (partial == NULL) {
            throw IllegalArgumentException("Partial cannot be NULL");
        }
        return compareTo(partial) > 0;
    }
    
    /**
     * Is this partial earlier than the specified partial.
     * <p>
     * The fields are compared in order, from largest to smallest.
     * The first field that is non-equal is used to determine the result.
     * <p>
     * You may not pass NULL into this method. This is because you need
     * a time zone to accurately determine the current date.
     *
     * @param partial  a partial to check against, must not be NULL
     * @return true if this date is before the date passed in
     * @throws IllegalArgumentException if the specified partial is NULL
     * @throws ClassCastException if the partial has field types that don't match
     * @since 1.1
     */
    bool isBefore(ReadablePartial *partial) {
        if (partial == NULL) {
            throw IllegalArgumentException("Partial cannot be NULL");
        }
        return compareTo(partial) < 0;
    }
    
    /**
     * Is this partial the same as the specified partial.
     * <p>
     * The fields are compared in order, from largest to smallest.
     * If all fields are equal, the result is true.
     * <p>
     * You may not pass NULL into this method. This is because you need
     * a time zone to accurately determine the current date.
     *
     * @param partial  a partial to check against, must not be NULL
     * @return true if this date is the same as the date passed in
     * @throws IllegalArgumentException if the specified partial is NULL
     * @throws ClassCastException if the partial has field types that don't match
     * @since 1.1
     */
    bool isEqual(ReadablePartial *partial) {
        if (partial == NULL) {
            throw IllegalArgumentException("Partial cannot be NULL");
        }
        return compareTo(partial) == 0;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Uses the specified formatter to convert this partial to a String.
     *
     * @param formatter  the formatter to use, NULL means use <code>toString()</code>.
     * @return the formatted string
     * @since 1.1
     */
    string toString(DateTimeFormatter *formatter) {
        if (formatter == NULL) {
            return Object::toString();
        }
        return formatter->print(this);
    }
    
};

CODATIME_END

#endif
