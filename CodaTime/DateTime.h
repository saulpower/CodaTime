//
//  DateTime.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__DateTime__
#define __CodaTime__DateTime__

#include "CodaTimeMacros.h"

#include "base/BaseDateTime.h"
#include "DateTimeUtils.h"
#include "Exceptions.h"
#include "field/AbstractReadableInstantFieldProperty.h"
#include "Locale.h"
#include "ReadableDateTime.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class LocalDateTime;
class LocalDate;
class LocalTime;

/**
 * DateTime is the standard implementation of an unmodifiable datetime class.
 * <p>
 * <code>DateTime</code> is the most widely used implementation of
 * {@link ReadableInstant}. As with all instants, it represents an exact
 * point on the time-line, but limited to the precision of milliseconds.
 * A <code>DateTime</code> calculates its fields with respect to a
 * {@link DateTimeZone time zone}.
 * <p>
 * Internally, the class holds two pieces of data. Firstly, it holds the
 * datetime as milliseconds from the Java epoch of 1970-01-01T00:00:00Z.
 * Secondly, it holds a {@link Chronology} which determines how the
 * millisecond instant value is converted into the date time fields.
 * The default Chronology is {@link ISOChronology} which is the agreed
 * international standard and compatible with the modern Gregorian calendar.
 * <p>
 * Each individual field can be queried in two ways:
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
 * <p>
 * DateTime is thread-safe and immutable, provided that the Chronology is as well.
 * All standard Chronology classes supplied are thread-safe and immutable.
 *
 * @author Stephen Colebourne
 * @author Kandarp Shah
 * @author Brian S O'Neill
 * @since 1.0
 * @see MutableDateTime
 */
class DateTime : public BaseDateTime, public virtual ReadableDateTime {
    
private:
    
    /** Serialization lock */
    static const long long serialVersionUID = -5171125899451703815L;
    
public:
    
    /**
     * DateTime.Property binds a DateTime to a DateTimeField allowing powerful
     * datetime functionality to be easily accessed.
     * <p>
     * The simplest use of this class is as an alternative get method, here used to
     * get the year '1972' (as an int) and the month 'December' (as a string).
     * <pre>
     * DateTime dt = new DateTime(1972, 12, 3, 0, 0, 0, 0);
     * int year = dt.year().get();
     * string monthStr = dt.month().getAsText();
     * </pre>
     * <p>
     * Methods are also provided that allow date modification. These return new instances
     * of DateTime - they do not modify the original. The example below yields two
     * independent immutable date objects 20 years apart.
     * <pre>
     * DateTime dt = new DateTime(1972, 12, 3, 0, 0, 0, 0);
     * DateTime dt20 = dt.year().addToCopy(20);
     * </pre>
     * Serious modification of dates (ie. more than just changing one or two fields)
     * should use the {@link org.joda.time.MutableDateTime MutableDateTime} class.
     * <p>
     * DateTime.Propery itself is thread-safe and immutable, as well as the
     * DateTime being operated on.
     *
     * @author Stephen Colebourne
     * @author Brian S O'Neill
     * @since 1.0
     */
    class Property : public AbstractReadableInstantFieldProperty {
        
    private:
        
        /** Serialization version */
        static const long long serialVersionUID = -6983323811635733510L;
        
        /** The instant this property is working against */
        DateTime *iInstant;
        /** The field this property is working against */
        const DateTimeField *iField;
        
        /**
         * Writes the property in a safe serialization format.
         */
        //    void writeObject(ObjectOutputStream oos) throws IOException {
        //        oos.writeObject(iInstant);
        //        oos.writeObject(iField->getType());
        //    }
        //
        //    /**
        //     * Reads the property from a safe serialization format.
        //     */
        //    void readObject(ObjectInputStream oos) throws IOException, ClassNotFoundException {
        //        iInstant = (DateTime) oos.readObject();
        //        DateTimeFieldType type = (DateTimeFieldType) oos.readObject();
        //        iField = type.getField(iInstant->getChronology());
        //    }
        
    public:
        
        /**
         * Constructor.
         *
         * @param instant  the instant to set
         * @param field  the field to use
         */
        Property(DateTime *instant, const DateTimeField *field) : AbstractReadableInstantFieldProperty() {
            iInstant = instant;
            iField = field;
        }
        
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
         * Gets the datetime being used.
         *
         * @return the datetime
         */
        DateTime *getDateTime() {
            return iInstant;
        }
        
