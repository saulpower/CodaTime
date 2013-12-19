//
//  LocalTime.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "LocalTime.h"

#include "Chronology.h"
#include "DateTimeFormat.h"
#include "DateTimeUtils.h"
#include "DurationField.h"
#include "DurationFieldType.h"

#include <vector>

CODATIME_BEGIN

const LocalTime *LocalTime::MIDNIGHT = new LocalTime(0, 0, 0, 0);

/**
 * Obtains a {@code LocalTime} set to the current system millisecond time
 * using <code>ISOChronology</code> in the default time zone.
 * The resulting object does not use the zone.
 *
 * @return the current time, not NULL
 * @since 2.0
 */
LocalTime *LocalTime::now() {
    return new LocalTime();
}

/**
 * Obtains a {@code LocalTime} set to the current system millisecond time
 * using <code>ISOChronology</code> in the specified time zone.
 * The resulting object does not use the zone.
 *
 * @param zone  the time zone, not NULL
 * @return the current time, not NULL
 * @since 2.0
 */
LocalTime *LocalTime::now(DateTimeZone *zone) {
    if (zone == NULL) {
        throw NullPointerException("Zone must not be NULL");
    }
    return new LocalTime(zone);
}

/**
 * Obtains a {@code LocalTime} set to the current system millisecond time
 * using the specified chronology.
 * The resulting object does not use the zone.
 *
 * @param chronology  the chronology, not NULL
 * @return the current time, not NULL
 * @since 2.0
 */
LocalTime *LocalTime::now(Chronology *chronology) {
    if (chronology == NULL) {
        throw NullPointerException("Chronology must not be NULL");
    }
    return new LocalTime(chronology);
}

//-----------------------------------------------------------------------
/**
 * Parses a {@code LocalTime} from the specified string.
 * <p>
 * This uses {@link ISODateTimeFormat#localTimeParser()}.
 *
 * @param str  the string to parse, not NULL
 * @since 2.0
 */
LocalTime *LocalTime::parse(string str) {
    return parse(str, ISODateTimeFormat::localTimeParser());
}

/**
 * Parses a {@code LocalTime} from the specified string using a formatter.
 *
 * @param str  the string to parse, not NULL
 * @param formatter  the formatter to use, not NULL
 * @since 2.0
 */
LocalTime *LocalTime::parse(string str, DateTimeFormatter *formatter) {
    return formatter->parseLocalTime(str);
}

//-----------------------------------------------------------------------
/**
 * Constructs a LocalTime *from the specified millis of day using the
 * ISO chronology.
 * <p>
 * The millisOfDay value may exceed the number of millis in one day,
 * but additional days will be ignored.
 * This method uses the UTC time zone internally.
 *
 * @param millisOfDay  the number of milliseconds into a day to convert
 */
LocalTime *LocalTime::fromMillisOfDay(int64_t millisOfDay) {
    return fromMillisOfDay(millisOfDay, NULL);
}

/**
 * Constructs a LocalTime *from the specified millis of day using the
 * specified chronology.
 * <p>
 * The millisOfDay value may exceed the number of millis in one day,
 * but additional days will be ignored.
 * This method uses the UTC time zone internally.
 *
 * @param millisOfDay  the number of milliseconds into a day to convert
 * @param chrono  the chronology, NULL means ISO chronology
 */
LocalTime *LocalTime::fromMillisOfDay(int64_t millisOfDay, Chronology *chrono) {
    chrono = DateTimeUtils::getChronology(chrono)->withUTC();
    return new LocalTime(millisOfDay, chrono);
}

//-----------------------------------------------------------------------
/**
 * Constructs an instance set to the current local time evaluated using
 * ISO chronology in the default zone.
 * <p>
 * Once the constructor is completed, the zone is no uint64_ter used.
 *
 * @see #now()
 */
LocalTime::LocalTime() : LocalTime(DateTimeUtils::currentTimeMillis(), ISOChronology::getInstance()) {
}

/**
 * Constructs an instance set to the current local time evaluated using
 * ISO chronology in the specified zone.
 * <p>
 * If the specified time zone is NULL, the default zone is used.
 * Once the constructor is completed, the zone is no uint64_ter used.
 *
 * @param zone  the time zone, NULL means default zone
 * @see #now(DateTimeZone)
 */
LocalTime::LocalTime(DateTimeZone *zone) : LocalTime(DateTimeUtils::currentTimeMillis(), ISOChronology::getInstance(zone)) {
}

