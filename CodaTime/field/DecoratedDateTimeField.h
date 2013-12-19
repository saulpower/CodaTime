//
//  DecoratedDateTimeField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_DecoratedDateTimeField_h
#define CodaTime_DecoratedDateTimeField_h

#include "CodaTimeMacros.h"

#include "DateTimeField.h"
#include "Exceptions.h"
#include "field/BaseDateTimeField.h"

CODATIME_BEGIN

/**
 * <code>DecoratedDateTimeField</code> extends {@link BaseDateTimeField},
 * implementing only the minimum required set of methods. These implemented
 * methods delegate to a wrapped field.
 * <p>
 * This design allows new DateTimeField types to be defined that piggyback on
 * top of another, inheriting all the safe method implementations from
 * BaseDateTimeField. Should any method require pure delegation to the
 * wrapped field, simply override and use the provided getWrappedField method.
 * <p>
 * DecoratedDateTimeField is thread-safe and immutable, and its subclasses must
 * be as well.
 *
 * @author Brian S O'Neill
 * @since 1.0
 * @see DelegatedDateTimeField
 */
class DecoratedDateTimeField : public BaseDateTimeField {
    
private:
    
    /** The DateTimeField being wrapped */
    const DateTimeField *iField;
    
protected:
    
    /**
     * Constructor.
     *
     * @param field  the field being decorated
     * @param type  allow type to be overridden
     */
    DecoratedDateTimeField(const DateTimeField *field, const DateTimeFieldType *type) : BaseDateTimeField(type) {

        if (field == NULL) {
            throw IllegalArgumentException("The field must not be NULL");
        }
        if (!field->isSupported()) {
            throw IllegalArgumentException("The field must be supported");
        }
        iField = field;
    }
    
public:
    
    virtual int get(int64_t instant) const = 0;
    
    /**
     * Gets the wrapped date time field.
     *
     * @return the wrapped DateTimeField
     */
    const DateTimeField *getWrappedField() const {
        return iField;
    }
    
    bool isLenient() const {
        return iField->isLenient();
    }
    
    int get(int64_t instant) {
        return iField->get(instant);
    }
    
    int64_t set(int64_t instant, int value) {
        return iField->set(instant, value);
    }
    
    const DurationField *getDurationField() const {
        return iField->getDurationField();
    }
    
    const DurationField *getRangeDurationField() const {
        return iField->getRangeDurationField();
    }
    
    int getMinimumValue() const {
        return iField->getMinimumValue();
    }
    
    int getMaximumValue() const {
        return iField->getMaximumValue();
    }
    
    int64_t roundFloor(int64_t instant) const {
        return iField->roundFloor(instant);
    }
    
};

CODATIME_END

#endif
