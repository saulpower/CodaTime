//
//  AbstractReadableInstantFieldProperty.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_AbstractReadableInstantFieldProperty_h
#define CodaTime_AbstractReadableInstantFieldProperty_h

#include "CodaTimeMacros.h"

#include "DateTimeField.h"
#include "DateTimeFieldType.h"
#include "Exceptions.h"
#include "field/FieldUtils.h"
#include "Interval.h"
#include "Locale.h"
#include "Object.h"

#include <string>

using namespace std;

CODATIME_BEGIN

/**
 * AbstractReadableInstantFieldProperty is a base class for binding a
 * ReadableInstant to a DateTimeField.
 * <p>
 * It allows the date and time manipulation code to be field based yet
 * still easy to use.
 * <p>
 * AbstractReadableInstantFieldProperty itself is thread-safe and immutable,
 * but the ReadableInstant being operated on may be mutable and not
 * thread-safe.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @author Mike Schrag
 * @since 1.0
 */
class AbstractReadableInstantFieldProperty : public virtual Object {
    
private:
    
    /** Serialization version. */
    static const long long serialVersionUID = 1971226328211649661L;
    
public:
    
    /**
     * Constructor.
     */
    AbstractReadableInstantFieldProperty() {
    }
    
    //-----------------------------------------------------------------------
    /**
     * Gets the field being used.
     *
     * @return the field
     */
    virtual const DateTimeField *getField() const = 0;
    
    /**
     * Gets the field type being used.
     *
     * @return the field type
     */
    const DateTimeFieldType *getFieldType() const {
        return getField()->getType();
    }
    
    /**
     * Gets the name of the field.
     *
     * @return the field name
     */
    string getName() const {
        return getField()->getName();
    }
    
    //-----------------------------------------------------------------------
    /**
     * Gets the value of this property from the instant.
     * <p>
     * For example, the following two lines of code are equivalent:
     * <pre>
     * datetime.getDayOfMonth();
     * datetime.dayOfMonth().get();
     * </pre>
     *
     * @return the current value
     * @see DateTimeField#get
     */
    int get() const {
        return getField()->get(getMillis());
    }
    
    /**
     * Gets the value of this property from the instant as a string.
     * <p>
     * This method returns the value converted to a <code>string</code>
     * using <code>Integer.tostring</code>. This method does NOT return
     * textual descriptions such as 'Monday' or 'January'.
     * See {@link #getAsText()} and {@link #getAsShortText()} for those.
     *
     * @return the current value
     * @see DateTimeField#get
     * @since 1.1
     */
    string getAsString() {
        return to_string(get());
    }
    
    /**
     * Gets the textual value of this property from the instant as a
     * string in the default locale.
     * <p>
     * This method returns the value converted to a <code>string</code>
     * returning the appropriate textual description wherever possible.
     * Thus, a day of week of 1 would return 'Monday' in English.
     *
     * @return the current text value
     * @see DateTimeField#getAsText
     */
    string getAsText() {
        return getAsText(NULL);
    }
    
    /**
     * Gets the textual value of this property from the instant as a
     * string in the specified locale.
     * <p>
     * This method returns the value converted to a <code>string</code>
     * returning the appropriate textual description wherever possible.
     * Thus, a day of week of 1 would return 'Monday' in English.
     *
     * @param locale  locale to use for selecting a text symbol, NULL means default
     * @return the current text value
     * @see DateTimeField#getAsText
     */
    string getAsText(Locale *locale) {
        return getField()->getAsText(getMillis(), locale);
    }
    
    /**
     * Gets the short textual value of this property from the instant as a
     * string in the default locale.
     * <p>
     * This method returns the value converted to a <code>string</code>
     * returning the appropriate textual description wherever possible.
     * Thus, a day of week of 1 would return 'Mon' in English.
     *
     * @return the current text value
     * @see DateTimeField#getAsShortText
     */
    string getAsShortText() {
        return getAsShortText(NULL);
    }
    
    /**
     * Gets the short textual value of this property from the instant as a
     * string in the specified locale.
     * <p>
     * This method returns the value converted to a <code>string</code>
     * returning the appropriate textual description wherever possible.
     * Thus, a day of week of 1 would return 'Mon' in English.
     *
     * @param locale  locale to use for selecting a text symbol, NULL means default
     * @return the current text value
     * @see DateTimeField#getAsShortText
     */
    string getAsShortText(Locale *locale) {
        return getField()->getAsShortText(getMillis(), locale);
    }
    
