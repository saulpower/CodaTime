//
//  MillisDurationField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_MillisDurationField_h
#define CodaTime_MillisDurationField_h

#include "CodaTimeMacros.h"

#include "DurationFieldType.h"
#include "field/FieldUtils.h"

#include <string>

using namespace std;

CODATIME_BEGIN

/**
 * Duration field class representing a field with a fixed unit length of one
 * millisecond.
 * <p>
 * MillisDurationField is thread-safe and immutable.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class MillisDurationField : public DurationField {
    
private:
    
    /** Serialization lock. */
    static const long long serialVersionUID = 2656707858124633367L;
    
    /**
     * Restricted constructor.
     */
    MillisDurationField() : DurationField() {
    }
    
    /**
     * Deserialize to the singleton.
     */
    Object *readResolve() {
        return INSTANCE;
    }
    
public:
    
    /** Singleton instance. */
    static DurationField *INSTANCE;
    
    //------------------------------------------------------------------------
    const DurationFieldType *getType() const {
        return DurationFieldType::millis();
    }
    
    const string getName() const {
        return string("millis");
    }
    
    /**
     * Returns true as this field is supported.
     *
     * @return true always
     */
    bool isSupported() const {
        return true;
    }
    
    /**
     * Returns true as this field is precise.
     *
     * @return true always
     */
    const bool isPrecise() const {
        return true;
    }
    
    /**
     * Returns the amount of milliseconds per unit value of this field.
     *
     * @return one always
     */
    int64_t getUnitMillis() const {
        return 1;
    }
    
    //------------------------------------------------------------------------
    int getValue(int64_t duration) {
        return FieldUtils::safeToInt(duration);
    }
    
    int64_t getValueAsLong(int64_t duration) {
        return duration;
    }
    
    int getValue(int64_t duration, int64_t instant) {
        return FieldUtils::safeToInt(duration);
    }
    
    int64_t getValueAsLong(int64_t duration, int64_t instant) {
        return duration;
    }
    
    int64_t getMillis(int value) {
        return value;
    }
    
    int64_t getMillis(int64_t value) {
        return value;
    }
    
    int64_t getMillis(int value, int64_t instant) {
        return value;
    }
    
    int64_t getMillis(int64_t value, int64_t instant) {
        return value;
    }
    
    int64_t add(int64_t instant, int value) const {
        return FieldUtils::safeAdd(instant, (int64_t) value);
    }
    
    int64_t add(int64_t instant, int64_t value) const {
        return FieldUtils::safeAdd(instant, value);
    }
    
    int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return FieldUtils::safeToInt(FieldUtils::safeSubtract(minuendInstant, subtrahendInstant));
    }
    
    int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return FieldUtils::safeSubtract(minuendInstant, subtrahendInstant);
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
    
    bool equals(const Object *obj) const {
        const MillisDurationField* field = dynamic_cast<const MillisDurationField*>(obj);
        if (field != 0) {
            return getUnitMillis() == field->getUnitMillis();
        }
        return false;
    }
    
    int hashCode() {
        return (int) getUnitMillis();
    }
    
    /**
     * Get a suitable debug string.
     *
     * @return debug string
     */
    string toString() {
        return string("DurationField[millis]");
    }
    
};

DurationField *MillisDurationField::INSTANCE = new MillisDurationField();

CODATIME_END

#endif
