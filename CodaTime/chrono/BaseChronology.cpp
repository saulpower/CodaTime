//
//  BaseChronology.c
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "BaseChronology.h"

#include "DateTimeField.h"
#include "DurationFieldType.h"
#include "Exceptions.h"
#include "field/FieldUtils.h"
#include "field/UnsupportedDateTimeField.h"
#include "field/UnsupportedDurationfield.h"
#include "ReadablePartial.h"
#include "ReadablePeriod.h"

CODATIME_BEGIN

int64_t BaseChronology::getDateTimeMillis(int yearNum, int monthOfYearNum, int dayOfMonthNum,
                          int millisOfDayNum) {
    int64_t instant = year()->set(0, yearNum);
    instant = monthOfYear()->set(instant, monthOfYearNum);
    instant = dayOfMonth()->set(instant, dayOfMonthNum);
    return millisOfDay()->set(instant, millisOfDayNum);
}

int64_t BaseChronology::getDateTimeMillis(int yearNum, int monthOfYearNum, int dayOfMonthNum,
                          int hourOfDayNum, int minuteOfHourNum,
                          int secondOfMinuteNum, int millisOfSecondNum) {
    int64_t instant = year()->set(0, yearNum);
    instant = monthOfYear()->set(instant, monthOfYearNum);
    instant = dayOfMonth()->set(instant, dayOfMonthNum);
    instant = hourOfDay()->set(instant, hourOfDayNum);
    instant = minuteOfHour()->set(instant, minuteOfHourNum);
    instant = secondOfMinute()->set(instant, secondOfMinuteNum);
    return millisOfSecond()->set(instant, millisOfSecondNum);
}

int64_t BaseChronology::getDateTimeMillis(int64_t instant,
                          int hourOfDayNum, int minuteOfHourNum,
                          int secondOfMinuteNum, int millisOfSecondNum) {
    instant = hourOfDay()->set(instant, hourOfDayNum);
    instant = minuteOfHour()->set(instant, minuteOfHourNum);
    instant = secondOfMinute()->set(instant, secondOfMinuteNum);
    return millisOfSecond()->set(instant, millisOfSecondNum);
}

//-----------------------------------------------------------------------
void BaseChronology::validate(ReadablePartial *partial, vector<int> values) {
    // check values in standard range, catching really stupid cases like -1
    // this means that the second check will not hit trouble
    int size = partial->size();
    for (int i = 0; i < size; i++) {
        int value = values[i];
        const DateTimeField *field = partial->getField(i);
        if (value < field->getMinimumValue()) {
            throw IllegalFieldValueException(field->getType()->toString(), to_string(value), to_string(field->getMinimumValue()), string());
        }
        if (value > field->getMaximumValue()) {
            throw IllegalFieldValueException(field->getType()->toString(), to_string(value), string(), to_string(field->getMaximumValue()));
        }
    }
    // check values in specific range, catching really odd cases like 30th Feb
    for (int i = 0; i < size; i++) {
        int value = values[i];
        const DateTimeField *field = partial->getField(i);
        if (value < field->getMinimumValue(partial, values)) {
            throw IllegalFieldValueException
            (field->getType()->toString(), to_string(value),
             to_string(field->getMinimumValue(partial, values)), string());
        }
        if (value > field->getMaximumValue(partial, values)) {
            throw IllegalFieldValueException
            (field->getType()->toString(), to_string(value),
             string(), to_string(field->getMaximumValue(partial, values)));
        }
    }
}

vector<int> BaseChronology::get(ReadablePartial *partial, int64_t instant) {
    int size = partial->size();
    vector<int> values = vector<int>(size);
    for (int i = 0; i < size; i++) {
        values[i] = partial->getFieldType(i)->getField(this)->get(instant);
    }
    return values;
}

int64_t BaseChronology::set(ReadablePartial *partial, int64_t instant) {
    for (int i = 0, isize = partial->size(); i < isize; i++) {
        instant = partial->getFieldType(i)->getField(this)->set(instant, partial->getValue(i));
    }
    return instant;
}

//-----------------------------------------------------------------------
vector<int> BaseChronology::get(ReadablePeriod *period, int64_t startInstant, int64_t endInstant) {
    int size = period->size();
    vector<int> values = vector<int>(size);
    if (startInstant != endInstant) {
        for (int i = 0; i < size; i++) {
            const DurationField *field = period->getFieldType(i)->getField(this);
            int value = field->getDifference(endInstant, startInstant);
            startInstant = field->add(startInstant, value);
            values[i] = value;
        }
    }
    return values;
}