/**
 * Constructs an instance set to the current local time evaluated using
 * specified chronology and zone.
 * <p>
 * If the chronology is NULL, ISO chronology in the default time zone is used.
 * Once the constructor is completed, the zone is no uint64_ter used.
 *
 * @param chronology  the chronology, NULL means ISOChronology in default zone
 * @see #now(Chronology)
 */
LocalTime::LocalTime(Chronology *chronology) : LocalTime(DateTimeUtils::currentTimeMillis(), chronology) {
}

//-----------------------------------------------------------------------
/**
 * Constructs an instance set to the local time defined by the specified
 * instant evaluated using ISO chronology in the default zone.
 * <p>
 * Once the constructor is completed, the zone is no uint64_ter used.
 *
 * @param instant  the milliseconds from 1970-01-01T00:00:00Z
 */
LocalTime::LocalTime(int64_t instant) : LocalTime(instant, ISOChronology::getInstance()) {
}

/**
 * Constructs an instance set to the local time defined by the specified
 * instant evaluated using ISO chronology in the specified zone.
 * <p>
 * If the specified time zone is NULL, the default zone is used.
 * Once the constructor is completed, the zone is no uint64_ter used.
 *
 * @param instant  the milliseconds from 1970-01-01T00:00:00Z
 * @param zone  the time zone, NULL means default zone
 */
LocalTime::LocalTime(int64_t instant, DateTimeZone *zone) : LocalTime(instant, ISOChronology::getInstance(zone)) {
}

/**
 * Constructs an instance set to the local time defined by the specified
 * instant evaluated using the specified chronology.
 * <p>
 * If the chronology is NULL, ISO chronology in the default zone is used.
 * Once the constructor is completed, the zone is no uint64_ter used.
 *
 * @param instant  the milliseconds from 1970-01-01T00:00:00Z
 * @param chronology  the chronology, NULL means ISOChronology in default zone
 */
LocalTime::LocalTime(int64_t instant, Chronology *chronology) {
    chronology = DateTimeUtils::getChronology(chronology);
    
    int64_t localMillis = chronology->getZone()->getMillisKeepLocal(DateTimeZone::UTC, instant);
    chronology = chronology->withUTC();
    iLocalMillis = chronology->millisOfDay()->get(localMillis);
    iChronology = chronology;
}

//-----------------------------------------------------------------------
/**
 * Constructs an instance from an Object that represents a datetime.
 * <p>
 * If the object contains no chronology, <code>ISOChronology</code> is used.
 * If the object contains no time zone, the default zone is used.
 * Once the constructor is completed, the zone is no uint64_ter used.
 * <p>
 * The recognised object types are defined in
 * {@link org.joda.time.convert.ConverterManager ConverterManager} and
 * include ReadablePartial, ReadableInstant, string, Calendar and Date.
 * The string formats are described by {@link ISODateTimeFormat#localTimeParser()}.
 * The default string converter ignores the zone and only parses the field values.
 *
 * @param instant  the datetime object
 * @throws IllegalArgumentException if the instant is invalid
 */
LocalTime::LocalTime(void *instant) : LocalTime(instant, (Chronology*) NULL) {
}

/**
 * Constructs an instance from an Object that represents a datetime,
 * forcing the time zone to that specified.
 * <p>
 * If the object contains no chronology, <code>ISOChronology</code> is used.
 * If the specified time zone is NULL, the default zone is used.
 * Once the constructor is completed, the zone is no uint64_ter used.
 * <p>
 * The recognised object types are defined in
 * {@link org.joda.time.convert.ConverterManager ConverterManager} and
 * include ReadablePartial, ReadableInstant, string, Calendar and Date.
 * The string formats are described by {@link ISODateTimeFormat#localTimeParser()}.
 * The default string converter ignores the zone and only parses the field values.
 *
 * @param instant  the datetime object
 * @param zone  the time zone
 * @throws IllegalArgumentException if the instant is invalid
 */
LocalTime::LocalTime(void *instant, DateTimeZone *zone) {
    PartialConverter *converter = ConverterManager::getInstance()->getPartialConverter(instant);
    Chronology *chronology = converter->getChronology(instant, zone);
    chronology = DateTimeUtils::getChronology(chronology);
    iChronology = chronology->withUTC();
    vector<int> values = converter->getPartialValues(this, instant, chronology, ISODateTimeFormat::localTimeParser());
    iLocalMillis = iChronology->getDateTimeMillis(0L, values[0], values[1], values[2], values[3]);
}

