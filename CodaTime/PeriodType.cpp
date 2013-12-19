//
//  PeriodType.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "PeriodType.h"

#include "DurationFieldType.h"
#include "Exceptions.h"
#include "field/FieldUtils.h"
#include "ReadablePeriod.h"

#include <functional>

CODATIME_BEGIN

int PeriodType::getIndexedField(ReadablePeriod *period, int index) const {
    int realIndex = iIndices[index];
    return (realIndex == -1 ? 0 : period->getValue(realIndex));
}

bool PeriodType::setIndexedField(ReadablePeriod *period, int index, vector<int> values, int newValue) const {
    int realIndex = iIndices[index];
    if (realIndex == -1) {
        throw UnsupportedOperationException("Field is not supported");
    }
    values[realIndex] = newValue;
    return true;
}

bool PeriodType::addIndexedField(ReadablePeriod *period, int index, vector<int> values, int valueToAdd) const {
    if (valueToAdd == 0) {
        return false;
    }
    int realIndex = iIndices[index];
    if (realIndex == -1) {
        throw UnsupportedOperationException("Field is not supported");
    }
    values[realIndex] = FieldUtils::safeAdd(values[realIndex], valueToAdd);
    return true;
}

PeriodType::PeriodType(string name, vector<const DurationFieldType*> types, vector<int> indices) {
    iName = name;
    iTypes = types;
    iIndices = indices;
}

PeriodType *PeriodType::standard() {
    PeriodType *type = cStandard;
    if (type == NULL) {
        type = new PeriodType(
                              string("Standard"),
                              {
                                  DurationFieldType::years(), DurationFieldType::months(),
                                  DurationFieldType::weeks(), DurationFieldType::days(),
                                  DurationFieldType::hours(), DurationFieldType::minutes(),
                                  DurationFieldType::seconds(), DurationFieldType::millis(),
                              },
                              { 0, 1, 2, 3, 4, 5, 6, 7, }
                              );
        cStandard = type;
    }
    return type;
}

PeriodType *PeriodType::yearMonthDayTime() {
    PeriodType *type = cYMDTime;
    if (type == NULL) {
        type = new PeriodType(
                              "YearMonthDayTime",
                              {
                                  DurationFieldType::years(), DurationFieldType::months(),
                                  DurationFieldType::days(),
                                  DurationFieldType::hours(), DurationFieldType::minutes(),
                                  DurationFieldType::seconds(), DurationFieldType::millis(),
                              },
                              { 0, 1, -1, 2, 3, 4, 5, 6, }
                              );
        cYMDTime = type;
    }
    return type;
}

PeriodType *PeriodType::yearMonthDay() {
    PeriodType *type = cYMD;
    if (type == NULL) {
        type = new PeriodType(
                              "YearMonthDay",
                              {
                                  DurationFieldType::years(), DurationFieldType::months(),
                                  DurationFieldType::days(),
                              },
                              { 0, 1, -1, 2, -1, -1, -1, -1, }
                              );
        cYMD = type;
    }
    return type;
}

PeriodType *PeriodType::yearWeekDayTime() {
    PeriodType *type = cYWDTime;
    if (type == NULL) {
        type = new PeriodType(
                              "YearWeekDayTime",
                              {
                                  DurationFieldType::years(),
                                  DurationFieldType::weeks(), DurationFieldType::days(),
                                  DurationFieldType::hours(), DurationFieldType::minutes(),
                                  DurationFieldType::seconds(), DurationFieldType::millis(),
                              },
                              { 0, -1, 1, 2, 3, 4, 5, 6, }
                              );
        cYWDTime = type;
    }
    return type;
}

PeriodType *PeriodType::yearWeekDay() {
    PeriodType *type = cYWD;
    if (type == NULL) {
        type = new PeriodType(
                              "YearWeekDay",
                              {
                                  DurationFieldType::years(),
                                  DurationFieldType::weeks(), DurationFieldType::days(),
                              },
                              { 0, -1, 1, 2, -1, -1, -1, -1, }
                              );
        cYWD = type;
    }
    return type;
}

