//
//  LocalTime.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__LocalTime__
#define __CodaTime__LocalTime__

#include "CodaTimeMacros.h"

#include "base/BaseLocal.h"
#include "ReadablePartial.h"
#include "field/AbstractReadableInstantFieldProperty.h"

#include "Exceptions.h"
#include "DateTimeField.h"
#include "DateTimeUtils.h"
#include "DurationFieldType.h"
#include "Chronology.h"

#include <string>
#include <set>

using namespace std;

CODATIME_BEGIN

class Locale;

/**
 * LocalTime is an immutable time class representing a time
 * without a time zone.
 * <p>
 * LocalTime implements the {@link ReadablePartial} interface.
 * To do this, the interface methods focus on the key fields -
 * HourOfDay, MinuteOfHour, SecondOfMinute and MillisOfSecond.
 * However, <b>all</b> time fields may in fact be queried.
 * <p>
 * Calculations on LocalTime are performed using a {@link Chronology}.
 * This chronology will be set internally to be in the UTC time zone
 * for all calculations.
 *
 * <p>Each individual field can be queried in two ways:
 * <ul>
 * <li><code>getHourOfDay()</code>
 * <li><code>hourOfDay().get()</code>
 * </ul>
 * The second technique also provides access to other useful methods on the
 * field:
 * <ul>
 * <li>numeric value
 * <li>text value
 * <li>short text value
 * <li>maximum/minimum values
 * <li>add/subtract
 * <li>set
 * <li>rounding
 * </ul>
 *
 * <p>
 * LocalTime is thread-safe and immutable, provided that the Chronology is as well.
 * All standard Chronology classes supplied are thread-safe and immutable.
 *
 * @author Stephen Colebourne
 * @since 1.3
 */
class LocalTime : public BaseLocal, public virtual ReadablePartial {
    
private:
    
    /** Serialization lock */
    static const int64_t serialVersionUID = -12873158713873L;
    
    /** The index of the hourOfDay field in the field array */
    static const int HOUR_OF_DAY = 0;
    /** The index of the minuteOfHour field in the field array */
    static const int MINUTE_OF_HOUR = 1;
    /** The index of the secondOfMinute field in the field array */
    static const int SECOND_OF_MINUTE = 2;
    /** The index of the millisOfSecond field in the field array */
    static const int MILLIS_OF_SECOND = 3;
    /** Set of known duration types. */
    static set<DurationFieldType*> TIME_DURATION_TYPES;
    
    struct StaticBlock {
        StaticBlock() {
            // TODO: uncomment
//            TIME_DURATION_TYPES.insert(DurationFieldType::millis());
//            TIME_DURATION_TYPES.insert(DurationFieldType::seconds());
//            TIME_DURATION_TYPES.insert(DurationFieldType::minutes());
//            TIME_DURATION_TYPES.insert(DurationFieldType::hours());
        }
    };
    
    static StaticBlock staticBlock;
    
    /** The local millis from 1970-01-01T00:00:00 */
    int64_t iLocalMillis;
    /** The chronology to use, in UTC */
    Chronology *iChronology;
    LocalTime *readResolve();
    LocalTime *withLocalMillis(int64_t newMillis);
    
protected:
    
    const DateTimeField *getField(int index, Chronology *chrono) const;
    int64_t getLocalMillis() const;
    
public:
    
    const DateTimeField *getField(int index) const { return AbstractPartial::getField(index); }
    
    const DateTimeFieldType *getFieldType(int index) const { return AbstractPartial::getFieldType(index); }
    
    int hashCode() { return AbstractPartial::hashCode(); }
    
