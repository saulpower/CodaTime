//
//  MutablePeriod.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "MutablePeriod.h"

#include "DateTimeUtils.h"
#include "DurationFieldType.h"
#include "field/FieldUtils.h"
#include "format/ISOPeriodFormat.h"
#include "format/PeriodFormatter.h"
#include "PeriodType.h"
#include "Period.h"
#include "ReadableInterval.h"

#include <vector>

CODATIME_BEGIN

MutablePeriod *MutablePeriod::parse(string str) {
    return parse(str, ISOPeriodFormat::standard());
}

MutablePeriod *MutablePeriod::parse(string str, PeriodFormatter *formatter) {
    return formatter->parsePeriod(str)->toMutablePeriod();
}

//-----------------------------------------------------------------------
MutablePeriod::MutablePeriod() : BasePeriod(0LL, (PeriodType*) NULL, (Chronology*) NULL) {}

MutablePeriod::MutablePeriod(const PeriodType *type) : BasePeriod(0LL, type, NULL) {}

MutablePeriod::MutablePeriod(int hours, int minutes, int seconds, int millis) : BasePeriod(0, 0, 0, 0, hours, minutes, seconds, millis, PeriodType::standard()) {}

MutablePeriod::MutablePeriod(int years, int months, int weeks, int days,
                     int hours, int minutes, int seconds, int millis) : BasePeriod(years, months, weeks, days, hours, minutes, seconds, millis, PeriodType::standard()) {}

MutablePeriod::MutablePeriod(int years, int months, int weeks, int days,
                     int hours, int minutes, int seconds, int millis, const PeriodType *type) : BasePeriod(years, months, weeks, days, hours, minutes, seconds, millis, type) {}

MutablePeriod::MutablePeriod(int64_t duration) : BasePeriod(duration) {}

MutablePeriod::MutablePeriod(int64_t duration, const PeriodType *type) : BasePeriod(duration, type, NULL) {}

MutablePeriod::MutablePeriod(int64_t duration, Chronology *chronology) : BasePeriod(duration, NULL, chronology) {}

MutablePeriod::MutablePeriod(int64_t duration, const PeriodType *type, Chronology *chronology) : BasePeriod(duration, type, chronology) {}

MutablePeriod::MutablePeriod(int64_t startInstant, int64_t endInstant) : BasePeriod(startInstant, endInstant, NULL, NULL) {}

MutablePeriod::MutablePeriod(int64_t startInstant, int64_t endInstant, const PeriodType *type) : BasePeriod(startInstant, endInstant, type, NULL) {}

MutablePeriod::MutablePeriod(int64_t startInstant, int64_t endInstant, Chronology *chrono) : BasePeriod(startInstant, endInstant, NULL, chrono) {}

MutablePeriod::MutablePeriod(int64_t startInstant, int64_t endInstant, const PeriodType *type, Chronology *chrono) : BasePeriod(startInstant, endInstant, type, chrono) {}

MutablePeriod::MutablePeriod(ReadableInstant *startInstant, ReadableInstant *endInstant) : BasePeriod(startInstant, endInstant, NULL) {}

MutablePeriod::MutablePeriod(ReadableInstant *startInstant, ReadableInstant *endInstant, const PeriodType *type) : BasePeriod(startInstant, endInstant, type) {}

MutablePeriod::MutablePeriod(ReadableInstant *startInstant, ReadableDuration *duration) : BasePeriod(startInstant, duration, NULL) {}

MutablePeriod::MutablePeriod(ReadableInstant *startInstant, ReadableDuration *duration, const PeriodType *type) : BasePeriod(startInstant, duration, type) {}

MutablePeriod::MutablePeriod(ReadableDuration *duration, ReadableInstant *endInstant) : BasePeriod(duration, endInstant, NULL) {}

MutablePeriod::MutablePeriod(ReadableDuration *duration, ReadableInstant *endInstant, const PeriodType *type) : BasePeriod(duration, endInstant, type) {}

MutablePeriod::MutablePeriod(Object *period) : BasePeriod(period, NULL, NULL) {}

MutablePeriod::MutablePeriod(Object *period, const PeriodType *type) : BasePeriod(period, type, NULL) {}