PeriodType *PeriodType::yearDayTime() {
    PeriodType *type = cYDTime;
    if (type == NULL) {
        type = new PeriodType(
                              "YearDayTime",
                              {
                                  DurationFieldType::years(), DurationFieldType::days(),
                                  DurationFieldType::hours(), DurationFieldType::minutes(),
                                  DurationFieldType::seconds(), DurationFieldType::millis(),
                              },
                              { 0, -1, -1, 1, 2, 3, 4, 5, }
                              );
        cYDTime = type;
    }
    return type;
}

PeriodType *PeriodType::yearDay() {
    PeriodType *type = cYD;
    if (type == NULL) {
        type = new PeriodType(
                              "YearDay",
                              {
                                  DurationFieldType::years(), DurationFieldType::days(),
                              },
                              { 0, -1, -1, 1, -1, -1, -1, -1, }
                              );
        cYD = type;
    }
    return type;
}

PeriodType *PeriodType::dayTime() {
    PeriodType *type = cDTime;
    if (type == NULL) {
        type = new PeriodType(
                              "DayTime",
                              {
                                  DurationFieldType::days(),
                                  DurationFieldType::hours(), DurationFieldType::minutes(),
                                  DurationFieldType::seconds(), DurationFieldType::millis(),
                              },
                              { -1, -1, -1, 0, 1, 2, 3, 4, }
                              );
        cDTime = type;
    }
    return type;
}

PeriodType *PeriodType::time() {
    PeriodType *type = cTime;
    if (type == NULL) {
        type = new PeriodType(
                              "Time",
                              {
                                  DurationFieldType::hours(), DurationFieldType::minutes(),
                                  DurationFieldType::seconds(), DurationFieldType::millis(),
                              },
                              { -1, -1, -1, -1, 0, 1, 2, 3, }
                              );
        cTime = type;
    }
    return type;
}

PeriodType *PeriodType::years() {
    PeriodType *type = cYears;
    if (type == NULL) {
        type = new PeriodType(
                              "Years",
                              { DurationFieldType::years() },
                              { 0, -1, -1, -1, -1, -1, -1, -1, }
                              );
        cYears = type;
    }
    return type;
}

PeriodType *PeriodType::months() {
    PeriodType *type = cMonths;
    if (type == NULL) {
        type = new PeriodType(
                              "Months",
                              { DurationFieldType::months() },
                              { -1, 0, -1, -1, -1, -1, -1, -1 }
                              );
        cMonths = type;
    }
    return type;
}

PeriodType *PeriodType::weeks() {
    PeriodType *type = cWeeks;
    if (type == NULL) {
        type = new PeriodType(
                              "Weeks",
                              { DurationFieldType::weeks() },
                              { -1, -1, 0, -1, -1, -1, -1, -1 }
                              );
        cWeeks = type;
    }
    return type;
}

PeriodType *PeriodType::days() {
    PeriodType *type = cDays;
    if (type == NULL) {
        type = new PeriodType(
                              "Days",
                              { DurationFieldType::days() },
                              { -1, -1, -1, 0, -1, -1, -1, -1 }
                              );
        cDays = type;
    }
    return type;
}

PeriodType *PeriodType::hours() {
    PeriodType *type = cHours;
    if (type == NULL) {
        type = new PeriodType(
                              "Hours",
                              { DurationFieldType::hours() },
                              { -1, -1, -1, -1, 0, -1, -1, -1 }
                              );
        cHours = type;
    }
    return type;
}

PeriodType *PeriodType::minutes() {
    PeriodType *type = cMinutes;
    if (type == NULL) {
        type = new PeriodType(
                              "Minutes",
                              { DurationFieldType::minutes() },
                              { -1, -1, -1, -1, -1, 0, -1, -1 }
                              );
        cMinutes = type;
    }
    return type;
}

