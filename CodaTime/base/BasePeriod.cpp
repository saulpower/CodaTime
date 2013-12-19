//
//  BasePeriod.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "BasePeriod.h"

#include "base/BaseLocal.h"
#include "Chronology.h"
#include "Exceptions.h"
#include "ReadablePeriod.h"
#include "Duration.h"
#include "DurationFieldType.h"
#include "DateTimeUtils.h"
#include "field/FieldUtils.h"
#include "PeriodType.h"
#include "chrono/ISOChronology.h"
#include "MutablePeriod.h"
#include "ReadWritablePeriod.h"

CODATIME_BEGIN

const BasePeriod::DummyPeriod *BasePeriod::DUMMY_PERIOD = new DummyPeriod();

void BasePeriod::checkAndUpdate(const DurationFieldType *type, vector<int> values, int newValue) {
    int index = indexOf(type);
    if (index == -1) {
        if (newValue != 0) {
            throw IllegalArgumentException("Period does not support field '" + type->getName() + "'");
        }
    } else {
        values[index] = newValue;
    }
}

void BasePeriod::setPeriodInternal(ReadablePeriod *period) {
    vector<int> newValues = vector<int>(size());
    for (int i = 0, isize = period->size(); i < isize; i++) {
        const DurationFieldType *type = period->getFieldType(i);
        int value = period->getValue(i);
        checkAndUpdate(type, newValues, value);
    }
    setValues(newValues);
}

vector<int> BasePeriod::setPeriodInternal(int years, int months, int weeks, int days,
                              int hours, int minutes, int seconds, int millis) {
    vector<int> newValues = vector<int>(size());
    checkAndUpdate(DurationFieldType::years(), newValues, years);
    checkAndUpdate(DurationFieldType::months(), newValues, months);
    checkAndUpdate(DurationFieldType::weeks(), newValues, weeks);
    checkAndUpdate(DurationFieldType::days(), newValues, days);
    checkAndUpdate(DurationFieldType::hours(), newValues, hours);
    checkAndUpdate(DurationFieldType::minutes(), newValues, minutes);
    checkAndUpdate(DurationFieldType::seconds(), newValues, seconds);
    checkAndUpdate(DurationFieldType::millis(), newValues, millis);
    return newValues;
}

BasePeriod::BasePeriod(int years, int months, int weeks, int days,
           int hours, int minutes, int seconds, int millis,
           const PeriodType *type) {
    type = checkPeriodType(type);
    iType = type;
    iValues = setPeriodInternal(years, months, weeks, days, hours, minutes, seconds, millis); // internal method
}

BasePeriod::BasePeriod(int64_t startInstant, int64_t endInstant, const PeriodType *type, Chronology *chrono) {
    type = checkPeriodType(type);
    chrono = DateTimeUtils::getChronology(chrono);
    iType = type;
    iValues = chrono->get(this, startInstant, endInstant);
}

BasePeriod::BasePeriod(ReadableInstant *startInstant, ReadableInstant *endInstant, const PeriodType *type) {
    type = checkPeriodType(type);
    if (startInstant == NULL && endInstant == NULL) {
        iType = type;
        iValues = vector<int>(size());
    } else {
        int64_t startMillis = DateTimeUtils::getInstantMillis(startInstant);
        int64_t endMillis = DateTimeUtils::getInstantMillis(endInstant);
        Chronology *chrono = DateTimeUtils::getIntervalChronology(startInstant, endInstant);
        iType = type;
        iValues = chrono->get(this, startMillis, endMillis);
    }
}

