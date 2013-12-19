//
//  BaseDateTimeField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BaseDateTimeField_h
#define CodaTime_BaseDateTimeField_h

#include "CodaTimeMacros.h"

#include "DateTimeField.h"
#include "DateTimeFieldType.h"
#include "Exceptions.h"
#include "field/FieldUtils.h"
#include "Readablepartial.h"

#include <vector>
#include <string>
#include <stdlib.h>

using namespace std;

CODATIME_BEGIN

class Locale;

/**
 * BaseDateTimeField provides the common behaviour for DateTimeField
 * implementations.
 * <p>
 * This class should generally not be used directly by API users. The
 * DateTimeField class should be used when different kinds of DateTimeField
 * objects are to be referenced.
 * <p>
 * BaseDateTimeField is thread-safe and immutable, and its subclasses must
 * be as well.
 *
 * @author Brian S O'Neill
 * @since 1.0
 * @see DecoratedDateTimeField
 */
class BaseDateTimeField : public DateTimeField {
    
private:
    
    /** The field type. */
    const DateTimeFieldType *iType;
    
protected:
    
    /**
     * Constructor.
     */
    BaseDateTimeField(const DateTimeFieldType *type) : DateTimeField() {
        if (type == NULL) {
            throw IllegalArgumentException("The type must not be NULL");
        }
        iType = type;
    }
    
    /**
     * Convert the specified text and locale into a value.
     *
     * @param text  the text to convert
     * @param locale  the locale to convert using
     * @return the value extracted from the text
     * @throws IllegalArgumentException if the text is invalid
     */
    int convertText(string text, Locale *locale) const {
        return atoi(text.c_str());
    }
    
public:
    
    const DateTimeFieldType *getType() const {
        return iType;
    }
    
    string getName() const {
        return iType->getName();
    }
    
    /**
     * @return true always
     */
    bool isSupported() const {
        return true;
    }
    
    // Main access API
    //------------------------------------------------------------------------
    /**
     * Get the value of this field from the milliseconds.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the value of the field, in the units of the field
     */
    virtual int get(int64_t instant) const = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Get the human-readable, text value of this field from the milliseconds.
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * The default implementation returns getAsText(get(instant), locale).
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @param locale the locale to use for selecting a text symbol, NULL means default
     * @return the text value of the field
     */
    string getAsText(int64_t instant, Locale *locale) const {
        return getAsText(get(instant), locale);
    }
    
    /**
     * Get the human-readable, text value of this field from the milliseconds.
     * <p>
     * The default implementation calls {@link #getAsText(long, Locale)}.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the text value of the field
     */
    string getAsText(int64_t instant) const {
        return getAsText(instant, NULL);
    }
    
    /**
     * Get the human-readable, text value of this field from a partial instant->
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * The default implementation returns getAsText(fieldValue, locale).
     *
     * @param partial  the partial instant to query
     * @param fieldValue  the field value of this field, provided for performance
     * @param locale  the locale to use for selecting a text symbol, NULL for default
     * @return the text value of the field
     */
    string getAsText(ReadablePartial *partial, int fieldValue, Locale *locale) const {
        return getAsText(fieldValue, locale);
    }
    
    /**
     * Get the human-readable, text value of this field from a partial instant->
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * The default implementation calls {@link ReadablePartial#get(DateTimeFieldType)}
     * and {@link #getAsText(ReadablePartial, int, Locale)}.
     *
     * @param partial  the partial instant to query
     * @param locale  the locale to use for selecting a text symbol, NULL for default
     * @return the text value of the field
     */
    string getAsText(ReadablePartial *partial, Locale *locale) const {
        return getAsText(partial, partial->get(getType()), locale);
    }
    
    /**
     * Get the human-readable, text value of this field from the field value.
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * The default implementation returns Integer.toString(get(instant)).
     * <p>
     * Note: subclasses that override this method should also override
     * getMaximumTextLength.
     *
     * @param fieldValue  the numeric value to convert to text
     * @param locale the locale to use for selecting a text symbol, NULL for default
     * @return the text value of the field
     */
    string getAsText(int fieldValue, Locale *locale) const {
        return to_string(fieldValue);
    }
    