    /**
     * LocalTime.Property binds a LocalTime to a DateTimeField *allowing
     * powerful datetime functionality to be easily accessed.
     * <p>
     * The simplest use of this class is as an alternative get method, here used to
     * get the minute '30'.
     * <pre>
     * LocalTime dt = new LocalTime(12, 30);
     * int year = dt.minuteOfHour().get();
     * </pre>
     * <p>
     * Methods are also provided that allow time modification. These return
     * new instances of LocalTime - they do not modify the original. The example
     * below yields two independent immutable date objects 2 hours apart.
     * <pre>
     * LocalTime dt1230 = new LocalTime(12, 30);
     * LocalTime dt1430 = dt1230.hourOfDay().setCopy(14);
     * </pre>
     * <p>
     * LocalTime.Property itself is thread-safe and immutable, as well as the
     * LocalTime being operated on.
     *
     * @author Stephen Colebourne
     * @author Brian S O'Neill
     * @since 1.3
     */
    class Property : public AbstractReadableInstantFieldProperty {
        
    private:
        
        /** Serialization version */
        static const long long serialVersionUID = -325842547277223L;
        
        /** The instant this property is working against */
        LocalTime *iInstant;
        /** The field this property is working against */
        const DateTimeField *iField;
        
    public:
        
        /**
         * Constructor.
         *
         * @param instant  the instant to set
         * @param field  the field to use
         */
        Property(LocalTime *instant, const DateTimeField *field) {
            iInstant = instant;
            iField = field;
        }
        
        /**
         * Writes the property in a safe serialization format.
         */
        //    void writeObject(ObjectOutputStream oos) throws IOException {
        //        oos.writeObject(iInstant);
        //        oos.writeObject(iField->getType());
        //    }
        
        /**
         * Reads the property from a safe serialization format.
         */
        //    void readObject(ObjectInputStream oos) throws IOException, ClassNotFoundException {
        //        iInstant = (LocalTime) oos.readObject();
        //        DateTimeFieldType type = (DateTimeFieldType) oos.readObject();
        //        iField = type.getField(iInstant->getChronology());
        //    }
        
        //-----------------------------------------------------------------------
        /**
         * Gets the field being used.
         *
         * @return the field
         */
        const DateTimeField *getField() const {
            return iField;
        }
        
        /**
         * Gets the LocalTime object linked to this property.
         *
         * @return the linked LocalTime
         */
        LocalTime *getLocalTime() {
            return iInstant;
        }
        
        //-----------------------------------------------------------------------
        /**
         * Adds to this field in a copy of this LocalTime.
         * <p>
         * The LocalTime attached to this property is unchanged by this call.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the LocalTime with the field value changed
         */
        LocalTime *addCopy(int value) {
            return iInstant->withLocalMillis(iField->add(iInstant->getLocalMillis(), value));
        }
        
        /**
         * Adds to this field in a copy of this LocalTime.
         * If the addition exceeds the maximum value (eg. 23:59) it will
         * wrap to the minimum value (eg. 00:00).
         * <p>
         * The LocalTime attached to this property is unchanged by this call.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the LocalTime with the field value changed
         */
        LocalTime *addCopy(int64_t value) {
            return iInstant->withLocalMillis(iField->add(iInstant->getLocalMillis(), value));
        }
        
        /**
         * Adds to this field in a copy of this LocalTime.
         * If the addition exceeds the maximum value (eg. 23:59) then
         * an exception will be thrown.
         * Contrast this behaviour to {@link #addCopy(int)}.
         * <p>
         * The LocalTime attached to this property is unchanged by this call.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the LocalTime with the field value changed
         * @throws IllegalArgumentException if the result is invalid
         */
        LocalTime *addNoWrapToCopy(int value) {
            int64_t millis = iField->add(iInstant->getLocalMillis(), value);
            int64_t rounded = iInstant->getChronology()->millisOfDay()->get(millis);
            if (rounded != millis) {
                throw IllegalArgumentException("The addition exceeded the boundaries of LocalTime");
            }
            return iInstant->withLocalMillis(millis);
        }
        