/**
 * Constructs an instance from an Object that represents a datetime,
 * using the specified chronology.
 * <p>
 * If the chronology is NULL, ISO in the default time zone is used.
 * Once the constructor is completed, the zone is no uint64_ter used.
 * <p>
 * The recognised object types are defined in
 * {@link org.joda.time.convert.ConverterManager ConverterManager} and
 * include ReadablePartial, ReadableInstant, string, Calendar and Date.
 * The string formats are described by {@link ISODateTimeFormat#localTimeParser()}.
 * The default string converter ignores the zone and only parses the field values.
 *
 * @param instant  the datetime object
 * @param chronology  the chronology
 * @throws IllegalArgumentException if the instant is invalid
 */
LocalTime::LocalTime(void *instant, Chronology *chronology) {
    PartialConverter *converter = ConverterManager::getInstance()->getPartialConverter(instant);
    chronology = converter->getChronology(instant, chronology);
    chronology = DateTimeUtils::getChronology(chronology);
    iChronology = chronology->withUTC();
    vector<int> values = converter->getPartialValues(this, instant, chronology, ISODateTimeFormat::localTimeParser());
    iLocalMillis = iChronology->getDateTimeMillis(0L, values[0], values[1], values[2], values[3]);
}

//-----------------------------------------------------------------------
/**
 * Constructs an instance set to the specified time
 * using <code>ISOChronology</code>.
 *
 * @param hourOfDay  the hour of the day, from 0 to 23
 * @param minuteOfHour  the minute of the hour, from 0 to 59
 */
LocalTime::LocalTime(int hourOfDay,
                     int minuteOfHour) : LocalTime(hourOfDay, minuteOfHour, 0, 0, ISOChronology::getInstanceUTC()) {
}

/**
 * Constructs an instance set to the specified time
 * using <code>ISOChronology</code>.
 *
 * @param hourOfDay  the hour of the day, from 0 to 23
 * @param minuteOfHour  the minute of the hour, from 0 to 59
 * @param secondOfMinute  the second of the minute, from 0 to 59
 */
LocalTime::LocalTime(int hourOfDay,
                 int minuteOfHour,
                     int secondOfMinute) : LocalTime(hourOfDay, minuteOfHour, secondOfMinute, 0, ISOChronology::getInstanceUTC()){
}

/**
 * Constructs an instance set to the specified time
 * using <code>ISOChronology</code>.
 *
 * @param hourOfDay  the hour of the day, from 0 to 23
 * @param minuteOfHour  the minute of the hour, from 0 to 59
 * @param secondOfMinute  the second of the minute, from 0 to 59
 * @param millisOfSecond  the millisecond of the second, from 0 to 999
 */
LocalTime::LocalTime(int hourOfDay,
                 int minuteOfHour,
                 int secondOfMinute,
                 int millisOfSecond) : LocalTime(hourOfDay, minuteOfHour, secondOfMinute,
                                                 millisOfSecond, ISOChronology::getInstanceUTC()) {
}

/**
 * Constructs an instance set to the specified time
 * using the specified chronology, whose zone is ignored.
 * <p>
 * If the chronology is NULL, <code>ISOChronology</code> is used.
 *
 * @param hourOfDay  the hour of the day, valid values defined by the chronology
 * @param minuteOfHour  the minute of the hour, valid values defined by the chronology
 * @param secondOfMinute  the second of the minute, valid values defined by the chronology
 * @param millisOfSecond  the millisecond of the second, valid values defined by the chronology
 * @param chronology  the chronology, NULL means ISOChronology in default zone
 */
LocalTime::LocalTime(int hourOfDay,
                 int minuteOfHour,
                 int secondOfMinute,
                 int millisOfSecond,
                 Chronology *chronology) {
    chronology = DateTimeUtils::getChronology(chronology)->withUTC();
    int64_t instant = chronology->getDateTimeMillis(0L, hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond);
    iChronology = chronology;
    iLocalMillis = instant;
}

/**
 * Handle broken serialization from other tools.
 * @return the resolved object, not NULL
 */
LocalTime *LocalTime::readResolve() {
    if (iChronology == NULL) {
        return new LocalTime(iLocalMillis, ISOChronology::getInstanceUTC());
    }
    if (DateTimeZone::UTC->equals(iChronology->getZone()) == false) {
        return new LocalTime(iLocalMillis, iChronology->withUTC());
    }
    return this;
}

//-----------------------------------------------------------------------
/**
 * Gets the number of fields in this partial, which is four.
 * The supported fields are HourOfDay, MinuteOfHour, SecondOfMinute
 * and MillisOfSecond.
 *
 * @return the field count, four
 */
