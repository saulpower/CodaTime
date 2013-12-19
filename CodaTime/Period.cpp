//
//  Period.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "chrono/ISOChronology.h"
#include "DateTimeConstants.h"
#include "DateTimeFieldType.h"
#include "DateTimeUtils.h"
#include "Duration.h"
#include "field/FieldUtils.h"
#include "format/ISOPeriodFormat.h"
#include "format/PeriodFormatter.h"
#include "Period.h"
#include "ReadablePartial.h"

#include "Exceptions.h"

CODATIME_BEGIN

const Period *Period::ZERO = new Period();

Period::Period(vector<int> values, const PeriodType *type) : BasePeriod(values, type) {
}

void Period::checkYearsAndMonths(string destintionType) {
    if (getMonths() != 0) {
        string err("Cannot convert to ");
        err.append(destintionType);
        err.append(" as this period contains months and months vary in.size()");
        throw UnsupportedOperationException(err);
    }
    if (getYears() != 0) {
        string err("Cannot convert to ");
        err.append(destintionType);
        err.append(" as this period contains years and years vary in.size()");
        throw UnsupportedOperationException(err);
    }
}

//-----------------------------------------------------------------------
Period *Period::parse(string str) {
    return parse(str, ISOPeriodFormat::standard());
}

Period *Period::parse(string str, PeriodFormatter *formatter) {
    return formatter->parsePeriod(str);
}

//-----------------------------------------------------------------------
Period *Period::years(int years) {
    return new Period({years, 0, 0, 0, 0, 0, 0, 0, 0}, PeriodType::standard());
}

Period *Period::months(int months) {
    return new Period({0, months, 0, 0, 0, 0, 0, 0}, PeriodType::standard());
}

Period *Period::weeks(int weeks) {
    return new Period({0, 0, weeks, 0, 0, 0, 0, 0}, PeriodType::standard());
}

Period *Period::days(int days) {
    return new Period({0, 0, 0, days, 0, 0, 0, 0}, PeriodType::standard());
}

Period *Period::hours(int hours) {
    return new Period({0, 0, 0, 0, hours, 0, 0, 0}, PeriodType::standard());
}

Period *Period::minutes(int minutes) {
    return new Period({0, 0, 0, 0, 0, minutes, 0, 0}, PeriodType::standard());
}

Period *Period::seconds(int seconds) {
    return new Period({0, 0, 0, 0, 0, 0, seconds, 0}, PeriodType::standard());
}

Period *Period::millis(int millis) {
    return new Period({0, 0, 0, 0, 0, 0, 0, millis}, PeriodType::standard());
}

//-----------------------------------------------------------------------
Period *Period::fieldDifference(ReadablePartial *start, ReadablePartial *end) {
    if (start == NULL || end == NULL) {
        throw IllegalArgumentException("ReadablePartial objects must not be NULL");
    }
    if (start->size() != end->size()) {
        throw IllegalArgumentException("ReadablePartial objects must have the same set of fields");
    }
    vector<const DurationFieldType*> types = vector<const DurationFieldType*>(start->size());
    vector<int> values = vector<int>(start->size());
    for (int i = 0, isize = start->size(); i < isize; i++) {
        if (start->getFieldType(i) != end->getFieldType(i)) {
            throw IllegalArgumentException("ReadablePartial objects must have the same set of fields");
        }
        types[i] = start->getFieldType(i)->getDurationType();
        if (i > 0 && types[i - 1] == types[i]) {
            throw IllegalArgumentException("ReadablePartial objects must not have overlapping fields");
        }
        values[i] = end->getValue(i) - start->getValue(i);
    }
    return new Period(values, PeriodType::forFields(types));
}

//-----------------------------------------------------------------------
Period::Period() : BasePeriod(0LL, (const PeriodType*) NULL, (Chronology*) NULL) {
}

Period::Period(int hours, int minutes, int seconds, int millis) : BasePeriod(0, 0, 0, 0, hours, minutes, seconds, millis, PeriodType::standard()) {
}

Period::Period(int years, int months, int weeks, int days,
       int hours, int minutes, int seconds, int millis) : BasePeriod(years, months, weeks, days, hours, minutes, seconds, millis, PeriodType::standard()) {
}

Period::Period(int years, int months, int weeks, int days,
       int hours, int minutes, int seconds, int millis, const PeriodType *type) : BasePeriod(years, months, weeks, days, hours, minutes, seconds, millis, type) {
}

Period::Period(int64_t duration) : BasePeriod(duration) {
}

Period::Period(int64_t duration, const PeriodType *type) : BasePeriod(duration, type, NULL) {
}

