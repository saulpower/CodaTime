//
//  Interval.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/14/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "Interval.h"

#include "DateTimeUtils.h"
#include "chrono/ISOChronology.h"

CODATIME_BEGIN

Interval *Interval::parse(string str) {
    return new Interval(str);
}

Interval::Interval(int64_t startInstant, int64_t endInstant, DateTimeZone *zone) : BaseInterval(startInstant, endInstant, ISOChronology::getInstance(zone)) {
}

Interval *Interval::toInterval() {
    return this;
}

Interval *Interval::overlap(ReadableInterval *interval) {
    interval = DateTimeUtils::getReadableInterval(interval);
    if (overlaps(interval) == false) {
        return NULL;
    }
    int64_t start = max(getStartMillis(), interval->getStartMillis());
    int64_t end = min(getEndMillis(), interval->getEndMillis());
    return new Interval(start, end, getChronology());
}

Interval *Interval::gap(ReadableInterval *interval) {
    interval = DateTimeUtils::getReadableInterval(interval);
    int64_t otherStart = interval->getStartMillis();
    int64_t otherEnd = interval->getEndMillis();
    int64_t thisStart = getStartMillis();
    int64_t thisEnd = getEndMillis();
    if (thisStart > otherEnd) {
        return new Interval(otherEnd, thisStart, getChronology());
    } else if (otherStart > thisEnd) {
        return new Interval(thisEnd, otherStart, getChronology());
    } else {
        return NULL;
    }
}

bool Interval::abuts(ReadableInterval *interval) {
    if (interval == NULL) {
        int64_t now = DateTimeUtils::currentTimeMillis();
        return (getStartMillis() == now || getEndMillis() == now);
    } else {
        return (interval->getEndMillis() == getStartMillis() ||
                getEndMillis() == interval->getStartMillis());
    }
}

Interval *Interval::withChronology(Chronology *chronology) {
    if (getChronology() == chronology) {
        return this;
    }
    return new Interval(getStartMillis(), getEndMillis(), chronology);
}

Interval *Interval::withStartMillis(int64_t startInstant) {
    if (startInstant == getStartMillis()) {
        return this;
    }
    return new Interval(startInstant, getEndMillis(), getChronology());
}

Interval *Interval::withStart(ReadableInstant *start) {
    int64_t startMillis = DateTimeUtils::getInstantMillis(start);
    return withStartMillis(startMillis);
}

Interval *Interval::withEndMillis(int64_t endInstant) {
    if (endInstant == getEndMillis()) {
        return this;
    }
    return new Interval(getStartMillis(), endInstant, getChronology());
}

Interval *Interval::withEnd(ReadableInstant *end) {
    int64_t endMillis = DateTimeUtils::getInstantMillis(end);
    return withEndMillis(endMillis);
}

Interval *Interval::withDurationAfterStart(ReadableDuration *duration) {
    int64_t durationMillis = DateTimeUtils::getDurationMillis(duration);
    if (durationMillis == toDurationMillis()) {
        return this;
    }
    Chronology *chrono = getChronology();
    int64_t startMillis = getStartMillis();
    int64_t endMillis = chrono->add(startMillis, durationMillis, 1);
    return new Interval(startMillis, endMillis, chrono);
}

Interval *Interval::withDurationBeforeEnd(ReadableDuration *duration) {
    int64_t durationMillis = DateTimeUtils::getDurationMillis(duration);
    if (durationMillis == toDurationMillis()) {
        return this;
    }
    Chronology *chrono = getChronology();
    int64_t endMillis = getEndMillis();
    int64_t startMillis = chrono->add(endMillis, durationMillis, -1);
    return new Interval(startMillis, endMillis, chrono);
}

Interval *Interval::withPeriodAfterStart(ReadablePeriod *period) {
    if (period == NULL) {
        return withDurationAfterStart(NULL);
    }
    Chronology *chrono = getChronology();
    int64_t startMillis = getStartMillis();
    int64_t endMillis = chrono->add(period, startMillis, 1);
    return new Interval(startMillis, endMillis, chrono);
}

Interval *Interval::withPeriodBeforeEnd(ReadablePeriod *period) {
    if (period == NULL) {
        return withDurationBeforeEnd(NULL);
    }
    Chronology *chrono = getChronology();
    int64_t endMillis = getEndMillis();
    int64_t startMillis = chrono->add(period, endMillis, -1);
    return new Interval(startMillis, endMillis, chrono);
}

CODATIME_END