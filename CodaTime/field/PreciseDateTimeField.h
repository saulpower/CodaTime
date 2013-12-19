//
//  PreciseDateTimeField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_PreciseDateTimeField_h
#define CodaTime_PreciseDateTimeField_h

#include "CodaTimeMacros.h"

#include "DurationField.h"
#include "Exceptions.h"
#include "field/PreciseDurationDateTimeField.h"
#include "field/FieldUtils.h"

CODATIME_BEGIN

/**
 * Precise datetime field, composed of two precise duration fields.
 * <p>
 * This DateTimeField is useful for defining DateTimeFields that are composed
 * of precise durations, like time of day fields. If either duration field is
 * imprecise, then an {@link ImpreciseDateTimeField} may be used instead.
 * <p>
 * PreciseDateTimeField is thread-safe and immutable.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 * @see ImpreciseDateTimeField
 */
class PreciseDateTimeField : public PreciseDurationDateTimeField {
    
private:
    
    static const long long serialVersionUID = -5586801265774496376L;
    
    /** The maximum range in the correct units */
    int iRange;
    
    const DurationField *iRangeField;
    
    
public:
    
    string getName() const {
        return PreciseDurationDateTimeField::getName();
    }
    
    /**
     * Constructor.
     *
     * @param type  the field type this field uses
     * @param unit  precise unit duration, like "seconds()".
     * @param range precise range duration, preferably a multiple of the unit,
     * like "minutes()".
     * @throws IllegalArgumentException if either duration field is imprecise
     * @throws IllegalArgumentException if unit milliseconds is less than one
     * or effective value range is less than two.
     */
    PreciseDateTimeField(const DateTimeFieldType *type,
                         const DurationField *unit, const DurationField *range) : PreciseDurationDateTimeField(type, unit) {
        
        if (!range->isPrecise()) {
            throw IllegalArgumentException("Range duration field must be precise");
        }
        
        int64_t rangeMillis = range->getUnitMillis();
        iRange = (int)(rangeMillis / getUnitMillis());
        if (iRange < 2) {
            throw IllegalArgumentException("The effective range must be at least 2");
        }
        
        iRangeField = range;
    }
    
    /**
     * Get the amount of fractional units from the specified time instant.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the amount of fractional units extracted from the input.
     */
    int get(int64_t instant) const {
        if (instant >= 0) {
            return (int) ((instant / getUnitMillis()) % iRange);
        } else {
            return iRange - 1 + (int) (((instant + 1) / getUnitMillis()) % iRange);
        }
    }
    
    /**
     * Add to the component of the specified time instant, wrapping around
     * within that component if necessary.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to add to
     * @param amount  the amount of units to add (can be negative).
     * @return the updated time instant.
     */
    int64_t addWrapField(int64_t instant, int amount) const {
        int thisValue = get(instant);
        int wrappedValue = FieldUtils::getWrappedValue
        (thisValue, amount, getMinimumValue(), getMaximumValue());
        // copy code from set() to avoid repeat call to get()
        return instant + (wrappedValue - thisValue) * getUnitMillis();
    }
    
    /**
     * Set the specified amount of units to the specified time instant.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to set in
     * @param value  value of units to set.
     * @return the updated time instant.
     * @throws IllegalArgumentException if value is too large or too small.
     */
    int64_t set(int64_t instant, int value) const {
        FieldUtils::verifyValueBounds(this, value, getMinimumValue(), getMaximumValue());
        return instant + (value - get(instant)) * iUnitMillis;
    }
    
    /**
     * Returns the range duration of this field. For example, if this field
     * represents "minute of hour", then the range duration field is an hours.
     *
     * @return the range duration of this field, or null if field has no range
     */
    const DurationField *getRangeDurationField() const {
        return iRangeField;
    }
    
    /**
     * Get the maximum value for the field.
     *
     * @return the maximum value
     */
    int getMaximumValue() const {
        return iRange - 1;
    }
    
    /**
     * Returns the range of the field in the field's units.
     * <p>
     * For example, 60 for seconds per minute. The field is allowed values
     * from 0 to range - 1.
     *
     * @return unit range
     */
    int getRange() const {
        return iRange;
    }
    
};

CODATIME_END

#endif