MutablePeriod::MutablePeriod(Object *period, Chronology *chrono) : BasePeriod(period, NULL, chrono) {}

MutablePeriod::MutablePeriod(Object *period, const PeriodType *type, Chronology *chrono) : BasePeriod(period, type, chrono) {}

//-----------------------------------------------------------------------
void MutablePeriod::clear() {
    BasePeriod::setValues(vector<int>(size()));
}

void MutablePeriod::setValue(int index, int value) {
    BasePeriod::setValue(index, value);
}

void MutablePeriod::set(DurationFieldType *field, int value) {
    BasePeriod::setField(field, value);
}

void MutablePeriod::setPeriod(ReadablePeriod *period) {
    BasePeriod::setPeriod(period);
}

void MutablePeriod::setPeriod(int years, int months, int weeks, int days,
                      int hours, int minutes, int seconds, int millis) {
    BasePeriod::setPeriod(years, months, weeks, days, hours, minutes, seconds, millis);
}

void MutablePeriod::setPeriod(ReadableInterval *interval) {
    if (interval == NULL) {
        setPeriod((int64_t) 0);
    } else {
        Chronology *chrono = DateTimeUtils::getChronology(interval->getChronology());
        setPeriod(interval->getStartMillis(), interval->getEndMillis(), chrono);
    }
}

void MutablePeriod::setPeriod(ReadableInstant *start, ReadableInstant *end) {
    if (start == end) {
        setPeriod((int64_t) 0);
    } else {
        int64_t startMillis = DateTimeUtils::getInstantMillis(start);
        int64_t endMillis = DateTimeUtils::getInstantMillis(end);
        Chronology *chrono = DateTimeUtils::getIntervalChronology(start, end);
        setPeriod(startMillis, endMillis, chrono);
    }
}

void MutablePeriod::setPeriod(int64_t startInstant, int64_t endInstant) {
    setPeriod(startInstant, endInstant, NULL);
}

void MutablePeriod::setPeriod(int64_t startInstant, int64_t endInstant, Chronology *chrono) {
    chrono = DateTimeUtils::getChronology(chrono);
    setValues(chrono->get(this, startInstant, endInstant));
}

void MutablePeriod::setPeriod(ReadableDuration *duration) {
    setPeriod(duration, NULL);
}

void MutablePeriod::setPeriod(ReadableDuration *duration, Chronology *chrono) {
    int64_t durationMillis = DateTimeUtils::getDurationMillis(duration);
    setPeriod(durationMillis, chrono);
}

void MutablePeriod::setPeriod(int64_t duration) {
    setPeriod(duration, (Chronology*) NULL);
}

void MutablePeriod::setPeriod(int64_t duration, Chronology *chrono) {
    chrono = DateTimeUtils::getChronology(chrono);
    setValues(chrono->get(this, duration));
}

//-----------------------------------------------------------------------
void MutablePeriod::add(DurationFieldType *field, int value) {
    BasePeriod::addField(field, value);
}

void MutablePeriod::add(ReadablePeriod *period) {
    BasePeriod::addPeriod(period);
}

void MutablePeriod::add(int years, int months, int weeks, int days,
                int hours, int minutes, int seconds, int millis) {
    setPeriod(FieldUtils::safeAdd(getYears(), years),
              FieldUtils::safeAdd(getMonths(), months),
              FieldUtils::safeAdd(getWeeks(), weeks),
              FieldUtils::safeAdd(getDays(), days),
              FieldUtils::safeAdd(getHours(), hours),
              FieldUtils::safeAdd(getMinutes(), minutes),
              FieldUtils::safeAdd(getSeconds(), seconds),
              FieldUtils::safeAdd(getMillis(), millis)
              );
}

void MutablePeriod::add(ReadableInterval *interval) {
    if (interval != NULL) {
        add(interval->toPeriod(getPeriodType()));
    }
}

void MutablePeriod::add(ReadableDuration *duration) {
    if (duration != NULL) {
        add(new Period(duration->getMillis(), getPeriodType()));
    }
}

