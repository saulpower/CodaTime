//
//  LocalDateTime.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__LocalDateTime__
#define __CodaTime__LocalDateTime__

#include "CodaTimeMacros.h"

#include "base/BaseLocal.h"
#include "chrono/ISOChronology.h"
#include "DateTimeField.h"
#include "DateTimeFieldType.h"
#include "DateTimeUtils.h"
#include "DurationFieldType.h"
#include "Exceptions.h"
#include "field/AbstractReadableInstantFieldProperty.h"
#include "LocalTime.h"
#include "ReadableDuration.h"
#include "ReadablePartial.h"

#include <string>
#include <vector>

using namespace std;

CODATIME_BEGIN

class ReadablePeriod;
class ReadablePartial;

class LocalDateTime : public BaseLocal, public ReadablePartial {
    
private:
    
    /** Serialization lock */
    static const long long serialVersionUID = -268716875315837168L;
    
    /** The index of the year field in the field array */
    static const int YEAR = 0;
    /** The index of the monthOfYear field in the field array */
    static const int MONTH_OF_YEAR = 1;
    /** The index of the dayOfMonth field in the field array */
    static const int DAY_OF_MONTH = 2;
    /** The index of the millis field in the field array */
    static const int MILLIS_OF_DAY = 3;
    
    /** The local millis from 1970-01-01T00:00:00 */
    const int64_t iLocalMillis;
    /** The chronology to use in UTC */
    Chronology *iChronology;
    
    Object *readResolve();
    
    int64_t init(int64_t instant, Chronology *chronology);
    int64_t init(Object *instant, DateTimeZone *zone);
    int64_t init(Object *instant, Chronology *chronology);
    int64_t init(int year,
                 int monthOfYear,
                 int dayOfMonth,
                 int hourOfDay,
                 int minuteOfHour,
                 int secondOfMinute,
                 int millisOfSecond,
                 Chronology *chronology);
    
public:
    
    /**
     * LocalDateTime.Property binds a LocalDateTime to a DateTimeField allowing
     * powerful datetime functionality to be easily accessed.
     * <p>
     * The simplest use of this class is as an alternative get method, here used to
     * get the year '1972' (as an int) and the month 'December' (as a string).
     * <pre>
     * LocalDateTime dt = new LocalDateTime(1972, 12, 3, 0, 0);
     * int year = dt.year().get();
     * string monthStr = dt.month().getAsText();
     * </pre>
     * <p>
     * Methods are also provided that allow date modification. These return
     * new instances of LocalDateTime - they do not modify the original.
     * The example below yields two independent immutable date objects
     * 20 years apart.
     * <pre>
     * LocalDateTime dt = new LocalDateTime(1972, 12, 3, 0, 0);
     * LocalDateTime dt1920 = dt.year().setCopy(1920);
     * </pre>
     * <p>
     * LocalDateTime.Property itself is thread-safe and immutable, as well as the
     * LocalDateTime being operated on.
     *
     * @author Stephen Colebourne
     * @author Brian S O'Neill
     * @since 1.3
     */
    class Property : public AbstractReadableInstantFieldProperty {
        
    private:
        
        /** Serialization version */
        static const long long serialVersionUID = -358138762846288L;
        
        /** The instant this property is working against */
        LocalDateTime *iInstant;
        /** The field this property is working against */
        const DateTimeField *iField;
        
        /**
         * Writes the property in a safe serialization format.
         */
        //    private void writeObject(ObjectOutputStream oos) throws IOException {
        //        oos.writeObject(iInstant);
        //        oos.writeObject(iField->getType());
        //    }
        
