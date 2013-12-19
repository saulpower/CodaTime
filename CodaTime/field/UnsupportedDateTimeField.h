//
//  UnsupportedDateTimeField.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_UnsupportedDateTimeField_h
#define CodaTime_UnsupportedDateTimeField_h

#include "CodaTimeMacros.h"

#include "DateTimeField.h"
#include "DateTimeFieldType.h"
#include "DurationField.h"
#include "Exceptions.h"
#include "Locale.h"

#include <map>
#include <vector>

using namespace std;

CODATIME_BEGIN

class UnsupportedDateTimeField : public DateTimeField {
    
private:
    
    /** Serialilzation version */
    static const long long serialVersionUID = -1934618396111902255L;
    
    /** The cache of unsupported datetime field instances */
    static map<const DateTimeFieldType*, UnsupportedDateTimeField*> cCache;
    
    /** The field type */
    const DateTimeFieldType *iType;
    /** The duration of the datetime field */
    const DurationField *iDurationField;
    
    /**
     * Constructor.
     *
     * @param type  the field type
     * @param durationField  the duration to use
     */
    UnsupportedDateTimeField(const DateTimeFieldType *type, const DurationField *durationField) {
        if (type == NULL || durationField == NULL) {
            throw IllegalArgumentException();
        }
        iType = type;
        iDurationField = durationField;
    }
    
    /**
     * Ensure proper singleton serialization
     */
//    void *readResolve() {
//        return getInstance(iType, iDurationField);
//    }
    
    UnsupportedOperationException unsupported() const {
        string err(iType->getName());
        err.append(" field is unsupported");
        return UnsupportedOperationException(err);
    }
    
public:
    
    /**
     * Gets an instance of UnsupportedDateTimeField for a specific named field.
     * Names should be of standard format, such as 'monthOfYear' or 'hourOfDay'.
     * The returned instance is cached.
     *
     * @param type  the type to obtain
     * @return the instance
     * @throws IllegalArgumentException if durationField is NULL
     */
    // TODO: implement synchronized
    static UnsupportedDateTimeField *getInstance(const DateTimeFieldType *type, const DurationField *durationField) {
        
        UnsupportedDateTimeField *field = cCache[type];
        if (field != NULL && field->getDurationField() != durationField) {
            field = NULL;
        }
        if (field == NULL) {
            field = new UnsupportedDateTimeField(type, durationField);
            cCache[type] = field;
        }
        return field;
    }
    
    //-----------------------------------------------------------------------
    // Design note: Simple accessors return a suitable value, but methods
    // intended to perform calculations throw an UnsupportedOperationException.
    
    const DateTimeFieldType *getType() const {
        return iType;
    }
    
    string getName() const {
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
     * This field is not lenient.
     *
     * @return false always
     */
    bool isLenient() const {
        return false;
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int get(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsText(int64_t instant, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsText(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsText(ReadablePartial *partial, int fieldValue, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsText(ReadablePartial *partial, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsText(int fieldValue, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsShortText(int64_t instant, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsShortText(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsShortText(ReadablePartial *partial, int fieldValue, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsShortText(ReadablePartial *partial, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    string getAsShortText(int fieldValue, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Delegates to the duration field.
     *
     * @throws UnsupportedOperationException if the duration is unsupported
     */
    int64_t add(int64_t instant, int value) const {
        return getDurationField()->add(instant, value);
    }
    
    /**
     * Delegates to the duration field.
     *
     * @throws UnsupportedOperationException if the duration is unsupported
     */
    int64_t add(int64_t instant, int64_t value) const {
        return getDurationField()->add(instant, value);
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    vector<int> add(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    vector<int> addWrapPartial(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t addWrapField(int64_t instant, int value) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    vector<int> addWrapField(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const {
        throw unsupported();
    }
    
    /**
     * Delegates to the duration field.
     *
     * @throws UnsupportedOperationException if the duration is unsupported
     */
    int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return getDurationField()->getDifference(minuendInstant, subtrahendInstant);
    }
    
    /**
     * Delegates to the duration field.
     *
     * @throws UnsupportedOperationException if the duration is unsupported
     */
    int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return getDurationField()->getDifferenceAsLong(minuendInstant, subtrahendInstant);
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t set(int64_t instant, int value) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    vector<int> set(ReadablePartial *instant, int fieldIndex, vector<int> values, int newValue) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t set(int64_t instant, string text, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t set(int64_t instant, string text) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    vector<int> set(ReadablePartial *instant, int fieldIndex, vector<int> values, string text, Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Even though this DateTimeField is unsupported, the duration field might
     * be supported.
     *
     * @return a possibly supported DurationField
     */
    const DurationField *getDurationField() const {
        return iDurationField;
    }
    
    /**
     * Always returns NULL.
     *
     * @return NULL always
     */
    const DurationField *getRangeDurationField() const {
        return NULL;
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    bool isLeap(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getLeapAmount(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always returns NULL.
     *
     * @return NULL always
     */
    DurationField *getLeapDurationField() const {
        return NULL;
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMinimumValue() const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMinimumValue(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMinimumValue(ReadablePartial *instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMinimumValue(ReadablePartial *instant, vector<int> values) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMaximumValue() const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMaximumValue(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMaximumValue(ReadablePartial *instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMaximumValue(ReadablePartial *instant, vector<int> values) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMaximumTextLength(Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int getMaximumShortTextLength(Locale *locale) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t roundFloor(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t roundCeiling(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t roundHalfFloor(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t roundHalfCeiling(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t roundHalfEven(int64_t instant) const {
        throw unsupported();
    }
    
    /**
     * Always throws UnsupportedOperationException
     *
     * @throws UnsupportedOperationException
     */
    int64_t remainder(int64_t instant) const {
        throw unsupported();
    }
    
    //------------------------------------------------------------------------
    /**
     * Get a suitable debug string.
     *
     * @return debug string
     */
    string toString() const {
        return "UnsupportedDateTimeField";
    }
    
};

CODATIME_END

#endif
