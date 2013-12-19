//
//  LocalDateTime.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "LocalDateTime.h"
#include "format/DateTimeFormat.h"

CODATIME_BEGIN

/**
 * Obtains a {@code LocalDateTime} set to the current system millisecond time
 * using <code>ISOChronology</code> in the specified time zone.
 * The resulting object does not use the zone.
 *
 * @param zone  the time zone, not NULL
 * @return the current date, not NULL
 * @since 2.0
 */
LocalDateTime *LocalDateTime::now(DateTimeZone *zone) {
    if (zone == NULL) {
        throw NullPointerException("Zone must not be NULL");
    }
    return new LocalDateTime(zone);
}

/**
 * Obtains a {@code LocalDateTime} set to the current system millisecond time
 * using the specified chronology->
 * The resulting object does not use the zone.
 *
 * @param chronology  the chronology, not NULL
 * @return the current date, not NULL
 * @since 2.0
 */
LocalDateTime *LocalDateTime::now(Chronology *chronology) {
    if (chronology == NULL) {
        throw NullPointerException("Chronology must not be NULL");
    }
    return new LocalDateTime(chronology);
}

int64_t LocalDateTime::init(int64_t instant, Chronology *chronology) {
    
    chronology = DateTimeUtils::getChronology(chronology);
    int64_t localMillis = chronology->getZone()->getMillisKeepLocal(DateTimeZone::UTC, instant);
    iChronology = chronology->withUTC();
    
    return localMillis;
}

/**
 * Constructs an instance set to the local time defined by the specified
 * instant evaluated using the specified chronology->
 * <p>
 * If the chronology is NULL, ISO chronology in the default zone is used.
 * Once the constructor is completed, the zone is no longer used.
 *
 * @param instant  the milliseconds from 1970-01-01T00:00:00Z
 * @param chronology  the chronology, NULL means ISOChronology in default zone
 */
LocalDateTime::LocalDateTime(int64_t instant, Chronology *chronology) :
    iLocalMillis(init(instant, chronology))
{
}

//-----------------------------------------------------------------------
/**
 * Constructs an instance from an Object that represents a datetime.
 * <p>
 * If the object contains no chronology, <code>ISOChronology</code> is used.
 * If the object contains no time zone, the default zone is used.
 * Once the constructor is completed, the zone is no longer used.
 * <p>
 * The recognised object types are defined in
 * {@link org.joda.time.convert.ConverterManager ConverterManager} and
 * include ReadablePartial, ReadableInstant, string, Calendar and Date.
 * The string formats are described by {@link ISODateTimeFormat#localDateOptionalTimeParser()}.
 * The default string converter ignores the zone and only parses the field values.
 *
 * @param instant  the datetime object
 * @throws IllegalArgumentException if the instant is invalid
 */
LocalDateTime::LocalDateTime(Object *instant) : LocalDateTime(instant, (Chronology*) NULL) {
}

int64_t LocalDateTime::init(Object *instant, DateTimeZone *zone) {
    
    PartialConverter *converter = ConverterManager::getInstance()->getPartialConverter(instant);
    Chronology *chronology = converter->getChronology(instant, zone);
    chronology = DateTimeUtils::getChronology(chronology);
    iChronology = chronology->withUTC();
    vector<int> values = converter->getPartialValues(this, instant, chronology, ISODateTimeFormat::localDateOptionalTimeParser());
    int64_t localMillis = iChronology->getDateTimeMillis(values[0], values[1], values[2], values[3]);
    
    return localMillis;
}

/**
 * Constructs an instance from an Object that represents a datetime,
 * forcing the time zone to that specified.
 * <p>
 * If the object contains no chronology, <code>ISOChronology</code> is used.
 * If the specified time zone is NULL, the default zone is used.
 * Once the constructor is completed, the zone is no longer used.
 * <p>
 * The recognised object types are defined in
 * {@link org.joda.time.convert.ConverterManager ConverterManager} and
 * include ReadablePartial, ReadableInstant, string, Calendar and Date.
 * The string formats are described by {@link ISODateTimeFormat#localDateOptionalTimeParser()}.
 * The default string converter ignores the zone and only parses the field values.
 *
 * @param instant  the datetime object
 * @param zone  the time zone
 * @throws IllegalArgumentException if the instant is invalid
 */
LocalDateTime::LocalDateTime(Object *instant, DateTimeZone *zone) : iLocalMillis(init(instant, zone)) {}

