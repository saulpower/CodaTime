//
//  DateTime.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "DateTime.h"

#include "format/DateTimeFormatter.h"
#include "LocalDateTime.h"

CODATIME_BEGIN

/**
 * Obtains a {@code DateTime} set to the current system millisecond time
 * using <code>ISOChronology</code> in the specified time zone.
 *
 * @param zone  the time zone, not null
 * @return the current date-time, not null
 * @since 2.0
 */
DateTime *DateTime::now(DateTimeZone *zone) {
    if (zone == NULL) {
        throw NullPointerException("Zone must not be null");
    }
    return new DateTime(zone);
}

/**
 * Obtains a {@code DateTime} set to the current system millisecond time
 * using the specified chronology.
 *
 * @param chronology  the chronology, not null
 * @return the current date-time, not null
 * @since 2.0
 */
DateTime *DateTime::now(Chronology *chronology) {
    if (chronology == NULL) {
        throw NullPointerException("Chronology must not be null");
    }
    return new DateTime(chronology);
}

//-----------------------------------------------------------------------
/**
 * Parses a {@code DateTime} from the specified string.
 * <p>
 * This uses {@link ISODateTimeFormat#dateTimeParser()}.
 *
 * @param str  the string to parse, not null
 * @since 2.0
 */
DateTime *DateTime::parse(string str) {
    return parse(str, ISODateTimeFormat::dateTimeParser()->withOffsetParsed());
}

/**
 * Parses a {@code DateTime} from the specified string using a formatter.
 *
 * @param str  the string to parse, not null
 * @param formatter  the formatter to use, not null
 * @since 2.0
 */
DateTime *DateTime::parse(string str, DateTimeFormatter *formatter) {
    return formatter->parseDateTime(str);
}


/**
 * Get this object as a DateTime *using ISOChronology in the default zone,
 * returning <code>this</code> if possible.
 *
 * @return a DateTime *using the same millis
 */
DateTime *DateTime::toDateTimeISO() {
    if (getChronology() == ISOChronology::getInstance()) {
        return this;
    }
    return BaseDateTime::toDateTimeISO();
}

/**
 * Get this object as a DateTime, returning <code>this</code> if possible.
 *
 * @param zone time zone to apply, or default if null
 * @return a DateTime *using the same millis
 */
DateTime *DateTime::toDateTime(DateTimeZone *zone) {
    zone = DateTimeUtils::getZone(zone);
    if (getZone() == zone) {
        return this;
    }
    return AbstractInstant::toDateTime(zone);
}

/**
 * Get this object as a DateTime, returning <code>this</code> if possible.
 *
 * @param Chronology *chronology to apply, or ISOChronology if null
 * @return a DateTime *using the same millis
 */