    //-----------------------------------------------------------------------
    /**
     * Get the human-readable, short text value of this field from the milliseconds.
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * The default implementation returns getAsShortText(get(instant), locale).
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @param locale the locale to use for selecting a text symbol, NULL means default
     * @return the text value of the field
     */
    string getAsShortText(int64_t instant, Locale *locale) const {
        return getAsShortText(get(instant), locale);
    }
    
    /**
     * Get the human-readable, short text value of this field from the milliseconds.
     * <p>
     * The default implementation calls {@link #getAsShortText(long, Locale)}.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the text value of the field
     */
    string getAsShortText(int64_t instant) const {
        return getAsShortText(instant, NULL);
    }
    
    /**
     * Get the human-readable, short text value of this field from a partial instant->
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * The default implementation returns getAsShortText(fieldValue, locale).
     *
     * @param partial  the partial instant to query
     * @param fieldValue  the field value of this field, provided for performance
     * @param locale  the locale to use for selecting a text symbol, NULL for default
     * @return the text value of the field
     */
    string getAsShortText(ReadablePartial *partial, int fieldValue, Locale *locale) const {
        return getAsShortText(fieldValue, locale);
    }
    
    /**
     * Get the human-readable, short text value of this field from a partial instant->
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * The default implementation calls {@link ReadablePartial#get(DateTimeFieldType)}
     * and {@link #getAsText(ReadablePartial, int, Locale)}.
     *
     * @param partial  the partial instant to query
     * @param locale  the locale to use for selecting a text symbol, NULL for default
     * @return the text value of the field
     */
    string getAsShortText(ReadablePartial *partial, Locale *locale) const {
        return getAsShortText(partial, partial->get(getType()), locale);
    }
    
    /**
     * Get the human-readable, short text value of this field from the field value.
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * The default implementation returns getAsText(fieldValue, locale).
     * <p>
     * Note: subclasses that override this method should also override
     * getMaximumShortTextLength.
     *
     * @param fieldValue  the numeric value to convert to text
     * @param locale the locale to use for selecting a text symbol, NULL for default
     * @return the text value of the field
     */
    string getAsShortText(int fieldValue, Locale *locale) const {
        return getAsText(fieldValue, locale);
    }
    
    //-----------------------------------------------------------------------
    /**
     * Adds a value (which may be negative) to the instant value,
     * overflowing into larger fields if necessary.
     * <p>
     * The value will be added to this field. If the value is too large to be
     * added solely to this field, larger fields will increase as required.
     * Smaller fields should be unaffected, except where the result would be
     * an invalid value for a smaller field. In this case the smaller field is
     * adjusted to be in range.
     * <p>
     * For example, in the ISO chronology:<br>
     * 2000-08-20 add six months is 2001-02-20<br>
     * 2000-08-20 add twenty months is 2002-04-20<br>
     * 2000-08-20 add minus nine months is 1999-11-20<br>
     * 2001-01-31 add one month  is 2001-02-28<br>
     * 2001-01-31 add two months is 2001-03-31<br>
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to add to
     * @param value  the value to add, in the units of the field
     * @return the updated milliseconds
     */
    int64_t add(int64_t instant, int value) const {
        return getDurationField()->add(instant, value);
    }
    
    /**
     * Adds a value (which may be negative) to the instant value,
     * overflowing into larger fields if necessary.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to add to
     * @param value  the int64_t value to add, in the units of the field
     * @return the updated milliseconds
     * @throws IllegalArgumentException if value is too large
     * @see #add(long,int)
     */
    int64_t add(int64_t instant, int64_t value) const {
        return getDurationField()->add(instant, value);
    }
    