        //-----------------------------------------------------------------------
        /**
         * Adds to this field in a copy of this DateTime.
         * <p>
         * The DateTime attached to this property is unchanged by this call.
         * This operation is faster than converting a DateTime to a MutableDateTime
         * and back again when setting one field. When setting multiple fields,
         * it is generally quicker to make the conversion to MutableDateTime.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the DateTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        DateTime *addToCopy(int value) {
            return iInstant->withMillis(iField->add(iInstant->getMillis(), value));
        }
        
        /**
         * Adds to this field in a copy of this DateTime.
         * <p>
         * The DateTime attached to this property is unchanged by this call.
         * This operation is faster than converting a DateTime to a MutableDateTime
         * and back again when setting one field. When setting multiple fields,
         * it is generally quicker to make the conversion to MutableDateTime.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the DateTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        DateTime *addToCopy(int64_t value) {
            return iInstant->withMillis(iField->add(iInstant->getMillis(), value));
        }
        
        /**
         * Adds to this field, possibly wrapped, in a copy of this DateTime.
         * A wrapped operation only changes this field.
         * Thus 31st January addWrapField one day goes to the 1st January.
         * <p>
         * The DateTime attached to this property is unchanged by this call.
         * This operation is faster than converting a DateTime to a MutableDateTime
         * and back again when setting one field. When setting multiple fields,
         * it is generally quicker to make the conversion to MutableDateTime.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the DateTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        DateTime *addWrapFieldToCopy(int value) {
            return iInstant->withMillis(iField->addWrapField(iInstant->getMillis(), value));
        }
        
        //-----------------------------------------------------------------------
        /**
         * Sets this field in a copy of the DateTime.
         * <p>
         * The DateTime attached to this property is unchanged by this call.
         * This operation is faster than converting a DateTime to a MutableDateTime
         * and back again when setting one field. When setting multiple fields,
         * it is generally quicker to make the conversion to MutableDateTime.
         *
         * @param value  the value to set the field in the copy to
         * @return a copy of the DateTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        DateTime *setCopy(int value) {
            return iInstant->withMillis(iField->set(iInstant->getMillis(), value));
        }
        
        /**
         * Sets this field in a copy of the DateTime to a parsed text value.
         * <p>
         * The DateTime attached to this property is unchanged by this call.
         * This operation is faster than converting a DateTime to a MutableDateTime
         * and back again when setting one field. When setting multiple fields,
         * it is generally quicker to make the conversion to MutableDateTime.
         *
         * @param text  the text value to set
         * @param Locale * optional Locale *to use for selecting a text symbol
         * @return a copy of the DateTime with the field value changed
         * @throws IllegalArgumentException if the text value isn't valid
         */
        DateTime *setCopy(string text, Locale *locale) {
            return iInstant->withMillis(iField->set(iInstant->getMillis(), text, locale));
        }
        
        /**
         * Sets this field in a copy of the DateTime to a parsed text value.
         * <p>
         * The DateTime attached to this property is unchanged by this call.
         * This operation is faster than converting a DateTime to a MutableDateTime
         * and back again when setting one field. When setting multiple fields,
         * it is generally quicker to make the conversion to MutableDateTime.
         *
         * @param text  the text value to set
         * @return a copy of the DateTime with the field value changed
         * @throws IllegalArgumentException if the text value isn't valid
         */
        DateTime *setCopy(string text) {
            return setCopy(text, NULL);
        }
        
        //-----------------------------------------------------------------------
        /**
         * Returns a new DateTime with this field set to the maximum value
         * for this field.
         * <p>
         * This operation is useful for obtaining a DateTime on the last day
         * of the month, as month lengths vary.
         * <pre>
         * DateTime lastDayOfMonth = dt.dayOfMonth().withMaximumValue();
         * </pre>
         * <p>
         * Where possible, the offset from UTC will be retained, thus applications
         * may need to call {@link DateTime#withLaterOffsetAtOverlap()} on the result
         * to force the later time during a DST overlap if desired.
         * <p>
         * From v2.2, this method handles a daylight svaings time gap, setting the
         * time to the last instant before the gap.
         * <p>
         * The DateTime attached to this property is unchanged by this call.
         *
         * @return a copy of the DateTime with this field set to its maximum
         * @since 1.2
         */
        DateTime *withMaximumValue() {
            try {
                return setCopy(getMaximumValue());
            } catch (exception ex) {
                //            if (IllegalInstantException::isIllegalInstant(ex)) {
                // adding MILLIS_PER_DAY is not perfect, but will work in almost all situations
                int64_t beforeGap = getChronology()->getZone()->previousTransition(getMillis() + DateTimeConstants::MILLIS_PER_DAY);
                return new DateTime(beforeGap, getChronology());
                //            }
                //            throw ex;
            }
        }
        