Period::Period(int64_t duration, Chronology *chronology) : BasePeriod(duration, NULL, chronology) {
}

Period::Period(int64_t duration, const PeriodType *type, Chronology *chronology) : BasePeriod(duration, type, chronology) {
}

Period::Period(int64_t startInstant, int64_t endInstant) : BasePeriod(startInstant, endInstant, NULL, NULL) {
}

Period::Period(int64_t startInstant, int64_t endInstant, const PeriodType *type) : BasePeriod(startInstant, endInstant, type, NULL) {
}

Period::Period(int64_t startInstant, int64_t endInstant, Chronology *chrono) : BasePeriod(startInstant, endInstant, NULL, chrono) {
}

Period::Period(int64_t startInstant, int64_t endInstant, const PeriodType *type, Chronology *chrono) : BasePeriod(startInstant, endInstant, type, chrono) {
}

Period::Period(ReadableInstant *startInstant, ReadableInstant *endInstant) : BasePeriod(startInstant, endInstant, NULL) {
}

Period::Period(ReadableInstant *startInstant, ReadableInstant *endInstant, const PeriodType *type) : BasePeriod(startInstant, endInstant, type) {
}

Period::Period(ReadablePartial *start, ReadablePartial *end) : BasePeriod(start, end, NULL) {
}

Period::Period(ReadablePartial *start, ReadablePartial *end, const PeriodType *type) : BasePeriod(start, end, type) {
}

Period::Period(ReadableInstant *startInstant, ReadableDuration *duration) : BasePeriod(startInstant, duration, NULL) {
}

Period::Period(ReadableInstant *startInstant, ReadableDuration *duration, const PeriodType *type) : BasePeriod(startInstant, duration, type) {
}

Period::Period(ReadableDuration *duration, ReadableInstant *endInstant) : BasePeriod(duration, endInstant, NULL) {
}

Period::Period(ReadableDuration *duration, ReadableInstant *endInstant, const PeriodType *type) : BasePeriod(duration, endInstant, type) {
}

Period::Period(Object *period) : BasePeriod(period, NULL, NULL) {
}

Period::Period(Object *period, const PeriodType *type) : BasePeriod(period, type, NULL) {
}

Period::Period(Object *period, Chronology *chrono) : BasePeriod(period, NULL, chrono) {
}

Period::Period(Object *period, const PeriodType *type, Chronology *chrono) : BasePeriod(period, type, chrono) {
}

//-----------------------------------------------------------------------
Period *Period::toPeriod() {
    return this;
}

//-----------------------------------------------------------------------
int Period::getYears() {
    return getPeriodType()->getIndexedField(this, PeriodType::YEAR_INDEX);
}

int Period::getMonths() {
    return getPeriodType()->getIndexedField(this, PeriodType::MONTH_INDEX);
}

int Period::getWeeks() {
    return getPeriodType()->getIndexedField(this, PeriodType::WEEK_INDEX);
}

int Period::getDays() {
    return getPeriodType()->getIndexedField(this, PeriodType::DAY_INDEX);
}

//-----------------------------------------------------------------------
int Period::getHours() {
    return getPeriodType()->getIndexedField(this, PeriodType::HOUR_INDEX);
}

int Period::getMinutes() {
    return getPeriodType()->getIndexedField(this, PeriodType::MINUTE_INDEX);
}

int Period::getSeconds() {
    return getPeriodType()->getIndexedField(this, PeriodType::SECOND_INDEX);
}

int Period::getMillis() {
    return getPeriodType()->getIndexedField(this, PeriodType::MILLI_INDEX);
}

//-----------------------------------------------------------------------
Period *Period::withPeriodType(const PeriodType *type) {
    type = DateTimeUtils::getPeriodType(type);
    if (type->equals(getPeriodType())) {
        return this;
    }
    return new Period(this, type);
}

Period *Period::withFields(ReadablePeriod *period) {
    if (period == NULL) {
        return this;
    }
    vector<int> newValues = getValues();  // cloned
    newValues = BasePeriod::mergePeriodInto(newValues, period);
    return new Period(newValues, getPeriodType());
}

//-----------------------------------------------------------------------
Period *Period::withField(DurationFieldType *field, int value) {
    if (field == NULL) {
        throw IllegalArgumentException("Field must not be NULL");
    }
    vector<int> newValues = getValues();  // cloned
    BasePeriod::setFieldInto(newValues, field, value);
    return new Period(newValues, getPeriodType());
}

