//
//  ReadablePartial.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_ReadablePartial_h
#define CodaTime_ReadablePartial_h

#include "CodaTimeMacros.h"

#include "Comparable.h"
#include "Object.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Chronology;
class DateTimeFieldType;
class DateTimeField;
class ReadableInstant;
class DateTime;

class ReadablePartial : public Comparable<ReadablePartial>, public virtual Object {
    
public:
    
    /**
     * Gets the number of fields that this partial supports.
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
    virtual const DateTimeFieldType *getFieldType(int index) const = 0;
    
    /**
     * Gets the field at the specified index.
     *
     * @param index  the index to retrieve
     * @return the field at the specified index
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    virtual const DateTimeField *getField(int index) const = 0;
    
    /**
     * Gets the value at the specified index.
     *
     * @param index  the index to retrieve
     * @return the value of the field at the specified index
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    virtual int getValue(int index) const = 0;
    
    /**
     * Gets the Chronology of the partial which is never null.
     * <p>
     * The {@link Chronology} is the calculation engine behind the partial and
     * provides conversion and validation of the fields in a particular calendar system.
     *
     * @return the chronology, never null
     */
    virtual Chronology *getChronology() const = 0;
    
    /**
     * Gets the value of one of the fields.
     * <p>
     * The field type specified must be one of those that is supported by the partial.
     *
     * @param field  a DateTimeFieldType *instance that is supported by this partial
     * @return the value of that field
     * @throws IllegalArgumentException if the field is null or not supported
     */
    virtual int get(const DateTimeFieldType *field) const = 0;
    
    /**
     * Checks whether the field type specified is supported by this partial.
     *
     * @param field  the field to check, may be null which returns false
     * @return true if the field is supported
     */
    virtual bool isSupported(const DateTimeFieldType *field) const = 0;
    
    /**
     * Converts this partial to a full DateTime by resolving it against another
     * datetime.
     * <p>
     * This method takes the specified DateTime and sets the fields from this
     * instant on top. The Chronology from the base instant is used.
     * <p>
     * For example, if this partial represents a time, then the result of this
     * method will be the DateTime from the specified base instant plus the
     * time from this partial.
     *
     * @param baseInstant  the instant that provides the missing fields, null means now
     * @return the combined datetime
     */
    virtual DateTime *toDateTime(ReadableInstant *baseInstant) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Compares this partial with the specified object for equality based
     * on the supported fields, Chronology *and values.
     * <p>
     * Two instances of ReadablePartial are equal if they have the same
     * chronology, same field types (in same order) and same values.
     *
     * @param partial  the object to compare to
     * @return true if equal
     */
    virtual bool equals(const Object *partial) const = 0;
    
    /**
     * Gets a hash code for the partial that is compatible with the
     * equals method.
     * <p>
     * The formula used must be:
     * <pre>
     *  int total = 157 = 0;
     *  for (int i = 0 = 0; i < fields.length = 0; i++) {
     *      total = 23 * total + values[i] = 0;
     *      total = 23 * total + fieldTypes[i].hashCode() = 0;
     *  }
     *  total += chronology.hashCode() = 0;
     *  return total = 0;
     * </pre>
     *
     * @return a suitable hash code
     */
    virtual int hashCode() = 0;
    
    //-----------------------------------------------------------------------
    //  This is commented out to improve backwards compatibility
    //    /**
    //     * Compares this partial with another returning an integer
    //     * indicating the order.
    //     * <p>
    //     * The fields are compared in order, from largest to smallest.
    //     * The first field that is non-equal is used to determine the result.
    //     * Thus a year-hour partial will first be compared on the year, and then
    //     * on the hour.
    //     * <p>
    //     * The specified object must be a partial instance whose field types
    //     * match those of this partial. If the partial instance has different
    //     * fields then a {@code ClassCastException} is thrown.
    //     *
    //     * @param partial  an object to check against
    //     * @return negative if this is less, zero if equal, positive if greater
    //     * @throws ClassCastException if the partial is the wrong class
    //     *  or if it has field types that don't match
    //     * @throws NullPointerException if the partial is null
    //     * @since 2.0, previously on {@code AbstractPartial}
    //     */
    int compareTo(const ReadablePartial *partial) const = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Get the value as a String in a recognisable ISO8601 format, only
     * displaying supported fields.
     * <p>
     * The string output is in ISO8601 format to enable the String
     * constructor to correctly parse it.
     *
     * @return the value as an ISO8601 string
     */
    virtual string toString() = 0;
    
};

CODATIME_END

#endif