    /**
     * Adds a value (which may be negative) to the partial instant,
     * throwing an exception if the maximum size of the instant is reached.
     * <p>
     * The value will be added to this field, overflowing into larger fields
     * if necessary. Smaller fields should be unaffected, except where the
     * result would be an invalid value for a smaller field. In this case the
     * smaller field is adjusted to be in range.
     * <p>
     * Partial instants only contain some fields. This may result in a maximum
     * possible value, such as TimeOfDay being limited to 23:59:59:999. If this
     * limit is breached by the add an exception is thrown.
     * <p>
     * For example, in the ISO chronology:<br>
     * 2000-08-20 add six months is 2000-02-20<br>
     * 2000-08-20 add twenty months is 2000-04-20<br>
     * 2000-08-20 add minus nine months is 2000-11-20<br>
     * 2001-01-31 add one month  is 2001-02-28<br>
     * 2001-01-31 add two months is 2001-03-31<br>
     *
     * @param instant  the partial instant
     * @param fieldIndex  the index of this field in the partial
     * @param values  the values of the partial instant which should be updated
     * @param valueToAdd  the value to add, in the units of the field
     * @return the passed in values
     * @throws IllegalArgumentException if the value is invalid or the maximum instant is reached
     */
    vector<int> add(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const {
        if (valueToAdd == 0) {
            return values;
        }
        // there are more efficient algorithms than this (especially for time only fields)
        // trouble is when dealing with days and months, so we use this technique of
        // adding/removing one from the larger field at a time
        const DateTimeField *nextField = NULL;
        
        while (valueToAdd > 0) {
            int max = getMaximumValue(instant, values);
            int64_t proposed = values[fieldIndex] + valueToAdd;
            if (proposed <= max) {
                values[fieldIndex] = (int) proposed;
                break;
            }
            if (nextField == NULL) {
                if (fieldIndex == 0) {
                    throw IllegalArgumentException("Maximum value exceeded for add");
                }
                nextField = instant->getField(fieldIndex - 1);
                // test only works if this field is UTC (ie. local)
                if (getRangeDurationField()->getType() != nextField->getDurationField()->getType()) {
                    throw IllegalArgumentException("Fields invalid for add");
                }
            }
            valueToAdd -= (max + 1) - values[fieldIndex];  // reduce the amount to add
            values = nextField->add(instant, fieldIndex - 1, values, 1);  // add 1 to next bigger field
            values[fieldIndex] = getMinimumValue(instant, values);  // reset this field to zero
        }
        while (valueToAdd < 0) {
            int min = getMinimumValue(instant, values);
            int64_t proposed = values[fieldIndex] + valueToAdd;
            if (proposed >= min) {
                values[fieldIndex] = (int) proposed;
                break;
            }
            if (nextField == NULL) {
                if (fieldIndex == 0) {
                    throw IllegalArgumentException("Maximum value exceeded for add");
                }
                nextField = instant->getField(fieldIndex - 1);
                if (getRangeDurationField()->getType() != nextField->getDurationField()->getType()) {
                    throw IllegalArgumentException("Fields invalid for add");
                }
            }
            valueToAdd -= (min - 1) - values[fieldIndex];  // reduce the amount to add
            values = nextField->add(instant, fieldIndex - 1, values, -1);  // subtract 1 from next bigger field
            values[fieldIndex] = getMaximumValue(instant, values);  // reset this field to max value
        }
        
        return set(instant, fieldIndex, values, values[fieldIndex]);  // adjusts smaller fields
    }
    
    /**
     * Adds a value (which may be negative) to the partial instant,
     * wrapping the whole partial if the maximum size of the partial is reached.
     * <p>
     * The value will be added to this field, overflowing into larger fields
     * if necessary. Smaller fields should be unaffected, except where the
     * result would be an invalid value for a smaller field. In this case the
     * smaller field is adjusted to be in range.
     * <p>
     * Partial instants only contain some fields. This may result in a maximum
     * possible value, such as TimeOfDay normally being limited to 23:59:59:999.
     * If ths limit is reached by the addition, this method will wrap back to
     * 00:00:00.000. In fact, you would generally only use this method for
     * classes that have a limitation such as this.
     * <p>
     * For example, in the ISO chronology:<br>
     * 10:20:30 add 20 minutes is 10:40:30<br>
     * 10:20:30 add 45 minutes is 11:05:30<br>
     * 10:20:30 add 16 hours is 02:20:30<br>
     *
     * @param instant  the partial instant
     * @param fieldIndex  the index of this field in the partial
     * @param values  the values of the partial instant which should be updated
     * @param valueToAdd  the value to add, in the units of the field
     * @return the passed in values
     * @throws IllegalArgumentException if the value is invalid or the maximum instant is reached
     */
    vector<int> addWrapPartial(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const {
        if (valueToAdd == 0) {
            return values;
        }
        // there are more efficient algorithms than this (especially for time only fields)
        // trouble is when dealing with days and months, so we use this technique of
        // adding/removing one from the larger field at a time
        const DateTimeField *nextField = NULL;
        
        while (valueToAdd > 0) {
            int max = getMaximumValue(instant, values);
            int64_t proposed = values[fieldIndex] + valueToAdd;
            if (proposed <= max) {
                values[fieldIndex] = (int) proposed;
                break;
            }
            if (nextField == NULL) {
                if (fieldIndex == 0) {
                    valueToAdd -= (max + 1) - values[fieldIndex];
                    values[fieldIndex] = getMinimumValue(instant, values);
                    continue;
                }
                nextField = instant->getField(fieldIndex - 1);
                // test only works if this field is UTC (ie. local)
                if (getRangeDurationField()->getType() != nextField->getDurationField()->getType()) {
                    throw IllegalArgumentException("Fields invalid for add");
                }
            }
            valueToAdd -= (max + 1) - values[fieldIndex];  // reduce the amount to add
            values = nextField->addWrapPartial(instant, fieldIndex - 1, values, 1);  // add 1 to next bigger field
            values[fieldIndex] = getMinimumValue(instant, values);  // reset this field to zero
        }
        while (valueToAdd < 0) {
            int min = getMinimumValue(instant, values);
            int64_t proposed = values[fieldIndex] + valueToAdd;
            if (proposed >= min) {
                values[fieldIndex] = (int) proposed;
                break;
            }
            if (nextField == NULL) {
                if (fieldIndex == 0) {
                    valueToAdd -= (min - 1) - values[fieldIndex];
                    values[fieldIndex] = getMaximumValue(instant, values);
                    continue;
                }
                nextField = instant->getField(fieldIndex - 1);
                if (getRangeDurationField()->getType() != nextField->getDurationField()->getType()) {
                    throw IllegalArgumentException("Fields invalid for add");
                }
            }
            valueToAdd -= (min - 1) - values[fieldIndex];  // reduce the amount to add
            values = nextField->addWrapPartial(instant, fieldIndex - 1, values, -1);  // subtract 1 from next bigger field
            values[fieldIndex] = getMaximumValue(instant, values);  // reset this field to max value
        }
        
        return set(instant, fieldIndex, values, values[fieldIndex]);  // adjusts smaller fields
    }
    
    /**
     * Adds a value (which may be negative) to the instant value,
     * wrapping within this field.
     * <p>
     * The value will be added to this field. If the value is too large to be
     * added solely to this field then it wraps. Larger fields are always
     * unaffected. Smaller fields should be unaffected, except where the
     * result would be an invalid value for a smaller field. In this case the
     * smaller field is adjusted to be in range.
     * <p>
     * For example, in the ISO chronology:<br>
     * 2000-08-20 addWrapField six months is 2000-02-20<br>
     * 2000-08-20 addWrapField twenty months is 2000-04-20<br>
     * 2000-08-20 addWrapField minus nine months is 2000-11-20<br>
     * 2001-01-31 addWrapField one month  is 2001-02-28<br>
     * 2001-01-31 addWrapField two months is 2001-03-31<br>
     * <p>
     * The default implementation internally calls set. Subclasses are
     * encouraged to provide a more efficient implementation.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to add to
     * @param value  the value to add, in the units of the field
     * @return the updated milliseconds
     */
    int64_t addWrapField(int64_t instant, int value) const {
        int current = get(instant);
        int wrapped = FieldUtils::getWrappedValue
        (current, value, getMinimumValue(instant), getMaximumValue(instant));
        return set(instant, wrapped);
    }
    
    /**
     * Adds a value (which may be negative) to the partial instant,
     * wrapping within this field.
     * <p>
     * The value will be added to this field. If the value is too large to be
     * added solely to this field then it wraps. Larger fields are always
     * unaffected. Smaller fields should be unaffected, except where the
     * result would be an invalid value for a smaller field. In this case the
     * smaller field is adjusted to be in range.
     * <p>
     * For example, in the ISO chronology:<br>
     * 2000-08-20 addWrapField six months is 2000-02-20<br>
     * 2000-08-20 addWrapField twenty months is 2000-04-20<br>
     * 2000-08-20 addWrapField minus nine months is 2000-11-20<br>
     * 2001-01-31 addWrapField one month  is 2001-02-28<br>
     * 2001-01-31 addWrapField two months is 2001-03-31<br>
     * <p>
     * The default implementation internally calls set. Subclasses are
     * encouraged to provide a more efficient implementation.
     *
     * @param instant  the partial instant
     * @param fieldIndex  the index of this field in the instant
     * @param values  the values of the partial instant which should be updated
     * @param valueToAdd  the value to add, in the units of the field
     * @return the passed in values
     * @throws IllegalArgumentException if the value is invalid
     */
    vector<int> addWrapField(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const {
        int current = values[fieldIndex];
        int wrapped = FieldUtils::getWrappedValue
        (current, valueToAdd, getMinimumValue(instant), getMaximumValue(instant));
        return set(instant, fieldIndex, values, wrapped);  // adjusts smaller fields
    }
    
    //-----------------------------------------------------------------------
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
     *
     * @param minuendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract from
     * @param subtrahendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract off the minuend
     * @return the difference in the units of this field
     */
    int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return getDurationField()->getDifference(minuendInstant, subtrahendInstant);
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
     *
     * @param minuendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract from
     * @param subtrahendInstant the milliseconds from 1970-01-01T00:00:00Z to
     * subtract off the minuend
     * @return the difference in the units of this field
     */
    int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const {
        return getDurationField()->getDifferenceAsLong(minuendInstant, subtrahendInstant);
    }
    
    /**
     * Sets a value in the milliseconds supplied.
     * <p>
     * The value of this field will be set.
     * If the value is invalid, an exception if thrown.
     * <p>
     * If setting this field would make other fields invalid, then those fields
     * may be changed. For example if the current date is the 31st January, and
     * the month is set to February, the day would be invalid. Instead, the day
     * would be changed to the closest value - the 28th/29th February as appropriate.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to set in
     * @param value  the value to set, in the units of the field
     * @return the updated milliseconds
     * @throws IllegalArgumentException if the value is invalid
     */
    virtual int64_t set(int64_t instant, int value) const = 0;
    
    /**
     * Sets a value using the specified partial instant->
     * <p>
     * The value of this field (specified by the index) will be set.
     * If the value is invalid, an exception if thrown.
     * <p>
     * If setting this field would make other fields invalid, then those fields
     * may be changed. For example if the current date is the 31st January, and
     * the month is set to February, the day would be invalid. Instead, the day
     * would be changed to the closest value - the 28th/29th February as appropriate.
     *
     * @param partial  the partial instant
     * @param fieldIndex  the index of this field in the instant
     * @param values  the values to update
     * @param newValue  the value to set, in the units of the field
     * @return the updated values
     * @throws IllegalArgumentException if the value is invalid
     */
    vector<int> set(ReadablePartial *partial, int fieldIndex, vector<int> values, int newValue) const {
        FieldUtils::verifyValueBounds(this, newValue, getMinimumValue(partial, values), getMaximumValue(partial, values));
        values[fieldIndex] = newValue;
        
        // may need to adjust smaller fields
        for (int i = fieldIndex + 1; i < partial->size(); i++) {
            const DateTimeField *field = partial->getField(i);
            if (values[i] > field->getMaximumValue(partial, values)) {
                values[i] = field->getMaximumValue(partial, values);
            }
            if (values[i] < field->getMinimumValue(partial, values)) {
                values[i] = field->getMinimumValue(partial, values);
            }
        }
        return values;
    }
    
    /**
     * Sets a value in the milliseconds supplied from a human-readable, text value.
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * This implementation uses <code>convertText(String, Locale)</code> and
     * {@link #set(long, int)}.
     * <p>
     * Note: subclasses that override this method should also override
     * getAsText.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to set in
     * @param text  the text value to set
     * @param locale the locale to use for selecting a text symbol, NULL for default
     * @return the updated milliseconds
     * @throws IllegalArgumentException if the text value is invalid
     */
    int64_t set(int64_t instant, string text, Locale *locale) const {
        int value = convertText(text, locale);
        return set(instant, value);
    }
    
    /**
     * Sets a value in the milliseconds supplied from a human-readable, text value.
     * <p>
     * This implementation uses {@link #set(long, String, Locale)}.
     * <p>
     * Note: subclasses that override this method should also override getAsText.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to set in
     * @param text  the text value to set
     * @return the updated milliseconds
     * @throws IllegalArgumentException if the text value is invalid
     */
    int64_t set(int64_t instant, string text) const {
        return set(instant, text, NULL);
    }
    
    /**
     * Sets a value in the milliseconds supplied from a human-readable, text value.
     * If the specified locale is NULL, the default locale is used.
     * <p>
     * This implementation uses <code>convertText(String, Locale)</code> and
     * {@link #set(ReadablePartial, int, vector<int>, int)}.
     *
     * @param instant  the partial instant
     * @param fieldIndex  the index of this field in the instant
     * @param values  the values of the partial instant which should be updated
     * @param text  the text value to set
     * @param locale the locale to use for selecting a text symbol, NULL for default
     * @return the passed in values
     * @throws IllegalArgumentException if the text value is invalid
     */
    vector<int> set(ReadablePartial *instant, int fieldIndex, vector<int> values, string text, Locale *locale) const {
        int value = convertText(text, locale);
        return set(instant, fieldIndex, values, value);
    }
    
    // Extra information API
    //------------------------------------------------------------------------
    /**
     * Returns the duration per unit value of this field. For example, if this
     * field represents "hour of day", then the unit duration is an hour.
     *
     * @return the duration of this field, or UnsupportedDurationField if field
     * has no duration
     */
    virtual const DurationField *getDurationField() const = 0;
    
    /**
     * Returns the range duration of this field. For example, if this field
     * represents "hour of day", then the range duration is a day.
     *
     * @return the range duration of this field, or NULL if field has no range
     */
    virtual const DurationField *getRangeDurationField() const = 0;
    
    /**
     * Returns whether this field is 'leap' for the specified instant->
     * <p>
     * For example, a leap year would return true, a non leap year would return
     * false.
     * <p>
     * This implementation returns false.
     *
     * @return true if the field is 'leap'
     */
    bool isLeap(int64_t instant) const {
        return false;
    }
    
    /**
     * Gets the amount by which this field is 'leap' for the specified instant->
     * <p>
     * For example, a leap year would return one, a non leap year would return
     * zero.
     * <p>
     * This implementation returns zero.
     */
    int getLeapAmount(int64_t instant) const {
        return 0;
    }
    
    /**
     * If this field were to leap, then it would be in units described by the
     * returned duration. If this field doesn't ever leap, NULL is returned.
     * <p>
     * This implementation returns NULL.
     */
    const DurationField *getLeapDurationField() const {
        return NULL;
    }
    
    /**
     * Get the minimum allowable value for this field.
     *
     * @return the minimum valid value for this field, in the units of the
     * field
     */
    virtual int getMinimumValue() const = 0;
    
    /**
     * Get the minimum value for this field evaluated at the specified time.
     * <p>
     * This implementation returns the same as {@link #getMinimumValue()}.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the minimum value for this field, in the units of the field
     */
    int getMinimumValue(int64_t instant) const {
        return getMinimumValue();
    }
    
    /**
     * Get the minimum value for this field evaluated at the specified instant->
     * <p>
     * This implementation returns the same as {@link #getMinimumValue()}.
     *
     * @param instant  the partial instant to query
     * @return the minimum value for this field, in the units of the field
     */
    int getMinimumValue(ReadablePartial *instant) const {
        return getMinimumValue();
    }
    
    /**
     * Get the minimum value for this field using the partial instant and
     * the specified values.
     * <p>
     * This implementation returns the same as {@link #getMinimumValue(ReadablePartial)}.
     *
     * @param instant  the partial instant to query
     * @param values  the values to use
     * @return the minimum value for this field, in the units of the field
     */
    int getMinimumValue(ReadablePartial *instant, vector<int> values) const {
        return getMinimumValue(instant);
    }
    
    /**
     * Get the maximum allowable value for this field.
     *
     * @return the maximum valid value for this field, in the units of the
     * field
     */
    virtual int getMaximumValue() const = 0;
    
    /**
     * Get the maximum value for this field evaluated at the specified time.
     * <p>
     * This implementation returns the same as {@link #getMaximumValue()}.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the maximum value for this field, in the units of the field
     */
    int getMaximumValue(int64_t instant) const {
        return getMaximumValue();
    }
    
    /**
     * Get the maximum value for this field evaluated at the specified instant->
     * <p>
     * This implementation returns the same as {@link #getMaximumValue()}.
     *
     * @param instant  the partial instant to query
     * @return the maximum value for this field, in the units of the field
     */
    int getMaximumValue(ReadablePartial *instant) const {
        return getMaximumValue();
    }
    
    /**
     * Get the maximum value for this field using the partial instant and
     * the specified values.
     * <p>
     * This implementation returns the same as {@link #getMaximumValue(ReadablePartial)}.
     *
     * @param instant  the partial instant to query
     * @param values  the values to use
     * @return the maximum value for this field, in the units of the field
     */
    int getMaximumValue(ReadablePartial *instant, vector<int> values) const {
        return getMaximumValue(instant);
    }
    
    /**
     * Get the maximum text value for this field. The default implementation
     * returns the equivalent of Integer.toString(getMaximumValue()).length()->
     *
     * @param locale  the locale to use for selecting a text symbol
     * @return the maximum text length
     */
    int getMaximumTextLength(Locale *locale) const {
        int max = getMaximumValue();
        if (max >= 0) {
            if (max < 10) {
                return 1;
            } else if (max < 100) {
                return 2;
            } else if (max < 1000) {
                return 3;
            }
        }
        return (int) to_string(max).size();
    }
    
    /**
     * Get the maximum short text value for this field. The default
     * implementation returns getMaximumTextLength()->
     *
     * @param locale  the locale to use for selecting a text symbol
     * @return the maximum short text length
     */
    int getMaximumShortTextLength(Locale *locale) const {
        return getMaximumTextLength(locale);
    }
    
    // Calculation API
    //------------------------------------------------------------------------
    /**
     * Round to the lowest whole unit of this field. After rounding, the value
     * of this field and all fields of a higher magnitude are retained. The
     * fractional millis that cannot be expressed in whole increments of this
     * field are set to minimum.
     * <p>
     * For example, a datetime of 2002-11-02T23:34:56.789, rounded to the
     * lowest whole hour is 2002-11-02T23:00:00.000.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to round
     * @return rounded milliseconds
     */
    virtual int64_t roundFloor(int64_t instant) const = 0;
    
    /**
     * Round to the highest whole unit of this field. The value of this field
     * and all fields of a higher magnitude may be incremented in order to
     * achieve this result. The fractional millis that cannot be expressed in
     * whole increments of this field are set to minimum.
     * <p>
     * For example, a datetime of 2002-11-02T23:34:56.789, rounded to the
     * highest whole hour is 2002-11-03T00:00:00.000.
     * <p>
     * The default implementation calls roundFloor, and if the instant is
     * modified as a result, adds one field unit. Subclasses are encouraged to
     * provide a more efficient implementation.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to round
     * @return rounded milliseconds
     */
    int64_t roundCeiling(int64_t instant) const {
        int64_t newInstant = roundFloor(instant);
        if (newInstant != instant) {
            instant = add(newInstant, 1);
        }
        return instant;
    }
    
    /**
     * Round to the nearest whole unit of this field. If the given millisecond
     * value is closer to the floor or is exactly halfway, this function
     * behaves like roundFloor. If the millisecond value is closer to the
     * ceiling, this function behaves like roundCeiling.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to round
     * @return rounded milliseconds
     */
    int64_t roundHalfFloor(int64_t instant) const {
        int64_t floor = roundFloor(instant);
        int64_t ceiling = roundCeiling(instant);
        
        int64_t diffFromFloor = instant - floor;
        int64_t diffToCeiling = ceiling - instant;
        
        if (diffFromFloor <= diffToCeiling) {
            // Closer to the floor, or halfway - round floor
            return floor;
        } else {
            return ceiling;
        }
    }
    
    /**
     * Round to the nearest whole unit of this field. If the given millisecond
     * value is closer to the floor, this function behaves like roundFloor. If
     * the millisecond value is closer to the ceiling or is exactly halfway,
     * this function behaves like roundCeiling.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to round
     * @return rounded milliseconds
     */
    int64_t roundHalfCeiling(int64_t instant) const {
        int64_t floor = roundFloor(instant);
        int64_t ceiling = roundCeiling(instant);
        
        int64_t diffFromFloor = instant - floor;
        int64_t diffToCeiling = ceiling - instant;
        
        if (diffToCeiling <= diffFromFloor) {
            // Closer to the ceiling, or halfway - round ceiling
            return ceiling;
        } else {
            return floor;
        }
    }
    
    /**
     * Round to the nearest whole unit of this field. If the given millisecond
     * value is closer to the floor, this function behaves like roundFloor. If
     * the millisecond value is closer to the ceiling, this function behaves
     * like roundCeiling.
     * <p>
     * If the millisecond value is exactly halfway between the floor and
     * ceiling, the ceiling is chosen over the floor only if it makes this
     * field's value even.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to round
     * @return rounded milliseconds
     */
    int64_t roundHalfEven(int64_t instant) const {
        int64_t floor = roundFloor(instant);
        int64_t ceiling = roundCeiling(instant);
        
        int64_t diffFromFloor = instant - floor;
        int64_t diffToCeiling = ceiling - instant;
        
        if (diffFromFloor < diffToCeiling) {
            // Closer to the floor - round floor
            return floor;
        } else if (diffToCeiling < diffFromFloor) {
            // Closer to the ceiling - round ceiling
            return ceiling;
        } else {
            // Round to the instant that makes this field even. If both values
            // make this field even (unlikely), favor the ceiling.
            if ((get(ceiling) & 1) == 0) {
                return ceiling;
            }
            return floor;
        }
    }
    
    /**
     * Returns the fractional duration milliseconds of this field. In other
     * words, calling remainder returns the duration that roundFloor would
     * subtract.
     * <p>
     * For example, on a datetime of 2002-11-02T23:34:56.789, the remainder by
     * hour is 34 minutes and 56.789 seconds.
     * <p>
     * The default implementation computes
     * <code>instant - roundFloor(instant)</code>. Subclasses are encouraged to
     * provide a more efficient implementation.
     *
     * @param instant the milliseconds from 1970-01-01T00:00:00Z to get the
     * remainder
     * @return remainder duration, in milliseconds
     */
    int64_t remainder(int64_t instant) const {
        return instant - roundFloor(instant);
    }
    
    /**
     * Get a suitable debug string.
     * 
     * @return debug string
     */
    string toString() const {
        string str("DateTimeField[");
        str.append(getName());
        str.append("]");
        return str;
    }
    
};

CODATIME_END

#endif