int LocalTime::size() const {
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
const DateTimeField *LocalTime::getField(int index, Chronology *chrono) const {
    switch (index) {
        case HOUR_OF_DAY:
            return chrono->hourOfDay();
        case MINUTE_OF_HOUR:
            return chrono->minuteOfHour();
        case SECOND_OF_MINUTE:
            return chrono->secondOfMinute();
        case MILLIS_OF_SECOND:
            return chrono->millisOfSecond();
        default:
            string err("Invalid index: ");
            err.append(to_string(index));
            throw IndexOutOfBoundsException(err);
    }
}

/**
 * Gets the value of the field at the specifed index.
 * <p>
 * This method is required to support the <code>ReadablePartial</code>
 * interface. The supported fields are HourOfDay, MinuteOfHour,
 * SecondOfMinute and MillisOfSecond.
 *
 * @param index  the index, zero to three
 * @return the value
 * @throws IndexOutOfBoundsException if the index is invalid
 */
int LocalTime::getValue(int index) const {
    switch (index) {
        case HOUR_OF_DAY:
            return getChronology()->hourOfDay()->get(getLocalMillis());
        case MINUTE_OF_HOUR:
            return getChronology()->minuteOfHour()->get(getLocalMillis());
        case SECOND_OF_MINUTE:
            return getChronology()->secondOfMinute()->get(getLocalMillis());
        case MILLIS_OF_SECOND:
            return getChronology()->millisOfSecond()->get(getLocalMillis());
        default:
            string err("Invalid index: ");
            err.append(to_string(index));
            throw IndexOutOfBoundsException(err);
    }
}

//-----------------------------------------------------------------------
/**
 * Get the value of one of the fields of time.
 * <p>
 * This method gets the value of the specified field->
 * For example:
 * <pre>
 * DateTime dt = new DateTime();
 * int hourOfDay = dt.get(DateTimeFieldtype->hourOfDay());
 * </pre>
 *
 * @param fieldType  a field type, usually obtained from DateTimeFieldType, not NULL
 * @return the value of that field
 * @throws IllegalArgumentException if the field type is NULL
 */
int LocalTime::get(const DateTimeFieldType *fieldType) const {
    if (fieldType == NULL) {
        throw IllegalArgumentException("The DateTimeFieldType *must not be NULL");
    }
    if (isSupported(fieldType) == false) {
        string err("Field '");
        err.append(fieldType->toString());
        err.append("' is not supported");
        throw IllegalArgumentException(err);
    }
    return fieldType->getField(getChronology())->get(getLocalMillis());
}

/**
 * Checks if the field type specified is supported by this
 * local time and chronology.
 * This can be used to avoid exceptions in {@link #get(DateTimeFieldType)}.
 *
 * @param type  a field type, usually obtained from DateTimeFieldType
 * @return true if the field type is supported
 */
bool LocalTime::isSupported(const DateTimeFieldType *type) const {
    if (type == NULL) {
        return false;
    }
    if (isSupported(type->getDurationType()) == false) {
        return false;
    }
    const DurationFieldType *range = type->getRangeDurationType();
    return (isSupported(range) || range == DurationFieldType::days());
}

/**
 * Checks if the duration type specified is supported by this
 * local time and chronology.
 *
 * @param type  a duration type, usually obtained from DurationFieldType
 * @return true if the field type is supported
 */
bool LocalTime::isSupported(const DurationFieldType *type) const {
    if (type == NULL) {
        return false;
    }
    const DurationField *field = type->getField(getChronology());
    bool contains = find(TIME_DURATION_TYPES.begin(), TIME_DURATION_TYPES.end(), type) != TIME_DURATION_TYPES.end();
    if (contains || field->getUnitMillis() < getChronology()->days()->getUnitMillis()) {
        return field->isSupported();
    }
    return false;
}

//-----------------------------------------------------------------------
/**
 * Gets the local milliseconds from the Java epoch
 * of 1970-01-01T00:00:00 (not fixed to any specific time zone).
 *
 * @return the number of milliseconds since 1970-01-01T00:00:00
 * @since 1.5 (previously private)
 */
int64_t LocalTime::getLocalMillis() const {
    return iLocalMillis;
}

/**
 * Gets the chronology of the time.
 *
 * @return the Chronology that the time is using
 */
Chronology *LocalTime::getChronology() const {
    return iChronology;
}

//-----------------------------------------------------------------------
/**
 * Compares this ReadablePartial with another returning true if the chronology,
 * field types and values are equal.
 *
 * @param partial  an object to check against
 * @return true if fields and values are equal
 */
bool LocalTime::equals(const Object *partial) const {
    // override to perform faster
    if (this == partial) {
        return true;
    }
    const LocalTime *other = dynamic_cast<const LocalTime*>(partial);
    if (other != 0) {
        if (iChronology->equals(other->iChronology)) {
            return iLocalMillis == other->iLocalMillis;
        }
    }
    return AbstractPartial::equals(partial);
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
int LocalTime::compareTo(const ReadablePartial *partial) const {
    // override to perform faster
//    if (this == partial) {
//        return 0;
//    }
    const LocalTime *other = dynamic_cast<const LocalTime*>(partial);
    if (other != 0) {
        if (iChronology->equals(other->iChronology)) {
            return (iLocalMillis < other->iLocalMillis ? -1 :
                    (iLocalMillis == other->iLocalMillis ? 0 : 1));
            
        }
    }
    return AbstractPartial::compareTo(partial);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this time with different local millis.
 * <p>
 * The returned object will be a new instance of the same type->
 * Only the millis will change, the chronology is kept.
 * The returned object will be either be a new instance or <code>this</code>.
 *
 * @param newMillis  the new millis, from 1970-01-01T00:00:00
 * @return a copy of this time with different millis
 */
LocalTime *LocalTime::withLocalMillis(int64_t newMillis) {
    return (newMillis == getLocalMillis() ? this : new LocalTime(newMillis, getChronology()));
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this time with the partial set of fields replacing
 * those from this instance.
 * <p>
 * For example, if the partial contains an hour and minute then those two
 * fields will be changed in the returned instance.
 * Unsupported fields are ignored.
 * If the partial is NULL, then <code>this</code> is returned.
 *
 * @param partial  the partial set of fields to apply to this time, NULL ignored
 * @return a copy of this time with a different set of fields
 * @throws IllegalArgumentException if any value is invalid
 */
LocalTime *LocalTime::withFields(ReadablePartial *partial) {
    if (partial == NULL) {
        return this;
    }
    return withLocalMillis(getChronology()->set(partial, getLocalMillis()));
}

/**
 * Returns a copy of this time with the specified field set
 * to a new value.
 * <p>
 * For example, if the field type is <code>hourOfDay</code> then the hour of day
 * field would be changed in the returned instance.
 * If the field type is NULL, then <code>this</code> is returned.
 * <p>
 * These lines are equivalent:
 * <pre>
 * LocalTime *updated = dt.withHourOfDay(6);
 * LocalTime *updated = dt.withField(DateTimeFieldtype->hourOfDay(), 6);
 * </pre>
 *
 * @param fieldType  the field type to set, not NULL
 * @param value  the value to set
 * @return a copy of this time with the field set
 * @throws IllegalArgumentException if the value is NULL or invalid
 */
LocalTime *LocalTime::withField(DateTimeFieldType *fieldType, int value) {
    if (fieldType == NULL) {
        throw IllegalArgumentException("Field must not be NULL");
    }
    if (isSupported(fieldType) == false) {
        string err("Field '");
        err.append(fieldType->toString());
        err.append("' is not supported");
        throw IllegalArgumentException(err);
    }
    int64_t instant = fieldType->getField(getChronology())->set(getLocalMillis(), value);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this time with the value of the specified
 * field increased.
 * <p>
 * If the addition is zero or the field is NULL, then <code>this</code>
 * is returned.
 * <p>
 * If the addition causes the maximum value of the field to be exceeded,
 * then the value will wrap. Thus 23:59 plus two minutes yields 00:01.
 * <p>
 * These lines are equivalent:
 * <pre>
 * LocalTime *added = dt.plusHours(6);
 * LocalTime *added = dt.withFieldAdded(DurationFieldtype->hours(), 6);
 * </pre>
 *
 * @param fieldType  the field type to add to, not NULL
 * @param amount  the amount to add
 * @return a copy of this time with the field updated
 * @throws IllegalArgumentException if the value is NULL or invalid
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalTime *LocalTime::withFieldAdded(DurationFieldType *fieldType, int amount) {
    if (fieldType == NULL) {
        throw IllegalArgumentException("Field must not be NULL");
    }
    if (isSupported(fieldType) == false) {
        string err("Field '");
        err.append(fieldType->toString());
        err.append("' is not supported");
        throw IllegalArgumentException(err);
    }
    if (amount == 0) {
        return this;
    }
    int64_t instant = fieldType->getField(getChronology())->add(getLocalMillis(), amount);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this time with the specified period added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 * <p>
 * This method is typically used to add multiple copies of complex
 * period instances. Adding one field is best achieved using methods
 * like {@link #withFieldAdded(DurationFieldType, int)}
 * or {@link #plusHours(int)}.
 *
 * @param period  the period to add to this one, NULL means zero
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this time with the period added
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalTime *LocalTime::withPeriodAdded(ReadablePeriod *period, int scalar) {
    if (period == NULL || scalar == 0) {
        return this;
    }
    int64_t instant = getChronology()->add(period, getLocalMillis(), scalar);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this time with the specified period added.
 * <p>
 * If the amount is zero or NULL, then <code>this</code> is returned.
 * <p>
 * This method is typically used to add complex period instances.
 * Adding one field is best achieved using methods
 * like {@link #plusHours(int)}.
 *
 * @param period  the period to add to this one, NULL means zero
 * @return a copy of this time with the period added
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalTime *LocalTime::plus(ReadablePeriod *period) {
    return withPeriodAdded(period, 1);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this time plus the specified number of hours.
 * <p>
 * This LocalTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalTime *added = dt.plusHours(6);
 * LocalTime *added = dt.plus(Period.hours(6));
 * LocalTime *added = dt.withFieldAdded(DurationFieldtype->hours(), 6);
 * </pre>
 *
 * @param hours  the amount of hours to add, may be negative
 * @return the new LocalTime *plus the increased hours
 */
LocalTime *LocalTime::plusHours(int hours) {
    if (hours == 0) {
        return this;
    }
    int64_t instant = getChronology()->hours()->add(getLocalMillis(), hours);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this time plus the specified number of minutes.
 * <p>
 * This LocalTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalTime *added = dt.plusMinutes(6);
 * LocalTime *added = dt.plus(Period.minutes(6));
 * LocalTime *added = dt.withFieldAdded(DurationFieldtype->minutes(), 6);
 * </pre>
 *
 * @param minutes  the amount of minutes to add, may be negative
 * @return the new LocalTime *plus the increased minutes
 */
LocalTime *LocalTime::plusMinutes(int minutes) {
    if (minutes == 0) {
        return this;
    }
    int64_t instant = getChronology()->minutes()->add(getLocalMillis(), minutes);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this time plus the specified number of seconds.
 * <p>
 * This LocalTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalTime *added = dt.plusSeconds(6);
 * LocalTime *added = dt.plus(Period.seconds(6));
 * LocalTime *added = dt.withFieldAdded(DurationFieldtype->seconds(), 6);
 * </pre>
 *
 * @param seconds  the amount of seconds to add, may be negative
 * @return the new LocalTime *plus the increased seconds
 */
LocalTime *LocalTime::plusSeconds(int seconds) {
    if (seconds == 0) {
        return this;
    }
    int64_t instant = getChronology()->seconds()->add(getLocalMillis(), seconds);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this time plus the specified number of millis.
 * <p>
 * This LocalTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalTime *added = dt.plusMillis(6);
 * LocalTime *added = dt.plus(Period.millis(6));
 * LocalTime *added = dt.withFieldAdded(DurationFieldtype->millis(), 6);
 * </pre>
 *
 * @param millis  the amount of millis to add, may be negative
 * @return the new LocalTime *plus the increased millis
 */
LocalTime *LocalTime::plusMillis(int millis) {
    if (millis == 0) {
        return this;
    }
    int64_t instant = getChronology()->millis()->add(getLocalMillis(), millis);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this time with the specified period taken away.
 * <p>
 * If the amount is zero or NULL, then <code>this</code> is returned.
 * <p>
 * This method is typically used to subtract complex period instances.
 * Subtracting one field is best achieved using methods
 * like {@link #minusHours(int)}.
 *
 * @param period  the period to reduce this instant by
 * @return a copy of this time with the period taken away
 * @throws ArithmeticException if the result exceeds the internal capacity
 */
LocalTime *LocalTime::minus(ReadablePeriod *period) {
    return withPeriodAdded(period, -1);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this time minus the specified number of hours.
 * <p>
 * This LocalTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalTime *subtracted = dt.minusHours(6);
 * LocalTime *subtracted = dt.minus(Period.hours(6));
 * LocalTime *subtracted = dt.withFieldAdded(DurationFieldtype->hours(), -6);
 * </pre>
 *
 * @param hours  the amount of hours to subtract, may be negative
 * @return the new LocalTime *minus the increased hours
 */
LocalTime *LocalTime::minusHours(int hours) {
    if (hours == 0) {
        return this;
    }
    int64_t instant = getChronology()->hours()->subtract(getLocalMillis(), hours);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this time minus the specified number of minutes.
 * <p>
 * This LocalTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalTime *subtracted = dt.minusMinutes(6);
 * LocalTime *subtracted = dt.minus(Period.minutes(6));
 * LocalTime *subtracted = dt.withFieldAdded(DurationFieldtype->minutes(), -6);
 * </pre>
 *
 * @param minutes  the amount of minutes to subtract, may be negative
 * @return the new LocalTime *minus the increased minutes
 */
LocalTime *LocalTime::minusMinutes(int minutes) {
    if (minutes == 0) {
        return this;
    }
    int64_t instant = getChronology()->minutes()->subtract(getLocalMillis(), minutes);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this time minus the specified number of seconds.
 * <p>
 * This LocalTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalTime *subtracted = dt.minusSeconds(6);
 * LocalTime *subtracted = dt.minus(Period.seconds(6));
 * LocalTime *subtracted = dt.withFieldAdded(DurationFieldtype->seconds(), -6);
 * </pre>
 *
 * @param seconds  the amount of seconds to subtract, may be negative
 * @return the new LocalTime *minus the increased seconds
 */
LocalTime *LocalTime::minusSeconds(int seconds) {
    if (seconds == 0) {
        return this;
    }
    int64_t instant = getChronology()->seconds()->subtract(getLocalMillis(), seconds);
    return withLocalMillis(instant);
}

/**
 * Returns a copy of this time minus the specified number of millis.
 * <p>
 * This LocalTime *instance is immutable and unaffected by this method call.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * LocalTime *subtracted = dt.minusMillis(6);
 * LocalTime *subtracted = dt.minus(Period.millis(6));
 * LocalTime *subtracted = dt.withFieldAdded(DurationFieldtype->millis(), -6);
 * </pre>
 *
 * @param millis  the amount of millis to subtract, may be negative
 * @return the new LocalTime *minus the increased millis
 */
LocalTime *LocalTime::minusMillis(int millis) {
    if (millis == 0) {
        return this;
    }
    int64_t instant = getChronology()->millis()->subtract(getLocalMillis(), millis);
    return withLocalMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Gets the property object for the specified type, which contains
 * many useful methods.
 *
 * @param fieldType  the field type to get the chronology for
 * @return the property object
 * @throws IllegalArgumentException if the field is NULL or unsupported
 */
LocalTime::Property *LocalTime::property(DateTimeFieldType *fieldType) {
    if (fieldType == NULL) {
        throw IllegalArgumentException("The DateTimeFieldType *must not be NULL");
    }
    if (isSupported(fieldType) == false) {
        string err("Field '");
        err.append(fieldType->toString());
        err.append("' is not supported");
        throw IllegalArgumentException(err);
    }
    return new Property(this, fieldType->getField(getChronology()));
}

//-----------------------------------------------------------------------
/**
 * Get the hour of day field value.
 *
 * @return the hour of day
 */
int LocalTime::getHourOfDay() {
    return getChronology()->hourOfDay()->get(getLocalMillis());
}

/**
 * Get the minute of hour field value.
 *
 * @return the minute of hour
 */
int LocalTime::getMinuteOfHour() {
    return getChronology()->minuteOfHour()->get(getLocalMillis());
}

/**
 * Get the second of minute field value.
 *
 * @return the second of minute
 */
int LocalTime::getSecondOfMinute() {
    return getChronology()->secondOfMinute()->get(getLocalMillis());
}

/**
 * Get the millis of second field value.
 *
 * @return the millis of second
 */
int LocalTime::getMillisOfSecond() {
    return getChronology()->millisOfSecond()->get(getLocalMillis());
}

/**
 * Get the millis of day field value.
 *
 * @return the millis of day
 */
int LocalTime::getMillisOfDay() {
    return getChronology()->millisOfDay()->get(getLocalMillis());
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this time with the hour of day field updated.
 * <p>
 * LocalTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * hour of day changed.
 *
 * @param hour  the hour of day to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 */
LocalTime *LocalTime::withHourOfDay(int hour) {
    return withLocalMillis(getChronology()->hourOfDay()->set(getLocalMillis(), hour));
}

/**
 * Returns a copy of this time with the minute of hour field updated.
 * <p>
 * LocalTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * minute of hour changed.
 *
 * @param minute  the minute of hour to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 */
LocalTime *LocalTime::withMinuteOfHour(int minute) {
    return withLocalMillis(getChronology()->minuteOfHour()->set(getLocalMillis(), minute));
}

/**
 * Returns a copy of this time with the second of minute field updated.
 * <p>
 * LocalTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * second of minute changed.
 *
 * @param second  the second of minute to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 */
LocalTime *LocalTime::withSecondOfMinute(int second) {
    return withLocalMillis(getChronology()->secondOfMinute()->set(getLocalMillis(), second));
}

/**
 * Returns a copy of this time with the millis of second field updated.
 * <p>
 * LocalTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * millis of second changed.
 *
 * @param millis  the millis of second to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 */
LocalTime *LocalTime::withMillisOfSecond(int millis) {
    return withLocalMillis(getChronology()->millisOfSecond()->set(getLocalMillis(), millis));
}

/**
 * Returns a copy of this time with the millis of day field updated.
 * <p>
 * LocalTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * millis of day changed.
 *
 * @param millis  the millis of day to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 */
LocalTime *LocalTime::withMillisOfDay(int millis) {
    return withLocalMillis(getChronology()->millisOfDay()->set(getLocalMillis(), millis));
}

//-----------------------------------------------------------------------
/**
 * Get the hour of day field property which provides access to advanced functionality.
 *
 * @return the hour of day property
 */
LocalTime::Property *LocalTime::hourOfDay() {
    return new Property(this, getChronology()->hourOfDay());
}

/**
 * Get the minute of hour field property which provides access to advanced functionality.
 *
 * @return the minute of hour property
 */
LocalTime::Property *LocalTime::minuteOfHour() {
    return new Property(this, getChronology()->minuteOfHour());
}

/**
 * Get the second of minute field property which provides access to advanced functionality.
 *
 * @return the second of minute property
 */
LocalTime::Property *LocalTime::secondOfMinute() {
    return new Property(this, getChronology()->secondOfMinute());
}

/**
 * Get the millis of second property which provides access to advanced functionality.
 *
 * @return the millis of second property
 */
LocalTime::Property *LocalTime::millisOfSecond() {
    return new Property(this, getChronology()->millisOfSecond());
}

/**
 * Get the millis of day property which provides access to advanced functionality.
 *
 * @return the millis of day property
 */
LocalTime::Property *LocalTime::millisOfDay() {
    return new Property(this, getChronology()->millisOfDay());
}

//-----------------------------------------------------------------------
/**
 * Converts this LocalTime *to a full datetime using the default time zone
 * setting the time fields from this instance and the date fields from
 * the current date.
 *
 * @return this time as a datetime using todays date
 */
DateTime *LocalTime::toDateTimeToday() {
    return toDateTimeToday(NULL);
}

/**
 * Converts this LocalTime *to a full datetime using the specified time zone
 * setting the time fields from this instance and the date fields from
 * the current time.
 * <p>
 * This method uses the chronology from this instance plus the time zone
 * specified.
 *
 * @param zone  the zone to use, NULL means default
 * @return this time as a datetime using todays date
 */
DateTime *LocalTime::toDateTimeToday(DateTimeZone *zone) {
    Chronology *chrono = getChronology()->withZone(zone);
    int64_t instantMillis = DateTimeUtils::currentTimeMillis();
    int64_t resolved = chrono->set(this, instantMillis);
    return new DateTime(resolved, chrono);
}

//-----------------------------------------------------------------------
/**
 * Output the time in ISO8601 format (HH:mm:ss.SSS).
 *
 * @return ISO8601 time formatted string.
 */
string LocalTime::toString() {
    return ISODateTimeFormat::time()->print(this);
}

/**
 * Output the time using the specified format pattern.
 *
 * @param pattern  the pattern specification, NULL means use <code>tostring</code>
 * @see org.joda.time.format.DateTimeFormat
 */
string LocalTime::toString(string pattern) {
    if (pattern.empty()) {
        return toString();
    }
    return DateTimeFormat::forPattern(pattern)->print(this);
}

/**
 * Output the time using the specified format pattern.
 *
 * @param pattern  the pattern specification, NULL means use <code>tostring</code>
 * @param locale  Locale to use, NULL means default
 * @see org.joda.time.format.DateTimeFormat
 */
string LocalTime::toString(string pattern, Locale *locale) {
    if (pattern.empty()) {
        return toString();
    }
    return DateTimeFormat::forPattern(pattern)->withLocale(locale)->print(this);
}

CODATIME_END