void MutablePeriod::add(int64_t duration) {
    add(new Period(duration, getPeriodType()));
}

void MutablePeriod::add(int64_t duration, Chronology *chrono) {
    add(new Period(duration, getPeriodType(), chrono));
}

//-----------------------------------------------------------------------
void MutablePeriod::mergePeriod(ReadablePeriod *period) {
    BasePeriod::mergePeriod(period);
}

//-----------------------------------------------------------------------
int MutablePeriod::getYears() {
    return getPeriodType()->getIndexedField(this, PeriodType::YEAR_INDEX);
}

int MutablePeriod::getMonths() {
    return getPeriodType()->getIndexedField(this, PeriodType::MONTH_INDEX);
}

int MutablePeriod::getWeeks() {
    return getPeriodType()->getIndexedField(this, PeriodType::WEEK_INDEX);
}

int MutablePeriod::getDays() {
    return getPeriodType()->getIndexedField(this, PeriodType::DAY_INDEX);
}

//-----------------------------------------------------------------------
int MutablePeriod::getHours() {
    return getPeriodType()->getIndexedField(this, PeriodType::HOUR_INDEX);
}

int MutablePeriod::getMinutes() {
    return getPeriodType()->getIndexedField(this, PeriodType::MINUTE_INDEX);
}

int MutablePeriod::getSeconds() {
    return getPeriodType()->getIndexedField(this, PeriodType::SECOND_INDEX);
}

int MutablePeriod::getMillis() {
    return getPeriodType()->getIndexedField(this, PeriodType::MILLI_INDEX);
}

//-----------------------------------------------------------------------
void MutablePeriod::setYears(int years) {
    BasePeriod::setField(DurationFieldType::years(), years);
}

void MutablePeriod::addYears(int years) {
    BasePeriod::addField(DurationFieldType::years(), years);
}

//-----------------------------------------------------------------------
void MutablePeriod::setMonths(int months) {
    BasePeriod::setField(DurationFieldType::months(), months);
}

void MutablePeriod::addMonths(int months) {
    BasePeriod::addField(DurationFieldType::months(), months);
}

//-----------------------------------------------------------------------
void MutablePeriod::setWeeks(int weeks) {
    BasePeriod::setField(DurationFieldType::weeks(), weeks);
}

void MutablePeriod::addWeeks(int weeks) {
    BasePeriod::addField(DurationFieldType::weeks(), weeks);
}

//-----------------------------------------------------------------------
void MutablePeriod::setDays(int days) {
    BasePeriod::setField(DurationFieldType::days(), days);
}

void MutablePeriod::addDays(int days) {
    BasePeriod::addField(DurationFieldType::days(), days);
}

//-----------------------------------------------------------------------
void MutablePeriod::setHours(int hours) {
    BasePeriod::setField(DurationFieldType::hours(), hours);
}

void MutablePeriod::addHours(int hours) {
    BasePeriod::addField(DurationFieldType::hours(), hours);
}

//-----------------------------------------------------------------------
void MutablePeriod::setMinutes(int minutes) {
    BasePeriod::setField(DurationFieldType::minutes(), minutes);
}

void MutablePeriod::addMinutes(int minutes) {
    BasePeriod::addField(DurationFieldType::minutes(), minutes);
}

//-----------------------------------------------------------------------
void MutablePeriod::setSeconds(int seconds) {
    BasePeriod::setField(DurationFieldType::seconds(), seconds);
}

void MutablePeriod::addSeconds(int seconds) {
    BasePeriod::addField(DurationFieldType::seconds(), seconds);
}

//-----------------------------------------------------------------------
void MutablePeriod::setMillis(int millis) {
    BasePeriod::setField(DurationFieldType::millis(), millis);
}

void MutablePeriod::addMillis(int millis) {
    BasePeriod::addField(DurationFieldType::millis(), millis);
}

// Misc
//-----------------------------------------------------------------------
//MutablePeriod *copy() {
//    return (MutablePeriod) clone();
//}
//
//Object *clone() {
//    try {
//        return BasePeriod::clone();
//    } catch (CloneNotSupportedException ex) {
//        throw new InternalError("Clone error");
//    }
//}

CODATIME_END