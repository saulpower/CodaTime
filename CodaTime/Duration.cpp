//
//  Duration.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "Duration.h"

#include "DateTimeConstants.h"
#include "DateTimeUtils.h"
#include "Exceptions.h"
#include "field/FieldUtils.h"

CODATIME_BEGIN

Duration *Duration::ZERO = new Duration(0LL);

Duration *Duration::parse(string str) {
    return new Duration(str);
}

Duration *Duration::standardDays(int64_t days) {
    if (days == 0) {
        return ZERO;
    }
    return new Duration(FieldUtils::safeMultiply(days, DateTimeConstants::MILLIS_PER_DAY));
}

Duration *Duration::standardHours(int64_t hours) {
    if (hours == 0) {
        return ZERO;
    }
    return new Duration(FieldUtils::safeMultiply(hours, DateTimeConstants::MILLIS_PER_HOUR));
}

Duration *Duration::standardMinutes(int64_t minutes) {
    if (minutes == 0) {
        return ZERO;
    }
    return new Duration(FieldUtils::safeMultiply(minutes, DateTimeConstants::MILLIS_PER_MINUTE));
}

Duration *Duration::standardSeconds(int64_t seconds) {
    if (seconds == 0) {
        return ZERO;
    }
    return new Duration(FieldUtils::safeMultiply(seconds, DateTimeConstants::MILLIS_PER_SECOND));
}

Duration *Duration::millis(int64_t millis) {
    if (millis == 0) {
        return ZERO;
    }
    return new Duration(millis);
}

Duration::Duration(int64_t duration) : BaseDuration(duration) {
}

Duration::Duration(int64_t startInstant, int64_t endInstant) : BaseDuration(startInstant, endInstant) {
}

Duration::Duration(ReadableInstant *start, ReadableInstant *end) : BaseDuration(start, end) {
}

Duration::Duration(string duration) : BaseDuration(duration) {
}

int64_t Duration::getStandardDays() {
    return getMillis() / DateTimeConstants::MILLIS_PER_DAY;
}

int64_t Duration::getStandardHours() {
    return getMillis() / DateTimeConstants::MILLIS_PER_HOUR;
}

int64_t Duration::getStandardMinutes() {
    return getMillis() / DateTimeConstants::MILLIS_PER_MINUTE;
}

int64_t Duration::getStandardSeconds() {
    return getMillis() / DateTimeConstants::MILLIS_PER_SECOND;
}

Duration *Duration::toDuration() {
    return this;
}

Days *Duration::toStandardDays() {
    int64_t days = getStandardDays();
    return Days::days(FieldUtils::safeToInt(days));
}

Hours *Duration::toStandardHours() {
    int64_t hours = getStandardHours();
    return Hours::hours(FieldUtils::safeToInt(hours));
}

Minutes *Duration::toStandardMinutes() {
    int64_t minutes = getStandardMinutes();
    return Minutes::minutes(FieldUtils::safeToInt(minutes));
}

Seconds *Duration::toStandardSeconds() {
    int64_t seconds = getStandardSeconds();
    return Seconds::seconds(FieldUtils::safeToInt(seconds));
}

Duration *Duration::withMillis(int64_t duration) {
    if (duration == getMillis()) {
        return this;
    }
    return new Duration(duration);
}

Duration *Duration::withDurationAdded(int64_t durationToAdd, int scalar) {
    if (durationToAdd == 0 || scalar == 0) {
        return this;
    }
    int64_t add = FieldUtils::safeMultiply(durationToAdd, scalar);
    int64_t duration = FieldUtils::safeAdd(getMillis(), add);
    return new Duration(duration);
}

Duration *Duration::withDurationAdded(ReadableDuration *durationToAdd, int scalar) {
    if (durationToAdd == NULL || scalar == 0) {
        return this;
    }
    return withDurationAdded(durationToAdd->getMillis(), scalar);
}

Duration *Duration::plus(int64_t amount) {
    return withDurationAdded(amount, 1);
}

Duration *Duration::plus(ReadableDuration *amount) {
    if (amount == NULL) {
        return this;
    }
    return withDurationAdded(amount->getMillis(), 1);
}

Duration *Duration::minus(int64_t amount) {
    return withDurationAdded(amount, -1);
}

Duration *Duration::minus(ReadableDuration *amount) {
    if (amount == NULL) {
        return this;
    }
    return withDurationAdded(amount->getMillis(), -1);
}

Duration *Duration::multipliedBy(int64_t multiplicand) {
    if (multiplicand == 1) {
        return this;
    }
    return new Duration(FieldUtils::safeMultiply(getMillis(), multiplicand));
}

Duration *Duration::dividedBy(int64_t divisor) {
    if (divisor == 1) {
        return this;
    }
    return new Duration(FieldUtils::safeDivide(getMillis(), divisor));
}

Duration *Duration::negated() {
    if (getMillis() == LONG_MIN) {
        throw ArithmeticException("Negation of this duration would overflow");
    }
    return new Duration(-getMillis());
}

CODATIME_END