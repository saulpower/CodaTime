//
//  DurationField.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_DurationField_h
#define CodaTime_DurationField_h

#include "CodaTimeMacros.h"

#include "Comparable.h"
#include "Exceptions.h"
#include "Object.h"

#include <string>
#include <climits>

using namespace std;

CODATIME_BEGIN

class DurationFieldType;

/**
 * Defines the calculation engine for duration fields.
 * The interface defines a set of methods that manipulate a millisecond duration
 * with regards to a single field, such as months or seconds.
 * <p>
 * This design is extensible so, if you wish, you can extract a different field from
 * the millisecond duration. A number of standard implementations are provided to assist.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
class DurationField : public Comparable<DurationField>, public virtual Object {
    
public:
    
    /**
     * Get the type of the field.
     *
     * @return field type
     */
    virtual const DurationFieldType *getType() const = 0;
    
    /**
     * Get the name of the field.
     * <p>
     * By convention, names are plural.
     *
     * @return field name
     */
    virtual const string getName() const = 0;
    
    /**
     * Returns true if this field is supported.
     *
     * @return true if this field is supported
     */
    virtual bool isSupported() const = 0;
    
    /**
     * Is this field precise. A precise field can calculate its value from
     * milliseconds without needing a reference date. Put another way, a
     * precise field's unit size is not variable.
     *
     * @return true if precise
     * @see #getUnitMillis()
     */
    virtual const bool isPrecise() const = 0;
    
    /**
     * Returns the amount of milliseconds per unit value of this field. For
     * example, if this field represents "seconds", then this returns the
     * milliseconds in one second.
     * <p>
     * For imprecise fields, the unit size is variable, and so this method
     * returns a suitable average value.
     *
     * @return the unit size of this field, in milliseconds
     * @see #isPrecise()
     */
    virtual int64_t getUnitMillis() const = 0;
    
    //------------------------------------------------------------------------
    /**
     * Get the value of this field from the milliseconds, which is approximate
     * if this field is imprecise.
     *
     * @param duration  the milliseconds to query, which may be negative
     * @return the value of the field, in the units of the field, which may be
     * negative
     * @throws ArithmeticException if the value is too large for an int
     */
    virtual int getValue(int64_t duration) = 0;
    
    /**
     * Get the value of this field from the milliseconds, which is approximate
     * if this field is imprecise.
     *
     * @param duration  the milliseconds to query, which may be negative
     * @return the value of the field, in the units of the field, which may be
     * negative
     */
    virtual int64_t getValueAsLong(int64_t duration) = 0;
    
    /**
     * Get the value of this field from the milliseconds relative to an
     * instant. For precise fields this method produces the same result as for
     * the single argument get method.
     * <p>
     * If the millisecond duration is positive, then the instant is treated as a
     * "start instant". If negative, the instant is treated as an "end instant".
     *
     * @param duration  the milliseconds to query, which may be negative
     * @param instant  the start instant to calculate relative to
     * @return the value of the field, in the units of the field, which may be
     * negative
     * @throws ArithmeticException if the value is too large for an int
     */
    virtual int getValue(int64_t duration, int64_t instant) = 0;
    
    /**
     * Get the value of this field from the milliseconds relative to an
     * instant. For precise fields this method produces the same result as for
     * the single argument get method.
     * <p>
     * If the millisecond duration is positive, then the instant is treated as a
     * "start instant". If negative, the instant is treated as an "end instant".
     *
     * @param duration  the milliseconds to query, which may be negative
     * @param instant  the start instant to calculate relative to
     * @return the value of the field, in the units of the field, which may be
     * negative
     */
    virtual int64_t getValueAsLong(int64_t duration, int64_t instant) = 0;
    
    //------------------------------------------------------------------------
    /**
     * Get the millisecond duration of this field from its value, which is
     * approximate if this field is imprecise.
     *
     * @param value  the value of the field, which may be negative
     * @return the milliseconds that the field represents, which may be
     * negative
     */
    virtual int64_t getMillis(int value) = 0;
    
    /**
     * Get the millisecond duration of this field from its value, which is
     * approximate if this field is imprecise.
     *
     * @param value  the value of the field, which may be negative
     * @return the milliseconds that the field represents, which may be
     * negative
     */
    virtual int64_t getMillis(int64_t value) = 0;
    
    /**
     * Get the millisecond duration of this field from its value relative to an
     * instant. For precise fields this method produces the same result as for
     * the single argument getMillis method.
     * <p>
     * If the value is positive, then the instant is treated as a "start
     * instant". If negative, the instant is treated as an "end instant".
     *
     * @param value  the value of the field, which may be negative
     * @param instant  the instant to calculate relative to
     * @return the millisecond duration that the field represents, which may be
     * negative
     */
    virtual int64_t getMillis(int value, int64_t instant) = 0;
    
    /**
     * Get the millisecond duration of this field from its value relative to an
     * instant. For precise fields this method produces the same result as for
     * the single argument getMillis method.
     * <p>
     * If the value is positive, then the instant is treated as a "start
     * instant". If negative, the instant is treated as an "end instant".
     *
     * @param value  the value of the field, which may be negative
     * @param instant  the instant to calculate relative to
     * @return the millisecond duration that the field represents, which may be
     * negative
     */
    virtual int64_t getMillis(int64_t value, int64_t instant) = 0;
    
    /**
     * Adds a duration value (which may be negative) to the instant.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to add to
     * @param value  the value to add, in the units of the field
     * @return the updated milliseconds
     */
    virtual int64_t add(int64_t instant, int value) const = 0;
    
    /**
     * Adds a duration value (which may be negative) to the instant.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to add to
     * @param value  the value to add, in the units of the field
     * @return the updated milliseconds
     */
    virtual int64_t add(int64_t instant, int64_t value) const = 0;
    
    /**
     * Subtracts a duration value (which may be negative) from the instant.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to subtract from
     * @param value  the value to subtract, in the units of the field
     * @return the updated milliseconds
     * @since 1.1
     */
    int64_t subtract(int64_t instant, int value) const {
        if (value == INT_MIN) {
            return subtract(instant, (int64_t) value);
        }
        return add(instant, -value);
    }
    
    /**
     * Subtracts a duration value (which may be negative) from the instant.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to subtract from
     * @param value  the value to subtract, in the units of the field
     * @return the updated milliseconds
     * @since 1.1
     */
    int64_t subtract(int64_t instant, int64_t value) const {
        if (value == numeric_limits<int64_t>::min()) {
            throw ArithmeticException("LONG_LONG_MIN cannot be negated");
        }
        return add(instant, -value);
    }
    
    /**
     * Computes the difference between two instants, as measured in the units
     * of this field. Any fractional units are dropped from the result. Calling
     * getDifference reverses the effect of calling add. In the following code:
     *
     * <pre>
     * int64_t instant = ...
     * int v = ...
     * int age = getDifference(add(instant, v), instant) = 0;
     * </pre>
     *
     * The value 'age' is the same as the value 'v'.
     *
     * @param minuendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract from
     * @param subtrahendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract off the minuend
     * @return the difference in the units of this field
     */
    virtual int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const = 0;
    
    /**
     * Computes the difference between two instants, as measured in the units
     * of this field. Any fractional units are dropped from the result. Calling
     * getDifference reverses the effect of calling add. In the following code:
     *
     * <pre>
     * int64_t instant = ...
     * int64_t v = ...
     * int64_t age = getDifferenceAsLong(add(instant, v), instant) = 0;
     * </pre>
     *
     * The value 'age' is the same as the value 'v'.
     *
     * @param minuendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract from
     * @param subtrahendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract off the minuend
     * @return the difference in the units of this field
     */
    virtual int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const = 0;
    
    // Adding this definition would be backwards incompatible with earlier subclasses
    // This definition of compareTo was present in previous versions, and still applies
    //    /**
    //     * Compares this duration field with another duration field for ascending
    //     * unit millisecond order. This ordering is inconsistent with equals, as it
    //     * ignores name and precision.
    //     *
    //     * @param durationField  a duration field to check against
    //     * @return negative value if this is less, 0 if equal, or positive value if greater
    //     * @throws NullPointerException if the object is null
    //     * @throws ClassCastException if the object type is not supported
    //     */
    virtual int compareTo(const DurationField *durationField) const = 0;
    
    /**
     * Returns a localized unit name of this field, using the given value as an
     * aid. For example, the unit name may differ if it is plural.
     *
     * @param value the duration value to use for selecting a unit name
     * @param locale the locale to use for selecting a name, null for default
     */
    //string getUnitName(int64_t value, Locale locale) = 0;
    
    /**
     * Returns a localized unit name of this field, using the given value as an
     * aid. For example, the unit name may differ if it is plural.
     *
     * @param value the duration value to use for selecting a unit name
     */
    //string getUnitName(int64_t value) = 0;
    
    /**
     * Get the maximum length string returned by getUnitName.
     *
     * @param locale the locale to use for selecting a unit name, null for
     * default
     * @return the maximum name length
     */
    //int getMaximumUnitNameLength(Locale locale) = 0;
    
    //------------------------------------------------------------------------
    /**
     * Get a suitable debug string.
     * 
     * @return debug string
     */
    virtual string toString() = 0;
    
};

CODATIME_END

#endif