        /**
         * Returns a new DateTime with this field set to the minimum value
         * for this field.
         * <p>
         * Where possible, the offset from UTC will be retained, thus applications
         * may need to call {@link DateTime#withEarlierOffsetAtOverlap()} on the result
         * to force the earlier time during a DST overlap if desired.
         * <p>
         * From v2.2, this method handles a daylight svaings time gap, setting the
         * time to the first instant after the gap.
         * <p>
         * The DateTime attached to this property is unchanged by this call.
         *
         * @return a copy of the DateTime with this field set to its minimum
         * @since 1.2
         */
        DateTime *withMinimumValue() {
            try {
                return setCopy(getMinimumValue());
            } catch (exception ex) {
                //            if (IllegalInstantException::isIllegalInstant(ex)) {
                // subtracting MILLIS_PER_DAY is not perfect, but will work in almost all situations
                int64_t afterGap = getChronology()->getZone()->nextTransition(getMillis() - DateTimeConstants::MILLIS_PER_DAY);
                return new DateTime(afterGap, getChronology());
                //            }
                //            throw ex;
            }
        }
        
        //-----------------------------------------------------------------------
        /**
         * Rounds to the lowest whole unit of this field on a copy of this DateTime.
         *
         * @return a copy of the DateTime with the field value changed
         */
        DateTime *roundFloorCopy() {
            return iInstant->withMillis(iField->roundFloor(iInstant->getMillis()));
        }
        
        /**
         * Rounds to the highest whole unit of this field on a copy of this DateTime.
         *
         * @return a copy of the DateTime with the field value changed
         */
        DateTime *roundCeilingCopy() {
            return iInstant->withMillis(iField->roundCeiling(iInstant->getMillis()));
        }
        
        /**
         * Rounds to the nearest whole unit of this field on a copy of this DateTime,
         * favoring the floor if halfway.
         *
         * @return a copy of the DateTime with the field value changed
         */
        DateTime *roundHalfFloorCopy() {
            return iInstant->withMillis(iField->roundHalfFloor(iInstant->getMillis()));
        }
        
        /**
         * Rounds to the nearest whole unit of this field on a copy of this DateTime,
         * favoring the ceiling if halfway.
         *
         * @return a copy of the DateTime with the field value changed
         */
        DateTime *roundHalfCeilingCopy() {
            return iInstant->withMillis(iField->roundHalfCeiling(iInstant->getMillis()));
        }
        
        /**
         * Rounds to the nearest whole unit of this field on a copy of this
         * DateTime.  If halfway, the ceiling is favored over the floor only if
         * it makes this field's value even.
         *
         * @return a copy of the DateTime with the field value changed
         */
        DateTime *roundHalfEvenCopy() {
            return iInstant->withMillis(iField->roundHalfEven(iInstant->getMillis()));
        }
        
    protected:
        
