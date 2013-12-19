//
//  BasicYearDateTimeField.c
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "BasicYearDateTimeField.h"

#include "chrono/BasicChronology.h"

CODATIME_BEGIN

BasicYearDateTimeField::BasicYearDateTimeField(BasicChronology *chronology) : ImpreciseDateTimeField(DateTimeFieldType::year(), chronology->getAverageMillisPerYear()) {
    iChronology = chronology;
}

int BasicYearDateTimeField::get(int64_t instant) const {
    return iChronology->getYear(instant);
}

int64_t BasicYearDateTimeField::addWrapField(int64_t instant, int years) const {
    if (years == 0) {
        return instant;
    }
    // Return newly calculated millis value
    int thisYear = iChronology->getYear(instant);
    int wrappedYear = FieldUtils::getWrappedValue
    (thisYear, years, iChronology->getMinYear(), iChronology->getMaxYear());
    return set(instant, wrappedYear);
}

int64_t BasicYearDateTimeField::set(int64_t instant, int year) const {
    FieldUtils::verifyValueBounds
    (this, year, iChronology->getMinYear(), iChronology->getMaxYear());
    return iChronology->setYear(instant, year);
}

int64_t BasicYearDateTimeField::getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const {
    if (minuendInstant < subtrahendInstant) {
        return -iChronology->getYearDifference(subtrahendInstant, minuendInstant);
    }
    return iChronology->getYearDifference(minuendInstant, subtrahendInstant);
}

const DurationField *BasicYearDateTimeField::getRangeDurationField() const {
    return NULL;
}

bool BasicYearDateTimeField::isLeap(int64_t instant) const {
    return iChronology->isLeapYear(get(instant));
}

int BasicYearDateTimeField::getLeapAmount(int64_t instant) const {
    if (iChronology->isLeapYear(get(instant))) {
        return 1;
    } else {
        return 0;
    }
}

const DurationField *BasicYearDateTimeField::getLeapDurationField() const {
    return iChronology->days();
}

int BasicYearDateTimeField::getMinimumValue() const {
    return iChronology->getMinYear();
}

int BasicYearDateTimeField::getMaximumValue() const {
    return iChronology->getMaxYear();
}

int64_t BasicYearDateTimeField::roundFloor(int64_t instant) const {
    return iChronology->getYearMillis(get(instant));
}

int64_t BasicYearDateTimeField::roundCeiling(int64_t instant) const {
    int year = get(instant);
    int64_t yearStartMillis = iChronology->getYearMillis(year);
    if (instant != yearStartMillis) {
        // Bump up to start of next year.
        instant = iChronology->getYearMillis(year + 1);
    }
    return instant;
}

int64_t BasicYearDateTimeField::remainder(int64_t instant) const {
    return instant - roundFloor(instant);
}

const Object *BasicYearDateTimeField::readResolve() {
    return iChronology->year();
}

CODATIME_END