    //-----------------------------------------------------------------------
    /**
     * Returns the difference between this field property instant and the one
     * passed in, in the units of this field. The sign of the difference
     * matches that of compareTo. In other words, this field property's instant
     * is the minuend.
     *
     * @param instant  the subtrahend, NULL means now
     * @return the difference in the units of this field
     * @see DateTimeField#getDifference
     */
    int getDifference(ReadableInstant *instant) {
        if (instant == NULL) {
            return getField()->getDifference(getMillis(), DateTimeUtils::currentTimeMillis());
        }
        return getField()->getDifference(getMillis(), instant->getMillis());
    }
    
    /**
     * Returns the difference between this field property instant and the one
     * passed in, in the units of this field. The sign of the difference
     * matches that of compareTo. In other words, this field property's instant
     * is the minuend.
     *
     * @param instant  the subtrahend, NULL means now
     * @return the difference in the units of this field
     * @see DateTimeField#getDifference
     */
    int64_t getDifferenceAsLong(ReadableInstant *instant) {
        if (instant == NULL) {
            return getField()->getDifferenceAsLong(getMillis(), DateTimeUtils::currentTimeMillis());
        }
        return getField()->getDifferenceAsLong(getMillis(), instant->getMillis());
    }
    
    //-----------------------------------------------------------------------
    /**
     * Returns the duration per unit value of this field. For example, if this
     * field represents "hour of day", then the duration is an hour.
     *
     * @return the duration of this field, or UnsupportedDurationField
     */
    const DurationField *getDurationField() {
        return getField()->getDurationField();
    }
    
    /**
     * Returns the range duration of this field. For example, if this field
     * represents "hour of day", then the range duration is a day.
     *
     * @return the range duration of this field, or NULL if field has no range
     */
    const DurationField *getRangeDurationField() {
        return getField()->getRangeDurationField();
    }
    
    /**
     * Gets whether this field is leap.
     *
     * @return true if a leap field
     * @see DateTimeField#isLeap
     */
    bool isLeap() {
        return getField()->isLeap(getMillis());
    }
    
    /**
     * Gets the amount by which this field is leap.
     *
     * @return the amount by which the field is leap
     * @see DateTimeField#getLeapAmount
     */
    int getLeapAmount() {
        return getField()->getLeapAmount(getMillis());
    }
    
    /**
     * If this field were to leap, then it would be in units described by the
     * returned duration. If this field doesn't ever leap, NULL is returned.
     */
    const DurationField *getLeapDurationField() {
        return getField()->getLeapDurationField();
    }
    
    //-----------------------------------------------------------------------
    /**
     * Gets the minimum value for the field ignoring the current time.
     *
     * @return the minimum value
     * @see DateTimeField#getMinimumValue
     */
    int getMinimumValueOverall() {
        return getField()->getMinimumValue();
    }
    
    /**
     * Gets the minimum value for the field.
     *
     * @return the minimum value
     * @see DateTimeField#getMinimumValue
     */
    int getMinimumValue() {
        return getField()->getMinimumValue(getMillis());
    }
    
    /**
     * Gets the maximum value for the field ignoring the current time.
     *
     * @return the maximum value
     * @see DateTimeField#getMaximumValue
     */
    int getMaximumValueOverall() {
        return getField()->getMaximumValue();
    }
    
    /**
     * Gets the maximum value for the field.
     *
     * @return the maximum value
     * @see DateTimeField#getMaximumValue
     */
    int getMaximumValue() {
        return getField()->getMaximumValue(getMillis());
    }
    
    /**
     * Gets the maximum text length for the field.
     *
     * @param locale  optional locale to use for selecting a text symbol
     * @return the maximum length
     * @see DateTimeField#getMaximumTextLength
     */
    int getMaximumTextLength(Locale *locale) {
        return getField()->getMaximumTextLength(locale);
    }
    