int64_t LocalDateTime::init(Object *instant, Chronology *chronology) {
    
    PartialConverter *converter = ConverterManager::getInstance()->getPartialConverter(instant);
    chronology = converter->getChronology(instant, chronology);
    chronology = DateTimeUtils::getChronology(chronology);
    iChronology = chronology->withUTC();
    vector<int> values = converter->getPartialValues(this, instant, chronology, ISODateTimeFormat::localDateOptionalTimeParser());
    int64_t localMillis = iChronology->getDateTimeMillis(values[0], values[1], values[2], values[3]);
    
    return localMillis;
}

/**
 * Constructs an instance from an Object that represents a datetime,
 * using the specified chronology->
 * <p>
 * If the chronology is NULL, ISO in the default time zone is used.
 * Once the constructor is completed, the zone is no longer used.
 * If the instant contains a chronology, it will be ignored.
 * For example, passing a {@code LocalDate} and a different chronology
 * will return a date with the year/month/day from the date applied
 * unaltered to the specified chronology->
 * <p>
 * The recognised object types are defined in
 * {@link org.joda.time.convert.ConverterManager ConverterManager} and
 * include ReadablePartial, ReadableInstant, string, Calendar and Date.
 * The string formats are described by {@link ISODateTimeFormat#localDateOptionalTimeParser()}.
 * The default string converter ignores the zone and only parses the field values.
 *
 * @param instant  the datetime object
 * @param chronology  the chronology
 * @throws IllegalArgumentException if the instant is invalid
 */
LocalDateTime::LocalDateTime(Object *instant, Chronology *chronology) : iLocalMillis(init(instant, chronology)) {}

//-----------------------------------------------------------------------
/**
 * Constructs an instance set to the specified date and time
 * using <code>ISOChronology</code>.
 *
 * @param year  the year
 * @param monthOfYear  the month of the year, from 1 to 12
 * @param dayOfMonth  the day of the month, from 1 to 31
 * @param hourOfDay  the hour of the day, from 0 to 23
 * @param minuteOfHour  the minute of the hour, from 0 to 59
 */
LocalDateTime::LocalDateTime(int year,
                     int monthOfYear,
                     int dayOfMonth,
                     int hourOfDay,
                     int minuteOfHour) : LocalDateTime(year, monthOfYear, dayOfMonth, hourOfDay,
                                                   minuteOfHour, 0, 0, ISOChronology::getInstanceUTC()) {
}

/**
 * Constructs an instance set to the specified date and time
 * using <code>ISOChronology</code>.
 *
 * @param year  the year
 * @param monthOfYear  the month of the year, from 1 to 12
 * @param dayOfMonth  the day of the month, from 1 to 31
 * @param hourOfDay  the hour of the day, from 0 to 23
 * @param minuteOfHour  the minute of the hour, from 0 to 59
 * @param secondOfMinute  the second of the minute, from 0 to 59
 */
LocalDateTime::LocalDateTime(int year,
                     int monthOfYear,
                     int dayOfMonth,
                     int hourOfDay,
                     int minuteOfHour,
                     int secondOfMinute) : LocalDateTime(year, monthOfYear, dayOfMonth, hourOfDay,
         minuteOfHour, secondOfMinute, 0, ISOChronology::getInstanceUTC()) {
}

/**
 * Constructs an instance set to the specified date and time
 * using <code>ISOChronology</code>.
 *
 * @param year  the year
 * @param monthOfYear  the month of the year, from 1 to 12
 * @param dayOfMonth  the day of the month, from 1 to 31
 * @param hourOfDay  the hour of the day, from 0 to 23
 * @param minuteOfHour  the minute of the hour, from 0 to 59
 * @param secondOfMinute  the second of the minute, from 0 to 59
 * @param millisOfSecond  the millisecond of the second, from 0 to 999
 */
LocalDateTime::LocalDateTime(int year,
                     int monthOfYear,
                     int dayOfMonth,
                     int hourOfDay,
                     int minuteOfHour,
                     int secondOfMinute,
                     int millisOfSecond) : LocalDateTime(year, monthOfYear, dayOfMonth, hourOfDay,
         minuteOfHour, secondOfMinute, millisOfSecond, ISOChronology::getInstanceUTC()) {
}