BasePeriod::BasePeriod(ReadablePartial *start, ReadablePartial *end, const PeriodType *type) {
    if (start == NULL || end == NULL) {
        throw IllegalArgumentException("ReadablePartial objects must not be NULL");
    }
    
    BaseLocal *startBase = dynamic_cast<BaseLocal*>(start);
    BaseLocal *endBase = dynamic_cast<BaseLocal*>(end);
    
    // TODO && start->getClass() == end.getClass()
    if (startBase != 0 && endBase != 0) {
        // for performance
        type = checkPeriodType(type);
        int64_t startMillis = startBase->getLocalMillis();
        int64_t endMillis = endBase->getLocalMillis();
        Chronology *chrono = start->getChronology();
        chrono = DateTimeUtils::getChronology(chrono);
        iType = type;
        iValues = chrono->get(this, startMillis, endMillis);
    } else {
        if (start->size() != end->size()) {
            throw IllegalArgumentException("ReadablePartial objects must have the same set of fields");
        }
        for (int i = 0, isize = start->size(); i < isize; i++) {
            if (start->getFieldType(i) != end->getFieldType(i)) {
                throw IllegalArgumentException("ReadablePartial objects must have the same set of fields");
            }
        }
        if (DateTimeUtils::isContiguous(start) == false) {
            throw IllegalArgumentException("ReadablePartial objects must be contiguous");
        }
        iType = checkPeriodType(type);
        Chronology *chrono = DateTimeUtils::getChronology(start->getChronology())->withUTC();
        iValues = chrono->get(this, chrono->set(start, 0L), chrono->set(end, 0L));
    }
}

BasePeriod::BasePeriod(ReadableInstant *startInstant, ReadableDuration *duration, const PeriodType *type) {
    type = checkPeriodType(type);
    int64_t startMillis = DateTimeUtils::getInstantMillis(startInstant);
    int64_t durationMillis = DateTimeUtils::getDurationMillis(duration);
    int64_t endMillis = FieldUtils::safeAdd(startMillis, durationMillis);
    Chronology *chrono = DateTimeUtils::getInstantChronology(startInstant);
    iType = type;
    iValues = chrono->get(this, startMillis, endMillis);
}

BasePeriod::BasePeriod(ReadableDuration *duration, ReadableInstant *endInstant, const PeriodType *type) {
    type = checkPeriodType(type);
    int64_t durationMillis = DateTimeUtils::getDurationMillis(duration);
    int64_t endMillis = DateTimeUtils::getInstantMillis(endInstant);
    int64_t startMillis = FieldUtils::safeSubtract(endMillis, durationMillis);
    Chronology *chrono = DateTimeUtils::getInstantChronology(endInstant);
    iType = type;
    iValues = chrono->get(this, startMillis, endMillis);
}

BasePeriod::BasePeriod(int64_t duration) {
    // bug [3264409]
    // calculation uses period type from a period object (bad design)
    // thus we use a dummy period object with the time type
    iType = PeriodType::standard();
    vector<int> values = ISOChronology::getInstanceUTC()->get(DUMMY_PERIOD, duration);
    iValues = vector<int>(8);
    iValues = values;
}

BasePeriod::BasePeriod(int64_t duration, const PeriodType *type, Chronology *chrono) {
    type = checkPeriodType(type);
    chrono = DateTimeUtils::getChronology(chrono);
    iType = type;
    iValues = chrono->get(this, duration);
}

BasePeriod::BasePeriod(Object *period, const PeriodType *type, Chronology *chrono) {
    PeriodConverter *converter = ConverterManager::getInstance()->getPeriodConverter(period);
    type = (type == NULL ? converter->getPeriodType(period) : type);
    type = checkPeriodType(type);
    iType = type;
    
    ReadWritablePeriod *thisReadWrite = dynamic_cast<ReadWritablePeriod*>(this);
    
    if (thisReadWrite != 0) {
        iValues = vector<int>(size());
        chrono = DateTimeUtils::getChronology(chrono);
        converter->setInto(thisReadWrite, period, chrono);
    } else {
        iValues = (new MutablePeriod(period, type, chrono))->getValues();
    }
}

BasePeriod::BasePeriod(vector<int> values, const PeriodType *type) {
    iType = type;
    iValues = values;
}