    /**
     * Gets the maximum short text length for the field.
     *
     * @param locale  optional locale to use for selecting a text symbol
     * @return the maximum length
     * @see DateTimeField#getMaximumShortTextLength
     */
    int getMaximumShortTextLength(Locale *locale) {
        return getField()->getMaximumShortTextLength(locale);
    }
    
    
    /**
     * Returns the fractional duration milliseconds of this field.
     *
     * @see DateTimeField#remainder
     * @return remainder duration, in milliseconds
     */
    int64_t remainder() {
        return getField()->remainder(getMillis());
    }
    
    /**
     * Returns the interval that represents the range of the minimum
     * and maximum values of this field.
     * <p>
     * For example, <code>datetime.monthOfYear().toInterval()</code>
     * will return an interval over the whole month.
     *
     * @return the interval of this field
     * @since 1.2
     */
    Interval *toInterval() {
        DateTimeField *field = getField();
        int64_t start = field->roundFloor(getMillis());
        int64_t end = field->add(start, 1);
        Interval *interval = new Interval(start, end);
        return interval;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Compare this field to the same field on another instant.
     * <p>
     * The comparison is based on the value of the same field type, irrespective
     * of any difference in chronology. Thus, if this property represents the
     * hourOfDay field, then the hourOfDay field of the other instant will be queried
     * whether in the same chronology or not.
     *
     * @param instant  the instant to compare to
     * @return negative value if this is less, 0 if equal, or positive value if greater
     * @throws IllegalArgumentException if the instant is NULL
     */
    int compareTo(const ReadableInstant *instant) const {
        if (instant == NULL) {
            throw IllegalArgumentException("The instant must not be NULL");
        }
        int thisValue = get();
        int otherValue = instant->get(getFieldType());
        if (thisValue < otherValue) {
            return -1;
        } else if (thisValue > otherValue) {
            return 1;
        } else {
            return 0;
        }
    }
    
    //-----------------------------------------------------------------------
    /**
     * Compare this field to the same field on another partial instant.
     * <p>
     * The comparison is based on the value of the same field type, irrespective
     * of any difference in chronology. Thus, if this property represents the
     * hourOfDay field, then the hourOfDay field of the other partial will be queried
     * whether in the same chronology or not.
     *
     * @param partial  the partial to compare to
     * @return negative value if this is less, 0 if equal, or positive value if greater
     * @throws IllegalArgumentException if the partial is NULL
     * @throws IllegalArgumentException if the partial doesn't support this field
     */
    int compareTo(const ReadablePartial *partial) const {
        if (partial == NULL) {
            throw IllegalArgumentException("The partial must not be NULL");
        }
        int thisValue = get();
        int otherValue = partial->get(getFieldType());
        if (thisValue < otherValue) {
            return -1;
        } else if (thisValue > otherValue) {
            return 1;
        } else {
            return 0;
        }
    }
    
    //-----------------------------------------------------------------------
    /**
     * Compares this property to another.
     *
     * @param object  the object to compare to
     * @return true if equal
     */
    bool equals(const Object *object) const {
//        if (this == object) {
//            return true;
//        }
        const AbstractReadableInstantFieldProperty *other = dynamic_cast<const AbstractReadableInstantFieldProperty*>(object);
        if (other == 0) {
            return false;
        }
        return get() == other->get() &&
        getFieldType()->equals(other->getFieldType()) &&
        FieldUtils::equals(getChronology(), other->getChronology());
    }
    
    /**
     * Returns a hashcode compatible with the equals method.
     *
     * @return the hashcode
     */
    int hashCode() {
        return get() * 17 + getFieldType()->hashCode() + getChronology()->hashCode();
    }
    
    //-----------------------------------------------------------------------
    /**
     * Output a debugging string.
     *
     * @return debugging string
     */
    string toString() {
        string str;
        str.append("Property[");
        str.append(getName());
        str.append("]");
        return str;
    }
    
protected:
    
    /**
     * Gets the milliseconds of the datetime that this property is linked to.
     *
     * @return the milliseconds
     */
    virtual int64_t getMillis() const = 0;
    
    /**
     * Gets the chronology of the datetime that this property is linked to.
     * <p>
     * This implementation throws UnsupportedOperationException, and must be
     * implemented by subclasses to enable the equals() and hashCode() methods.
     *
     * @return the chronology
     * @since 1.4
     */
    Chronology *getChronology() const {
        throw UnsupportedOperationException("The method getChronology() was added in v1.4 and needs to be implemented by subclasses of AbstractReadableInstantFieldProperty");
    }

};

CODATIME_END

#endif