int64_t LocalDateTime::init(int year,
             int monthOfYear,
             int dayOfMonth,
             int hourOfDay,
             int minuteOfHour,
             int secondOfMinute,
             int millisOfSecond,
             Chronology *chronology) {
    chronology = DateTimeUtils::getChronology(chronology)->withUTC();
    int64_t instant = chronology->getDateTimeMillis(year, monthOfYear, dayOfMonth,
                                                    hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond);
    iChronology = chronology;
    return instant;
}

/**
 * Constructs an instance set to the specified date and time
 * using the specified chronology, whose zone is ignored.
 * <p>
 * If the chronology is NULL, <code>ISOChronology</code> is used.
 *
 * @param year  the year, valid values defined by the chronology
 * @param monthOfYear  the month of the year, valid values defined by the chronology
 * @param dayOfMonth  the day of the month, valid values defined by the chronology
 * @param hourOfDay  the hour of the day, valid values defined by the chronology
 * @param minuteOfHour  the minute of the hour, valid values defined by the chronology
 * @param secondOfMinute  the second of the minute, valid values defined by the chronology
 * @param millisOfSecond  the millisecond of the second, valid values defined by the chronology
 * @param chronology  the chronology, NULL means ISOChronology in default zone
 */
LocalDateTime::LocalDateTime(int year,
                     int monthOfYear,
                     int dayOfMonth,
                     int hourOfDay,
                     int minuteOfHour,
                     int secondOfMinute,
                     int millisOfSecond,
                             Chronology *chronology) : BaseLocal(), iLocalMillis(init(year, monthOfYear, dayOfMonth, hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond, chronology)) {
}

/**
 * Handle broken serialization from other tools.
 * @return the resolved object, not NULL
 */
Object *LocalDateTime::readResolve() {
    if (iChronology == NULL) {
        return new LocalDateTime(iLocalMillis, ISOChronology::getInstanceUTC());
    }
    if (DateTimeZone::UTC->equals(iChronology->getZone()) == false) {
        return new LocalDateTime(iLocalMillis, iChronology->withUTC());
    }
    return this;
}

//-----------------------------------------------------------------------
/**
 * Gets the number of fields in this partial, which is four.
 * The supported fields are Year, MonthOfDay, DayOfMonth and MillisOfDay.
 *
 * @return the field count, four
 */
int LocalDateTime::size() const {
    return 4;
}

/**
 * Gets the field for a specific index in the chronology specified.
 * <p>
 * This method must not use any instance variables.
 *
 * @param index  the index to retrieve
 * @param chrono  the chronology to use
 * @return the field
 */
const DateTimeField *LocalDateTime::getField(int index, Chronology *chrono) const {
    switch (index) {
        case YEAR:
            return chrono->year();
        case MONTH_OF_YEAR:
            return chrono->monthOfYear();
        case DAY_OF_MONTH:
            return chrono->dayOfMonth();
        case MILLIS_OF_DAY:
            return chrono->millisOfDay();
        default:
            throw IndexOutOfBoundsException("Invalid index: " + to_string(index));
    }
}

/**
 * Gets the value of the field at the specifed index.
 * <p>
 * This method is required to support the <code>ReadablePartial</code>
 * interface. The supported fields are Year, MonthOfDay, DayOfMonth and MillisOfDay.
 *
 * @param index  the index, zero to two
 * @return the value
 * @throws IndexOutOfBoundsException if the index is invalid
 */
int LocalDateTime::getValue(int index) const {
    switch (index) {
        case YEAR:
            return getChronology()->year()->get(getLocalMillis());
        case MONTH_OF_YEAR:
            return getChronology()->monthOfYear()->get(getLocalMillis());
        case DAY_OF_MONTH:
            return getChronology()->dayOfMonth()->get(getLocalMillis());
        case MILLIS_OF_DAY:
            return getChronology()->millisOfDay()->get(getLocalMillis());
        default:
            throw IndexOutOfBoundsException("Invalid index: " + to_string(index));
    }
}

//-----------------------------------------------------------------------
/**
 * Get the value of one of the fields of a datetime.
 * <p>
 * This method gets the value of the specified field.
 * For example:
 * <pre>
 * DateTime dt = new DateTime();
 * int year = dt.get(DateTimeFieldtype->year());
 * </pre>
 *
 * @param type  a field type, usually obtained from DateTimeFieldType, not NULL
 * @return the value of that field
 * @throws IllegalArgumentException if the field type is NULL
 */
int LocalDateTime::get(const DateTimeFieldType *type) const {
    if (type == NULL) {
        throw IllegalArgumentException("The DateTimeFieldType must not be NULL");
    }
    return type->getField(getChronology())->get(getLocalMillis());
}