vector<int> BaseChronology::get(ReadablePeriod *period, int64_t duration) {
    int size = period->size();
    vector<int> values = vector<int>(size);
    if (duration != 0) {
        int64_t current = 0;
        for (int i = 0; i < size; i++) {
            const DurationField *field = period->getFieldType(i)->getField(this);
            if (field->isPrecise()) {
                int value = field->getDifference(duration, current);
                current = field->add(current, value);
                values[i] = value;
            }
        }
    }
    return values;
}

int64_t BaseChronology::add(ReadablePeriod *period, int64_t instant, int scalar) {
    if (scalar != 0 && period != NULL) {
        for (int i = 0, isize = period->size(); i < isize; i++) {
            int64_t value = period->getValue(i); // use int64_t to allow for multiplication (fits OK)
            if (value != 0) {
                instant = period->getFieldType(i)->getField(this)->add(instant, value * scalar);
            }
        }
    }
    return instant;
}

//-----------------------------------------------------------------------
int64_t BaseChronology::add(int64_t instant, int64_t duration, int scalar) {
    if (duration == 0 || scalar == 0) {
        return instant;
    }
    int64_t add = FieldUtils::safeMultiply(duration, scalar);
    return FieldUtils::safeAdd(instant, add);
}

// Millis
//-----------------------------------------------------------------------
const DurationField *BaseChronology::millis() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::millis());
}

const DateTimeField *BaseChronology::millisOfSecond() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::millisOfSecond(), millis());
}

const DateTimeField *BaseChronology::millisOfDay() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::millisOfDay(), millis());
}

// Second
//-----------------------------------------------------------------------
const DurationField *BaseChronology::seconds() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::seconds());
}

const DateTimeField *BaseChronology::secondOfMinute() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::secondOfMinute(), seconds());
}

const DateTimeField *BaseChronology::secondOfDay() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::secondOfDay(), seconds());
}

// Minute
//-----------------------------------------------------------------------
const DurationField *BaseChronology::minutes() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::minutes());
}

const DateTimeField *BaseChronology::minuteOfHour() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::minuteOfHour(), minutes());
}

const DateTimeField *BaseChronology::minuteOfDay() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::minuteOfDay(), minutes());
}

// Hour
//-----------------------------------------------------------------------
const DurationField *BaseChronology::hours() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::hours());
}

const DateTimeField *BaseChronology::hourOfDay() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::hourOfDay(), hours());
}

const DateTimeField *BaseChronology::clockhourOfDay() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::clockhourOfDay(), hours());
}

// Halfday
//-----------------------------------------------------------------------
const DurationField *BaseChronology::halfdays() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::halfdays());
}

const DateTimeField *BaseChronology::hourOfHalfday() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::hourOfHalfday(), hours());
}

const DateTimeField *BaseChronology::clockhourOfHalfday() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::clockhourOfHalfday(), hours());
}

const DateTimeField *BaseChronology::halfdayOfDay() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::halfdayOfDay(), halfdays());
}

// Day
//-----------------------------------------------------------------------
const DurationField *BaseChronology::days() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::days());
}

const DateTimeField *BaseChronology::dayOfWeek() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::dayOfWeek(), days());
}

const DateTimeField *BaseChronology::dayOfMonth() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::dayOfMonth(), days());
}

const DateTimeField *BaseChronology::dayOfYear() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::dayOfYear(), days());
}

// Week
//-----------------------------------------------------------------------
const DurationField *BaseChronology::weeks() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::weeks());
}

const DateTimeField *BaseChronology::weekOfWeekyear() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::weekOfWeekyear(), weeks());
}

// Weekyear
//-----------------------------------------------------------------------
const DurationField *BaseChronology::weekyears() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::weekyears());
}

const DateTimeField *BaseChronology::weekyear() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::weekyear(), weekyears());
}

const DateTimeField *BaseChronology::weekyearOfCentury() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::weekyearOfCentury(), weekyears());
}

// Month
//-----------------------------------------------------------------------
const DurationField *BaseChronology::months() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::months());
}

const DateTimeField *BaseChronology::monthOfYear() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::monthOfYear(), months());
}

// Year
//-----------------------------------------------------------------------
const DurationField *BaseChronology::years() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::years());
}

const DateTimeField *BaseChronology::year() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::year(), years());
}

const DateTimeField *BaseChronology::yearOfEra() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::yearOfEra(), years());
}

const DateTimeField *BaseChronology::yearOfCentury() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::yearOfCentury(), years());
}

// Century
//-----------------------------------------------------------------------
const DurationField *BaseChronology::centuries() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::centuries());
}

const DateTimeField *BaseChronology::centuryOfEra() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::centuryOfEra(), centuries());
}

// Era
//-----------------------------------------------------------------------
const DurationField *BaseChronology::eras() {
    return UnsupportedDurationfield::getInstance(DurationFieldType::eras());
}

const DateTimeField *BaseChronology::era() {
    return UnsupportedDateTimeField::getInstance(DateTimeFieldType::era(), eras());
}

CODATIME_END