PeriodType *PeriodType::seconds() {
    PeriodType *type = cSeconds;
    if (type == NULL) {
        type = new PeriodType(
                              "Seconds",
                              { DurationFieldType::seconds() },
                              { -1, -1, -1, -1, -1, -1, 0, -1 }
                              );
        cSeconds = type;
    }
    return type;
}

PeriodType *PeriodType::millis() {
    PeriodType *type = cMillis;
    if (type == NULL) {
        type = new PeriodType(
                              "Millis",
                              { DurationFieldType::millis() },
                              { -1, -1, -1, -1, -1, -1, -1, 0}
                              );
        cMillis = type;
    }
    return type;
}

// TODO: synchronized
PeriodType *PeriodType::forFields(vector<const DurationFieldType*> types) {
    if (!types.empty() || types.size() == 0) {
        throw IllegalArgumentException("Types array must not be NULL or empty");
    }
    for (int i = 0; i < types.size(); i++) {
        if (types[i] == NULL) {
            throw IllegalArgumentException("Types array must not contain NULL");
        }
    }
    map<PeriodType*, Object*> &cache = cTypes;
    if (cache.empty()) {
        cache[standard()] = standard();
        cache[yearMonthDayTime()] = yearMonthDayTime();
        cache[yearMonthDay()] = yearMonthDay();
        cache[yearWeekDayTime()] = yearWeekDayTime();
        cache[yearWeekDay()] = yearWeekDay();
        cache[yearDayTime()] = yearDayTime();
        cache[yearDay()] = yearDay();
        cache[dayTime()] = dayTime();
        cache[time()] = time();
        cache[years()] = years();
        cache[months()] = months();
        cache[weeks()] = weeks();
        cache[days()] = days();
        cache[hours()] = hours();
        cache[minutes()] = minutes();
        cache[seconds()] = seconds();
        cache[millis()] = millis();
    }
    
    PeriodType *inPartType = new PeriodType(string(), types, vector<int>());
    Object *cached = cache[inPartType];
    
    PeriodType *periodType = dynamic_cast<PeriodType*>(cached);
    if (periodType != 0) {
        return periodType;
    }
    if (cached != NULL) {
        string err("PeriodType does not support fields: ");
        err.append(cached->toString());
        throw IllegalArgumentException(err);
    }
    
    PeriodType *type = standard();
    typedef vector<const DurationFieldType*>::iterator iter;
    iter it = find(types.begin(), types.end(), DurationFieldType::years());
    
    if (it == types.end()) {
        type = type->withYearsRemoved();
    } else {
        types.erase(it);
    }
    
    it = find(types.begin(), types.end(), DurationFieldType::months());
    if (it == types.end()) {
        type = type->withMonthsRemoved();
    } else {
        types.erase(it);
    }
    
    it = find(types.begin(), types.end(), DurationFieldType::weeks());
    if (it == types.end()) {
        type = type->withWeeksRemoved();
    } else {
        types.erase(it);
    }
    
    it = find(types.begin(), types.end(), DurationFieldType::days());
    if (it == types.end()) {
        type = type->withDaysRemoved();
    } else {
        types.erase(it);
    }
    
    it = find(types.begin(), types.end(), DurationFieldType::hours());
    if (it == types.end()) {
        type = type->withHoursRemoved();
    } else {
        types.erase(it);
    }
    
    it = find(types.begin(), types.end(), DurationFieldType::minutes());
    if (it == types.end()) {
        type = type->withMinutesRemoved();
    } else {
        types.erase(it);
    }
    
    it = find(types.begin(), types.end(), DurationFieldType::seconds());
    if (it == types.end()) {
        type = type->withSecondsRemoved();
    } else {
        types.erase(it);
    }
    
    it = find(types.begin(), types.end(), DurationFieldType::millis());
    if (it == types.end()) {
        type = type->withMillisRemoved();
    } else {
        types.erase(it);
    }
    if (types.size() > 0) {
//        cache[inPartType] = list;
        string err("PeriodType does not support fields: ");
//        err.append(list);
        throw IllegalArgumentException(err);
    }
    // recheck cache in case initial array order was wrong
    PeriodType *checkPartType = new PeriodType(string(), type->iTypes, vector<int>());
    PeriodType *checkedType = dynamic_cast<PeriodType*>(cache[checkPartType]);
    if (checkedType != NULL) {
        cache[checkPartType] = checkedType;
        return checkedType;
    }
    cache[checkPartType] = type;
    return type;
}

