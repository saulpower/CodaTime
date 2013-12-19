//
//  FieldUtils::h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_FieldUtils_h
#define CodaTime_FieldUtils_h

#include "CodaTimeMacros.h"

#include "DateTimeFieldType.h"
#include "DateTimeField.h"
#include "Exceptions.h"
#include "Object.h"

#include <string>

using namespace std;

CODATIME_BEGIN

/**
 * General utilities that don't fit elsewhere.
 * <p>
 * FieldUtils is thread-safe and immutable.
 *
 * @author Stephen Colebourne
 * @since 1.0
 */
class FieldUtils {
    
private:
    
    /**
     * Restricted constructor.
     */
    FieldUtils() {
    }
    
public:
    
    //------------------------------------------------------------------------
    /**
     * Negates the input throwing an exception if it can't negate it.
     *
     * @param value  the value to negate
     * @return the negated value
     * @throws ArithmeticException if the value is INT_MIN
     * @since 1.1
     */
    static int safeNegate(int value) {
        if (value == INT_MIN) {
            throw ArithmeticException("INT_MIN cannot be negated");
        }
        return -value;
    }
    
    /**
     * Add two values throwing an exception if overflow occurs.
     *
     * @param val1  the first value
     * @param val2  the second value
     * @return the new total
     * @throws ArithmeticException if the value is too big or too small
     */
    static int safeAdd(int val1, int val2) {
        int sum = val1 + val2;
        // If there is a sign change, but the two values have the same sign...
        if ((val1 ^ sum) < 0 && (val1 ^ val2) >= 0) {
            string err("The calculation caused an overflow: ");
            err.append(to_string(val1));
            err.append(" + ");
            err.append(to_string(val2));
            throw ArithmeticException(err);
        }
        return sum;
    }
    
    /**
     * Add two values throwing an exception if overflow occurs.
     *
     * @param val1  the first value
     * @param val2  the second value
     * @return the new total
     * @throws ArithmeticException if the value is too big or too small
     */
    static int64_t safeAdd(int64_t val1, int64_t val2) {
        int64_t sum = val1 + val2;
        // If there is a sign change, but the two values have the same sign...
        if ((val1 ^ sum) < 0 && (val1 ^ val2) >= 0) {
            string err("The calculation caused an overflow: ");
            err.append(to_string(val1));
            err.append(" + ");
            err.append(to_string(val2));
            throw ArithmeticException(err);
        }
        return sum;
    }
    
    /**
     * Subtracts two values throwing an exception if overflow occurs.
     *
     * @param val1  the first value, to be taken away from
     * @param val2  the second value, the amount to take away
     * @return the new total
     * @throws ArithmeticException if the value is too big or too small
     */
    static int64_t safeSubtract(int64_t val1, int64_t val2) {
        int64_t diff = val1 - val2;
        // If there is a sign change, but the two values have different signs...
        if ((val1 ^ diff) < 0 && (val1 ^ val2) < 0) {
            string err("The calculation caused an overflow: ");
            err.append(to_string(val1));
            err.append(" + ");
            err.append(to_string(val2));
            throw ArithmeticException(err);
        }
        return diff;
    }
    
    /**
     * Multiply two values throwing an exception if overflow occurs.
     *
     * @param val1  the first value
     * @param val2  the second value
     * @return the new total
     * @throws ArithmeticException if the value is too big or too small
     * @since 1.2
     */
    static int safeMultiply(int val1, int val2) {
        int64_t total = (int64_t) val1 * (int64_t) val2;
        if (total < INT_MIN || total > INT_MAX) {
            string err("Multiplication overflows an int: ");
            err.append(to_string(val1));
            err.append(" * ");
            err.append(to_string(val2));
            throw ArithmeticException(err);
        }
        return (int) total;
    }
    
    /**
     * Multiply two values throwing an exception if overflow occurs.
     *
     * @param val1  the first value
     * @param val2  the second value
     * @return the new total
     * @throws ArithmeticException if the value is too big or too small
     * @since 1.2
     */
    static int64_t safeMultiply(int64_t val1, int val2) {
        switch (val2) {
            case -1:
                if (val1 == LONG_MIN) {
                    string err("Multiplication overflows an long: ");
                    err.append(to_string(val1));
                    err.append(" * ");
                    err.append(to_string(val2));
                    throw ArithmeticException(err);
                }
                return -val1;
            case 0:
                return 0L;
            case 1:
                return val1;
        }
        int64_t total = val1 * val2;
        if (total / val2 != val1) {
            string err("Multiplication overflows an long: ");
            err.append(to_string(val1));
            err.append(" * ");
            err.append(to_string(val2));
            throw ArithmeticException(err);
        }
        return total;
    }
    
    /**
     * Multiply two values throwing an exception if overflow occurs.
     *
     * @param val1  the first value
     * @param val2  the second value
     * @return the new total
     * @throws ArithmeticException if the value is too big or too small
     */
    static int64_t safeMultiply(int64_t val1, int64_t val2) {
        if (val2 == 1) {
            return val1;
        }
        if (val1 == 1) {
            return val2;
        }
        if (val1 == 0 || val2 == 0) {
            return 0;
        }
        int64_t total = val1 * val2;
        if (total / val2 != val1 || (val1 == LONG_MIN && val2 == -1) || (val2 == LONG_MIN && val1 == -1)) {
            string err("Multiplication overflows an long: ");
            err.append(to_string(val1));
            err.append(" * ");
            err.append(to_string(val2));
            throw ArithmeticException(err);
        }
        return total;
    }
    
