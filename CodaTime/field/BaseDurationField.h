//
//  BaseDurationField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BaseDurationField_h
#define CodaTime_BaseDurationField_h

#include "CodaTimeMacros.h"

#include "DurationField.h"
#include "DurationFieldType.h"
#include "Exceptions.h"
#include "field/FieldUtils.h"

CODATIME_BEGIN

/**
 * BaseDurationField provides the common behaviour for DurationField
 * implementations.
 * <p>
 * This class should generally not be used directly by API users. The
 * DurationField class should be used when different kinds of DurationField
 * objects are to be referenced.
 * <p>
 * BaseDurationField is thread-safe and immutable, and its subclasses must
 * be as well.
 *
 * @author Brian S O'Neill
 * @see DecoratedDurationField
 * @since 1.0
 */
class BaseDurationField : public DurationField {
    
private:
    
    /** Serialization lock. */
    static const long long serialVersionUID = -2554245107589433218L;
    
    /** A desriptive name for the field. */
    const DurationFieldType *iType;
    
protected:
    
    BaseDurationField(const DurationFieldType *type) : DurationField() {
        if (type == NULL) {
            throw IllegalArgumentException("The type must not be NULL");
        }
        iType = type;
    }
    
public:
    
    const DurationFieldType *getType() const {
        return iType;
    }
    
    const string getName() const {
        return iType->getName();
    }
    
    /**
     * @return true always
     */
    bool isSupported() const {
        return true;
    }
    
    //------------------------------------------------------------------------
    /**
     * Get the value of this field from the milliseconds, which is approximate
     * if this field is imprecise.
     *
     * @param duration  the milliseconds to query, which may be negative
     * @return the value of the field, in the units of the field, which may be
     * negative
     */
    int getValue(int64_t duration) {
        return FieldUtils::safeToInt(getValueAsLong(duration));
    }
    
    /**
     * Get the value of this field from the milliseconds, which is approximate
     * if this field is imprecise.
     *
     * @param duration  the milliseconds to query, which may be negative
     * @return the value of the field, in the units of the field, which may be
     * negative
     */
    int64_t getValueAsLong(int64_t duration) {
        return duration / getUnitMillis();
    }
    
    virtual int64_t getValueAsLong(int64_t duration, int64_t instant) = 0;
    
    /**
     * Get the value of this field from the milliseconds relative to an
     * instant.
     *
     * <p>If the milliseconds is positive, then the instant is treated as a
     * "start instant". If negative, the instant is treated as an "end
     * instant".
     *
     * <p>The default implementation returns
     * <code>Utils.safeToInt(getAsLong(millisDuration, instant))</code>.
     *
     * @param duration  the milliseconds to query, which may be negative
     * @param instant  the start instant to calculate relative to
     * @return the value of the field, in the units of the field, which may be
     * negative
     */
    int getValue(int64_t duration, int64_t instant) {
        return FieldUtils::safeToInt(getValueAsLong(duration, instant));
    }
    
    /**
     * Get the millisecond duration of this field from its value, which is
     * approximate if this field is imprecise.
     *
     * @param value  the value of the field, which may be negative
     * @return the milliseconds that the field represents, which may be
     * negative
     */
    int64_t getMillis(int value) {
        return value * getUnitMillis();  // safe
    }
    
    /**
     * Get the millisecond duration of this field from its value, which is
     * approximate if this field is imprecise.
     *
     * @param value  the value of the field, which may be negative
     * @return the milliseconds that the field represents, which may be
     * negative
     */
    int64_t getMillis(int64_t value) {
        return FieldUtils::safeMultiply(value, getUnitMillis());
    }
    
    // Calculation API
    //------------------------------------------------------------------------
    int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return FieldUtils::safeToInt(getDifferenceAsLong(minuendInstant, subtrahendInstant));
    }
    
    //------------------------------------------------------------------------
    int compareTo(const DurationField *otherField) const {
        int64_t otherMillis = otherField->getUnitMillis();
        int64_t thisMillis = getUnitMillis();
        // cannot do (thisMillis - otherMillis) as can overflow
        if (thisMillis == otherMillis) {
            return 0;
        }
        if (thisMillis < otherMillis) {
            return -1;
        } else {
            return 1;
        }
    }
    
    /**
     * Get a suitable debug string.
     *
     * @return debug string
     */
    string toString() {
        string str("DurationField[");
        str.append(getName());
        str.append("]");
        return str;
    }
    
};

CODATIME_END

#endif