        /**
         * Adds to this field, possibly wrapped, in a copy of this LocalTime.
         * A field wrapped operation only changes this field.
         * Thus 10:59 plusWrapField one minute goes to 10:00.
         * <p>
         * The LocalTime attached to this property is unchanged by this call.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the LocalTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        LocalTime *addWrapFieldToCopy(int value) {
            return iInstant->withLocalMillis(iField->addWrapField(iInstant->getLocalMillis(), value));
        }
        
        //-----------------------------------------------------------------------
        /**
         * Sets this field in a copy of the LocalTime.
         * <p>
         * The LocalTime attached to this property is unchanged by this call.
         *
         * @param value  the value to set the field in the copy to
         * @return a copy of the LocalTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        LocalTime *setCopy(int value) {
            return iInstant->withLocalMillis(iField->set(iInstant->getLocalMillis(), value));
        }
        
        /**
         * Sets this field in a copy of the LocalTime to a parsed text value.
         * <p>
         * The LocalTime attached to this property is unchanged by this call.
         *
         * @param text  the text value to set
         * @param locale  optional locale to use for selecting a text symbol
         * @return a copy of the LocalTime with the field value changed
         * @throws IllegalArgumentException if the text value isn't valid
         */
        LocalTime *setCopy(string text, Locale *locale) {
            return iInstant->withLocalMillis(iField->set(iInstant->getLocalMillis(), text, locale));
        }
        
        /**
         * Sets this field in a copy of the LocalTime to a parsed text value.
         * <p>
         * The LocalTime attached to this property is unchanged by this call.
         *
         * @param text  the text value to set
         * @return a copy of the LocalTime with the field value changed
         * @throws IllegalArgumentException if the text value isn't valid
         */
        LocalTime *setCopy(string text) {
            return setCopy(text, NULL);
        }
        
        //-----------------------------------------------------------------------
        /**
         * Returns a new LocalTime with this field set to the maximum value
         * for this field.
         * <p>
         * The LocalTime attached to this property is unchanged by this call.
         *
         * @return a copy of the LocalTime with this field set to its maximum
         */
        LocalTime *withMaximumValue() {
            return setCopy(getMaximumValue());
        }
        
        /**
         * Returns a new LocalTime with this field set to the minimum value
         * for this field.
         * <p>
         * The LocalTime attached to this property is unchanged by this call.
         *
         * @return a copy of the LocalTime with this field set to its minimum
         */
        LocalTime *withMinimumValue() {
            return setCopy(getMinimumValue());
        }
        
        //-----------------------------------------------------------------------
        /**
         * Rounds to the lowest whole unit of this field on a copy of this
         * LocalTime.
         * <p>
         * For example, rounding floor on the hourOfDay field of a LocalTime
         * where the time is 10:30 would result in new LocalTime with the
         * time of 10:00.
         *
         * @return a copy of the LocalTime with the field value changed
         */
        LocalTime *roundFloorCopy() {
            return iInstant->withLocalMillis(iField->roundFloor(iInstant->getLocalMillis()));
        }
        
        /**
         * Rounds to the highest whole unit of this field on a copy of this
         * LocalTime.
         * <p>
         * For example, rounding floor on the hourOfDay field of a LocalTime
         * where the time is 10:30 would result in new LocalTime with the
         * time of 11:00.
         *
         * @return a copy of the LocalTime with the field value changed
         */
        LocalTime *roundCeilingCopy() {
            return iInstant->withLocalMillis(iField->roundCeiling(iInstant->getLocalMillis()));
        }
        
        /**
         * Rounds to the nearest whole unit of this field on a copy of this
         * LocalTime, favoring the floor if halfway.
         *
         * @return a copy of the LocalTime with the field value changed
         */
        LocalTime *roundHalfFloorCopy() {
            return iInstant->withLocalMillis(iField->roundHalfFloor(iInstant->getLocalMillis()));
        }
        
        /**
         * Rounds to the nearest whole unit of this field on a copy of this
         * LocalTime, favoring the ceiling if halfway.
         *
         * @return a copy of the LocalTime with the field value changed
         */
        LocalTime *roundHalfCeilingCopy() {
            return iInstant->withLocalMillis(iField->roundHalfCeiling(iInstant->getLocalMillis()));
        }
        
        /**
         * Rounds to the nearest whole unit of this field on a copy of this
         * LocalTime.  If halfway, the ceiling is favored over the floor
         * only if it makes this field's value even.
         *
         * @return a copy of the LocalTime with the field value changed
         */
        LocalTime *roundHalfEvenCopy() {
            return iInstant->withLocalMillis(iField->roundHalfEven(iInstant->getLocalMillis()));
        }
        