    /**
     * Divides the dividend by the divisor throwing an exception if
     * overflow occurs or the divisor is zero.
     *
     * @param dividend  the dividend
     * @param divisor  the divisor
     * @return the new total
     * @throws ArithmeticException if the operation overflows or the divisor is zero
     */
    static int64_t safeDivide(int64_t dividend, int64_t divisor) {
        if (dividend == LONG_MIN && divisor == -1L) {
            string err("Division overflows an long: ");
            err.append(to_string(dividend));
            err.append(" / ");
            err.append(to_string(divisor));
            throw ArithmeticException(err);
        }
        return dividend / divisor;
    }
    
    /**
     * Casts to an int throwing an exception if overflow occurs.
     *
     * @param value  the value
     * @return the value as an int
     * @throws ArithmeticException if the value is too big or too small
     */
    static int safeToInt(int64_t value) {
        if (INT_MIN <= value && value <= INT_MAX) {
            return (int) value;
        }
        string err("Value cannot fit in an int: ");
        err.append(to_string(value));
        throw ArithmeticException(err);
    }
    
    /**
     * Multiply two values to return an int throwing an exception if overflow occurs.
     *
     * @param val1  the first value
     * @param val2  the second value
     * @return the new total
     * @throws ArithmeticException if the value is too big or too small
     */
    static int safeMultiplyToInt(int64_t val1, int64_t val2) {
        int64_t val = FieldUtils::safeMultiply(val1, val2);
        return FieldUtils::safeToInt(val);
    }
    
    //-----------------------------------------------------------------------
    /**
     * Verify that input values are within specified bounds.
     *
     * @param value  the value to check
     * @param lowerBound  the lower bound allowed for value
     * @param upperBound  the upper bound allowed for value
     * @throws IllegalFieldValueException if value is not in the specified bounds
     */
    static void verifyValueBounds(const DateTimeField *field,
                                         int value, int lowerBound, int upperBound) {
        if ((value < lowerBound) || (value > upperBound)) {
            throw new IllegalFieldValueException
            (field->getType()->toString(), to_string(value),
             to_string(lowerBound), to_string(upperBound));
        }
    }
    
    /**
     * Verify that input values are within specified bounds.
     *
     * @param value  the value to check
     * @param lowerBound  the lower bound allowed for value
     * @param upperBound  the upper bound allowed for value
     * @throws IllegalFieldValueException if value is not in the specified bounds
     * @since 1.1
     */
    static void verifyValueBounds(const DateTimeFieldType *fieldType,
                                         int value, int lowerBound, int upperBound) {
        if ((value < lowerBound) || (value > upperBound)) {
            throw new IllegalFieldValueException
            (fieldType->toString(), to_string(value),
             to_string(lowerBound), to_string(upperBound));
        }
    }
    
    /**
     * Verify that input values are within specified bounds.
     *
     * @param value  the value to check
     * @param lowerBound  the lower bound allowed for value
     * @param upperBound  the upper bound allowed for value
     * @throws IllegalFieldValueException if value is not in the specified bounds
     */
    static void verifyValueBounds(string fieldName,
                                         int value, int lowerBound, int upperBound) {
        if ((value < lowerBound) || (value > upperBound)) {
            throw new IllegalFieldValueException
            (fieldName, to_string(value),
             to_string(lowerBound), to_string(upperBound));
        }
    }
    
    /**
     * Utility method used by addWrapField implementations to ensure the new
     * value lies within the field's legal value range.
     *
     * @param currentValue the current value of the data, which may lie outside
     * the wrapped value range
     * @param wrapValue  the value to add to current value before
     *  wrapping.  This may be negative.
     * @param minValue the wrap range minimum value.
     * @param maxValue the wrap range maximum value.  This must be
     *  greater than minValue (checked by the method).
     * @return the wrapped value
     * @throws IllegalArgumentException if minValue is greater
     *  than or equal to maxValue
     */
    static int getWrappedValue(int currentValue, int wrapValue,
                                      int minValue, int maxValue) {
        return getWrappedValue(currentValue + wrapValue, minValue, maxValue);
    }
    
    /**
     * Utility method that ensures the given value lies within the field's
     * legal value range.
     *
     * @param value  the value to fit into the wrapped value range
     * @param minValue the wrap range minimum value.
     * @param maxValue the wrap range maximum value.  This must be
     *  greater than minValue (checked by the method).
     * @return the wrapped value
     * @throws IllegalArgumentException if minValue is greater
     *  than or equal to maxValue
     */
    static int getWrappedValue(int value, int minValue, int maxValue) {
        if (minValue >= maxValue) {
            throw new IllegalArgumentException("MIN > MAX");
        }
        
        int wrapRange = maxValue - minValue + 1;
        value -= minValue;
        
        if (value >= 0) {
            return (value % wrapRange) + minValue;
        }
        
        int remByRange = (-value) % wrapRange;
        
        if (remByRange == 0) {
            return 0 + minValue;
        }
        return (wrapRange - remByRange) + minValue;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Compares two objects as equals handling null.
     *
     * @param object1  the first object
     * @param object2  the second object
     * @return true if equal
     * @since 1.4
     */
    static bool equals(const Object *object1, Object *object2) {
        if (object1 == object2) {
            return true;
        }
        if (object1 == NULL || object2 == NULL) {
            return false;
        }
        return object1->equals(object2);
    }
    
};

CODATIME_END

#endif
