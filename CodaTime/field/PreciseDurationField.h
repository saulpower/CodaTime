//
//  PreciseDurationField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_PreciseDurationField_h
#define CodaTime_PreciseDurationField_h

#include "CodaTimeMacros.h"

#include "field/BaseDurationField.h"
#include "field/FieldUtils.h"

CODATIME_BEGIN

/**
 * Duration field class representing a field with a fixed unit length.
 * <p>
 * PreciseDurationField is thread-safe and immutable.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
class PreciseDurationField : public BaseDurationField {
    
private:
    
    static const long long serialVersionUID = -8346152187724495365L;
    
    /** The size of the unit */
    int64_t iUnitMillis;
    
public:
    
    const DurationFieldType *getType() const {
        return BaseDurationField::getType();
    }
    
    /**
     * Constructor.
     *
     * @param type  the field type
     * @param unitMillis  the unit milliseconds
     */
    PreciseDurationField(const DurationFieldType *type, int64_t unitMillis) : BaseDurationField(type) {
        iUnitMillis = unitMillis;
    }
    
    //------------------------------------------------------------------------
    /**
     * This field is precise.
     *
     * @return true always
     */
    const bool isPrecise() const {
        return true;
    }
    
    /**
     * Returns the amount of milliseconds per unit value of this field.
     *
     * @return the unit size of this field, in milliseconds
     */
    int64_t getUnitMillis() const {
        return iUnitMillis;
    }
    
    //------------------------------------------------------------------------
    /**
     * Get the value of this field from the milliseconds.
     *
     * @param duration  the milliseconds to query, which may be negative
     * @param instant  ignored
     * @return the value of the field, in the units of the field, which may be
     * negative
     */
    int64_t getValueAsLong(int64_t duration, int64_t instant) {
        return duration / iUnitMillis;  // safe
    }
    
    /**
     * Get the millisecond duration of this field from its value.
     *
     * @param value  the value of the field, which may be negative
     * @param instant  ignored
     * @return the milliseconds that the field represents, which may be
     * negative
     */
    int64_t getMillis(int value, int64_t instant) {
        return value * iUnitMillis;  // safe
    }
    
    /**
     * Get the millisecond duration of this field from its value.
     *
     * @param value  the value of the field, which may be negative
     * @param instant  ignored
     * @return the milliseconds that the field represents, which may be
     * negative
     */
    int64_t getMillis(int64_t value, int64_t instant) {
        return FieldUtils::safeMultiply(value, iUnitMillis);
    }
    
    int64_t add(int64_t instant, int value) const {
        int64_t addition = value * iUnitMillis;  // safe
        return FieldUtils::safeAdd(instant, addition);
    }
    
    int64_t add(int64_t instant, int64_t value) const {
        int64_t addition = FieldUtils::safeMultiply(value, iUnitMillis);
        return FieldUtils::safeAdd(instant, addition);
    }
    
    int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const {
        int64_t difference = FieldUtils::safeSubtract(minuendInstant, subtrahendInstant);
        return difference / iUnitMillis;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Compares this duration field to another.
     * Two fields are equal if of the same type and duration.
     *
     * @param obj  the object to compare to
     * @return if equal
     */
    bool equals(const Object *obj) const {
        
        if (this == obj) {
            return true;
        }
        
        const PreciseDurationField *other = dynamic_cast<const PreciseDurationField*>(obj);
        
        if (other != 0) {
            return (getType() == other->getType()) && (iUnitMillis == other->iUnitMillis);
        }
        return false;
    }
    
    /**
     * Gets a hash code for this instance.
     *
     * @return a suitable hashcode
     */
    int hashCode() {
        int64_t millis = iUnitMillis;
        int hash = (int) (millis ^ (millis >> 32));
        hash += getType()->hashCode();
        return hash;
    }
    
};

CODATIME_END

#endif