    protected:
        
        /**
         * Gets the milliseconds of the time that this property is linked to.
         *
         * @return the milliseconds
         */
        int64_t getMillis() const {
            return iInstant->getLocalMillis();
        }
        
        /**
         * Gets the chronology of the datetime that this property is linked to.
         *
         * @return the chronology
         * @since 1.4
         */
        Chronology *getChronology() const {
            return iInstant->getChronology();
        }
    };
    
    /** Constant for midnight. */
    static const LocalTime *MIDNIGHT;
    
    static LocalTime *now();
    static LocalTime *now(DateTimeZone *zone);
    static LocalTime *now(Chronology *chronology);
    
    static LocalTime *parse(string str);
    static LocalTime *parse(string str, DateTimeFormatter *formatter);
    static LocalTime *fromMillisOfDay(int64_t millisOfDay);
    static LocalTime *fromMillisOfDay(int64_t millisOfDay, Chronology *chrono);
    
    LocalTime();
    LocalTime(DateTimeZone *zone);
    LocalTime(Chronology *chronology);
    LocalTime(int64_t instant);
    LocalTime(int64_t instant, DateTimeZone *zone);
    LocalTime(int64_t instant, Chronology *chronology);
    LocalTime(void *instant);
    LocalTime(void *instant, DateTimeZone *zone);
    LocalTime(void *instant, Chronology *chronology);
    LocalTime(int hourOfDay,
              int minuteOfHour);
    LocalTime(int hourOfDay,
              int minuteOfHour,
              int secondOfMinute);
    LocalTime(int hourOfDay,
              int minuteOfHour,
              int secondOfMinute,
              int millisOfSecond);
    LocalTime(int hourOfDay,
              int minuteOfHour,
              int secondOfMinute,
              int millisOfSecond,
              Chronology *chronology);
    
    int size() const;
    int getValue(int index) const;
    int get(const DateTimeFieldType *fieldType) const;
    bool isSupported(const DateTimeFieldType *type) const;
    bool isSupported(const DurationFieldType *type) const;
    Chronology *getChronology() const;
    bool equals(const Object *partial) const;
    int compareTo(const ReadablePartial *partial) const;
    
    LocalTime *withFields(ReadablePartial *partial);
    LocalTime *withField(DateTimeFieldType *fieldType, int value);
    LocalTime *withFieldAdded(DurationFieldType *fieldType, int amount);
    LocalTime *withPeriodAdded(ReadablePeriod *period, int scalar);
    
    LocalTime *plus(ReadablePeriod *period);
    LocalTime *plusHours(int hours);
    LocalTime *plusMinutes(int minutes);
    LocalTime *plusSeconds(int seconds);
    LocalTime *plusMillis(int millis);
    
    LocalTime *minus(ReadablePeriod *period);
    LocalTime *minusHours(int hours);
    LocalTime *minusMinutes(int minutes);
    LocalTime *minusSeconds(int seconds);
    LocalTime *minusMillis(int millis);
    
    Property *property(DateTimeFieldType *fieldType);
    
    int getHourOfDay();
    int getMinuteOfHour();
    int getSecondOfMinute();
    int getMillisOfSecond();
    int getMillisOfDay();
    
    LocalTime *withHourOfDay(int hour);
    LocalTime *withMinuteOfHour(int minute);
    LocalTime *withSecondOfMinute(int second);
    LocalTime *withMillisOfSecond(int millis);
    LocalTime *withMillisOfDay(int millis);
    
    Property *hourOfDay();
    Property *minuteOfHour();
    Property *secondOfMinute();
    Property *millisOfSecond();
    Property *millisOfDay();
    
    DateTime *toDateTime(ReadableInstant *baseInstant) { return AbstractPartial::toDateTime(baseInstant); }
    DateTime *toDateTimeToday();
    DateTime *toDateTimeToday(DateTimeZone *zone);
    
    string toString();
    string toString(string pattern);
    string toString(string pattern, Locale *locale);
};

CODATIME_END

#endif /* defined(__CodaTime__LocalTime__) */
