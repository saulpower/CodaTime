//
//  DateTimeField.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_DateTimeField_h
#define CodaTime_DateTimeField_h

#include "CodaTimeMacros.h"

#include "Object.h"

#include <vector>
#include <string>

using namespace std;

CODATIME_BEGIN

class ReadablePartial;
class Locale;
class DurationField;
class DateTimeFieldType;

/**
 * Defines the calculation engine for date and time fields.
 * The interface defines a set of methods that manipulate a millisecond datetime
 * with regards to a single field, such as monthOfYear or secondOfMinute.
 * <p>
 * This design is extensible so, if you wish, you can extract a different field from
 * the milliseconds. A number of standard implementations are provided to assist.
 *
 * @author Guy Allard
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
class DateTimeField : public virtual Object {
    
public:
    
    /**
     * Get the type of the field.
     *
     * @return field type
     */
    virtual const DateTimeFieldType *getType() const = 0;
    
    /**
     * Get the name of the field.
     * <p>
     * By convention, names follow a pattern of "dddOfRrr", where "ddd" represents
     * the (singular) duration unit field name and "Rrr" represents the (singular)
     * duration range field name. If the range field is not applicable, then
     * the name of the field is simply the (singular) duration field name.
     *
     * @return field name
     */
    virtual string getName() const = 0;
    
    /**
     * Returns true if this field is supported.
     *
     * @return true if this field is supported
     */
    virtual bool isSupported() const = 0;
    
    /**
     * Returns true if the set method is lenient. If so, it accepts values that
     * are out of bounds. For example, a lenient day of month field accepts 32
     * for January, converting it to February 1.
     *
     * @return true if this field is lenient
     */
    virtual bool isLenient() const = 0;
    
    // Main access API
    //------------------------------------------------------------------------
    /**
     * Get the value of this field from the milliseconds.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the value of the field, in the units of the field
     */
    virtual int get(int64_t instant) const = 0;
    
    /**
     * Get the human-readable, text value of this field from the milliseconds.
     * If the specified locale is null, the default locale is used.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @param locale the locale to use for selecting a text symbol, null for default
     * @return the text value of the field
     */
    virtual string getAsText(int64_t instant, Locale *locale) const = 0;
    
    /**
     * Get the human-readable, text value of this field from the milliseconds.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the text value of the field
     */
    virtual string getAsText(int64_t instant) const = 0;
    
    /**
     * Get the human-readable, text value of this field from a partial instant.
     * If the specified locale is null, the default locale is used.
     *
     * @param partial  the partial instant to query
     * @param fieldValue  the field value of this field, provided for performance
     * @param locale  the locale to use for selecting a text symbol, null for default
     * @return the text value of the field
     */
    virtual string getAsText(ReadablePartial *partial, int fieldValue, Locale *locale) const = 0;
    
    /**
     * Get the human-readable, text value of this field from a partial instant.
     * If the specified locale is null, the default locale is used.
     *
     * @param partial  the partial instant to query
     * @param locale  the locale to use for selecting a text symbol, null for default
     * @return the text value of the field
     */
    virtual string getAsText(ReadablePartial *partial, Locale *locale) const = 0;
    
    /**
     * Get the human-readable, text value of this field from the field value.
     * If the specified locale is null, the default locale is used.
     *
     * @param fieldValue  the numeric value to convert to text
     * @param locale the locale to use for selecting a text symbol, null for default
     * @return the text value of the field
     */
    virtual string getAsText(int fieldValue, Locale *locale) const = 0;
    
    /**
     * Get the human-readable, short text value of this field from the
     * milliseconds.  If the specified locale is null, the default locale is used.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @param locale the locale to use for selecting a text symbol, null for default
     * @return the short text value of the field
     */
    virtual string getAsShortText(int64_t instant, Locale *locale) const = 0;
    
    /**
     * Get the human-readable, short text value of this field from the
     * milliseconds.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the short text value of the field
     */
    virtual string getAsShortText(int64_t instant) const = 0;
    
    /**
     * Get the human-readable, short text value of this field from a partial instant.
     * If the specified locale is null, the default locale is used.
     *
     * @param partial  the partial instant to query
     * @param fieldValue  the field value of this field, provided for performance
     * @param locale  the locale to use for selecting a text symbol, null for default
     * @return the text value of the field
     */
    virtual string getAsShortText(ReadablePartial *partial, int fieldValue, Locale *locale) const = 0;
    
    /**
     * Get the human-readable, short text value of this field from a partial instant.
     * If the specified locale is null, the default locale is used.
     *
     * @param partial  the partial instant to query
     * @param locale  the locale to use for selecting a text symbol, null for default
     * @return the text value of the field
     */
    virtual string getAsShortText(ReadablePartial *partial, Locale *locale) const = 0;
    
    /**
     * Get the human-readable, short text value of this field from the field value.
     * If the specified locale is null, the default locale is used.
     *
     * @param fieldValue  the numeric value to convert to text
     * @param locale the locale to use for selecting a text symbol, null for default
     * @return the text value of the field
     */
    virtual string getAsShortText(int fieldValue, Locale *locale) const = 0;
    
    /**
     * Adds a value (which may be negative) to the millis value,
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
    virtual int64_t add(int64_t instant, int value) const = 0;
    
    /**
     * Adds a value (which may be negative) to the millis value,
     * overflowing into larger fields if necessary.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to add to
     * @param value  the int64_t value to add, in the units of the field
     * @return the updated milliseconds
     * @throws IllegalArgumentException if value is too large
     * @see #add(uint64_t,int)
     */
    virtual int64_t add(int64_t instant, int64_t value) const = 0;
    
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
     * @param fieldIndex  the index of this field in the instant
     * @param values  the values of the partial instant which should be updated
     * @param valueToAdd  the value to add, in the units of the field
     * @return the passed in values
     * @throws IllegalArgumentException if the value is invalid or the maximum instant is reached
     */
    virtual vector<int> add(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const = 0;
    
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
    virtual vector<int> addWrapPartial(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const = 0;
    
    /**
     * Adds a value (which may be negative) to the millis value,
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
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to add to
     * @param value  the value to add, in the units of the field
     * @return the updated milliseconds
     */
    virtual int64_t addWrapField(int64_t instant, int value) const = 0;
    
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
     *
     * @param instant  the partial instant
     * @param fieldIndex  the index of this field in the instant
     * @param values  the values of the partial instant which should be updated
     * @param valueToAdd  the value to add, in the units of the field
     * @return the passed in values
     * @throws IllegalArgumentException if the value is invalid
     */
    virtual vector<int> addWrapField(ReadablePartial *instant, int fieldIndex, vector<int> values, int valueToAdd) const = 0;
    
    /**
     * Computes the difference between two instants, as measured in the units
     * of this field. Any fractional units are dropped from the result. Calling
     * getDifference reverses the effect of calling add. In the following code:
     *
     * <pre>
     * int64_t instant = ...
     * int v = ...
     * int age = getDifference(add(instant, v), instant) = 0;
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
    virtual int getDifference(int64_t minuendInstant, int64_t subtrahendInstant) const = 0;
    
    /**
     * Computes the difference between two instants, as measured in the units
     * of this field. Any fractional units are dropped from the result. Calling
     * getDifference reverses the effect of calling add. In the following code:
     *
     * <pre>
     * int64_t instant = ...
     * int64_t v = ...
     * int64_t age = getDifferenceAsLong(add(instant, v), instant) = 0;
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
    virtual int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const = 0;
    
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
     * Sets a value using the specified partial instant.
     * <p>
     * The value of this field (specified by the index) will be set.
     * If the value is invalid, an exception if thrown.
     * <p>
     * If setting this field would make other fields invalid, then those fields
     * may be changed. For example if the current date is the 31st January, and
     * the month is set to February, the day would be invalid. Instead, the day
     * would be changed to the closest value - the 28th/29th February as appropriate.
     *
     * @param instant  the partial instant
     * @param fieldIndex  the index of this field in the instant
     * @param values  the values of the partial instant which should be updated
     * @param newValue  the value to set, in the units of the field
     * @return the passed in values
     * @throws IllegalArgumentException if the value is invalid
     */
    virtual vector<int> set(ReadablePartial *instant, int fieldIndex, vector<int> values, int newValue) const = 0;
    
    /**
     * Sets a value in the milliseconds supplied from a human-readable, text value.
     * If the specified locale is null, the default locale is used.
     * <p>
     * If setting this field would make other fields invalid, then those fields
     * may be changed. For example if the current date is the 31st January, and
     * the month is set to February, the day would be invalid. Instead, the day
     * would be changed to the closest value - the 28th/29th February as appropriate.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to set in
     * @param text  the text value to set
     * @param locale the locale to use for selecting a text symbol, null for default
     * @return the updated milliseconds
     * @throws IllegalArgumentException if the text value is invalid
     */
    virtual int64_t set(int64_t instant, string text, Locale *locale) const = 0;
    
    /**
     * Sets a value in the milliseconds supplied from a human-readable, text value.
     * <p>
     * If setting this field would make other fields invalid, then those fields
     * may be changed. For example if the current date is the 31st January, and
     * the month is set to February, the day would be invalid. Instead, the day
     * would be changed to the closest value - the 28th/29th February as appropriate.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to set in
     * @param text  the text value to set
     * @return the updated milliseconds
     * @throws IllegalArgumentException if the text value is invalid
     */
    virtual int64_t set(int64_t instant, string text) const = 0;
    
    /**
     * Sets a value in the milliseconds supplied from a human-readable, text value.
     * If the specified locale is null, the default locale is used.
     * <p>
     * If setting this field would make other fields invalid, then those fields
     * may be changed. For example if the current date is the 31st January, and
     * the month is set to February, the day would be invalid. Instead, the day
     * would be changed to the closest value - the 28th/29th February as appropriate.
     *
     * @param instant  the partial instant
     * @param fieldIndex  the index of this field in the instant
     * @param values  the values of the partial instant which should be updated
     * @param text  the text value to set
     * @param locale the locale to use for selecting a text symbol, null for default
     * @return the passed in values
     * @throws IllegalArgumentException if the text value is invalid
     */
    virtual vector<int> set(ReadablePartial *instant, int fieldIndex, vector<int> values, string text, Locale *locale) const = 0;
    
    // Extra information API
    //------------------------------------------------------------------------
    /**
     * Returns the duration per unit value of this field. For example, if this
     * field represents "hour of day", then the duration is an hour.
     *
     * @return the duration of this field, or UnsupportedDurationField if field
     * has no duration
     */
    virtual const DurationField *getDurationField() const = 0;
    
    /**
     * Returns the range duration of this field. For example, if this field
     * represents "hour of day", then the range duration is a day.
     *
     * @return the range duration of this field, or null if field has no range
     */
    virtual const DurationField *getRangeDurationField() const = 0;
    
    /**
     * Returns whether this field is 'leap' for the specified instant.
     * <p>
     * For example, a leap year would return true, a non leap year would return
     * false.
     *
     * @param instant  the instant to check for leap status
     * @return true if the field is 'leap'
     */
    virtual bool isLeap(int64_t instant) const = 0;
    
    /**
     * Gets the amount by which this field is 'leap' for the specified instant.
     * <p>
     * For example, a leap year would return one, a non leap year would return
     * zero.
     *
     * @param instant  the instant to check for leap status
     * @return the amount, in units of the leap duration field, that the field is leap
     */
    virtual int getLeapAmount(int64_t instant) const = 0;
    
    /**
     * If this field were to leap, then it would be in units described by the
     * returned duration. If this field doesn't ever leap, null is returned.
     *
     * @return the leap duration field if field can be leap, null if it can't
     */
    virtual const DurationField *getLeapDurationField() const = 0;
    
    /**
     * Get the minimum allowable value for this field.
     *
     * @return the minimum valid value for this field, in the units of the
     * field
     */
    virtual int getMinimumValue() const = 0;
    
    /**
     * Get the minimum value for this field evaluated at the specified time.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the minimum value for this field, in the units of the field
     */
    virtual int getMinimumValue(int64_t instant) const = 0;
    
    /**
     * Get the minimum value for this field evaluated at the specified time.
     *
     * @param instant  the partial instant to query
     * @return the minimum value for this field, in the units of the field
     */
    virtual int getMinimumValue(ReadablePartial *instant) const = 0;
    
    /**
     * Get the minimum value for this field using the partial instant and
     * the specified values.
     *
     * @param instant  the partial instant to query
     * @param values  the values to use
     * @return the minimum value for this field, in the units of the field
     */
    virtual int getMinimumValue(ReadablePartial *instant, vector<int> values) const = 0;
    
    /**
     * Get the maximum allowable value for this field.
     *
     * @return the maximum valid value for this field, in the units of the
     * field
     */
    virtual int getMaximumValue() const = 0;
    
    /**
     * Get the maximum value for this field evaluated at the specified time.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to query
     * @return the maximum value for this field, in the units of the field
     */
    virtual int getMaximumValue(int64_t instant) const = 0;
    
    /**
     * Get the maximum value for this field evaluated at the specified time.
     *
     * @param instant  the partial instant to query
     * @return the maximum value for this field, in the units of the field
     */
    virtual int getMaximumValue(ReadablePartial *instant) const = 0;
    
    /**
     * Get the maximum value for this field using the partial instant and
     * the specified values.
     *
     * @param instant  the partial instant to query
     * @param values  the values to use
     * @return the maximum value for this field, in the units of the field
     */
    virtual int getMaximumValue(ReadablePartial *instant, vector<int> values) const = 0;
    
    /**
     * Get the maximum text value for this field.
     *
     * @param locale  the locale to use for selecting a text symbol
     * @return the maximum text length
     */
    virtual int getMaximumTextLength(Locale *locale) const = 0;
    
    /**
     * Get the maximum short text value for this field.
     *
     * @param locale  the locale to use for selecting a text symbol
     * @return the maximum short text length
     */
    virtual int getMaximumShortTextLength(Locale *locale) const = 0;
    
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
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to round
     * @return rounded milliseconds
     */
    virtual int64_t roundCeiling(int64_t instant) const = 0;
    
    /**
     * Round to the nearest whole unit of this field. If the given millisecond
     * value is closer to the floor or is exactly halfway, this function
     * behaves like roundFloor. If the millisecond value is closer to the
     * ceiling, this function behaves like roundCeiling.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to round
     * @return rounded milliseconds
     */
    virtual int64_t roundHalfFloor(int64_t instant) const = 0;
    
    /**
     * Round to the nearest whole unit of this field. If the given millisecond
     * value is closer to the floor, this function behaves like roundFloor. If
     * the millisecond value is closer to the ceiling or is exactly halfway,
     * this function behaves like roundCeiling.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z to round
     * @return rounded milliseconds
     */
    virtual int64_t roundHalfCeiling(int64_t instant) const = 0;
    
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
    virtual int64_t roundHalfEven(int64_t instant) const = 0;
    
    /**
     * Returns the fractional duration milliseconds of this field. In other
     * words, calling remainder returns the duration that roundFloor would
     * subtract.
     * <p>
     * For example, on a datetime of 2002-11-02T23:34:56.789, the remainder by
     * hour is 34 minutes and 56.789 seconds.
     *
     * @param instant the milliseconds from 1970-01-01T00:00:00Z to get the
     * remainder
     * @return remainder duration, in milliseconds
     */
    virtual int64_t remainder(int64_t instant) const = 0;
    
    /**
     * Get a suitable debug string.
     * 
     * @return debug string
     */
    virtual string toString() const = 0;
    
};

CODATIME_END

#endif