Period *Period::withFieldAdded(DurationFieldType *field, int value) {
    if (field == NULL) {
        throw IllegalArgumentException("Field must not be NULL");
    }
    if (value == 0) {
        return this;
    }
    vector<int> newValues = getValues();  // cloned
    BasePeriod::addFieldInto(newValues, field, value);
    return new Period(newValues, getPeriodType());
}

//-----------------------------------------------------------------------
Period *Period::withYears(int years) {
    vector<int> values = getValues();  // cloned
    getPeriodType()->setIndexedField(this, PeriodType::YEAR_INDEX, values, years);
    return new Period(values, getPeriodType());
}

Period *Period::withMonths(int months) {
    vector<int> values = getValues();  // cloned
    getPeriodType()->setIndexedField(this, PeriodType::MONTH_INDEX, values, months);
    return new Period(values, getPeriodType());
}

Period *Period::withWeeks(int weeks) {
    vector<int> values = getValues();  // cloned
    getPeriodType()->setIndexedField(this, PeriodType::WEEK_INDEX, values, weeks);
    return new Period(values, getPeriodType());
}

Period *Period::withDays(int days) {
    vector<int> values = getValues();  // cloned
    getPeriodType()->setIndexedField(this, PeriodType::DAY_INDEX, values, days);
    return new Period(values, getPeriodType());
}

Period *Period::withHours(int hours) {
    vector<int> values = getValues();  // cloned
    getPeriodType()->setIndexedField(this, PeriodType::HOUR_INDEX, values, hours);
    return new Period(values, getPeriodType());
}

Period *Period::withMinutes(int minutes) {
    vector<int> values = getValues();  // cloned
    getPeriodType()->setIndexedField(this, PeriodType::MINUTE_INDEX, values, minutes);
    return new Period(values, getPeriodType());
}

Period *Period::withSeconds(int seconds) {
    vector<int> values = getValues();  // cloned
    getPeriodType()->setIndexedField(this, PeriodType::SECOND_INDEX, values, seconds);
    return new Period(values, getPeriodType());
}

Period *Period::withMillis(int millis) {
    vector<int> values = getValues();  // cloned
    getPeriodType()->setIndexedField(this, PeriodType::MILLI_INDEX, values, millis);
    return new Period(values, getPeriodType());
}

//-----------------------------------------------------------------------
Period *Period::plus(ReadablePeriod *period) {
    if (period == NULL) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::YEAR_INDEX, values, period->get(DurationFieldType::YEARS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::MONTH_INDEX, values, period->get(DurationFieldType::MONTHS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::WEEK_INDEX, values, period->get(DurationFieldType::WEEKS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::DAY_INDEX, values, period->get(DurationFieldType::DAYS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::HOUR_INDEX, values, period->get(DurationFieldType::HOURS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::MINUTE_INDEX, values, period->get(DurationFieldType::MINUTES_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::SECOND_INDEX, values, period->get(DurationFieldType::SECONDS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::MILLI_INDEX, values, period->get(DurationFieldType::MILLIS_TYPE));
    return new Period(values, getPeriodType());
}

//-----------------------------------------------------------------------
Period *Period::plusYears(int years) {
    if (years == 0) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::YEAR_INDEX, values, years);
    return new Period(values, getPeriodType());
}

Period *Period::plusMonths(int months) {
    if (months == 0) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::MONTH_INDEX, values, months);
    return new Period(values, getPeriodType());
}

Period *Period::plusWeeks(int weeks) {
    if (weeks == 0) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::WEEK_INDEX, values, weeks);
    return new Period(values, getPeriodType());
}

Period *Period::plusDays(int days) {
    if (days == 0) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::DAY_INDEX, values, days);
    return new Period(values, getPeriodType());
}

Period *Period::plusHours(int hours) {
    if (hours == 0) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::HOUR_INDEX, values, hours);
    return new Period(values, getPeriodType());
}

Period *Period::plusMinutes(int minutes) {
    if (minutes == 0) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::MINUTE_INDEX, values, minutes);
    return new Period(values, getPeriodType());
}

Period *Period::plusSeconds(int seconds) {
    if (seconds == 0) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::SECOND_INDEX, values, seconds);
    return new Period(values, getPeriodType());
}

Period *Period::plusMillis(int millis) {
    if (millis == 0) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::MILLI_INDEX, values, millis);
    return new Period(values, getPeriodType());
}