const PeriodType *BasePeriod::checkPeriodType(const PeriodType *type) {
    return DateTimeUtils::getPeriodType(type);
}

void BasePeriod::setPeriod(ReadablePeriod *period) {
    if (period == NULL) {
        setValues(vector<int>(size()));
    } else {
        setPeriodInternal(period);
    }
}

void BasePeriod::setPeriod(int years, int months, int weeks, int days,
               int hours, int minutes, int seconds, int millis) {
    vector<int> newValues = setPeriodInternal(years, months, weeks, days, hours, minutes, seconds, millis);
    setValues(newValues);
}

void BasePeriod::setField(const DurationFieldType *field, int value) {
    setFieldInto(iValues, field, value);
}

void BasePeriod::setFieldInto(vector<int> values, const DurationFieldType *field, int value) {
    int index = indexOf(field);
    if (index == -1) {
        if (value != 0 || field == NULL) {
            string err("Period does not support field '");
            err.append(field->toString());
            err.append("'");
            throw IllegalArgumentException(err);
        }
    } else {
        values[index] = value;
    }
}

void BasePeriod::addField(const DurationFieldType *field, int value) {
    addFieldInto(iValues, field, value);
}

void BasePeriod::addFieldInto(vector<int> values, const DurationFieldType *field, int value) {
    int index = indexOf(field);
    if (index == -1) {
        if (value != 0 || field == NULL) {
            string err("Period does not support field '");
            err.append(field->toString());
            err.append("'");
            throw IllegalArgumentException(err);
        }
    } else {
        values[index] = FieldUtils::safeAdd(values[index], value);
    }
}

void BasePeriod::mergePeriod(ReadablePeriod *period) {
    if (period != NULL) {
        setValues(mergePeriodInto(getValues(), period));
    }
}

vector<int> BasePeriod::mergePeriodInto(vector<int> values, ReadablePeriod *period) {
    for (int i = 0, isize = period->size(); i < isize; i++) {
        const DurationFieldType *type = period->getFieldType(i);
        int value = period->getValue(i);
        checkAndUpdate(type, values, value);
    }
    return values;
}

void BasePeriod::addPeriod(ReadablePeriod *period) {
    if (period != NULL) {
        setValues(addPeriodInto(getValues(), period));
    }
}

vector<int> BasePeriod::addPeriodInto(vector<int> values, ReadablePeriod *period) {
    for (int i = 0, isize = period->size(); i < isize; i++) {
        const DurationFieldType *type = period->getFieldType(i);
        int value = period->getValue(i);
        if (value != 0) {
            int index = indexOf(type);
            if (index == -1) {
                string err("Period does not support field '");
                err.append(type->getName());
                err.append("'");
                throw IllegalArgumentException(err);
            } else {
                values[index] = FieldUtils::safeAdd(getValue(index), value);
            }
        }
    }
    return values;
}

void BasePeriod::setValue(int index, int value) {
    iValues[index] = value;
}

void BasePeriod::setValues(vector<int> values) {
    iValues = values;
}

const PeriodType *BasePeriod::getPeriodType() const {
    return iType;
}

int BasePeriod::getValue(int index) const {
    return iValues[index];
}

Duration *BasePeriod::toDurationFrom(ReadableInstant *startInstant) {
    int64_t startMillis = DateTimeUtils::getInstantMillis(startInstant);
    Chronology *chrono = DateTimeUtils::getInstantChronology(startInstant);
    int64_t endMillis = chrono->add(this, startMillis, 1);
    return new Duration(startMillis, endMillis);
}

Duration *BasePeriod::toDurationTo(ReadableInstant *endInstant) {
    int64_t endMillis = DateTimeUtils::getInstantMillis(endInstant);
    Chronology *chrono = DateTimeUtils::getInstantChronology(endInstant);
    int64_t startMillis = chrono->add(this, endMillis, -1);
    return new Duration(startMillis, endMillis);
}

CODATIME_END