/**
 * Checks if the field type specified is supported by this
 * local datetime and chronology->
 * This can be used to avoid exceptions in {@link #get(DateTimeFieldType)}.
 *
 * @param type  a field type, usually obtained from DateTimeFieldType
 * @return true if the field type is supported
 */
bool LocalDateTime::isSupported(const DateTimeFieldType *type) const {
    if (type == NULL) {
        return false;
    }
    return type->getField(getChronology())->isSupported();
}

/**
 * Checks if the duration type specified is supported by this
 * local datetime and chronology->
 *
 * @param type  a duration type, usually obtained from DurationFieldType
 * @return true if the field type is supported
 */
bool LocalDateTime::isSupported(const DurationFieldType *type) const {
    if (type == NULL) {
        return false;
    }
    return type->getField(getChronology())->isSupported();
}

/**
 * Compares this ReadablePartial with another returning true if the chronology,
 * field types and values are equal.
 *
 * @param partial  an object to check against
 * @return true if fields and values are equal
 */
bool LocalDateTime::equals(const Object *partial) const {
    // override to perform faster
    if (this == partial) {
        return true;
    }
    const LocalDateTime *other = dynamic_cast<const LocalDateTime*>(partial);
    if (other != 0) {
        if (iChronology->equals(other->iChronology)) {
            return iLocalMillis == other->iLocalMillis;
        }
    }
    return BaseLocal::equals(partial);
}

/**
 * Compares this partial with another returning an integer
 * indicating the order.
 * <p>
 * The fields are compared in order, from largest to smallest.
 * The first field that is non-equal is used to determine the result.
 * <p>
 * The specified object must be a partial instance whose field types
 * match those of this partial.
 *
 * @param partial  an object to check against
 * @return negative if this is less, zero if equal, positive if greater
 * @throws ClassCastException if the partial is the wrong class
 *  or if it has field types that don't match
 * @throws NullPointerException if the partial is NULL
 */
int LocalDateTime::compareTo(const ReadablePartial *partial) const {
    // override to perform faster
//    if (this == partial) {
//        return 0;
//    }
    const LocalDateTime *other = dynamic_cast<const LocalDateTime*>(partial);
    if (other != 0) {
        if (iChronology->equals(other->iChronology)) {
            return (iLocalMillis < other->iLocalMillis ? -1 :
                    (iLocalMillis == other->iLocalMillis ? 0 : 1));
            
        }
    }
    return BaseLocal::compareTo(partial);
}

//-----------------------------------------------------------------------


/**
 * Converts this object to a DateTime using the specified zone.
 * <p>
 * This method will throw an exception if the DateTime *that would be
 * created does not exist when the time zone is taken into account.
 *
 * @param zone time zone to apply, or default if NULL
 * @return a DateTime using the same millis
 */
DateTime *LocalDateTime::toDateTime(DateTimeZone *zone) {
    zone = DateTimeUtils::getZone(zone);
    Chronology *chrono = iChronology->withZone(zone);
    return new DateTime(
                        getYear(), getMonthOfYear(), getDayOfMonth(),
                        getHourOfDay(), getMinuteOfHour(),
                        getSecondOfMinute(), getMillisOfSecond(), chrono);
}

//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
/**
 * Returns a copy of this datetime with the specified date,
 * retaining the time fields.
 * <p>
 * If the date is already the date passed in, then <code>this</code> is returned.
 * <p>
 * To set a single field use the properties, for example:
 * <pre>
 * DateTime set = dt.monthOfYear()->setCopy(6);
 * </pre>
 *
 * @param year  the new year value
 * @param monthOfYear  the new monthOfYear value
 * @param dayOfMonth  the new dayOfMonth value
 * @return a copy of this datetime with a different date
 * @throws IllegalArgumentException if any value if invalid
 */
