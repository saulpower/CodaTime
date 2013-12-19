//
//  AbstractDateTime.c
//  CodaTime
//
//  Created by Saul Howard on 12/19/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "AbstractDateTime.h"

#include "Chronology.h"
#include "DateTimeField.h"
#include "DateTimeFieldType.h"
#include "Exceptions.h"
#include "format/DateTimeFormat.h"
#include "format/DateTimeFormatter.h"

CODATIME_BEGIN

int AbstractDateTime::get(const DateTimeFieldType *type) const {
    if (type == NULL) {
        throw IllegalArgumentException("The DateTimeFieldType must not be NULL");
    }
    return type->getField(getChronology())->get(getMillis());
}

//-----------------------------------------------------------------------
int AbstractDateTime::getEra() {
    return getChronology()->era()->get(getMillis());
}

int AbstractDateTime::getCenturyOfEra() {
    return getChronology()->centuryOfEra()->get(getMillis());
}

int AbstractDateTime::getYearOfEra() {
    return getChronology()->yearOfEra()->get(getMillis());
}

int AbstractDateTime::getYearOfCentury() {
    return getChronology()->yearOfCentury()->get(getMillis());
}

int AbstractDateTime::getYear() {
    return getChronology()->year()->get(getMillis());
}

int AbstractDateTime::getWeekyear() {
    return getChronology()->weekyear()->get(getMillis());
}

int AbstractDateTime::getMonthOfYear() {
    return getChronology()->monthOfYear()->get(getMillis());
}

int AbstractDateTime::getWeekOfWeekyear() {
    return getChronology()->weekOfWeekyear()->get(getMillis());
}

int AbstractDateTime::getDayOfYear() {
    return getChronology()->dayOfYear()->get(getMillis());
}

int AbstractDateTime::getDayOfMonth() {
    return getChronology()->dayOfMonth()->get(getMillis());
}

int AbstractDateTime::getDayOfWeek() {
    return getChronology()->dayOfWeek()->get(getMillis());
}

//-----------------------------------------------------------------------
int AbstractDateTime::getHourOfDay() {
    return getChronology()->hourOfDay()->get(getMillis());
}

int AbstractDateTime::getMinuteOfDay() {
    return getChronology()->minuteOfDay()->get(getMillis());
}

int AbstractDateTime::getMinuteOfHour() {
    return getChronology()->minuteOfHour()->get(getMillis());
}

int AbstractDateTime::getSecondOfDay() {
    return getChronology()->secondOfDay()->get(getMillis());
}

int AbstractDateTime::getSecondOfMinute() {
    return getChronology()->secondOfMinute()->get(getMillis());
}

int AbstractDateTime::getMillisOfDay() {
    return getChronology()->millisOfDay()->get(getMillis());
}

int AbstractDateTime::getMillisOfSecond() {
    return getChronology()->millisOfSecond()->get(getMillis());
}

string AbstractDateTime::toString(string pattern) {
    if (pattern.empty()) {
        return Object::toString();
    }
    return DateTimeFormat::forPattern(pattern)->print(this);
}

string AbstractDateTime::toString(string pattern, Locale *locale) {
    if (pattern.empty()) {
        return Object::toString();
    }
    return DateTimeFormat::forPattern(pattern)->withLocale(locale)->print(this);
}

CODATIME_END