//-----------------------------------------------------------------------
Period *Period::minus(ReadablePeriod *period) {
    if (period == NULL) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    getPeriodType()->addIndexedField(this, PeriodType::YEAR_INDEX, values, -period->get(DurationFieldType::YEARS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::MONTH_INDEX, values, -period->get(DurationFieldType::MONTHS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::WEEK_INDEX, values, -period->get(DurationFieldType::WEEKS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::DAY_INDEX, values, -period->get(DurationFieldType::DAYS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::HOUR_INDEX, values, -period->get(DurationFieldType::HOURS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::MINUTE_INDEX, values, -period->get(DurationFieldType::MINUTES_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::SECOND_INDEX, values, -period->get(DurationFieldType::SECONDS_TYPE));
    getPeriodType()->addIndexedField(this, PeriodType::MILLI_INDEX, values, -period->get(DurationFieldType::MILLIS_TYPE));
    return new Period(values, getPeriodType());
}

//-----------------------------------------------------------------------
Period *Period::minusYears(int years) {
    return plusYears(-years);
}

Period *Period::minusMonths(int months) {
    return plusMonths(-months);
}

Period *Period::minusWeeks(int weeks) {
    return plusWeeks(-weeks);
}

Period *Period::minusDays(int days) {
    return plusDays(-days);
}

Period *Period::minusHours(int hours) {
    return plusHours(-hours);
}

Period *Period::minusMinutes(int minutes) {
    return plusMinutes(-minutes);
}

Period *Period::minusSeconds(int seconds) {
    return plusSeconds(-seconds);
}

Period *Period::minusMillis(int millis) {
    return plusMillis(-millis);
}

Period *Period::multipliedBy(int scalar) {
    if (this == ZERO || scalar == 1) {
        return this;
    }
    vector<int> values = getValues();  // cloned
    for (int i = 0; i < values.size(); i++) {
        values[i] = FieldUtils::safeMultiply(values[i], scalar);
    }
    return new Period(values, getPeriodType());
}

Period *Period::negated() {
    return multipliedBy(-1);
}

Weeks *Period::toStandardWeeks() {
    checkYearsAndMonths("Weeks");
    int64_t millis = getMillis();  // assign to a int64_t
    millis += ((int64_t) getSeconds()) * DateTimeConstants::MILLIS_PER_SECOND;
    millis += ((int64_t) getMinutes()) * DateTimeConstants::MILLIS_PER_MINUTE;
    millis += ((int64_t) getHours()) * DateTimeConstants::MILLIS_PER_HOUR;
    millis += ((int64_t) getDays()) * DateTimeConstants::MILLIS_PER_DAY;
    int64_t weeks = ((int64_t) getWeeks()) + millis / DateTimeConstants::MILLIS_PER_WEEK;
    return Weeks::weeks(FieldUtils::safeToInt(weeks));
}

Days *Period::toStandardDays() {
    checkYearsAndMonths("Days");
    int64_t millis = getMillis();  // assign to a int64_t
    millis += ((int64_t) getSeconds()) * DateTimeConstants::MILLIS_PER_SECOND;
    millis += ((int64_t) getMinutes()) * DateTimeConstants::MILLIS_PER_MINUTE;
    millis += ((int64_t) getHours()) * DateTimeConstants::MILLIS_PER_HOUR;
    int64_t days = millis / DateTimeConstants::MILLIS_PER_DAY;
    days = FieldUtils::safeAdd(days, (int64_t) getDays());
    days = FieldUtils::safeAdd(days, ((int64_t) getWeeks()) * ((int64_t) DateTimeConstants::DAYS_PER_WEEK));
    return Days::days(FieldUtils::safeToInt(days));
}

Hours *Period::toStandardHours() {
    checkYearsAndMonths("Hours");
    int64_t millis = getMillis();  // assign to a int64_t
    millis += ((int64_t) getSeconds()) * DateTimeConstants::MILLIS_PER_SECOND;
    millis += ((int64_t) getMinutes()) * DateTimeConstants::MILLIS_PER_MINUTE;
    int64_t hours = millis / DateTimeConstants::MILLIS_PER_HOUR;
    hours = FieldUtils::safeAdd(hours, (int64_t) getHours());
    hours = FieldUtils::safeAdd(hours, ((int64_t) getDays()) * ((int64_t) DateTimeConstants::HOURS_PER_DAY));
    hours = FieldUtils::safeAdd(hours, ((int64_t) getWeeks()) * ((int64_t) DateTimeConstants::HOURS_PER_WEEK));
    return Hours::hours(FieldUtils::safeToInt(hours));
}

Minutes *Period::toStandardMinutes() {
    checkYearsAndMonths("Minutes");
    int64_t millis = getMillis();  // assign to a int64_t
    millis += ((int64_t) getSeconds()) * DateTimeConstants::MILLIS_PER_SECOND;
    int64_t minutes = millis / DateTimeConstants::MILLIS_PER_MINUTE;
    minutes = FieldUtils::safeAdd(minutes, (int64_t) getMinutes());
    minutes = FieldUtils::safeAdd(minutes, ((int64_t) getHours()) * ((int64_t) DateTimeConstants::MINUTES_PER_HOUR));
    minutes = FieldUtils::safeAdd(minutes, ((int64_t) getDays()) * ((int64_t) DateTimeConstants::MINUTES_PER_DAY));
    minutes = FieldUtils::safeAdd(minutes, ((int64_t) getWeeks()) * ((int64_t) DateTimeConstants::MINUTES_PER_WEEK));
    return Minutes::minutes(FieldUtils::safeToInt(minutes));
}

Seconds *Period::toStandardSeconds() {
    checkYearsAndMonths("Seconds");
    int64_t seconds = getMillis() / DateTimeConstants::MILLIS_PER_SECOND;
    seconds = FieldUtils::safeAdd(seconds, (int64_t) getSeconds());
    seconds = FieldUtils::safeAdd(seconds, ((int64_t) getMinutes()) * ((int64_t) DateTimeConstants::SECONDS_PER_MINUTE));
    seconds = FieldUtils::safeAdd(seconds, ((int64_t) getHours()) * ((int64_t) DateTimeConstants::SECONDS_PER_HOUR));
    seconds = FieldUtils::safeAdd(seconds, ((int64_t) getDays()) * ((int64_t) DateTimeConstants::SECONDS_PER_DAY));
    seconds = FieldUtils::safeAdd(seconds, ((int64_t) getWeeks()) * ((int64_t) DateTimeConstants::SECONDS_PER_WEEK));
    return Seconds::seconds(FieldUtils::safeToInt(seconds));
}

Duration *Period::toStandardDuration() {
    checkYearsAndMonths("Duration");
    int64_t millis = getMillis();  // no overflow can happen, even with Integer.MAX_VALUEs
    millis += (((int64_t) getSeconds()) * ((int64_t) DateTimeConstants::MILLIS_PER_SECOND));
    millis += (((int64_t) getMinutes()) * ((int64_t) DateTimeConstants::MILLIS_PER_MINUTE));
    millis += (((int64_t) getHours()) * ((int64_t) DateTimeConstants::MILLIS_PER_HOUR));
    millis += (((int64_t) getDays()) * ((int64_t) DateTimeConstants::MILLIS_PER_DAY));
    millis += (((int64_t) getWeeks()) * ((int64_t) DateTimeConstants::MILLIS_PER_WEEK));
    return new Duration(millis);
}

Period *Period::normalizedStandard() {
    return normalizedStandard(PeriodType::standard());
}

Period *Period::normalizedStandard(const PeriodType *type) {
    type = DateTimeUtils::getPeriodType(type);
    int64_t millis = getMillis();  // no overflow can happen, even with Integer.MAX_VALUEs
    millis += (((int64_t) getSeconds()) * ((int64_t) DateTimeConstants::MILLIS_PER_SECOND));
    millis += (((int64_t) getMinutes()) * ((int64_t) DateTimeConstants::MILLIS_PER_MINUTE));
    millis += (((int64_t) getHours()) * ((int64_t) DateTimeConstants::MILLIS_PER_HOUR));
    millis += (((int64_t) getDays()) * ((int64_t) DateTimeConstants::MILLIS_PER_DAY));
    millis += (((int64_t) getWeeks()) * ((int64_t) DateTimeConstants::MILLIS_PER_WEEK));
    Period *result = new Period(millis, type, ISOChronology::getInstanceUTC());
    int years = getYears();
    int months = getMonths();
    if (years != 0 || months != 0) {
        int64_t totalMonths = years * 12L + months;
        if (type->isSupported(DurationFieldType::YEARS_TYPE)) {
            int normalizedYears = FieldUtils::safeToInt(totalMonths / 12);
            result = result->withYears(normalizedYears);
            totalMonths = totalMonths - (normalizedYears * 12);
        }
        if (type->isSupported(DurationFieldType::MONTHS_TYPE)) {
            int normalizedMonths = FieldUtils::safeToInt(totalMonths);
            result = result->withMonths(normalizedMonths);
            totalMonths = totalMonths - normalizedMonths;
        }
        if (totalMonths != 0) {
            string err("Unable to normalize as PeriodType is missing either years or months but period has a month/year amount: ");
            err.append(toString());
            throw UnsupportedOperationException(err);
        }
    }
    return result;
}

CODATIME_END