        /**
         * Reads the property from a safe serialization format.
         */
        //    private void readObject(ObjectInputStream oos) throws IOException, ClassNotFoundException {
        //        iInstant = (LocalDateTime) oos.readObject();
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
        Property(LocalDateTime *instant, const DateTimeField *field) : AbstractReadableInstantFieldProperty() {
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
         * Gets the LocalDateTime object linked to this property.
         *
         * @return the linked LocalDateTime
         */
        LocalDateTime *getLocalDateTime() {
            return iInstant;
        }
        
        //-----------------------------------------------------------------------
        /**
         * Adds to this field in a copy of this LocalDateTime.
         * <p>
         * The LocalDateTime attached to this property is unchanged by this call.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the LocalDateTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        LocalDateTime *addToCopy(int value) {
            return iInstant->withLocalMillis(iField->add(iInstant->getLocalMillis(), value));
        }
        
        /**
         * Adds to this field in a copy of this LocalDateTime.
         * <p>
         * The LocalDateTime attached to this property is unchanged by this call.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the LocalDateTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        LocalDateTime *addToCopy(int64_t value) {
            return iInstant->withLocalMillis(iField->add(iInstant->getLocalMillis(), value));
        }
        
        /**
         * Adds to this field, possibly wrapped, in a copy of this LocalDateTime.
         * A field wrapped operation only changes this field.
         * Thus 31st January addWrapField one day goes to the 1st January.
         * <p>
         * The LocalDateTime attached to this property is unchanged by this call.
         *
         * @param value  the value to add to the field in the copy
         * @return a copy of the LocalDateTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        LocalDateTime *addWrapFieldToCopy(int value) {
            return iInstant->withLocalMillis(iField->addWrapField(iInstant->getLocalMillis(), value));
        }
        
        //-----------------------------------------------------------------------
        /**
         * Sets this field in a copy of the LocalDateTime.
         * <p>
         * The LocalDateTime attached to this property is unchanged by this call.
         *
         * @param value  the value to set the field in the copy to
         * @return a copy of the LocalDateTime with the field value changed
         * @throws IllegalArgumentException if the value isn't valid
         */
        LocalDateTime *setCopy(int value) {
            return iInstant->withLocalMillis(iField->set(iInstant->getLocalMillis(), value));
        }
        
        /**
         * Sets this field in a copy of the LocalDateTime to a parsed text value.
         * <p>
         * The LocalDateTime attached to this property is unchanged by this call.
         *
         * @param text  the text value to set
         * @param locale  optional locale to use for selecting a text symbol
         * @return a copy of the LocalDateTime with the field value changed
         * @throws IllegalArgumentException if the text value isn't valid
         */
        LocalDateTime *setCopy(string text, Locale *locale) {
            return iInstant->withLocalMillis(iField->set(iInstant->getLocalMillis(), text, locale));
        }
        
        /**
         * Sets this field in a copy of the LocalDateTime to a parsed text value.
         * <p>
         * The LocalDateTime attached to this property is unchanged by this call.
         *
         * @param text  the text value to set
         * @return a copy of the LocalDateTime with the field value changed
         * @throws IllegalArgumentException if the text value isn't valid
         */
        LocalDateTime *setCopy(string text) {
            return setCopy(text, NULL);
        }
        
        //-----------------------------------------------------------------------
        /**
         * Returns a new LocalDateTime with this field set to the maximum value
         * for this field.
         * <p>
         * This operation is useful for obtaining a LocalDateTime on the last day
         * of the month, as month lengths vary.
         * <pre>
         * LocalDateTime lastDayOfMonth = dt.dayOfMonth().withMaximumValue();
         * </pre>
         * <p>
         * The LocalDateTime attached to this property is unchanged by this call.
         *
         * @return a copy of the LocalDateTime with this field set to its maximum
         */
        LocalDateTime *withMaximumValue() {
            return setCopy(getMaximumValue());
        }
        
        /**
         * Returns a new LocalDateTime with this field set to the minimum value
         * for this field.
         * <p>
         * The LocalDateTime attached to this property is unchanged by this call.
         *
         * @return a copy of the LocalDateTime with this field set to its minimum
         */
        LocalDateTime *withMinimumValue() {
            return setCopy(getMinimumValue());
        }
        
        //-----------------------------------------------------------------------
        /**
         * Rounds to the lowest whole unit of this field on a copy of this
         * LocalDateTime.
         * <p>
         * For example, rounding floor on the hourOfDay field of a LocalDateTime
         * where the time is 10:30 would result in new LocalDateTime with the
         * time of 10:00.
         *
         * @return a copy of the LocalDateTime with the field value changed
         */
        LocalDateTime *roundFloorCopy() {
            return iInstant->withLocalMillis(iField->roundFloor(iInstant->getLocalMillis()));
        }
        
        /**
         * Rounds to the highest whole unit of this field on a copy of this
         * LocalDateTime.
         * <p>
         * For example, rounding floor on the hourOfDay field of a LocalDateTime
         * where the time is 10:30 would result in new LocalDateTime with the
         * time of 11:00.
         *
         * @return a copy of the LocalDateTime with the field value changed
         */
        LocalDateTime *roundCeilingCopy() {
            return iInstant->withLocalMillis(iField->roundCeiling(iInstant->getLocalMillis()));
        }
        
        /**
         * Rounds to the nearest whole unit of this field on a copy of this
         * LocalDateTime, favoring the floor if halfway.
         *
         * @return a copy of the LocalDateTime with the field value changed
         */
        LocalDateTime *roundHalfFloorCopy() {
            return iInstant->withLocalMillis(iField->roundHalfFloor(iInstant->getLocalMillis()));
        }
        
        /**
         * Rounds to the nearest whole unit of this field on a copy of this
         * LocalDateTime, favoring the ceiling if halfway.
         *
         * @return a copy of the LocalDateTime with the field value changed
         */
        LocalDateTime *roundHalfCeilingCopy() {
            return iInstant->withLocalMillis(iField->roundHalfCeiling(iInstant->getLocalMillis()));
        }
        
        /**
         * Rounds to the nearest whole unit of this field on a copy of this
         * LocalDateTime.  If halfway, the ceiling is favored over the floor
         * only if it makes this field's value even.
         *
         * @return a copy of the LocalDateTime with the field value changed
         */
        LocalDateTime *roundHalfEvenCopy() {
            return iInstant->withLocalMillis(iField->roundHalfEven(iInstant->getLocalMillis()));
        }
        
    protected:
        
        /**
         * Gets the milliseconds of the datetime that this property is linked to.
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
    
    /**
     * Obtains a {@code LocalDateTime} set to the current system millisecond time
     * using <code>ISOChronology</code> in the default time zone.
     * The resulting object does not use the zone.
     *
     * @return the current date, not NULL
     * @since 2.0
     */
    static LocalDateTime *now() { return new LocalDateTime(); }
    
    static LocalDateTime *now(DateTimeZone *zone);
    static LocalDateTime *now(Chronology *chronology);
    
    /**
     * Parses a {@code LocalDateTime} from the specified string.
     * <p>
     * This uses {@link ISODateTimeFormat#localDateOptionalTimeParser()}.
     *
     * @param str  the string to parse, not NULL
     * @since 2.0
     */
    static LocalDateTime *parse(string str) { return parse(str, ISODateTimeFormat::localDateOptionalTimeParser()); }
    
    /**
     * Parses a {@code LocalDateTime} from the specified string using a formatter->
     *
     * @param str  the string to parse, not NULL
     * @param formatter  the formatter to use, not NULL
     * @since 2.0
     */
    static LocalDateTime *parse(string str, DateTimeFormatter *formatter) { return formatter->parseLocalDateTime(str); }
    
    const DateTimeFieldType *getFieldType(int index) const {
        return AbstractPartial::getFieldType(index);
    }
    
    const DateTimeField *getField(int index) const {
        return AbstractPartial::getField(index);
    }
    
    int get(const DateTimeFieldType *field) const;
    
    DateTime *toDateTime(ReadableInstant *baseInstant) {
        return BaseLocal::toDateTime(baseInstant);
    }
    
    int hashCode() {
        return BaseLocal::hashCode();
    }
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance set to the current local time evaluated using
     * ISO chronology in the default zone.
     * <p>
     * Once the constructor is completed, the zone is no longer used.
     *
     * @see #now()
     */
    LocalDateTime() : LocalDateTime(DateTimeUtils::currentTimeMillis(), ISOChronology::getInstance()) {}
    
    /**
     * Constructs an instance set to the current local time evaluated using
     * ISO chronology in the specified zone.
     * <p>
     * If the specified time zone is NULL, the default zone is used.
     * Once the constructor is completed, the zone is no longer used.
     *
     * @param zone  the time zone, NULL means default zone
     * @see #now(DateTimeZone)
     */
    LocalDateTime(DateTimeZone *zone) : LocalDateTime(DateTimeUtils::currentTimeMillis(), ISOChronology::getInstance(zone)) {}
    
    /**
     * Constructs an instance set to the current local time evaluated using
     * specified chronology->
     * <p>
     * If the chronology is NULL, ISO chronology in the default time zone is used.
     * Once the constructor is completed, the zone is no longer used.
     *
     * @param chronology  the chronology, NULL means ISOChronology in default zone
     * @see #now(Chronology)
     */
    LocalDateTime(Chronology *chronology) : LocalDateTime(DateTimeUtils::currentTimeMillis(), chronology) {}
    
    //-----------------------------------------------------------------------
    /**
     * Constructs an instance set to the local time defined by the specified
     * instant evaluated using ISO chronology in the default zone.
     * <p>
     * Once the constructor is completed, the zone is no longer used.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z
     */
    LocalDateTime(int64_t instant) : LocalDateTime(instant, ISOChronology::getInstance()) {}
    
    /**
     * Constructs an instance set to the local time defined by the specified
     * instant evaluated using ISO chronology in the specified zone.
     * <p>
     * If the specified time zone is NULL, the default zone is used.
     * Once the constructor is completed, the zone is no longer used.
     *
     * @param instant  the milliseconds from 1970-01-01T00:00:00Z
     * @param zone  the time zone, NULL means default zone
     */
    LocalDateTime(int64_t instant, DateTimeZone *zone) : LocalDateTime(instant, ISOChronology::getInstance(zone)) {}
    
    LocalDateTime(int64_t instant, Chronology *chronology);
    LocalDateTime(Object *instant);
    LocalDateTime(Object *instant, DateTimeZone *zone);
    LocalDateTime(Object *instant, Chronology *chronology);
    LocalDateTime(int year,
                  int monthOfYear,
                  int dayOfMonth,
                  int hourOfDay,
                  int minuteOfHour);
    LocalDateTime(int year,
                  int monthOfYear,
                  int dayOfMonth,
                  int hourOfDay,
                  int minuteOfHour,
                  int secondOfMinute);
    LocalDateTime(int year,
                  int monthOfYear,
                  int dayOfMonth,
                  int hourOfDay,
                  int minuteOfHour,
                  int secondOfMinute,
                  int millisOfSecond);
    LocalDateTime(int year,
                  int monthOfYear,
                  int dayOfMonth,
                  int hourOfDay,
                  int minuteOfHour,
                  int secondOfMinute,
                  int millisOfSecond,
                  Chronology *chronology);
    
    int size() const;
    int getValue(int index) const;
    bool isSupported(const DateTimeFieldType *type) const;
    bool isSupported(const DurationFieldType *type) const;
    
    
    /**
     * Gets the chronology of the datetime.
     *
     * @return the Chronology that the datetime is using
     */
    Chronology *getChronology() const { return iChronology; }
    
    bool equals(const Object *partial) const;
    int compareTo(const ReadablePartial *partial) const;
    
    /**
     * Converts this object to a DateTime using the default zone.
     * <p>
     * This method will throw an exception if the DateTime *that would be
     * created does not exist when the time zone is taken into account.
     *
     * @return <code>this</code>
     */
    DateTime *toDateTime() { return toDateTime((DateTimeZone*) NULL); }
    DateTime *toDateTime(DateTimeZone *zone);
    
    /**
     * Converts this object to a LocalDate with the same date and chronology->
     *
     * @return a LocalDate with the same date and chronology
     */
    LocalDate *toLocalDate() { return new LocalDate(getLocalMillis(), getChronology()); }
    
    /**
     * Converts this object to a LocalTime with the same time and chronology->
     *
     * @return a LocalTime with the same time and chronology
     */
    LocalTime *toLocalTime() { return new LocalTime(getLocalMillis(), getChronology()); }
    
    //-----------------------------------------------------------------------
    /**
     * Returns a copy of this datetime with different local millis.
     * <p>
     * The returned object will be a new instance of the same type->
     * Only the millis will change, the chronology is kept.
     * The returned object will be either be a new instance or <code>this</code>.
     *
     * @param newMillis  the new millis, from 1970-01-01T00:00:00
     * @return a copy of this datetime with different millis
     */
    LocalDateTime *withLocalMillis(int64_t newMillis) { return (newMillis == getLocalMillis() ? this : new LocalDateTime(newMillis, getChronology())); }
    LocalDateTime *withDate(int year, int monthOfYear, int dayOfMonth);
    LocalDateTime *withTime(int hourOfDay, int minuteOfHour, int secondOfMinute, int millisOfSecond);
    LocalDateTime *withFields(ReadablePartial *partial);
    LocalDateTime *withField(DateTimeFieldType *fieldType, int value);
    LocalDateTime *withFieldAdded(DurationFieldType *fieldType, int amount);
    LocalDateTime *withDurationAdded(ReadableDuration *durationToAdd, int scalar);
    LocalDateTime *withPeriodAdded(ReadablePeriod *period, int scalar);
    
    LocalDateTime *plus(ReadableDuration *duration);
    LocalDateTime *plus(ReadablePeriod *period);
    LocalDateTime *plusYears(int years);
    LocalDateTime *plusMonths(int months);
    LocalDateTime *plusWeeks(int weeks);
    LocalDateTime *plusDays(int days);
    LocalDateTime *plusHours(int hours);
    LocalDateTime *plusMinutes(int minutes);
    LocalDateTime *plusSeconds(int seconds);
    LocalDateTime *plusMillis(int millis);
    
    LocalDateTime *minus(ReadableDuration *duration);
    LocalDateTime *minus(ReadablePeriod *period);
    LocalDateTime *minusYears(int years);
    LocalDateTime *minusMonths(int months);
    LocalDateTime *minusWeeks(int weeks);
    LocalDateTime *minusDays(int days);
    LocalDateTime *minusHours(int hours);
    LocalDateTime *minusMinutes(int minutes);
    LocalDateTime *minusSeconds(int seconds);
    LocalDateTime *minusMillis(int millis);
    
    Property *property(DateTimeFieldType *fieldType);
    
    /**
     * Get the era field value.
     *
     * @return the era
     */
    int getEra() { return getChronology()->era()->get(getLocalMillis()); }
    
    /**
     * Get the year of era field value.
     *
     * @return the year of era
     */
    int getCenturyOfEra() { return getChronology()->centuryOfEra()->get(getLocalMillis()); }
    
    /**
     * Get the year of era field value.
     *
     * @return the year of era
     */
    int getYearOfEra() { return getChronology()->yearOfEra()->get(getLocalMillis()); }
    
    /**
     * Get the year of century field value.
     *
     * @return the year of century
     */
    int getYearOfCentury() { return getChronology()->yearOfCentury()->get(getLocalMillis()); }
    
    /**
     * Get the year field value.
     *
     * @return the year
     */
    int getYear() { return getChronology()->year()->get(getLocalMillis()); }
    
    /**
     * Get the weekyear field value.
     * <p>
     * The weekyear is the year that matches with the weekOfWeekyear field.
     * In the standard ISO8601 week algorithm, the first week of the year
     * is that in which at least 4 days are in the year. As a result of this
     * definition, day 1 of the first week may be in the previous year.
     * The weekyear allows you to query the effective year for that day.
     *
     * @return the weekyear
     */
    int getWeekyear() { return getChronology()->weekyear()->get(getLocalMillis()); }
    
    /**
     * Get the month of year field value.
     *
     * @return the month of year
     */
    int getMonthOfYear() { return getChronology()->monthOfYear()->get(getLocalMillis()); }
    
    /**
     * Get the week of weekyear field value.
     * <p>
     * This field is associated with the "weekyear" via {@link #getWeekyear()}.
     * In the standard ISO8601 week algorithm, the first week of the year
     * is that in which at least 4 days are in the year. As a result of this
     * definition, day 1 of the first week may be in the previous year.
     *
     * @return the week of a week based year
     */
    int getWeekOfWeekyear() { return getChronology()->weekOfWeekyear()->get(getLocalMillis()); }
    
    /**
     * Get the day of year field value.
     *
     * @return the day of year
     */
    int getDayOfYear() { return getChronology()->dayOfYear()->get(getLocalMillis()); }
    
    /**
     * Get the day of month field value.
     * <p>
     * The values for the day of month are defined in {@link org.joda.time.DateTimeConstants}.
     *
     * @return the day of month
     */
    int getDayOfMonth() { return getChronology()->dayOfMonth()->get(getLocalMillis()); }
    
    /**
     * Get the day of week field value.
     * <p>
     * The values for the day of week are defined in {@link org.joda.time.DateTimeConstants}.
     *
     * @return the day of week
     */
    int getDayOfWeek() { return getChronology()->dayOfWeek()->get(getLocalMillis()); }
    
    //-----------------------------------------------------------------------
    /**
     * Get the hour of day field value.
     *
     * @return the hour of day
     */
    int getHourOfDay() { return getChronology()->hourOfDay()->get(getLocalMillis()); }
    
    /**
     * Get the minute of hour field value.
     *
     * @return the minute of hour
     */
    int getMinuteOfHour() { return getChronology()->minuteOfHour()->get(getLocalMillis()); }
    
    /**
     * Get the second of minute field value.
     *
     * @return the second of minute
     */
    int getSecondOfMinute() { return getChronology()->secondOfMinute()->get(getLocalMillis()); }
    
    /**
     * Get the millis of second field value.
     *
     * @return the millis of second
     */
    int getMillisOfSecond() { return getChronology()->millisOfSecond()->get(getLocalMillis()); }
    
    /**
     * Get the millis of day field value.
     *
     * @return the millis of day
     */
    int getMillisOfDay() { return getChronology()->millisOfDay()->get(getLocalMillis()); }
    
    //-----------------------------------------------------------------------
    /**
     * Returns a copy of this datetime with the era field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * era changed.
     *
     * @param era  the era to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withEra(int era) { return withLocalMillis(getChronology()->era()->set(getLocalMillis(), era)); }
    
    /**
     * Returns a copy of this datetime with the century of era field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * century of era changed.
     *
     * @param centuryOfEra  the centurey of era to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withCenturyOfEra(int centuryOfEra) { return withLocalMillis(getChronology()->centuryOfEra()->set(getLocalMillis(), centuryOfEra)); }
    
    /**
     * Returns a copy of this datetime with the year of era field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * year of era changed.
     *
     * @param yearOfEra  the year of era to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withYearOfEra(int yearOfEra) { return withLocalMillis(getChronology()->yearOfEra()->set(getLocalMillis(), yearOfEra)); }
    
    /**
     * Returns a copy of this datetime with the year of century field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * year of century changed.
     *
     * @param yearOfCentury  the year of century to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withYearOfCentury(int yearOfCentury) { return withLocalMillis(getChronology()->yearOfCentury()->set(getLocalMillis(), yearOfCentury)); }
    
    /**
     * Returns a copy of this datetime with the year field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * year changed.
     *
     * @param year  the year to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withYear(int year) { return withLocalMillis(getChronology()->year()->set(getLocalMillis(), year)); }
    
    /**
     * Returns a copy of this datetime with the weekyear field updated.
     * <p>
     * The weekyear is the year that matches with the weekOfWeekyear field.
     * In the standard ISO8601 week algorithm, the first week of the year
     * is that in which at least 4 days are in the year. As a result of this
     * definition, day 1 of the first week may be in the previous year.
     * The weekyear allows you to query the effective year for that day.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * weekyear changed.
     *
     * @param weekyear  the weekyear to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withWeekyear(int weekyear) { return withLocalMillis(getChronology()->weekyear()->set(getLocalMillis(), weekyear)); }
    
    /**
     * Returns a copy of this datetime with the month of year field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * month of year changed.
     *
     * @param monthOfYear  the month of year to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withMonthOfYear(int monthOfYear) { return withLocalMillis(getChronology()->monthOfYear()->set(getLocalMillis(), monthOfYear)); }
    
    /**
     * Returns a copy of this datetime with the week of weekyear field updated.
     * <p>
     * This field is associated with the "weekyear" via {@link #withWeekyear(int)}.
     * In the standard ISO8601 week algorithm, the first week of the year
     * is that in which at least 4 days are in the year. As a result of this
     * definition, day 1 of the first week may be in the previous year.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * week of weekyear changed.
     *
     * @param weekOfWeekyear  the week of weekyear to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withWeekOfWeekyear(int weekOfWeekyear) { return withLocalMillis(getChronology()->weekOfWeekyear()->set(getLocalMillis(), weekOfWeekyear)); }
    
    /**
     * Returns a copy of this datetime with the day of year field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * day of year changed.
     *
     * @param dayOfYear  the day of year to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withDayOfYear(int dayOfYear) { return withLocalMillis(getChronology()->dayOfYear()->set(getLocalMillis(), dayOfYear)); }
    
    /**
     * Returns a copy of this datetime with the day of month field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * day of month changed.
     *
     * @param dayOfMonth  the day of month to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withDayOfMonth(int dayOfMonth) { return withLocalMillis(getChronology()->dayOfMonth()->set(getLocalMillis(), dayOfMonth)); }
    
    /**
     * Returns a copy of this datetime with the day of week field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * day of week changed.
     *
     * @param dayOfWeek  the day of week to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withDayOfWeek(int dayOfWeek) { return withLocalMillis(getChronology()->dayOfWeek()->set(getLocalMillis(), dayOfWeek)); }
    
    //-----------------------------------------------------------------------
    /**
     * Returns a copy of this datetime with the hour of day field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * hour of day changed.
     *
     * @param hour  the hour of day to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withHourOfDay(int hour) { return withLocalMillis(getChronology()->hourOfDay()->set(getLocalMillis(), hour)); }
    
    /**
     * Returns a copy of this datetime with the minute of hour field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * minute of hour changed.
     *
     * @param minute  the minute of hour to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withMinuteOfHour(int minute) { return withLocalMillis(getChronology()->minuteOfHour()->set(getLocalMillis(), minute)); }
    
    /**
     * Returns a copy of this datetime with the second of minute field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * second of minute changed.
     *
     * @param second  the second of minute to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withSecondOfMinute(int second) { return withLocalMillis(getChronology()->secondOfMinute()->set(getLocalMillis(), second)); }
    
    /**
     * Returns a copy of this datetime with the millis of second field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * millis of second changed.
     *
     * @param millis  the millis of second to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withMillisOfSecond(int millis) { return withLocalMillis(getChronology()->millisOfSecond()->set(getLocalMillis(), millis)); }
    
    /**
     * Returns a copy of this datetime with the millis of day field updated.
     * <p>
     * LocalDateTime *is immutable, so there are no set methods.
     * Instead, this method returns a new instance with the value of
     * millis of day changed.
     *
     * @param millis  the millis of day to set
     * @return a copy of this object with the field set
     * @throws IllegalArgumentException if the value is invalid
     */
    LocalDateTime *withMillisOfDay(int millis) { return withLocalMillis(getChronology()->millisOfDay()->set(getLocalMillis(), millis)); }
    
    //-----------------------------------------------------------------------
    /**
     * Get the era property which provides access to advanced functionality.
     *
     * @return the era property
     */
    Property *era() { return new Property(this, getChronology()->era()); }
    
    /**
     * Get the century of era property which provides access to advanced functionality.
     *
     * @return the year of era property
     */
    Property *centuryOfEra() { return new Property(this, getChronology()->centuryOfEra()); }
    
    /**
     * Get the year of century property which provides access to advanced functionality.
     *
     * @return the year of era property
     */
    Property *yearOfCentury() { return new Property(this, getChronology()->yearOfCentury()); }
    
    /**
     * Get the year of era property which provides access to advanced functionality.
     *
     * @return the year of era property
     */
    Property *yearOfEra() { return new Property(this, getChronology()->yearOfEra()); }
    
    /**
     * Get the year property which provides access to advanced functionality.
     *
     * @return the year property
     */
    Property *year() { return new Property(this, getChronology()->year()); }
    
    /**
     * Get the weekyear property which provides access to advanced functionality.
     *
     * @return the weekyear property
     */
    Property *weekyear() { return new Property(this, getChronology()->weekyear()); }
    
    /**
     * Get the month of year property which provides access to advanced functionality.
     *
     * @return the month of year property
     */
    Property *monthOfYear() { return new Property(this, getChronology()->monthOfYear()); }
    
    /**
     * Get the week of a week based year property which provides access to advanced functionality.
     *
     * @return the week of a week based year property
     */
    Property *weekOfWeekyear() { return new Property(this, getChronology()->weekOfWeekyear()); }
    
    /**
     * Get the day of year property which provides access to advanced functionality.
     *
     * @return the day of year property
     */
    Property *dayOfYear() { return new Property(this, getChronology()->dayOfYear()); }
    
    /**
     * Get the day of month property which provides access to advanced functionality.
     *
     * @return the day of month property
     */
    Property *dayOfMonth() { return new Property(this, getChronology()->dayOfMonth()); }
    
    /**
     * Get the day of week property which provides access to advanced functionality.
     *
     * @return the day of week property
     */
    Property *dayOfWeek() { return new Property(this, getChronology()->dayOfWeek()); }
    
    //-----------------------------------------------------------------------
    /**
     * Get the hour of day field property which provides access to advanced functionality.
     *
     * @return the hour of day property
     */
    Property *hourOfDay() { return new Property(this, getChronology()->hourOfDay()); }
    
    /**
     * Get the minute of hour field property which provides access to advanced functionality.
     *
     * @return the minute of hour property
     */
    Property *minuteOfHour() { return new Property(this, getChronology()->minuteOfHour()); }
    
    /**
     * Get the second of minute field property which provides access to advanced functionality.
     *
     * @return the second of minute property
     */
    Property *secondOfMinute() { return new Property(this, getChronology()->secondOfMinute()); }
    
    /**
     * Get the millis of second property which provides access to advanced functionality.
     *
     * @return the millis of second property
     */
    Property *millisOfSecond() { return new Property(this, getChronology()->millisOfSecond()); }
    
    /**
     * Get the millis of day property which provides access to advanced functionality.
     *
     * @return the millis of day property
     */
    Property *millisOfDay() { return new Property(this, getChronology()->millisOfDay()); }
    
    //-----------------------------------------------------------------------
    /**
     * Output the date time in ISO8601 format (yyyy-MM-ddTHH:mm:ss.SSS).
     *
     * @return ISO8601 time formatted string.
     */
    string toString() { return ISODateTimeFormat::dateTime()->print(this); }
    string toString(string pattern);
    string toString(string pattern, Locale *locale);
    
protected:
    
    const DateTimeField *getField(int index, Chronology *chrono) const;
    /**
     * Gets the milliseconds of the datetime instant from the Java epoch
     * of 1970-01-01T00:00:00 (not fixed to any specific time zone).
     *
     * @return the number of milliseconds since 1970-01-01T00:00:00
     * @since 1.5 (previously private)
     */
    int64_t getLocalMillis() const { return iLocalMillis; }
    
};

CODATIME_END

#endif /* defined(__CodaTime__LocalDateTime__) */
