//
//  AbstractInterval.c
//  CodaTime
//
//  Created by Saul Howard on 12/19/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "AbstractInterval.h"

#include "DateTime.h"
#include "Duration.h"
#include "Period.h"
//#include "MutableInterval.h"
#include "Exceptions.h"
#include "DateTimeUtils.h"
#include "field/FieldUtils.h"
#include "format/DateTimeFormatter.h"
#include "Chronology.h"
#include "Interval.h"

CODATIME_BEGIN

void AbstractInterval::checkInterval(int64_t start, int64_t end) {
    if (end < start) {
        throw IllegalArgumentException("The end instant must be greater or equal to the start");
    }
}

DateTime *AbstractInterval::getStart() {
    return new DateTime(getStartMillis(), getChronology());
}

DateTime *AbstractInterval::getEnd() {
    return new DateTime(getEndMillis(), getChronology());
}

bool AbstractInterval::contains(int64_t millisInstant) {
    int64_t thisStart = getStartMillis();
    int64_t thisEnd = getEndMillis();
    return (millisInstant >= thisStart && millisInstant < thisEnd);
}

bool AbstractInterval::containsNow() {
    return contains(DateTimeUtils::currentTimeMillis());
}

bool AbstractInterval::contains(ReadableInstant *instant) {
    if (instant == NULL) {
        return containsNow();
    }
    return contains(instant->getMillis());
}

bool AbstractInterval::contains(ReadableInterval *interval) {
    if (interval == NULL) {
        return containsNow();
    }
    int64_t otherStart = interval->getStartMillis();
    int64_t otherEnd = interval->getEndMillis();
    int64_t thisStart = getStartMillis();
    int64_t thisEnd = getEndMillis();
    return (thisStart <= otherStart && otherStart < thisEnd && otherEnd <= thisEnd);
}

bool AbstractInterval::overlaps(ReadableInterval *interval) {
    int64_t thisStart = getStartMillis();
    int64_t thisEnd = getEndMillis();
    if (interval == NULL) {
        int64_t now = DateTimeUtils::currentTimeMillis();
        return (thisStart < now && now < thisEnd);
    }  else {
        int64_t otherStart = interval->getStartMillis();
        int64_t otherEnd = interval->getEndMillis();
        return (thisStart < otherEnd && otherStart < thisEnd);
    }
}

//-----------------------------------------------------------------------
bool AbstractInterval::isEqual(ReadableInterval *other) {
    return getStartMillis() == other->getStartMillis() &&
    getEndMillis() == other->getEndMillis();
}

bool AbstractInterval::isBefore(int64_t millisInstant) {
    return (getEndMillis() <= millisInstant);
}

bool AbstractInterval::isBeforeNow() {
    return isBefore(DateTimeUtils::currentTimeMillis());
}

bool AbstractInterval::isBefore(ReadableInstant *instant) {
    if (instant == NULL) {
        return isBeforeNow();
    }
    return isBefore(instant->getMillis());
}

bool AbstractInterval::isBefore(ReadableInterval *interval) {
    if (interval == NULL) {
        return isBeforeNow();
    }
    return isBefore(interval->getStartMillis());
}

//-----------------------------------------------------------------------
bool AbstractInterval::isAfter(int64_t millisInstant) {
    return (getStartMillis() > millisInstant);
}

bool AbstractInterval::isAfterNow() {
    return isAfter(DateTimeUtils::currentTimeMillis());
}

bool AbstractInterval::isAfter(ReadableInstant *instant) {
    if (instant == NULL) {
        return isAfterNow();
    }
    return isAfter(instant->getMillis());
}

bool AbstractInterval::isAfter(ReadableInterval *interval) {
    int64_t endMillis;
    if (interval == NULL) {
        endMillis = DateTimeUtils::currentTimeMillis();
    } else {
        endMillis = interval->getEndMillis();
    }
    return (getStartMillis() >= endMillis);
}

//-----------------------------------------------------------------------
Interval *AbstractInterval::toInterval() {
    return new Interval(getStartMillis(), getEndMillis(), getChronology());
}

MutableInterval *AbstractInterval::toMutableInterval() {
    return new MutableInterval(getStartMillis(), getEndMillis(), getChronology());
}

int64_t AbstractInterval::toDurationMillis() {
    return FieldUtils::safeAdd(getEndMillis(), -getStartMillis());
}

Duration *AbstractInterval::toDuration() {
    int64_t durMillis = toDurationMillis();
    if (durMillis == 0) {
        return Duration::ZERO;
    } else {
        return new Duration(durMillis);
    }
}

//-----------------------------------------------------------------------
Period *AbstractInterval::toPeriod() {
    return new Period(getStartMillis(), getEndMillis(), getChronology());
}

Period *AbstractInterval::toPeriod(const PeriodType *type) {
    return new Period(getStartMillis(), getEndMillis(), type, getChronology());
}

//-----------------------------------------------------------------------
bool AbstractInterval::equals(const Object *readableInterval) const {
    if (this == readableInterval) {
        return true;
    }
    const ReadableInterval *other = dynamic_cast<const ReadableInterval*>(readableInterval);
    if (other == 0) {
        return false;
    }
    return getStartMillis() == other->getStartMillis() &&
    getEndMillis() == other->getEndMillis() &&
    FieldUtils::equals(getChronology(), other->getChronology());
}

int AbstractInterval::hashCode() {
    int64_t start = getStartMillis();
    int64_t end = getEndMillis();
    int result = 97;
    result = 31 * result + ((int) (start ^ (start >> 32)));
    result = 31 * result + ((int) (end ^ (end >> 32)));
    result = 31 * result + getChronology()->hashCode();
    return result;
}

string AbstractInterval::toString() {
    DateTimeFormatter *printer = ISODateTimeFormat::dateTime();
    printer = printer->withChronology(getChronology());
    string buf;
    printer->printTo(buf, getStartMillis());
    buf.insert(0, "/");
    printer->printTo(buf, getEndMillis());
    return string(buf);
}

CODATIME_END