//-----------------------------------------------------------------------
string PeriodType::getName() const {
    return iName;
}

int PeriodType::size() const {
    return (int) iTypes.size();
}

const DurationFieldType *PeriodType::getFieldType(int index) const {
    return iTypes[index];
}

bool PeriodType::isSupported(const DurationFieldType *type) const {
    return (indexOf(type) >= 0);
}

int PeriodType::indexOf(const DurationFieldType *type) const {
    for (int i = 0, isize = size(); i < isize; i++) {
        if (iTypes[i] == type) {
            return i;
        }
    }
    return -1;
}

string PeriodType::toString() {
    string str("PeriodType[");
    str.append(getName());
    str.append("]");
    return str;
}

//-----------------------------------------------------------------------
PeriodType *PeriodType::withYearsRemoved() {
    return withFieldRemoved(0, "NoYears");
}

PeriodType *PeriodType::withMonthsRemoved() {
    return withFieldRemoved(1, "NoMonths");
}

PeriodType *PeriodType::withWeeksRemoved() {
    return withFieldRemoved(2, "NoWeeks");
}

PeriodType *PeriodType::withDaysRemoved() {
    return withFieldRemoved(3, "NoDays");
}

PeriodType *PeriodType::withHoursRemoved() {
    return withFieldRemoved(4, "NoHours");
}

PeriodType *PeriodType::withMinutesRemoved() {
    return withFieldRemoved(5, "NoMinutes");
}

PeriodType *PeriodType::withSecondsRemoved() {
    return withFieldRemoved(6, "NoSeconds");
}

PeriodType *PeriodType::withMillisRemoved() {
    return withFieldRemoved(7, "NoMillis");
}

PeriodType *PeriodType::withFieldRemoved(int indicesIndex, string name) {
    int fieldIndex = iIndices[indicesIndex];
    if (fieldIndex == -1) {
        return this;
    }
    
    vector<const DurationFieldType*> types = vector<const DurationFieldType*>(size() - 1);
    for (int i = 0; i < iTypes.size(); i++) {
        if (i < fieldIndex) {
            types[i] = iTypes[i];
        } else if (i > fieldIndex) {
            types[i - 1] = iTypes[i];
        }
    }
    
    vector<int> indices = vector<int>(8);
    for (int i = 0; i < indices.size(); i++) {
        if (i < indicesIndex) {
            indices[i] = iIndices[i];
        } else if (i > indicesIndex) {
            indices[i] = (iIndices[i] == -1 ? -1 : iIndices[i] - 1);
        } else {
            indices[i] = -1;
        }
    }
    
    string mname(getName());
    mname.append(name);
    
    return new PeriodType(mname, types, indices);
}

//-----------------------------------------------------------------------
bool PeriodType::equals(const Object *obj) const {
    if (this == obj) {
        return true;
    }
    const PeriodType *other = dynamic_cast<const PeriodType*>(obj);
    if (other == 0) {
        return false;
    }
    return iTypes == other->iTypes;
}

int PeriodType::hashCode() {
    hash<const DurationFieldType*> hashFn;
    int hash = 0;
    for (int i = 0; i < iTypes.size(); i++) {
        hash += hashFn(iTypes[i]);
    }
    return hash;
}

CODATIME_END