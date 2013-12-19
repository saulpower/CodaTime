//
//  PreciseDurationDateTimeField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_PreciseDurationDateTimeField_h
#define CodaTime_PreciseDurationDateTimeField_h

#include "CodaTimeMacros.h"

#include "Exceptions.h"
#include "field/BaseDateTimeField.h"
#include "field/FieldUtils.h"

CODATIME_BEGIN

/**
 * Precise datetime field, which has a precise unit duration field.
 * <p>
 * PreciseDurationDateTimeField is thread-safe and immutable, and its
 * subclasses must be as well.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class PreciseDurationDateTimeField : public BaseDateTimeField {
    
private:
    
    static const long long serialVersionUID = 5004523158306266035L;
    
    const DurationField *iUnitField;
    
public:
    
    virtual int get(int64_t instant) const = 0;
    
    string getName() const {
        return BaseDateTimeField::getName();
    }
    
    /**
     * Constructor.
     *
     * @param type  the field type
     * @param unit  precise unit duration, like "days()".
     * @throws IllegalArgumentException if duration field is imprecise
     * @throws IllegalArgumentException if unit milliseconds is less than one
     */
    PreciseDurationDateTimeField(const DateTimeFieldType *type, const DurationField *unit) : BaseDateTimeField(type) {
        
        if (!unit->isPrecise()) {
            throw IllegalArgumentException("Unit duration field must be precise");
        }
        
        iUnitMillis = unit->getUnitMillis();
        if (iUnitMillis < 1) {
            throw IllegalArgumentException("The unit milliseconds must be at least 1");
        }
        
        iUnitField = unit;
    }
    
    /**
     * Returns false by default.
     */
    bool isLenient() const {
        return false;
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
        FieldUtils::verifyValueBounds(this, value, getMinimumValue(),
                                     getMaximumValueForSet(instant, value));
        return instant + (value - get(instant)) * iUnitMillis;
    }
    
    /**
     * This method assumes that this field is properly rounded on
     * 1970-01-01T00:00:00. If the rounding alignment differs, override this
     * method as follows:
     * <pre>
     * return super.roundFloor(instant + ALIGNMENT_MILLIS) - ALIGNMENT_MILLIS;
     * </pre>
     */
    int64_t roundFloor(int64_t instant) const {
        if (instant >= 0) {
            return instant - instant % iUnitMillis;
        } else {
            instant += 1;
            return instant - instant % iUnitMillis - iUnitMillis;
        }
    }
    
    /**
     * This method assumes that this field is properly rounded on
     * 1970-01-01T00:00:00. If the rounding alignment differs, override this
     * method as follows:
     * <pre>
     * return super.roundCeiling(instant + ALIGNMENT_MILLIS) - ALIGNMENT_MILLIS;
     * </pre>
     */
    int64_t roundCeiling(int64_t instant) const {
        if (instant > 0) {
            instant -= 1;
            return instant - instant % iUnitMillis + iUnitMillis;
        } else {
            return instant - instant % iUnitMillis;
        }
    }
    
    /**
     * This method assumes that this field is properly rounded on
     * 1970-01-01T00:00:00. If the rounding alignment differs, override this
     * method as follows:
     * <pre>
     * return super.remainder(instant + ALIGNMENT_MILLIS);
     * </pre>
     */
    int64_t remainder(int64_t instant) const {
        if (instant >= 0) {
            return instant % iUnitMillis;
        } else {
            return (instant + 1) % iUnitMillis + iUnitMillis - 1;
        }
    }
    
    /**
     * Returns the duration per unit value of this field. For example, if this
     * field represents "minute of hour", then the duration field is minutes.
     *
     * @return the duration of this field, or UnsupportedDurationField if field
     * has no duration
     */
    const DurationField *getDurationField() const {
        return iUnitField;
    }
    
    /**
     * Get the minimum value for the field.
     *
     * @return the minimum value
     */
    int getMinimumValue() const {
        return 0;
    }
    
    const int64_t getUnitMillis() const {
        return iUnitMillis;
    }
    
protected:
    
    /** The fractional unit in millis */
    int64_t iUnitMillis;
    
    /**
     * Called by the set method to get the maximum allowed value. By default,
     * returns getMaximumValue(instant). Override to provide a faster
     * implementation.
     */
    int getMaximumValueForSet(int64_t instant, int value) const {
        return getMaximumValue(instant);
    }
    
};

CODATIME_END

#endif
