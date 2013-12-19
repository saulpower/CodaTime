//
//  Instant.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "Instant.h"

#include "DateTimeUtils.h"
#include "chrono/ISOChronology.h"

CODATIME_BEGIN

//-----------------------------------------------------------------------
/**
 * Obtains an {@code Instant} set to the current system millisecond time.
 *
 * @return the current instant, not null
 * @since 2.0
 */
Instant *Instant::now() {
    return new Instant();
}

//-----------------------------------------------------------------------
/**
 * Parses a {@code Instant} from the specified string.
 * <p>
 * This uses {@link ISODateTimeFormat#dateTimeParser()}.
 *
 * @param str  the string to parse, not null
 * @since 2.0
 */
Instant *Instant::parse(string str) {
    return parse(str, ISODateTimeFormat::dateTimeParser());
}

/**
 * Parses a {@code Instant} from the specified string using a formatter.
 *
 * @param str  the string to parse, not null
 * @param formatter  the formatter to use, not null
 * @since 2.0
 */
Instant *Instant::parse(string str, DateTimeFormatter *formatter) {
    return formatter->parseDateTime(str)->toInstant();
}

//-----------------------------------------------------------------------
/**
 * Constructs an instance set to the current system millisecond time.
 *
 * @see #now()
 */
Instant::Instant() {
    iMillis = DateTimeUtils::currentTimeMillis();
}

/**
 * Constructs an instance set to the milliseconds from 1970-01-01T00:00:00Z.
 *
 * @param instant  the milliseconds from 1970-01-01T00:00:00Z
 */
Instant::Instant(int64_t instant) {
    iMillis = instant;
}

/**
 * Constructs an instance from an Object that represents a datetime.
 * <p>
 * The recognised object types are defined in {@link ConverterManager} and
 * include String, Calendar and Date.
 *
 * @param instant  the datetime object, null means now
 * @throws IllegalArgumentException if the instant is invalid
 */
Instant::Instant(void *instant) {
    InstantConverter *converter = ConverterManager::getInstance()->getInstantConverter(instant);
    iMillis = converter->getInstantMillis(instant, ISOChronology::getInstanceUTC());
}

//-----------------------------------------------------------------------
/**
 * Get this object as an Instant by returning <code>this</code>.
 *
 * @return <code>this</code>
 */
Instant *Instant::toInstant() {
    return this;
}

//-----------------------------------------------------------------------
/**
 * Gets a copy of this instant with different millis.
 * <p>
 * The returned object will be either be a new Instant or <code>this</code>.
 *
 * @param newMillis  the new millis, from 1970-01-01T00:00:00Z
 * @return a copy of this instant with different millis
 */
Instant *Instant::withMillis(int64_t newMillis) {
    return (newMillis == iMillis ? this : new Instant(newMillis));
}

/**
 * Gets a copy of this instant with the specified duration added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 *
 * @param durationToAdd  the duration to add to this one
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this instant with the duration added
 * @throws ArithmeticException if the new instant exceeds the capacity of a uint64_t
 */
Instant *Instant::withDurationAdded(int64_t durationToAdd, int scalar) {
    if (durationToAdd == 0 || scalar == 0) {
        return this;
    }
    int64_t instant = getChronology()->add(getMillis(), durationToAdd, scalar);
    return withMillis(instant);
}

/**
 * Gets a copy of this instant with the specified duration added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 *
 * @param durationToAdd  the duration to add to this one, null means zero
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this instant with the duration added
 * @throws ArithmeticException if the new instant exceeds the capacity of a uint64_t
 */
Instant *Instant::withDurationAdded(ReadableDuration *durationToAdd, int scalar) {
    if (durationToAdd == NULL || scalar == 0) {
        return this;
    }
    return withDurationAdded(durationToAdd->getMillis(), scalar);
}

//-----------------------------------------------------------------------
/**
 * Gets a copy of this instant with the specified duration added.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 *
 * @param duration  the duration to add to this one
 * @return a copy of this instant with the duration added
 * @throws ArithmeticException if the new instant exceeds the capacity of a uint64_t
 */
Instant *Instant::plus(int64_t duration) {
    return withDurationAdded(duration, 1);
}

/**
 * Gets a copy of this instant with the specified duration added.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 *
 * @param duration  the duration to add to this one, null means zero
 * @return a copy of this instant with the duration added
 * @throws ArithmeticException if the new instant exceeds the capacity of a uint64_t
 */
Instant *Instant::plus(ReadableDuration *duration) {
    return withDurationAdded(duration, 1);
}

//-----------------------------------------------------------------------
/**
 * Gets a copy of this instant with the specified duration taken away.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 *
 * @param duration  the duration to reduce this instant by
 * @return a copy of this instant with the duration taken away
 * @throws ArithmeticException if the new instant exceeds the capacity of a uint64_t
 */
Instant *Instant::minus(int64_t duration) {
    return withDurationAdded(duration, -1);
}

/**
 * Gets a copy of this instant with the specified duration taken away.
 * <p>
 * If the amount is zero or null, then <code>this</code> is returned.
 *
 * @param duration  the duration to reduce this instant by
 * @return a copy of this instant with the duration taken away
 * @throws ArithmeticException if the new instant exceeds the capacity of a uint64_t
 */
Instant *Instant::minus(ReadableDuration *duration) {
    return withDurationAdded(duration, -1);
}

//-----------------------------------------------------------------------
/**
 * Gets the milliseconds of the instant.
 *
 * @return the number of milliseconds since 1970-01-01T00:00:00Z
 */
int64_t Instant::getMillis() {
    return iMillis;
}

/**
 * Gets the chronology of the instant, which is ISO in the UTC zone.
 * <p>
 * This method returns {@link ISOChronology#getInstanceUTC()} which
 * corresponds to the definition of the Java epoch 1970-01-01T00:00:00Z.
 *
 * @return ISO in the UTC zone
 */
Chronology *Instant::getChronology() {
    return ISOChronology::getInstanceUTC();
}

//-----------------------------------------------------------------------
/**
 * Get this object as a DateTime using ISOChronology in the default zone.
 * <p>
 * This method returns a DateTime object in the default zone.
 * This differs from the similarly named method on DateTime, DateMidnight
 * or MutableDateTime which retains the time zone. The difference is
 * because Instant really represents a time <i>without</i> a zone,
 * thus calling this method we really have no zone to 'retain' and
 * hence expect to switch to the default zone.
 * <p>
 * This method definition preserves compatibility with earlier versions
 * of Joda-Time.
 *
 * @return a DateTime using the same millis
 */
DateTime *Instant::toDateTime() {
    return new DateTime(getMillis(), ISOChronology::getInstance());
}

/**
 * Get this object as a MutableDateTime using ISOChronology in the default zone.
 * <p>
 * This method returns a MutableDateTime object in the default zone.
 * This differs from the similarly named method on DateTime, DateMidnight
 * or MutableDateTime which retains the time zone. The difference is
 * because Instant really represents a time <i>without</i> a zone,
 * thus calling this method we really have no zone to 'retain' and
 * hence expect to switch to the default zone.
 * <p>
 * This method definition preserves compatibility with earlier versions
 * of Joda-Time.
 *
 * @return a MutableDateTime using the same millis
 */
MutableDateTime *Instant::toMutableDateTime() {
    return new MutableDateTime(getMillis(), ISOChronology::getInstance());
}

CODATIME_END