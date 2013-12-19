//
//  ZeroIsMaxDateTimeField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_ZeroIsMaxDateTimeField_h
#define CodaTime_ZeroIsMaxDateTimeField_h

#include "CodaTimeMacros.h"

#include "DateTimeField.h"
#include "Exceptions.h"
#include "field/DecoratedDateTimeField.h"
#include "field/FieldUtils.h"

#include <vector>

using namespace std;

CODATIME_BEGIN

/**
 * Wraps another field such that zero values are replaced with one more than
 * it's maximum. This is particularly useful for implementing an clockhourOfDay
 * field, where the midnight value of 0 is replaced with 24.
 * <p>
 * ZeroIsMaxDateTimeField is thread-safe and immutable.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class ZeroIsMaxDateTimeField : public DecoratedDateTimeField {
    
public:
    
    /**
     * Constructor.
     *
     * @param field  the base field
     * @param type  the field type this field will actually use
     * @throws IllegalArgumentException if wrapped field's minimum value is not zero
     */
    ZeroIsMaxDateTimeField(const DateTimeField *field, const DateTimeFieldType *type) : DecoratedDateTimeField(field, type) {
        if (field->getMinimumValue() != 0) {
            throw IllegalArgumentException("Wrapped field's minumum value must be zero");
        }
    }
    
    int get(int64_t instant) const {
        int value = getWrappedField()->get(instant);
        if (value == 0) {
            value = getMaximumValue();
        }
        return value;
    }
    
    int64_t add(int64_t instant, int value) const {
        return getWrappedField()->add(instant, value);
    }
    
    int64_t add(int64_t instant, int64_t value) const {
        return getWrappedField()->add(instant, value);
    }
    
    int64_t addWrapField(int64_t instant, int value) const {
        return getWrappedField()->addWrapField(instant, value);
    }
    
    vector<int> addWrapField(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const {
        return getWrappedField()->addWrapField(instant, fieldIndex, values, valueToAdd);
    }
    
    int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return getWrappedField()->getDifference(minuendInstant, subtrahendInstant);
    }
    
    int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return getWrappedField()->getDifferenceAsLong(minuendInstant, subtrahendInstant);
    }
    
    int64_t set(int64_t instant, int value) const {
        int max = getMaximumValue();
        FieldUtils::verifyValueBounds(this, value, 1, max);
        if (value == max) {
            value = 0;
        }
        return getWrappedField()->set(instant, value);
    }
    
    bool isLeap(int64_t instant) const {
        return getWrappedField()->isLeap(instant);
    }
    
    int getLeapAmount(int64_t instant) const {
        return getWrappedField()->getLeapAmount(instant);
    }
    
    const DurationField *getLeapDurationField() const {
        return getWrappedField()->getLeapDurationField();
    }
    
    /**
     * Always returns 1.
     *
     * @return the minimum value of 1
     */
    int getMinimumValue() const {
        return 1;
    }
    
    /**
     * Always returns 1.
     *
     * @return the minimum value of 1
     */
    int getMinimumValue(int64_t instant) const {
        return 1;
    }
    
    /**
     * Always returns 1.
     *
     * @return the minimum value of 1
     */
    int getMinimumValue(ReadablePartial instant) const {
        return 1;
    }
    
    /**
     * Always returns 1.
     *
     * @return the minimum value of 1
     */
    int getMinimumValue(ReadablePartial instant, vector<int> values) const {
        return 1;
    }
    
    /**
     * Get the maximum value for the field, which is one more than the wrapped
     * field's maximum value.
     *
     * @return the maximum value
     */
    int getMaximumValue() const {
        return getWrappedField()->getMaximumValue() + 1;
    }
    
    /**
     * Get the maximum value for the field, which is one more than the wrapped
     * field's maximum value.
     *
     * @return the maximum value
     */
    int getMaximumValue(int64_t instant) const {
        return getWrappedField()->getMaximumValue(instant) + 1;
    }
    
    /**
     * Get the maximum value for the field, which is one more than the wrapped
     * field's maximum value.
     *
     * @return the maximum value
     */
    int getMaximumValue(ReadablePartial *instant) const {
        return getWrappedField()->getMaximumValue(instant) + 1;
    }
    
    /**
     * Get the maximum value for the field, which is one more than the wrapped
     * field's maximum value.
     *
     * @return the maximum value
     */
    int getMaximumValue(ReadablePartial *instant, vector<int> values) const {
        return getWrappedField()->getMaximumValue(instant, values) + 1;
    }
    
    int64_t roundFloor(int64_t instant) const {
        return getWrappedField()->roundFloor(instant);
    }
    
    int64_t roundCeiling(int64_t instant) const {
        return getWrappedField()->roundCeiling(instant);
    }
    
    int64_t roundHalfFloor(int64_t instant) const {
        return getWrappedField()->roundHalfFloor(instant);
    }
    
    int64_t roundHalfCeiling(int64_t instant) const {
        return getWrappedField()->roundHalfCeiling(instant);
    }
    
    int64_t roundHalfEven(int64_t instant) const {
        return getWrappedField()->roundHalfEven(instant);
    }
    
    int64_t remainder(int64_t instant) const {
        return getWrappedField()->remainder(instant);
    }
    
};

CODATIME_END

#endif
