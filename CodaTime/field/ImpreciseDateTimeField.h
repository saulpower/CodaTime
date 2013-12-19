//
//  ImpreciseDateTimeField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_ImpreciseDateTimeField_h
#define CodaTime_ImpreciseDateTimeField_h

#include "CodaTimeMacros.h"

#include "field/BaseDateTimeField.h"
#include "field/BaseDurationField.h"

CODATIME_BEGIN

/**
 * virtual datetime field class that defines its own DurationField, which
 * delegates back into this ImpreciseDateTimeField.
 * <p>
 * This DateTimeField is useful for defining DateTimeFields that are composed
 * of imprecise durations. If both duration fields are precise, then a
 * {@link PreciseDateTimeField} should be used instead.
 * <p>
 * When defining imprecise DateTimeFields where a matching DurationField is
 * already available, just extend BaseDateTimeField directly so as not to
 * create redundant DurationField instances.
 * <p>
 * ImpreciseDateTimeField is thread-safe and immutable, and its subclasses must
 * be as well.
 *
 * @author Brian S O'Neill
 * @see PreciseDateTimeField
 * @since 1.0
 */
class ImpreciseDateTimeField : public BaseDateTimeField {
    
private:
    
    int64_t iUnitMillis;
    const DurationField *iDurationField;
    
    class LinkedDurationField : public BaseDurationField {
        
    private:
        
        ImpreciseDateTimeField *impreciseDateTimeField;
        
    public:
        
        LinkedDurationField(const DurationFieldType *type, ImpreciseDateTimeField *impreciseDateTimeField) : BaseDurationField(type), impreciseDateTimeField(impreciseDateTimeField) {
        }
        
        const bool isPrecise() const {
            return false;
        }
        
        int64_t getUnitMillis() const {
            return impreciseDateTimeField->iUnitMillis;
        }
        
        int getValue(int64_t duration, int64_t instant) {
            return impreciseDateTimeField->getDifference(instant + duration, instant);
        }
        
        int64_t getValueAsLong(int64_t duration, int64_t instant) {
            return impreciseDateTimeField->getDifferenceAsLong(instant + duration, instant);
        }
        
        int64_t getMillis(int value, int64_t instant) {
            return impreciseDateTimeField->add(instant, value) - instant;
        }
        
        int64_t getMillis(int64_t value, int64_t instant) {
            return impreciseDateTimeField->add(instant, value) - instant;
        }
        
        int64_t add(int64_t instant, int value) const {
            return impreciseDateTimeField->add(instant, value);
        }
        
        int64_t add(int64_t instant, int64_t value) const {
            return impreciseDateTimeField->add(instant, value);
        }
        
        int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const {
            return impreciseDateTimeField->getDifference(minuendInstant, subtrahendInstant);
        }
        
        int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const {
            return impreciseDateTimeField->getDifferenceAsLong(minuendInstant, subtrahendInstant);
        }
    };
    
public:
    
    /**
     * Constructor.
     *
     * @param type  the field type
     * @param unitMillis  the average duration unit milliseconds
     */
    ImpreciseDateTimeField(const DateTimeFieldType *type, int64_t unitMillis) : BaseDateTimeField(type) {
        iUnitMillis = unitMillis;
        iDurationField = new LinkedDurationField(type->getDurationType(), this);
    }
    
    virtual int get(int64_t instant) const = 0;
    
    virtual int64_t set(int64_t instant, int value) const = 0;
    
    virtual int64_t add(int64_t instant, int value) const = 0;
    
    virtual int64_t add(int64_t instant, int64_t value) const = 0;
    
    /**
     * Computes the difference between two instants, as measured in the units
     * of this field. Any fractional units are dropped from the result. Calling
     * getDifference reverses the effect of calling add. In the following code:
     *
     * <pre>
     * int64_t instant = ...
     * int v = ...
     * int age = getDifference(add(instant, v), instant);
     * </pre>
     *
     * The value 'age' is the same as the value 'v'.
     * <p>
     * The default implementation call getDifferenceAsint64_t and converts the
     * return value to an int.
     *
     * @param minuendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract from
     * @param subtrahendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract off the minuend
     * @return the difference in the units of this field
     */
    int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return FieldUtils::safeToInt(getDifferenceAsLong(minuendInstant, subtrahendInstant));
    }
    
    /**
     * Computes the difference between two instants, as measured in the units
     * of this field. Any fractional units are dropped from the result. Calling
     * getDifference reverses the effect of calling add. In the following code:
     *
     * <pre>
     * int64_t instant = ...
     * int64_t v = ...
     * int64_t age = getDifferenceAsLong(add(instant, v), instant);
     * </pre>
     *
     * The value 'age' is the same as the value 'v'.
     * <p>
     * The default implementation performs a guess-and-check algorithm using
     * getDurationField().getUnitMillis() and the add() method. Subclasses are
     * encouraged to provide a more efficient implementation.
     *
     * @param minuendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract from
     * @param subtrahendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract off the minuend
     * @return the difference in the units of this field
     */
    int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const {
        if (minuendInstant < subtrahendInstant) {
            return -getDifferenceAsLong(subtrahendInstant, minuendInstant);
        }
        
        int64_t difference = (minuendInstant - subtrahendInstant) / iUnitMillis;
        if (add(subtrahendInstant, difference) < minuendInstant) {
            do {
                difference++;
            } while (add(subtrahendInstant, difference) <= minuendInstant);
            difference--;
        } else if (add(subtrahendInstant, difference) > minuendInstant) {
            do {
                difference--;
            } while (add(subtrahendInstant, difference) > minuendInstant);
        }
        return difference;
    }
    
    const DurationField *getDurationField() const {
        return iDurationField;
    }
    
    virtual const DurationField *getRangeDurationField() const = 0;
    
    virtual int64_t roundFloor(int64_t instant) const = 0;
    
protected:
    
    int64_t getDurationUnitMillis() const {
        return iUnitMillis;
    }
    
};

CODATIME_END

#endif
