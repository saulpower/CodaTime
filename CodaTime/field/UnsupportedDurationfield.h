//
//  UnsupportedDurationField.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_UnsupportedDurationField_h
#define CodaTime_UnsupportedDurationField_h

#include "CodaTimeMacros.h"

#include "DurationField.h"
#include "DurationFieldType.h"
#include "Exceptions.h"

#include <map>

using namespace std;

CODATIME_BEGIN

class UnsupportedDurationfield : public DurationField {
    
private:
    
    /** Serialization lock. */
    static const long long serialVersionUID = -6390301302770925357L;
    
    /** The cache of unsupported duration field instances */
    static map<const DurationFieldType*, UnsupportedDurationfield*> cCache;
    
    /**
     * Ensure proper singleton serialization
     */
    const Object *readResolve() {
        return getInstance(iType);
    }
    
    UnsupportedOperationException unsupported() const {
        string err(iType->getName());
        err.append(" field is unsupported");
        return UnsupportedOperationException(err);
    }
    
    /** The name of the field */
    const DurationFieldType *iType;
    
    /**
     * Constructor.
     *
     * @param type  the type to use
     */
    UnsupportedDurationfield(const DurationFieldType *type) {
        iType = type;
    }
    
public:
    
    /**
     * Gets an instance of UnsupportedDurationField for a specific named field.
     * The returned instance is cached.
     *
     * @param type  the type to obtain
     * @return the instance
     */
    // TODO: synchronized
    static UnsupportedDurationfield *getInstance(const DurationFieldType *type) {
        UnsupportedDurationfield *field = cCache[type];
        if (field == NULL) {
            field = new UnsupportedDurationfield(type);
            cCache[type] = field;
        }
        return field;
    }
    
    //-----------------------------------------------------------------------
    // Design note: Simple Accessors return a suitable value, but methods
    // intended to perform calculations throw an UnsupportedOperationException.
    
    const DurationFieldType *getType() const {
        return iType;
    }
    
    const string getName() const {
        return iType->getName();
    }
    
    /**
     * This field is not supported.
     *
     * @return false always
     */
    bool isSupported() const {
        return false;
    }
    
    /**
     * This field is precise.
     *
     * @return true always
     */
    const bool isPrecise() const {
        return true;
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getValue(int64_t duration) {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t getValueAsLong(int64_t duration) {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getValue(int64_t duration, int64_t instant) {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t getValueAsLong(int64_t duration, int64_t instant) {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t getMillis(int value) {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t getMillis(int64_t value) {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t getMillis(int value, int64_t instant) {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t getMillis(int64_t value, int64_t instant) {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t add(int64_t instant, int value) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t add(int64_t instant, int64_t value) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const {
        throw unsupported();
    }
    
    /**
     * Always returns zero.
     *
     * @return zero always
     */
    int64_t getUnitMillis() const {
        return 0;
    }
    
    /**
     * Always returns zero, indicating that sort order is not relevent.
     *
     * @return zero always
     */
    int compareTo(const DurationField *durationField) const {
        return 0;
    }
    
    //------------------------------------------------------------------------
    /**
     * Compares this duration field to another.
     *
     * @param obj  the object to compare to
     * @return true if equal
     */
    // TODO: implement equals
    bool equals(const Object *obj) const {
        if (this == obj) {
            return true;
        }
        
        const UnsupportedDurationfield *other = dynamic_cast<const UnsupportedDurationfield*>(obj);
        
        if (other != 0) {
            if (other->getName().empty()) {
                return true;
            }
            return (other->getName().compare(getName()) == 0);
        }
        return false;
    }
    
    /**
     * Gets a suitable hashcode.
     *
     * @return the hashcode
     */
    // TODO: implement hash function
    int hashCode() {
        return 0;//getName()->hashCode();
    }
    
    /**
     * Get a suitable debug string.
     *
     * @return debug string
     */
    string toString() {
        string str("UnsupportedDurationField[");
        str.append(getName());
        str.append("]");
        return str;
    }
    
};

CODATIME_END

#endif