LocalDateTime *LocalDateTime::withDate(int year, int monthOfYear, int dayOfMonth) {
    Chronology *chrono = getChronology();
    int64_t instant = getLocalMillis();
    instant = chrono->year()->set(instant, year);
    instant = chrono->monthOfYear()->set(instant, monthOfYear);
    instant = chrono->dayOfMonth()->set(instant, dayOfMonth);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime with the specified time,
 * retaining the date fields.
 * <p>
 * If the time is already the time passed in, then <code>this</code> is returned.
 * <p>
 * To set a single field use the properties, for example:
 * <pre>
 * LocalDateTime *set = dt.hourOfDay()->setCopy(6);
 * </pre>
 *
 * @param hourOfDay  the hour of the day
 * @param minuteOfHour  the minute of the hour
 * @param secondOfMinute  the second of the minute
 * @param millisOfSecond  the millisecond of the second
 * @return a copy of this datetime with a different time
 * @throws IllegalArgumentException if any value if invalid
 */
LocalDateTime *LocalDateTime::withTime(int hourOfDay, int minuteOfHour, int secondOfMinute, int millisOfSecond) {
    Chronology *chrono = getChronology();
    int64_t instant = getLocalMillis();
    instant = chrono->hourOfDay()->set(instant, hourOfDay);
    instant = chrono->minuteOfHour()->set(instant, minuteOfHour);
    instant = chrono->secondOfMinute()->set(instant, secondOfMinute);
    instant = chrono->millisOfSecond()->set(instant, millisOfSecond);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this datetime with the partial set of fields
 * replacing those from this instance.
 * <p>
 * For example, if the partial is a <code>TimeOfDay</code> then the time fields
 * would be changed in the returned instance.
 * If the partial is NULL, then <code>this</code> is returned.
 *
 * @param partial  the partial set of fields to apply to this datetime, NULL ignored
 * @return a copy of this datetime with a different set of fields
 * @throws IllegalArgumentException if any value is invalid
 */
LocalDateTime *LocalDateTime::withFields(ReadablePartial *partial) {
    if (partial == NULL) {
        return this;
    }
    return withLocalMillis(getChronology()->set(partial, getLocalMillis()));
}

/**
 * Returns a copy of this datetime with the specified field set to a new value.
 * <p>
 * For example, if the field type is <code>hourOfDay</code> then the hour of day
 * field would be changed in the returned instance.
 * If the field type is NULL, then <code>this</code> is returned.
 * <p>
 * These three lines are equivalent:
 * <pre>
 * LocalDateTime *updated = dt.withField(DateTimeFieldtype->dayOfMonth(), 6);
 * LocalDateTime *updated = dt.dayOfMonth()->setCopy(6);
 * LocalDateTime *updated = dt.property(DateTimeFieldtype->dayOfMonth()).setCopy(6);
 * </pre>
 *
 * @param fieldType  the field type to set, not NULL
 * @param value  the value to set
 * @return a copy of this datetime with the field set
 * @throws IllegalArgumentException if the value is NULL or invalid
 */
LocalDateTime *LocalDateTime::withField(DateTimeFieldType *fieldType, int value) {
    if (fieldType == NULL) {
        throw IllegalArgumentException("Field must not be NULL");
    }
    int64_t instant = fieldType->getField(getChronology())->set(getLocalMillis(), value);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime with the value of the specified
 * field increased.
 * <p>
 * If the addition is zero or the field is NULL, then <code>this</code> is returned.
 * <p>
 * These three lines are equivalent:
 * <pre>
 * LocalDateTime *added = dt.withFieldAdded(DurationFieldtype->years(), 6);
 * LocalDateTime *added = dt.plusYears(6);
 * LocalDateTime *added = dt.plus(Period.years(6));
 * </pre>
 *
 * @param fieldType  the field type to add to, not NULL
 * @param amount  the amount to add
 * @return a copy of this datetime with the field updated
 * @throws IllegalArgumentException if the value is NULL or invalid
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalDateTime *LocalDateTime::withFieldAdded(DurationFieldType *fieldType, int amount) {
    if (fieldType == NULL) {
        throw IllegalArgumentException("Field must not be NULL");
    }
    if (amount == 0) {
        return this;
    }
    int64_t instant = fieldType->getField(getChronology())->add(getLocalMillis(), amount);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this datetime with the specified duration added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 *
 * @param durationToAdd  the duration to add to this one, NULL means zero
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this datetime with the duration added
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalDateTime *LocalDateTime::withDurationAdded(ReadableDuration *durationToAdd, int scalar) {
    if (durationToAdd == NULL || scalar == 0) {
        return this;
    }
    int64_t instant = getChronology()->add(getLocalMillis(), durationToAdd->getMillis(), scalar);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime with the specified period added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 * <p>
 * This method is typically used to add multiple copies of complex
 * period instances. Adding one field is best achieved using methods
 * like {@link #withFieldAdded(DurationFieldType, int)}
 * or {@link #plusYears(int)}.
 *
 * @param period  the period to add to this one, NULL means zero
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this datetime with the period added
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalDateTime *LocalDateTime::withPeriodAdded(ReadablePeriod *period, int scalar) {
    if (period == NULL || scalar == 0) {
        return this;
    }
    int64_t instant = getChronology()->add(period, getLocalMillis(), scalar);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this datetime with the specified duration added.
 * <p>
 * If the amount is zero or NULL, then <code>this</code> is returned.
 *
 * @param duration  the duration to add to this one, NULL means zero
 * @return a copy of this datetime with the duration added
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalDateTime *LocalDateTime::plus(ReadableDuration *duration) {
    return withDurationAdded(duration, 1);
}

/**
 * Returns a copy of this datetime with the specified period added.
 * <p>
 * If the amount is zero or NULL, then <code>this</code> is returned.
 * <p>
 * This method is typically used to add complex period instances.
 * Adding one field is best achieved using methods
 * like {@link #plusYears(int)}.
 *
 * @param period  the period to add to this one, NULL means zero
 * @return a copy of this datetime with the period added
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalDateTime *LocalDateTime::plus(ReadablePeriod *period) {
    return withPeriodAdded(period, 1);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this datetime plus the specified number of years.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *added = dt.plusYears(6);
 * LocalDateTime *added = dt.plus(Period.years(6));
 * LocalDateTime *added = dt.withFieldAdded(DurationFieldtype->years(), 6);
 * </pre>
 *
 * @param years  the amount of years to add, may be negative
 * @return the new LocalDateTime *plus the increased years
 */
LocalDateTime *LocalDateTime::plusYears(int years) {
    if (years == 0) {
        return this;
    }
    int64_t instant = getChronology()->years()->add(getLocalMillis(), years);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime plus the specified number of months.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *added = dt.plusMonths(6);
 * LocalDateTime *added = dt.plus(Period.months(6));
 * LocalDateTime *added = dt.withFieldAdded(DurationFieldtype->months(), 6);
 * </pre>
 *
 * @param months  the amount of months to add, may be negative
 * @return the new LocalDateTime *plus the increased months
 */
LocalDateTime *LocalDateTime::plusMonths(int months) {
    if (months == 0) {
        return this;
    }
    int64_t instant = getChronology()->months()->add(getLocalMillis(), months);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime plus the specified number of weeks.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *added = dt.plusWeeks(6);
 * LocalDateTime *added = dt.plus(Period.weeks(6));
 * LocalDateTime *added = dt.withFieldAdded(DurationFieldtype->weeks(), 6);
 * </pre>
 *
 * @param weeks  the amount of weeks to add, may be negative
 * @return the new LocalDateTime *plus the increased weeks
 */
LocalDateTime *LocalDateTime::plusWeeks(int weeks) {
    if (weeks == 0) {
        return this;
    }
    int64_t instant = getChronology()->weeks()->add(getLocalMillis(), weeks);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime plus the specified number of days.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *added = dt.plusDays(6);
 * LocalDateTime *added = dt.plus(Period.days(6));
 * LocalDateTime *added = dt.withFieldAdded(DurationFieldtype->days(), 6);
 * </pre>
 *
 * @param days  the amount of days to add, may be negative
 * @return the new LocalDateTime *plus the increased days
 */
LocalDateTime *LocalDateTime::plusDays(int days) {
    if (days == 0) {
        return this;
    }
    int64_t instant = getChronology()->days()->add(getLocalMillis(), days);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this datetime plus the specified number of hours.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *added = dt.plusHours(6);
 * LocalDateTime *added = dt.plus(Period.hours(6));
 * LocalDateTime *added = dt.withFieldAdded(DurationFieldtype->hours(), 6);
 * </pre>
 *
 * @param hours  the amount of hours to add, may be negative
 * @return the new LocalDateTime *plus the increased hours
 */
LocalDateTime *LocalDateTime::plusHours(int hours) {
    if (hours == 0) {
        return this;
    }
    int64_t instant = getChronology()->hours()->add(getLocalMillis(), hours);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime plus the specified number of minutes.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *added = dt.plusMinutes(6);
 * LocalDateTime *added = dt.plus(Period.minutes(6));
 * LocalDateTime *added = dt.withFieldAdded(DurationFieldtype->minutes(), 6);
 * </pre>
 *
 * @param minutes  the amount of minutes to add, may be negative
 * @return the new LocalDateTime *plus the increased minutes
 */
LocalDateTime *LocalDateTime::plusMinutes(int minutes) {
    if (minutes == 0) {
        return this;
    }
    int64_t instant = getChronology()->minutes()->add(getLocalMillis(), minutes);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime plus the specified number of seconds.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *added = dt.plusSeconds(6);
 * LocalDateTime *added = dt.plus(Period.seconds(6));
 * LocalDateTime *added = dt.withFieldAdded(DurationFieldtype->seconds(), 6);
 * </pre>
 *
 * @param seconds  the amount of seconds to add, may be negative
 * @return the new LocalDateTime *plus the increased seconds
 */
LocalDateTime *LocalDateTime::plusSeconds(int seconds) {
    if (seconds == 0) {
        return this;
    }
    int64_t instant = getChronology()->seconds()->add(getLocalMillis(), seconds);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime plus the specified number of millis.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *added = dt.plusMillis(6);
 * LocalDateTime *added = dt.plus(Period.millis(6));
 * LocalDateTime *added = dt.withFieldAdded(DurationFieldtype->millis(), 6);
 * </pre>
 *
 * @param millis  the amount of millis to add, may be negative
 * @return the new LocalDateTime *plus the increased millis
 */
LocalDateTime *LocalDateTime::plusMillis(int millis) {
    if (millis == 0) {
        return this;
    }
    int64_t instant = getChronology()->millis()->add(getLocalMillis(), millis);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this datetime with the specified duration taken away.
 * <p>
 * If the amount is zero or NULL, then <code>this</code> is returned.
 *
 * @param duration  the duration to reduce this instant by
 * @return a copy of this datetime with the duration taken away
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalDateTime *LocalDateTime::minus(ReadableDuration *duration) {
    return withDurationAdded(duration, -1);
}

/**
 * Returns a copy of this datetime with the specified period taken away.
 * <p>
 * If the amount is zero or NULL, then <code>this</code> is returned.
 * <p>
 * This method is typically used to subtract complex period instances.
 * Subtracting one field is best achieved using methods
 * like {@link #minusYears(int)}.
 *
 * @param period  the period to reduce this instant by
 * @return a copy of this datetime with the period taken away
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalDateTime *LocalDateTime::minus(ReadablePeriod *period) {
    return withPeriodAdded(period, -1);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this datetime minus the specified number of years.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *subtracted = dt.minusYears(6);
 * LocalDateTime *subtracted = dt.minus(Period.years(6));
 * LocalDateTime *subtracted = dt.withFieldAdded(DurationFieldtype->years(), -6);
 * </pre>
 *
 * @param years  the amount of years to subtract, may be negative
 * @return the new LocalDateTime *minus the increased years
 */
LocalDateTime *LocalDateTime::minusYears(int years) {
    if (years == 0) {
        return this;
    }
    int64_t instant = getChronology()->years()->subtract(getLocalMillis(), years);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime minus the specified number of months.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *subtracted = dt.minusMonths(6);
 * LocalDateTime *subtracted = dt.minus(Period.months(6));
 * LocalDateTime *subtracted = dt.withFieldAdded(DurationFieldtype->months(), -6);
 * </pre>
 *
 * @param months  the amount of months to subtract, may be negative
 * @return the new LocalDateTime *minus the increased months
 */
LocalDateTime *LocalDateTime::minusMonths(int months) {
    if (months == 0) {
        return this;
    }
    int64_t instant = getChronology()->months()->subtract(getLocalMillis(), months);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime minus the specified number of weeks.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *subtracted = dt.minusWeeks(6);
 * LocalDateTime *subtracted = dt.minus(Period.weeks(6));
 * LocalDateTime *subtracted = dt.withFieldAdded(DurationFieldtype->weeks(), -6);
 * </pre>
 *
 * @param weeks  the amount of weeks to subtract, may be negative
 * @return the new LocalDateTime *minus the increased weeks
 */
LocalDateTime *LocalDateTime::minusWeeks(int weeks) {
    if (weeks == 0) {
        return this;
    }
    int64_t instant = getChronology()->weeks()->subtract(getLocalMillis(), weeks);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime minus the specified number of days.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *subtracted = dt.minusDays(6);
 * LocalDateTime *subtracted = dt.minus(Period.days(6));
 * LocalDateTime *subtracted = dt.withFieldAdded(DurationFieldtype->days(), -6);
 * </pre>
 *
 * @param days  the amount of days to subtract, may be negative
 * @return the new LocalDateTime *minus the increased days
 */
LocalDateTime *LocalDateTime::minusDays(int days) {
    if (days == 0) {
        return this;
    }
    int64_t instant = getChronology()->days()->subtract(getLocalMillis(), days);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this datetime minus the specified number of hours.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *subtracted = dt.minusHours(6);
 * LocalDateTime *subtracted = dt.minus(Period.hours(6));
 * LocalDateTime *subtracted = dt.withFieldAdded(DurationFieldtype->hours(), -6);
 * </pre>
 *
 * @param hours  the amount of hours to subtract, may be negative
 * @return the new LocalDateTime *minus the increased hours
 */
LocalDateTime *LocalDateTime::minusHours(int hours) {
    if (hours == 0) {
        return this;
    }
    int64_t instant = getChronology()->hours()->subtract(getLocalMillis(), hours);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime minus the specified number of minutes.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *subtracted = dt.minusMinutes(6);
 * LocalDateTime *subtracted = dt.minus(Period.minutes(6));
 * LocalDateTime *subtracted = dt.withFieldAdded(DurationFieldtype->minutes(), -6);
 * </pre>
 *
 * @param minutes  the amount of minutes to subtract, may be negative
 * @return the new LocalDateTime *minus the increased minutes
 */
LocalDateTime *LocalDateTime::minusMinutes(int minutes) {
    if (minutes == 0) {
        return this;
    }
    int64_t instant = getChronology()->minutes()->subtract(getLocalMillis(), minutes);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime minus the specified number of seconds.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *subtracted = dt.minusSeconds(6);
 * LocalDateTime *subtracted = dt.minus(Period.seconds(6));
 * LocalDateTime *subtracted = dt.withFieldAdded(DurationFieldtype->seconds(), -6);
 * </pre>
 *
 * @param seconds  the amount of seconds to subtract, may be negative
 * @return the new LocalDateTime *minus the increased seconds
 */
LocalDateTime *LocalDateTime::minusSeconds(int seconds) {
    if (seconds == 0) {
        return this;
    }
    int64_t instant = getChronology()->seconds()->subtract(getLocalMillis(), seconds);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this datetime minus the specified number of millis.
 * <p>
 * This LocalDateTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalDateTime *subtracted = dt.minusMillis(6);
 * LocalDateTime *subtracted = dt.minus(Period.millis(6));
 * LocalDateTime *subtracted = dt.withFieldAdded(DurationFieldtype->millis(), -6);
 * </pre>
 *
 * @param millis  the amount of millis to subtract, may be negative
 * @return the new LocalDateTime *minus the increased millis
 */
LocalDateTime *LocalDateTime::minusMillis(int millis) {
    if (millis == 0) {
        return this;
    }
    int64_t instant = getChronology()->millis()->subtract(getLocalMillis(), millis);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Gets the property object for the specified type, which contains many
 * useful methods.
 *
 * @param fieldType  the field type to get the chronology for
 * @return the property object
 * @throws IllegalArgumentException if the field is NULL or unsupported
 */
LocalDateTime::Property *LocalDateTime::property(DateTimeFieldType *fieldType) {
    if (fieldType == NULL) {
        throw IllegalArgumentException("The DateTimeFieldType must not be NULL");
    }
    if (isSupported(fieldType) == false) {
        throw IllegalArgumentException("Field '" + fieldType->toString() + "' is not supported");
    }
    return new Property(this, fieldType->getField(getChronology()));
}

//-----------------------------------------------------------------------


/**
 * Output the date using the specified format pattern.
 *
 * @param pattern  the pattern specification, NULL means use <code>toString</code>
 * @see org.joda.time.format.DateTimeFormat
 */
string LocalDateTime::toString(string pattern) {
    if (pattern.empty()) {
        return toString();
    }
    return DateTimeFormat::forPattern(pattern)->print(this);
}

/**
 * Output the date using the specified format pattern.
 *
 * @param pattern  the pattern specification, NULL means use <code>toString</code>
 * @param locale  Locale to use, NULL means default
 * @see org.joda.time.format.DateTimeFormat
 */
string LocalDateTime::toString(string pattern, Locale *locale) {
    if (pattern.empty()) {
        return toString();
    }
    return DateTimeFormat::forPattern(pattern)->withLocale(locale)->print(this);
}

CODATIME_END