DateTime *DateTime::toDateTime(Chronology *chronology) {
    chronology = DateTimeUtils::getChronology(chronology);
    if (getChronology() == chronology) {
        return this;
    }
    return AbstractInstant::toDateTime(chronology);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *with different millis.
 * <p>
 * The returned object will be either be a new instance or <code>this</code>.
 * Only the millis will change, the chronology and time zone are kept.
 *
 * @param newMillis  the new millis, from 1970-01-01T00:00:00Z
 * @return a copy of this DateTime *with different millis
 */
DateTime *DateTime::withMillis(int64_t newMillis) {
    return (newMillis == getMillis() ? this : new DateTime(newMillis, getChronology()));
}

/**
 * Returns a copy of this DateTime *with a different chronology.
 * <p>
 * The returned object will be either be a new instance or <code>this</code>.
 * Only the chronology will change, the millis are kept.
 *
 * @param newChronology  the new chronology, null means ISO default
 * @return a copy of this DateTime *with a different chronology
 */
DateTime *DateTime::withChronology(Chronology *newChronology) {
    newChronology = DateTimeUtils::getChronology(newChronology);
    return (newChronology == getChronology() ? this : new DateTime(getMillis(), newChronology));
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *with a different time zone, preserving the
 * millisecond instant.
 * <p>
 * This method is useful for finding the local time in another timezone.
 * For example, if this instant holds 12:30 in Europe/London, the result
 * from this method with Europe/Paris would be 13:30.
 * <p>
 * The returned object will be a new instance of the same implementation type->
 * This method changes the time zone, and does not change the
 * millisecond instant, with the effect that the field values usually change.
 * The returned object will be either be a new instance or <code>this</code>.
 *
 * @param newZone  the new time zone
 * @return a copy of this DateTime *with a different time zone
 * @see #withZoneRetainFields
 */
DateTime *DateTime::withZone(DateTimeZone *newZone) {
    return withChronology(getChronology()->withZone(newZone));
}

/**
 * Returns a copy of this DateTime *with a different time zone, preserving the
 * field values.
 * <p>
 * This method is useful for finding the millisecond time in another timezone.
 * For example, if this instant holds 12:30 in Europe/London (ie. 12:30Z),
 * the result from this method with Europe/Paris would be 12:30 (ie. 11:30Z).
 * <p>
 * The returned object will be a new instance of the same implementation type->
 * This method changes the time zone and the millisecond instant to keep
 * the field values the same.
 * The returned object will be either be a new instance or <code>this</code>.
 *
 * @param newZone  the new time zone, null means default
 * @return a copy of this DateTime *with a different time zone
 * @see #withZone
 */
DateTime *DateTime::withZoneRetainFields(DateTimeZone *newZone) {
    newZone = DateTimeUtils::getZone(newZone);
    DateTimeZone *originalZone = DateTimeUtils::getZone(getZone());
    if (newZone == originalZone) {
        return this;
    }
    
    int64_t millis = originalZone->getMillisKeepLocal(newZone, getMillis());
    return new DateTime(millis, getChronology()->withZone(newZone));
}

/**
 * Returns a copy of this ZonedDateTime *changing the zone offset to the earlier
 * of the two valid offsets at a local time-line overlap.
 * <p>
 * This method only has any effect when the local time-line overlaps, such as at
 * an autumn daylight savings cutover. In this scenario, there are two valid offsets
 * for the local date-time. Calling this method will return a date-time with the
 * earlier of the two selected.
 * <p>
 * If this method is called when it is not an overlap, this is returned.
 * <p>
 * This instance is immutable and unaffected by this method call.
 *
 * @return a copy of this DateTime *with the earliest valid offset for the local datetime
 */
DateTime *DateTime::withEarlierOffsetAtOverlap() {
    int64_t newMillis = getZone()->adjustOffset(getMillis(), false);
    return withMillis(newMillis);
}

/**
 * Returns a copy of this ZonedDateTime *changing the zone offset to the later
 * of the two valid offsets at a local time-line overlap.
 * <p>
 * This method only has any effect when the local time-line overlaps, such as at
 * an autumn daylight savings cutover. In this scenario, there are two valid offsets
 * for the local date-time. Calling this method will return a date-time with the
 * later of the two selected.
 * <p>
 * If this method is called when it is not an overlap, this is returned.
 * <p>
 * This instance is immutable and unaffected by this method call.
 *
 * @return a copy of this DateTime *with the latest valid offset for the local datetime
 */
DateTime *DateTime::withLaterOffsetAtOverlap() {
    int64_t newMillis = getZone()->adjustOffset(getMillis(), true);
    return withMillis(newMillis);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *with the specified date, retaining the time fields.
 * <p>
 * If the date is already the date passed in, then <code>this</code> is returned.
 * <p>
 * To set a single field use the properties, for example:
 * <pre>
 * DateTime *set = monthOfYear()->setCopy(6);
 * </pre>
 * <p>
 * This instance is immutable and unaffected by this method call.
 *
 * @param year  the new year value
 * @param monthOfYear  the new monthOfYear value
 * @param dayOfMonth  the new dayOfMonth value
 * @return a copy of this DateTime *with a different date
 * @throws IllegalArgumentException if any value if invalid
 */
DateTime *DateTime::withDate(int year, int monthOfYear, int dayOfMonth) {
    Chronology *chrono = getChronology();
    int64_t instant = getMillis();
    instant = chrono->year()->set(instant, year);
    instant = chrono->monthOfYear()->set(instant, monthOfYear);
    instant = chrono->dayOfMonth()->set(instant, dayOfMonth);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *with the specified time, retaining the date fields.
 * <p>
 * If the time is already the time passed in, then <code>this</code> is returned.
 * <p>
 * To set a single field use the properties, for example:
 * <pre>
 * DateTime *set = dt.hourOfDay()->setCopy(6);
 * </pre>
 * <p>
 * This instance is immutable and unaffected by this method call.
 *
 * @param hourOfDay  the hour of the day
 * @param minuteOfHour  the minute of the hour
 * @param secondOfMinute  the second of the minute
 * @param millisOfSecond  the millisecond of the second
 * @return a copy of this DateTime *with a different time
 * @throws IllegalArgumentException if any value if invalid
 */
DateTime *DateTime::withTime(int hourOfDay, int minuteOfHour, int secondOfMinute, int millisOfSecond) {
    Chronology *chrono = getChronology();
    int64_t instant = getMillis();
    instant = chrono->hourOfDay()->set(instant, hourOfDay);
    instant = chrono->minuteOfHour()->set(instant, minuteOfHour);
    instant = chrono->secondOfMinute()->set(instant, secondOfMinute);
    instant = chrono->millisOfSecond()->set(instant, millisOfSecond);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *with the time set to the start of the day.
 * <p>
 * The time will normally be midnight, as that is the earliest time on
 * any given day. However, in some time zones when Daylight Savings Time
 * starts, there is no midnight because time jumps from 11:59 to 01:00.
 * This method handles that situation by returning 01:00 on that date.
 * <p>
 * This instance is immutable and unaffected by this method call.
 *
 * @return a copy of this DateTime *with the time set to the start of the day, not null
 */
DateTime *DateTime::withTimeAtStartOfDay() {
    return toLocalDate()->toDateTimeAtStartOfDay(getZone());
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *with the partial set of fields replacing those
 * from this instance.
 * <p>
 * For example, if the partial is a <code>TimeOfDay</code> then the time fields
 * would be changed in the returned instance.
 * If the partial is null, then <code>this</code> is returned.
 *
 * @param partial  the partial set of fields to apply to this datetime, null ignored
 * @return a copy of this DateTime *with a different set of fields
 * @throws IllegalArgumentException if any value is invalid
 */
DateTime *DateTime::withFields(ReadablePartial *partial) {
    if (partial == NULL) {
        return this;
    }
    return withMillis(getChronology()->set(partial, getMillis()));
}

/**
 * Returns a copy of this DateTime *with the specified field set to a new value.
 * <p>
 * For example, if the field type is <code>hourOfDay</code> then the hour of day
 * field would be changed in the returned instance.
 * If the field type is null, then <code>this</code> is returned.
 * <p>
 * These three lines are equivalent:
 * <pre>
 * DateTime *updated = dt.withField(DateTimeFieldType.dayOfMonth(), 6);
 * DateTime *updated = dt.dayOfMonth()->setCopy(6);
 * DateTime *updated = dt.property(DateTimeFieldType.dayOfMonth()).setCopy(6);
 * </pre>
 *
 * @param fieldType  the field type to set, not null
 * @param value  the value to set
 * @return a copy of this DateTime *with the field set
 * @throws IllegalArgumentException if the value is null or invalid
 */
DateTime *DateTime::withField(DateTimeFieldType *fieldType, int value) {
    if (fieldType == NULL) {
        throw IllegalArgumentException("Field must not be null");
    }
    int64_t instant = fieldType->getField(getChronology())->set(getMillis(), value);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *with the value of the specified field increased.
 * <p>
 * If the addition is zero or the field is null, then <code>this</code> is returned.
 * <p>
 * These three lines are equivalent:
 * <pre>
 * DateTime *added = dt.withFieldAdded(DurationFieldType.years(), 6);
 * DateTime *added = dt.plusYears(6);
 * DateTime *added = dt.plus(Period.years(6));
 * </pre>
 *
 * @param fieldType  the field type to add to, not null
 * @param amount  the amount to add
 * @return a copy of this DateTime *with the field updated
 * @throws IllegalArgumentException if the value is null or invalid
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::withFieldAdded(DurationFieldType *fieldType, int amount) {
    if (fieldType == NULL) {
        throw IllegalArgumentException("Field must not be null");
    }
    if (amount == 0) {
        return this;
    }
    int64_t instant = fieldType->getField(getChronology())->add(getMillis(), amount);
    return withMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *with the specified duration added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 *
 * @param durationToAdd  the duration to add to this one
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this DateTime *with the duration added
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::withDurationAdded(int64_t durationToAdd, int scalar) {
    if (durationToAdd == 0 || scalar == 0) {
        return this;
    }
    int64_t instant = getChronology()->add(getMillis(), durationToAdd, scalar);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *with the specified duration added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 *
 * @param durationToAdd  the duration to add to this one, null means zero
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this DateTime *with the duration added
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::withDurationAdded(ReadableDuration *durationToAdd, int scalar) {
    if (durationToAdd == NULL || scalar == 0) {
        return this;
    }
    return withDurationAdded(durationToAdd->getMillis(), scalar);
}

/**
 * Returns a copy of this DateTime *with the specified period added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 * <p>
 * This method is typically used to add multiple copies of complex
 * period instances. Adding one field is best achieved using methods
 * like {@link #withFieldAdded(DurationFieldType, int)}
 * or {@link #plusYears(int)}.
 *
 * @param period  the period to add to this one, null means zero
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this DateTime *with the period added
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::withPeriodAdded(ReadablePeriod *period, int scalar) {
    if (period == NULL || scalar == 0) {
        return this;
    }
    int64_t instant = getChronology()->add(period, getMillis(), scalar);
    return withMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *with the specified duration added.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param duration  the duration, in millis, to add to this one
 * @return a copy of this DateTime *with the duration added
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::plus(int64_t duration) {
    return withDurationAdded(duration, 1);
}

/**
 * Returns a copy of this DateTime *with the specified duration added.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param duration  the duration to add to this one, null means zero
 * @return a copy of this DateTime *with the duration added
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::plus(ReadableDuration *duration) {
    return withDurationAdded(duration, 1);
}

/**
 * Returns a copy of this DateTime *with the specified period added.
 * <p>
 * This method will add each element of the period one by one, from largest
 * to smallest, adjusting the DateTime *to be accurate between each.
 * <p>
 * Thus, adding a period of one month and one day to 2007-03-31 will
 * work as follows:
 * First add one month and adjust, resulting in 2007-04-30
 * Then add one day and adjust, resulting in 2007-05-01.
 * <p>
 * This method is typically used to add complex period instances.
 * Adding one field is best achieved using methods
 * like {@link #plusYears(int)}.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param period  the duration to add to this one, null means zero
 * @return a copy of this DateTime *with the period added
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::plus(ReadablePeriod *period) {
    return withPeriodAdded(period, 1);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *plus the specified number of years.
 * <p>
 * The calculation will do its best to only change the year field
 * retaining the same month of year.
 * However, in certain circumstances, it may be necessary to alter
 * smaller fields. For example, 2008-02-29 plus one year cannot result
 * in 2009-02-29, so the day of month is adjusted to 2009-02-28.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *added = dt.plusYears(6);
 * DateTime *added = dt.plus(Period.years(6));
 * DateTime *added = dt.withFieldAdded(DurationFieldType.years(), 6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param years  the amount of years to add, may be negative
 * @return the new DateTime *plus the increased years
 * @since 1.1
 */
DateTime *DateTime::plusYears(int years) {
    if (years == 0) {
        return this;
    }
    int64_t instant = getChronology()->years()->add(getMillis(), years);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *plus the specified number of months.
 * <p>
 * The calculation will do its best to only change the month field
 * retaining the same day of month.
 * However, in certain circumstances, it may be necessary to alter
 * smaller fields. For example, 2007-03-31 plus one month cannot result
 * in 2007-04-31, so the day of month is adjusted to 2007-04-30.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *added = dt.plusMonths(6);
 * DateTime *added = dt.plus(Period.months(6));
 * DateTime *added = dt.withFieldAdded(DurationFieldType.months(), 6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param months  the amount of months to add, may be negative
 * @return the new DateTime *plus the increased months
 * @since 1.1
 */
DateTime *DateTime::plusMonths(int months) {
    if (months == 0) {
        return this;
    }
    int64_t instant = getChronology()->months()->add(getMillis(), months);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *plus the specified number of weeks.
 * <p>
 * The calculation operates as if it were adding the equivalent in days.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *added = dt.plusWeeks(6);
 * DateTime *added = dt.plus(Period.weeks(6));
 * DateTime *added = dt.withFieldAdded(DurationFieldType.weeks(), 6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param weeks  the amount of weeks to add, may be negative
 * @return the new DateTime *plus the increased weeks
 * @since 1.1
 */
DateTime *DateTime::plusWeeks(int weeks) {
    if (weeks == 0) {
        return this;
    }
    int64_t instant = getChronology()->weeks()->add(getMillis(), weeks);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *plus the specified number of days.
 * <p>
 * The calculation will do its best to only change the day field
 * retaining the same time of day.
 * However, in certain circumstances, typically daylight savings cutover,
 * it may be necessary to alter the time fields.
 * <p>
 * In spring an hour is typically removed. If adding one day results in
 * the time being within the cutover then the time is adjusted to be
 * within summer time. For example, if the cutover is from 01:59 to 03:00
 * and the result of this method would have been 02:30, then the result
 * will be adjusted to 03:30.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *added = dt.plusDays(6);
 * DateTime *added = dt.plus(Period.days(6));
 * DateTime *added = dt.withFieldAdded(DurationFieldType.days(), 6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param days  the amount of days to add, may be negative
 * @return the new DateTime *plus the increased days
 * @since 1.1
 */
DateTime *DateTime::plusDays(int days) {
    if (days == 0) {
        return this;
    }
    int64_t instant = getChronology()->days()->add(getMillis(), days);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *plus the specified number of hours.
 * <p>
 * The calculation will add a duration equivalent to the number of hours
 * expressed in milliseconds.
 * <p>
 * For example, if a spring daylight savings cutover is from 01:59 to 03:00
 * then adding one hour to 01:30 will result in 03:30. This is a duration
 * of one hour later, even though the hour field value changed from 1 to 3.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *added = dt.plusHours(6);
 * DateTime *added = dt.plus(Period.hours(6));
 * DateTime *added = dt.withFieldAdded(DurationFieldType.hours(), 6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param hours  the amount of hours to add, may be negative
 * @return the new DateTime *plus the increased hours
 * @since 1.1
 */
DateTime *DateTime::plusHours(int hours) {
    if (hours == 0) {
        return this;
    }
    int64_t instant = getChronology()->hours()->add(getMillis(), hours);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *plus the specified number of minutes.
 * <p>
 * The calculation will add a duration equivalent to the number of minutes
 * expressed in milliseconds.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *added = dt.plusMinutes(6);
 * DateTime *added = dt.plus(Period.minutes(6));
 * DateTime *added = dt.withFieldAdded(DurationFieldType.minutes(), 6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param minutes  the amount of minutes to add, may be negative
 * @return the new DateTime *plus the increased minutes
 * @since 1.1
 */
DateTime *DateTime::plusMinutes(int minutes) {
    if (minutes == 0) {
        return this;
    }
    int64_t instant = getChronology()->minutes()->add(getMillis(), minutes);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *plus the specified number of seconds.
 * <p>
 * The calculation will add a duration equivalent to the number of seconds
 * expressed in milliseconds.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *added = dt.plusSeconds(6);
 * DateTime *added = dt.plus(Period.seconds(6));
 * DateTime *added = dt.withFieldAdded(DurationFieldType.seconds(), 6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param seconds  the amount of seconds to add, may be negative
 * @return the new DateTime *plus the increased seconds
 * @since 1.1
 */
DateTime *DateTime::plusSeconds(int seconds) {
    if (seconds == 0) {
        return this;
    }
    int64_t instant = getChronology()->seconds()->add(getMillis(), seconds);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *plus the specified number of millis.
 * <p>
 * The calculation will add a duration equivalent to the number of milliseconds.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *added = dt.plusMillis(6);
 * DateTime *added = dt.plus(Period.millis(6));
 * DateTime *added = dt.withFieldAdded(DurationFieldType.millis(), 6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param millis  the amount of millis to add, may be negative
 * @return the new DateTime *plus the increased millis
 * @since 1.1
 */
DateTime *DateTime::plusMillis(int millis) {
    if (millis == 0) {
        return this;
    }
    int64_t instant = getChronology()->millis()->add(getMillis(), millis);
    return withMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *with the specified duration taken away.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param duration  the duration, in millis, to reduce this instant by
 * @return a copy of this DateTime *with the duration taken away
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::minus(int64_t duration) {
    return withDurationAdded(duration, -1);
}

/**
 * Returns a copy of this DateTime *with the specified duration taken away.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param duration  the duration to reduce this instant by
 * @return a copy of this DateTime *with the duration taken away
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::minus(ReadableDuration *duration) {
    return withDurationAdded(duration, -1);
}

/**
 * Returns a copy of this DateTime *with the specified period taken away.
 * <p>
 * This method will subtract each element of the period one by one, from
 * largest to smallest, adjusting the DateTime *to be accurate between each.
 * <p>
 * Thus, subtracting a period of one month and one day from 2007-05-31 will
 * work as follows:
 * First subtract one month and adjust, resulting in 2007-04-30
 * Then subtract one day and adjust, resulting in 2007-04-29.
 * Note that the day has been adjusted by two.
 * <p>
 * This method is typically used to subtract complex period instances.
 * Subtracting one field is best achieved using methods
 * like {@link #minusYears(int)}.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param period  the period to reduce this instant by
 * @return a copy of this DateTime *with the period taken away
 * @throws ArithmeticException if the new DateTime *exceeds the capacity of a long
 */
DateTime *DateTime::minus(ReadablePeriod *period) {
    return withPeriodAdded(period, -1);
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *minus the specified number of years.
 * <p>
 * The calculation will do its best to only change the year field
 * retaining the same month of year.
 * However, in certain circumstances, it may be necessary to alter
 * smaller fields. For example, 2008-02-29 minus one year cannot result
 * in 2007-02-29, so the day of month is adjusted to 2007-02-28.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *subtracted = dt.minusYears(6);
 * DateTime *subtracted = dt.minus(Period.years(6));
 * DateTime *subtracted = dt.withFieldAdded(DurationFieldType.years(), -6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param years  the amount of years to subtract, may be negative
 * @return the new DateTime *minus the increased years
 * @since 1.1
 */
DateTime *DateTime::minusYears(int years) {
    if (years == 0) {
        return this;
    }
    int64_t instant = getChronology()->years()->subtract(getMillis(), years);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *minus the specified number of months.
 * <p>
 * The calculation will do its best to only change the month field
 * retaining the same day of month.
 * However, in certain circumstances, it may be necessary to alter
 * smaller fields. For example, 2007-05-31 minus one month cannot result
 * in 2007-04-31, so the day of month is adjusted to 2007-04-30.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *subtracted = dt.minusMonths(6);
 * DateTime *subtracted = dt.minus(Period.months(6));
 * DateTime *subtracted = dt.withFieldAdded(DurationFieldType.months(), -6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param months  the amount of months to subtract, may be negative
 * @return the new DateTime *minus the increased months
 * @since 1.1
 */
DateTime *DateTime::minusMonths(int months) {
    if (months == 0) {
        return this;
    }
    int64_t instant = getChronology()->months()->subtract(getMillis(), months);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *minus the specified number of weeks.
 * <p>
 * The calculation operates as if it were subtracting the equivalent in days.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *subtracted = dt.minusWeeks(6);
 * DateTime *subtracted = dt.minus(Period.weeks(6));
 * DateTime *subtracted = dt.withFieldAdded(DurationFieldType.weeks(), -6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param weeks  the amount of weeks to subtract, may be negative
 * @return the new DateTime *minus the increased weeks
 * @since 1.1
 */
DateTime *DateTime::minusWeeks(int weeks) {
    if (weeks == 0) {
        return this;
    }
    int64_t instant = getChronology()->weeks()->subtract(getMillis(), weeks);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *minus the specified number of days.
 * <p>
 * The calculation will do its best to only change the day field
 * retaining the same time of day.
 * However, in certain circumstances, typically daylight savings cutover,
 * it may be necessary to alter the time fields.
 * <p>
 * In spring an hour is typically removed. If subtracting one day results
 * in the time being within the cutover then the time is adjusted to be
 * within summer time. For example, if the cutover is from 01:59 to 03:00
 * and the result of this method would have been 02:30, then the result
 * will be adjusted to 03:30.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *subtracted = dt.minusDays(6);
 * DateTime *subtracted = dt.minus(Period.days(6));
 * DateTime *subtracted = dt.withFieldAdded(DurationFieldType.days(), -6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param days  the amount of days to subtract, may be negative
 * @return the new DateTime *minus the increased days
 * @since 1.1
 */
DateTime *DateTime::minusDays(int days) {
    if (days == 0) {
        return this;
    }
    int64_t instant = getChronology()->days()->subtract(getMillis(), days);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *minus the specified number of hours.
 * <p>
 * The calculation will subtract a duration equivalent to the number of
 * hours expressed in milliseconds.
 * <p>
 * For example, if a spring daylight savings cutover is from 01:59 to 03:00
 * then subtracting one hour from 03:30 will result in 01:30. This is a
 * duration of one hour earlier, even though the hour field value changed
 * from 3 to 1.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *subtracted = dt.minusHours(6);
 * DateTime *subtracted = dt.minus(Period.hours(6));
 * DateTime *subtracted = dt.withFieldAdded(DurationFieldType.hours(), -6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param hours  the amount of hours to subtract, may be negative
 * @return the new DateTime *minus the increased hours
 * @since 1.1
 */
DateTime *DateTime::minusHours(int hours) {
    if (hours == 0) {
        return this;
    }
    int64_t instant = getChronology()->hours()->subtract(getMillis(), hours);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *minus the specified number of minutes.
 * <p>
 * The calculation will subtract a duration equivalent to the number of
 * minutes expressed in milliseconds.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *subtracted = dt.minusMinutes(6);
 * DateTime *subtracted = dt.minus(Period.minutes(6));
 * DateTime *subtracted = dt.withFieldAdded(DurationFieldType.minutes(), -6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param minutes  the amount of minutes to subtract, may be negative
 * @return the new DateTime *minus the increased minutes
 * @since 1.1
 */
DateTime *DateTime::minusMinutes(int minutes) {
    if (minutes == 0) {
        return this;
    }
    int64_t instant = getChronology()->minutes()->subtract(getMillis(), minutes);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *minus the specified number of seconds.
 * <p>
 * The calculation will subtract a duration equivalent to the number of
 * seconds expressed in milliseconds.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *subtracted = dt.minusSeconds(6);
 * DateTime *subtracted = dt.minus(Period.seconds(6));
 * DateTime *subtracted = dt.withFieldAdded(DurationFieldType.seconds(), -6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param seconds  the amount of seconds to subtract, may be negative
 * @return the new DateTime *minus the increased seconds
 * @since 1.1
 */
DateTime *DateTime::minusSeconds(int seconds) {
    if (seconds == 0) {
        return this;
    }
    int64_t instant = getChronology()->seconds()->subtract(getMillis(), seconds);
    return withMillis(instant);
}

/**
 * Returns a copy of this DateTime *minus the specified number of millis.
 * <p>
 * The calculation will subtract a duration equivalent to the number of
 * milliseconds.
 * <p>
 * The following three lines are identical in effect:
 * <pre>
 * DateTime *subtracted = dt.minusMillis(6);
 * DateTime *subtracted = dt.minus(Period.millis(6));
 * DateTime *subtracted = dt.withFieldAdded(DurationFieldType.millis(), -6);
 * </pre>
 * <p>
 * This DateTime *instance is immutable and unaffected by this method call.
 *
 * @param millis  the amount of millis to subtract, may be negative
 * @return the new DateTime *minus the increased millis
 * @since 1.1
 */
DateTime *DateTime::minusMillis(int millis) {
    if (millis == 0) {
        return this;
    }
    int64_t instant = getChronology()->millis()->subtract(getMillis(), millis);
    return withMillis(instant);
}

//-----------------------------------------------------------------------
/**
 * Gets the property object for the specified type, which contains many useful methods.
 *
 * @param type  the field type to get the chronology for
 * @return the property object
 * @throws IllegalArgumentException if the field is null or unsupported
 */
DateTime::Property *DateTime::property(DateTimeFieldType *type) {
    if (type == NULL) {
        throw IllegalArgumentException("The DateTimeFieldType must not be null");
    }
    const DateTimeField *field = type->getField(getChronology());
    if (field->isSupported() == false) {
        string err("Field '");
        err.append(type->toString());
        err.append("' is not supported");
        throw IllegalArgumentException(err);
    }
    return new Property(this, field);
}

/**
 * Converts this object to a <code>LocalDateTime</code> with
 * the same DateTime *and chronology.
 *
 * @return a LocalDateTime *with the same DateTime *and chronology
 * @since 1.3
 */
LocalDateTime *DateTime::toLocalDateTime() {
    return new LocalDateTime(getMillis(), getChronology());
}

/**
 * Converts this object to a <code>LocalDate</code> with the
 * same date and chronology.
 *
 * @return a LocalDate with the same date and chronology
 * @since 1.3
 */
LocalDate *DateTime::toLocalDate() {
    return new LocalDate(getMillis(), getChronology());
}

/**
 * Converts this object to a <code>LocalTime</code> with the
 * same time and chronology.
 *
 * @return a LocalTime with the same time and chronology
 * @since 1.3
 */
LocalTime *DateTime::toLocalTime() {
    return new LocalTime(getMillis(), getChronology());
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *with the era field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * era changed.
 *
 * @param era  the era to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withEra(int era) {
    return withMillis(getChronology()->era()->set(getMillis(), era));
}

/**
 * Returns a copy of this DateTime *with the century of era field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * century of era changed.
 *
 * @param centuryOfEra  the centurey of era to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withCenturyOfEra(int centuryOfEra) {
    return withMillis(getChronology()->centuryOfEra()->set(getMillis(), centuryOfEra));
}

/**
 * Returns a copy of this DateTime *with the year of era field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * year of era changed.
 *
 * @param yearOfEra  the year of era to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withYearOfEra(int yearOfEra) {
    return withMillis(getChronology()->yearOfEra()->set(getMillis(), yearOfEra));
}

/**
 * Returns a copy of this DateTime *with the year of century field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * year of century changed.
 *
 * @param yearOfCentury  the year of century to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withYearOfCentury(int yearOfCentury) {
    return withMillis(getChronology()->yearOfCentury()->set(getMillis(), yearOfCentury));
}

/**
 * Returns a copy of this DateTime *with the year field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * year changed.
 *
 * @param year  the year to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withYear(int year) {
    return withMillis(getChronology()->year()->set(getMillis(), year));
}

/**
 * Returns a copy of this DateTime *with the weekyear field updated.
 * <p>
 * The weekyear is the year that matches with the weekOfWeekyear field.
 * In the standard ISO8601 week algorithm, the first week of the year
 * is that in which at least 4 days are in the year. As a result of this
 * definition, day 1 of the first week may be in the previous year.
 * The weekyear allows you to query the effective year for that day.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * weekyear changed.
 *
 * @param weekyear  the weekyear to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withWeekyear(int weekyear) {
    return withMillis(getChronology()->weekyear()->set(getMillis(), weekyear));
}

/**
 * Returns a copy of this DateTime *with the month of year field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * month of year changed.
 *
 * @param monthOfYear  the month of year to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withMonthOfYear(int monthOfYear) {
    return withMillis(getChronology()->monthOfYear()->set(getMillis(), monthOfYear));
}

/**
 * Returns a copy of this DateTime *with the week of weekyear field updated.
 * <p>
 * This field is associated with the "weekyear" via {@link #withWeekyear(int)}.
 * In the standard ISO8601 week algorithm, the first week of the year
 * is that in which at least 4 days are in the year. As a result of this
 * definition, day 1 of the first week may be in the previous year.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * week of weekyear changed.
 *
 * @param weekOfWeekyear  the week of weekyear to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withWeekOfWeekyear(int weekOfWeekyear) {
    return withMillis(getChronology()->weekOfWeekyear()->set(getMillis(), weekOfWeekyear));
}

/**
 * Returns a copy of this DateTime *with the day of year field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * day of year changed.
 *
 * @param dayOfYear  the day of year to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withDayOfYear(int dayOfYear) {
    return withMillis(getChronology()->dayOfYear()->set(getMillis(), dayOfYear));
}

/**
 * Returns a copy of this DateTime *with the day of month field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * day of month changed.
 *
 * @param dayOfMonth  the day of month to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withDayOfMonth(int dayOfMonth) {
    return withMillis(getChronology()->dayOfMonth()->set(getMillis(), dayOfMonth));
}

/**
 * Returns a copy of this DateTime *with the day of week field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * day of week changed.
 *
 * @param dayOfWeek  the day of week to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withDayOfWeek(int dayOfWeek) {
    return withMillis(getChronology()->dayOfWeek()->set(getMillis(), dayOfWeek));
}

//-----------------------------------------------------------------------
/**
 * Returns a copy of this DateTime *with the hour of day field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * hour of day changed.
 *
 * @param hour  the hour of day to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withHourOfDay(int hour) {
    return withMillis(getChronology()->hourOfDay()->set(getMillis(), hour));
}

/**
 * Returns a copy of this DateTime *with the minute of hour updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * minute of hour changed.
 *
 * @param minute  the minute of hour to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withMinuteOfHour(int minute) {
    return withMillis(getChronology()->minuteOfHour()->set(getMillis(), minute));
}

/**
 * Returns a copy of this DateTime *with the second of minute field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * second of minute changed.
 *
 * @param second  the second of minute to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withSecondOfMinute(int second) {
    return withMillis(getChronology()->secondOfMinute()->set(getMillis(), second));
}

/**
 * Returns a copy of this DateTime *with the millis of second field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * millis of second changed.
 *
 * @param millis  the millis of second to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withMillisOfSecond(int millis) {
    return withMillis(getChronology()->millisOfSecond()->set(getMillis(), millis));
}

/**
 * Returns a copy of this DateTime *with the millis of day field updated.
 * <p>
 * DateTime *is immutable, so there are no set methods.
 * Instead, this method returns a new instance with the value of
 * millis of day changed.
 *
 * @param millis  the millis of day to set
 * @return a copy of this object with the field set
 * @throws IllegalArgumentException if the value is invalid
 * @since 1.3
 */
DateTime *DateTime::withMillisOfDay(int millis) {
    return withMillis(getChronology()->millisOfDay()->set(getMillis(), millis));
}

// Date properties
//-----------------------------------------------------------------------
/**
 * Get the era property which provides access to advanced functionality.
 *
 * @return the era property
 */
DateTime::Property *DateTime::era() {
    return new Property(this, getChronology()->era());
}

/**
 * Get the century of era property which provides access to advanced functionality.
 *
 * @return the year of era property
 */
DateTime::Property *DateTime::centuryOfEra() {
    return new Property(this, getChronology()->centuryOfEra());
}

/**
 * Get the year of century property which provides access to advanced functionality.
 *
 * @return the year of era property
 */
DateTime::Property *DateTime::yearOfCentury() {
    return new Property(this, getChronology()->yearOfCentury());
}

/**
 * Get the year of era property which provides access to advanced functionality.
 *
 * @return the year of era property
 */
DateTime::Property *DateTime::yearOfEra() {
    return new Property(this, getChronology()->yearOfEra());
}

/**
 * Get the year property which provides access to advanced functionality.
 *
 * @return the year property
 */
DateTime::Property *DateTime::year() {
    return new Property(this, getChronology()->year());
}

/**
 * Get the year of a week based year property which provides access to advanced functionality.
 *
 * @return the year of a week based year property
 */
DateTime::Property *DateTime::weekyear() {
    return new Property(this, getChronology()->weekyear());
}

/**
 * Get the month of year property which provides access to advanced functionality.
 *
 * @return the month of year property
 */
DateTime::Property *DateTime::monthOfYear() {
    return new Property(this, getChronology()->monthOfYear());
}

/**
 * Get the week of a week based year property which provides access to advanced functionality.
 *
 * @return the week of a week based year property
 */
DateTime::Property *DateTime::weekOfWeekyear() {
    return new Property(this, getChronology()->weekOfWeekyear());
}

/**
 * Get the day of year property which provides access to advanced functionality.
 *
 * @return the day of year property
 */
DateTime::Property *DateTime::dayOfYear() {
    return new Property(this, getChronology()->dayOfYear());
}

/**
 * Get the day of month property which provides access to advanced functionality.
 *
 * @return the day of month property
 */
DateTime::Property *DateTime::dayOfMonth() {
    return new Property(this, getChronology()->dayOfMonth());
}

/**
 * Get the day of week property which provides access to advanced functionality.
 *
 * @return the day of week property
 */
DateTime::Property *DateTime::dayOfWeek() {
    return new Property(this, getChronology()->dayOfWeek());
}

// Time properties
//-----------------------------------------------------------------------
/**
 * Get the hour of day field property which provides access to advanced functionality.
 *
 * @return the hour of day property
 */
DateTime::Property *DateTime::hourOfDay() {
    return new Property(this, getChronology()->hourOfDay());
}

/**
 * Get the minute of day property which provides access to advanced functionality.
 *
 * @return the minute of day property
 */
DateTime::Property *DateTime::minuteOfDay() {
    return new Property(this, getChronology()->minuteOfDay());
}

/**
 * Get the minute of hour field property which provides access to advanced functionality.
 *
 * @return the minute of hour property
 */
DateTime::Property *DateTime::minuteOfHour() {
    return new Property(this, getChronology()->minuteOfHour());
}

/**
 * Get the second of day property which provides access to advanced functionality.
 *
 * @return the second of day property
 */
DateTime::Property *DateTime::secondOfDay() {
    return new Property(this, getChronology()->secondOfDay());
}

/**
 * Get the second of minute field property which provides access to advanced functionality.
 *
 * @return the second of minute property
 */
DateTime::Property *DateTime::secondOfMinute() {
    return new Property(this, getChronology()->secondOfMinute());
}

/**
 * Get the millis of day property which provides access to advanced functionality.
 *
 * @return the millis of day property
 */
DateTime::Property *DateTime::millisOfDay() {
    return new Property(this, getChronology()->millisOfDay());
}

/**
 * Get the millis of second property which provides access to advanced functionality.
 *
 * @return the millis of second property
 */
DateTime::Property *DateTime::millisOfSecond() {
    return new Property(this, getChronology()->millisOfSecond());
}

CODATIME_END