        /**
         * Gets the milliseconds of the datetime that this property is linked to.
         *
         * @return the milliseconds
         */
        int64_t getMillis() const {
            return iInstant->getMillis();
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
    
    /**
     * Obtains a {@code DateTime} set to the current system millisecond time
     * using <code>ISOChronology</code> in the default time zone.
     *
     * @return the current date-time, not null
     * @since 2.0
     */
    static DateTime *now() { return new DateTime(); }
    
    static DateTime *now(DateTimeZone *zone);
    static DateTime *now(Chronology *chronology);
    static DateTime *parse(string str);
    static DateTime *parse(string str, DateTimeFormatter *formatter);
    
    int compareTo(const ReadableInstant *other) const { return AbstractInstant::compareTo(other); }
    
    int get(const DateTimeFieldType *type) const { return AbstractInstant::get(type); }
    
    bool equals(const Object *readableInstant) const { return AbstractInstant::equals(readableInstant); }
    
    MutableDateTime *toMutableDateTime() { return AbstractInstant::toMutableDateTime(); }
    
    /**
     * Constructs an instance set to the current system millisecond time
     * using <code>ISOChronology</code> in the default time zone.
     *
     * @see #now()
     */
    DateTime() : BaseDateTime() {}
    
    /**
     * Constructs an instance set to the current system millisecond time
     * using <code>ISOChronology</code> in the specified time zone.
     * <p>
     * If the specified time zone is null, the default zone is used.
     *
     * @param zone  the time zone, null means default zone
     * @see #now(DateTimeZone)
     */
    DateTime(DateTimeZone *zone) : BaseDateTime(zone) {}
    
    /**
     * Constructs an instance set to the current system millisecond time
     * using the specified chronology.
     * <p>
     * If the chronology is null, <code>ISOChronology</code>
     * in the default time zone is used.
     *
     * @param chronology  the chronology, null means ISOChronology in default zone
     * @see #now(Chronology)
     */
    DateTime(Chronology *chronology) : BaseDateTime(chronology) {}
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance set to the milliseconds from 1970-01-01T00:00:00Z
     * using <code>ISOChronology</code> in the default time zone.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z
     */
    DateTime(int64_t instant) : BaseDateTime(instant) {}
    
    /**
     * Constructs an instance set to the milliseconds from 1970-01-01T00:00:00Z
     * using <code>ISOChronology</code> in the specified time zone.
     * <p>
     * If the specified time zone is null, the default zone is used.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z
     * @param zone  the time zone, null means default zone
     */
    DateTime(int64_t instant, DateTimeZone *zone) : BaseDateTime(instant, zone) {}
    
    /**
     * Constructs an instance set to the milliseconds from 1970-01-01T00:00:00Z
     * using the specified chronology.
     * <p>
     * If the chronology is null, <code>ISOChronology</code>
     * in the default time zone is used.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z
     * @param chronology  the chronology, null means ISOChronology in default zone
     */
    DateTime(int64_t instant, Chronology *chronology) : BaseDateTime(instant, chronology) {}
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance from an Object that represents a datetime.
     * <p>
     * If the object implies a chronology (such as GregorianCalendar does),
     * then that chronology will be used. Otherwise, ISO default is used.
     * Thus if a GregorianCalendar is passed in, the chronology used will
     * be GJ, but if a Date is passed in the chronology will be ISO.
     * <p>
     * The recognised object types are defined in
     * {@link org.joda.time.convert.ConverterManager ConverterManager} and
     * include ReadableInstant, string, Calendar and Date.
     * The string formats are described by {@link ISODateTimeFormat#dateTimeParser()}.
     *
     * @param instant  the DateTime *object, null means now
     * @throws IllegalArgumentException if the instant is invalid
     */
    DateTime(Object *instant) : BaseDateTime(instant, (Chronology*) NULL) {}
    
    /**
     * Constructs an instance from an Object that represents a datetime,
     * forcing the time zone to that specified.
     * <p>
     * If the object implies a chronology (such as GregorianCalendar does),
     * then that chronology will be used, but with the time zone adjusted.
     * Otherwise, ISO is used in the specified time zone.
     * If the specified time zone is null, the default zone is used.
     * Thus if a GregorianCalendar is passed in, the chronology used will
     * be GJ, but if a Date is passed in the chronology will be ISO.
     * <p>
     * The recognised object types are defined in
     * {@link org.joda.time.convert.ConverterManager ConverterManager} and
     * include ReadableInstant, string, Calendar and Date.
     * The string formats are described by {@link ISODateTimeFormat#dateTimeParser()}.
     *
     * @param instant  the DateTime *object, null means now
     * @param zone  the time zone, null means default time zone
     * @throws IllegalArgumentException if the instant is invalid
     */
    DateTime(Object *instant, DateTimeZone *zone) : BaseDateTime(instant, zone) {}
    
    /**
     * Constructs an instance from an Object that represents a datetime,
     * using the specified chronology.
     * <p>
     * If the chronology is null, ISO in the default time zone is used.
     * Any chronology implied by the object (such as GregorianCalendar does)
     * is ignored.
     * <p>
     * The recognised object types are defined in
     * {@link org.joda.time.convert.ConverterManager ConverterManager} and
     * include ReadableInstant, string, Calendar and Date.
     * The string formats are described by {@link ISODateTimeFormat#dateTimeParser()}.
     *
     * @param instant  the DateTime *object, null means now
     * @param chronology  the chronology, null means ISO in default zone
     * @throws IllegalArgumentException if the instant is invalid
     */
    DateTime(Object *instant, Chronology *chronology) : BaseDateTime(instant, DateTimeUtils::getChronology(chronology)) {}
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance from DateTime *field values
     * using <code>ISOChronology</code> in the default time zone.
     *
     * @param year  the year
     * @param monthOfYear  the month of the year, from 1 to 12
     * @param dayOfMonth  the day of the month, from 1 to 31
     * @param hourOfDay  the hour of the day, from 0 to 23
     * @param minuteOfHour  the minute of the hour, from 0 to 59
     * @since 2.0
     */
    DateTime(int year,
                    int monthOfYear,
                    int dayOfMonth,
                    int hourOfDay,
                    int minuteOfHour) : BaseDateTime(year, monthOfYear, dayOfMonth, hourOfDay, minuteOfHour, 0, 0) {}
    
    /**
     * Constructs an instance from DateTime *field values
     * using <code>ISOChronology</code> in the specified time zone.
     * <p>
     * If the specified time zone is null, the default zone is used.
     *
     * @param year  the year
     * @param monthOfYear  the month of the year, from 1 to 12
     * @param dayOfMonth  the day of the month, from 1 to 31
     * @param hourOfDay  the hour of the day, from 0 to 23
     * @param minuteOfHour  the minute of the hour, from 0 to 59
     * @param zone  the time zone, null means default time zone
     * @since 2.0
     */
    DateTime(int year,
                    int monthOfYear,
                    int dayOfMonth,
                    int hourOfDay,
                    int minuteOfHour,
                    DateTimeZone *zone) : BaseDateTime(year, monthOfYear, dayOfMonth,
                                                       hourOfDay, minuteOfHour, 0, 0, zone) {}
    
    /**
     * Constructs an instance from DateTime *field values
     * using the specified chronology.
     * <p>
     * If the chronology is null, <code>ISOChronology</code>
     * in the default time zone is used.
     *
     * @param year  the year, valid values defined by the chronology
     * @param monthOfYear  the month of the year, valid values defined by the chronology
     * @param dayOfMonth  the day of the month, valid values defined by the chronology
     * @param hourOfDay  the hour of the day, valid values defined by the chronology
     * @param minuteOfHour  the minute of the hour, valid values defined by the chronology
     * @param chronology  the chronology, null means ISOChronology in default zone
     * @since 2.0
     */
    DateTime(int year,
                    int monthOfYear,
                    int dayOfMonth,
                    int hourOfDay,
                    int minuteOfHour,
                    Chronology *chronology) : BaseDateTime(year, monthOfYear, dayOfMonth,
                                                           hourOfDay, minuteOfHour, 0, 0, chronology) {}
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance from DateTime *field values
     * using <code>ISOChronology</code> in the default time zone.
     *
     * @param year  the year
     * @param monthOfYear  the month of the year, from 1 to 12
     * @param dayOfMonth  the day of the month, from 1 to 31
     * @param hourOfDay  the hour of the day, from 0 to 23
     * @param minuteOfHour  the minute of the hour, from 0 to 59
     * @param secondOfMinute  the second of the minute, from 0 to 59
     * @since 2.0
     */
    DateTime(int year,
                    int monthOfYear,
                    int dayOfMonth,
                    int hourOfDay,
                    int minuteOfHour,
             int secondOfMinute) : BaseDateTime(year, monthOfYear, dayOfMonth, hourOfDay, minuteOfHour, secondOfMinute, 0) {}
    
    /**
     * Constructs an instance from DateTime *field values
     * using <code>ISOChronology</code> in the specified time zone.
     * <p>
     * If the specified time zone is null, the default zone is used.
     *
     * @param year  the year
     * @param monthOfYear  the month of the year, from 1 to 12
     * @param dayOfMonth  the day of the month, from 1 to 31
     * @param hourOfDay  the hour of the day, from 0 to 23
     * @param minuteOfHour  the minute of the hour, from 0 to 59
     * @param secondOfMinute  the second of the minute, from 0 to 59
     * @param zone  the time zone, null means default time zone
     * @since 2.0
     */
    DateTime(int year,
                    int monthOfYear,
                    int dayOfMonth,
                    int hourOfDay,
                    int minuteOfHour,
                    int secondOfMinute,
                    DateTimeZone *zone) : BaseDateTime(year, monthOfYear, dayOfMonth,
                                                       hourOfDay, minuteOfHour, secondOfMinute, 0, zone) {}
    
    /**
     * Constructs an instance from DateTime *field values
     * using the specified chronology.
     * <p>
     * If the chronology is null, <code>ISOChronology</code>
     * in the default time zone is used.
     *
     * @param year  the year, valid values defined by the chronology
     * @param monthOfYear  the month of the year, valid values defined by the chronology
     * @param dayOfMonth  the day of the month, valid values defined by the chronology
     * @param hourOfDay  the hour of the day, valid values defined by the chronology
     * @param minuteOfHour  the minute of the hour, valid values defined by the chronology
     * @param secondOfMinute  the second of the minute, valid values defined by the chronology
     * @param chronology  the chronology, null means ISOChronology in default zone
     * @since 2.0
     */
    DateTime(int year,
                    int monthOfYear,
                    int dayOfMonth,
                    int hourOfDay,
                    int minuteOfHour,
                    int secondOfMinute,
             Chronology *chronology) : BaseDateTime(year, monthOfYear, dayOfMonth,
                                                    hourOfDay, minuteOfHour, secondOfMinute, 0, chronology) {}
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance from DateTime *field values
     * using <code>ISOChronology</code> in the default time zone.
     *
     * @param year  the year
     * @param monthOfYear  the month of the year, from 1 to 12
     * @param dayOfMonth  the day of the month, from 1 to 31
     * @param hourOfDay  the hour of the day, from 0 to 23
     * @param minuteOfHour  the minute of the hour, from 0 to 59
     * @param secondOfMinute  the second of the minute, from 0 to 59
     * @param millisOfSecond  the millisecond of the second, from 0 to 999
     */
    DateTime(int year,
                    int monthOfYear,
                    int dayOfMonth,
                    int hourOfDay,
                    int minuteOfHour,
                    int secondOfMinute,
             int millisOfSecond) : BaseDateTime(year, monthOfYear, dayOfMonth, hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond) {}
    
    /**
     * Constructs an instance from DateTime *field values
     * using <code>ISOChronology</code> in the specified time zone.
     * <p>
     * If the specified time zone is null, the default zone is used.
     *
     * @param year  the year
     * @param monthOfYear  the month of the year, from 1 to 12
     * @param dayOfMonth  the day of the month, from 1 to 31
     * @param hourOfDay  the hour of the day, from 0 to 23
     * @param minuteOfHour  the minute of the hour, from 0 to 59
     * @param secondOfMinute  the second of the minute, from 0 to 59
     * @param millisOfSecond  the millisecond of the second, from 0 to 999
     * @param zone  the time zone, null means default time zone
     */
    DateTime(int year,
                    int monthOfYear,
                    int dayOfMonth,
                    int hourOfDay,
                    int minuteOfHour,
                    int secondOfMinute,
                    int millisOfSecond,
                    DateTimeZone *zone) : BaseDateTime(year, monthOfYear, dayOfMonth,
                                                       hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond, zone) {}
    
    /**
     * Constructs an instance from DateTime *field values
     * using the specified chronology.
     * <p>
     * If the chronology is null, <code>ISOChronology</code>
     * in the default time zone is used.
     *
     * @param year  the year, valid values defined by the chronology
     * @param monthOfYear  the month of the year, valid values defined by the chronology
     * @param dayOfMonth  the day of the month, valid values defined by the chronology
     * @param hourOfDay  the hour of the day, valid values defined by the chronology
     * @param minuteOfHour  the minute of the hour, valid values defined by the chronology
     * @param secondOfMinute  the second of the minute, valid values defined by the chronology
     * @param millisOfSecond  the millisecond of the second, valid values defined by the chronology
     * @param chronology  the chronology, null means ISOChronology in default zone
     */
    DateTime(int year,
                    int monthOfYear,
                    int dayOfMonth,
                    int hourOfDay,
                    int minuteOfHour,
                    int secondOfMinute,
                    int millisOfSecond,
                    Chronology *chronology) : BaseDateTime(year, monthOfYear, dayOfMonth,
                                                           hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond, chronology) {}
    
    //-----------------------------------------------------------------------
    /**
     * Get this object as a DateTime *by returning <code>this</code>.
     *
     * @return <code>this</code>
     */
    DateTime *toDateTime() { return this; }
    
    DateTime *toDateTimeISO();
    DateTime *toDateTime(DateTimeZone *zone);
    DateTime *toDateTime(Chronology *chronology);
    
    DateTime *withMillis(int64_t newMillis);
    DateTime *withChronology(Chronology *newChronology);
    DateTime *withZone(DateTimeZone *newZone);
    DateTime *withZoneRetainFields(DateTimeZone *newZone);
    DateTime *withEarlierOffsetAtOverlap();
    DateTime *withLaterOffsetAtOverlap();
    DateTime *withDate(int year, int monthOfYear, int dayOfMonth);
    DateTime *withTime(int hourOfDay, int minuteOfHour, int secondOfMinute, int millisOfSecond);
    DateTime *withTimeAtStartOfDay();
    DateTime *withFields(ReadablePartial *partial);
    DateTime *withField(DateTimeFieldType *fieldType, int value);
    DateTime *withFieldAdded(DurationFieldType *fieldType, int amount);
    DateTime *withDurationAdded(int64_t durationToAdd, int scalar);
    DateTime *withDurationAdded(ReadableDuration *durationToAdd, int scalar);
    DateTime *withPeriodAdded(ReadablePeriod *period, int scalar);
    
    DateTime *plus(int64_t duration);
    DateTime *plus(ReadableDuration *duration);
    DateTime *plus(ReadablePeriod *period);
    DateTime *plusYears(int years);
    DateTime *plusMonths(int months);
    DateTime *plusWeeks(int weeks);
    DateTime *plusDays(int days);
    DateTime *plusHours(int hours);
    DateTime *plusMinutes(int minutes);
    DateTime *plusSeconds(int seconds);
    DateTime *plusMillis(int millis);
    
    DateTime *minus(int64_t duration);
    DateTime *minus(ReadableDuration *duration);
    DateTime *minus(ReadablePeriod *period);
    DateTime *minusYears(int years);
    DateTime *minusMonths(int months);
    DateTime *minusWeeks(int weeks);
    DateTime *minusDays(int days);
    DateTime *minusHours(int hours);
    DateTime *minusMinutes(int minutes);
    DateTime *minusSeconds(int seconds);
    DateTime *minusMillis(int millis);
    
    Property *property(DateTimeFieldType *type);
    
    LocalDateTime *toLocalDateTime();
    LocalDate *toLocalDate();
    LocalTime *toLocalTime();
    
    DateTime *withEra(int era);
    DateTime *withCenturyOfEra(int centuryOfEra);
    DateTime *withYearOfEra(int yearOfEra);
    DateTime *withYearOfCentury(int yearOfCentury);
    DateTime *withYear(int year);
    DateTime *withWeekyear(int weekyear);
    DateTime *withMonthOfYear(int monthOfYear);
    DateTime *withWeekOfWeekyear(int weekOfWeekyear);
    DateTime *withDayOfYear(int dayOfYear);
    DateTime *withDayOfMonth(int dayOfMonth);
    DateTime *withDayOfWeek(int dayOfWeek);
    DateTime *withHourOfDay(int hour);
    DateTime *withMinuteOfHour(int minute);
    DateTime *withSecondOfMinute(int second);
    DateTime *withMillisOfSecond(int millis);
    DateTime *withMillisOfDay(int millis);
    
    Property *era();
    Property *centuryOfEra();
    Property *yearOfCentury();
    Property *yearOfEra();
    Property *year();
    Property *weekyear();
    Property *monthOfYear();
    Property *weekOfWeekyear();
    Property *dayOfYear();
    Property *dayOfMonth();
    Property *dayOfWeek();
    Property *hourOfDay();
    Property *minuteOfDay();
    Property *minuteOfHour();
    Property *secondOfDay();
    Property *secondOfMinute();
    Property *millisOfDay();
    Property *millisOfSecond();
};

CODATIME_END

#endif /* defined(__CodaTime__DateTime__) */
