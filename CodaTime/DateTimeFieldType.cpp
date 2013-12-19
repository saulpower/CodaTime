//
//  DateTimeFieldType.c
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "DateTimeFieldType.h"

#include "DateTimeField.h"
#include "DurationFieldType.h"
#include "DateTimeUtils.h"
#include "Chronology.h"

CODATIME_BEGIN

const DateTimeFieldType *DateTimeFieldType::ERA_TYPE = new StandardDateTimeFieldType("era", DateTimeFieldType::ERA, DurationFieldType::eras(), NULL);
const DateTimeFieldType *DateTimeFieldType::YEAR_OF_ERA_TYPE = new StandardDateTimeFieldType("yearOfEra", DateTimeFieldType::YEAR_OF_ERA, DurationFieldType::years(), DurationFieldType::eras());
const DateTimeFieldType *DateTimeFieldType::CENTURY_OF_ERA_TYPE = new StandardDateTimeFieldType("centuryOfEra", DateTimeFieldType::CENTURY_OF_ERA, DurationFieldType::centuries(), DurationFieldType::eras());
const DateTimeFieldType *DateTimeFieldType::YEAR_OF_CENTURY_TYPE = new StandardDateTimeFieldType("yearOfCentury", DateTimeFieldType::YEAR_OF_CENTURY, DurationFieldType::years(), DurationFieldType::centuries());
const DateTimeFieldType *DateTimeFieldType::YEAR_TYPE = new StandardDateTimeFieldType("year", DateTimeFieldType::YEAR, DurationFieldType::years(), NULL);
const DateTimeFieldType *DateTimeFieldType::DAY_OF_YEAR_TYPE = new StandardDateTimeFieldType("dayOfYear", DateTimeFieldType::DAY_OF_YEAR, DurationFieldType::days(), DurationFieldType::years());
const DateTimeFieldType *DateTimeFieldType::MONTH_OF_YEAR_TYPE = new StandardDateTimeFieldType("monthOfYear", DateTimeFieldType::MONTH_OF_YEAR, DurationFieldType::months(), DurationFieldType::years());
const DateTimeFieldType *DateTimeFieldType::DAY_OF_MONTH_TYPE = new StandardDateTimeFieldType("dayOfMonth", DateTimeFieldType::DAY_OF_MONTH, DurationFieldType::days(), DurationFieldType::months());
const DateTimeFieldType *DateTimeFieldType::WEEKYEAR_OF_CENTURY_TYPE = new StandardDateTimeFieldType("weekyearOfCentury", DateTimeFieldType::WEEKYEAR_OF_CENTURY, DurationFieldType::weekyears(), DurationFieldType::centuries());
const DateTimeFieldType *DateTimeFieldType::WEEKYEAR_TYPE = new StandardDateTimeFieldType("weekyear", DateTimeFieldType::WEEKYEAR, DurationFieldType::weekyears(), NULL);
const DateTimeFieldType *DateTimeFieldType::WEEK_OF_WEEKYEAR_TYPE = new StandardDateTimeFieldType("weekOfWeekyear", DateTimeFieldType::WEEK_OF_WEEKYEAR, DurationFieldType::weeks(), DurationFieldType::weekyears());
const DateTimeFieldType *DateTimeFieldType::DAY_OF_WEEK_TYPE = new StandardDateTimeFieldType("dayOfWeek", DateTimeFieldType::DAY_OF_WEEK, DurationFieldType::days(), DurationFieldType::weeks());
const DateTimeFieldType *DateTimeFieldType::HALFDAY_OF_DAY_TYPE = new StandardDateTimeFieldType("halfdayOfDay", DateTimeFieldType::HALFDAY_OF_DAY, DurationFieldType::halfdays(), DurationFieldType::days());
const DateTimeFieldType *DateTimeFieldType::HOUR_OF_HALFDAY_TYPE = new StandardDateTimeFieldType("hourOfHalfday", DateTimeFieldType::HOUR_OF_HALFDAY, DurationFieldType::hours(), DurationFieldType::halfdays());
const DateTimeFieldType *DateTimeFieldType::CLOCKHOUR_OF_HALFDAY_TYPE = new StandardDateTimeFieldType("clockhourOfHalfday", DateTimeFieldType::CLOCKHOUR_OF_HALFDAY, DurationFieldType::hours(), DurationFieldType::halfdays());
const DateTimeFieldType *DateTimeFieldType::CLOCKHOUR_OF_DAY_TYPE = new StandardDateTimeFieldType("clockhourOfDay", DateTimeFieldType::CLOCKHOUR_OF_DAY, DurationFieldType::hours(), DurationFieldType::days());
const DateTimeFieldType *DateTimeFieldType::HOUR_OF_DAY_TYPE = new StandardDateTimeFieldType("hourOfDay", DateTimeFieldType::HOUR_OF_DAY, DurationFieldType::hours(), DurationFieldType::days());
const DateTimeFieldType *DateTimeFieldType::MINUTE_OF_DAY_TYPE = new StandardDateTimeFieldType("minuteOfDay", DateTimeFieldType::MINUTE_OF_DAY, DurationFieldType::minutes(), DurationFieldType::days());
const DateTimeFieldType *DateTimeFieldType::MINUTE_OF_HOUR_TYPE = new StandardDateTimeFieldType("minuteOfHour", DateTimeFieldType::MINUTE_OF_HOUR, DurationFieldType::minutes(), DurationFieldType::hours());
const DateTimeFieldType *DateTimeFieldType::SECOND_OF_DAY_TYPE = new StandardDateTimeFieldType("secondOfDay", DateTimeFieldType::SECOND_OF_DAY, DurationFieldType::seconds(), DurationFieldType::days());
const DateTimeFieldType *DateTimeFieldType::SECOND_OF_MINUTE_TYPE = new StandardDateTimeFieldType("secondOfMinute", DateTimeFieldType::SECOND_OF_MINUTE, DurationFieldType::seconds(), DurationFieldType::minutes());
const DateTimeFieldType *DateTimeFieldType::MILLIS_OF_DAY_TYPE = new StandardDateTimeFieldType("millisOfDay", DateTimeFieldType::MILLIS_OF_DAY, DurationFieldType::millis(), DurationFieldType::days());
const DateTimeFieldType *DateTimeFieldType::MILLIS_OF_SECOND_TYPE = new StandardDateTimeFieldType("millisOfSecond", DateTimeFieldType::MILLIS_OF_SECOND, DurationFieldType::millis(), DurationFieldType::seconds());

bool DateTimeFieldType::isSupported(Chronology *chronology) {
    return getField(chronology)->isSupported();
}

/**
 * Get a suitable debug string.
 *
 * @return debug string
 */
string DateTimeFieldType::toString() const {
    return getName();
}

StandardDateTimeFieldType::StandardDateTimeFieldType(string name, unsigned char ordinal,
                          const DurationFieldType *unitType, const DurationFieldType *rangeType) : DateTimeFieldType(name) {
    iOrdinal = ordinal;
    iUnitType = unitType;
    iRangeType = rangeType;
}

/** @inheritdoc */
const DurationFieldType *StandardDateTimeFieldType::getDurationType() const {
    return iUnitType;
}

/** @inheritdoc */
const DurationFieldType *StandardDateTimeFieldType::getRangeDurationType() const {
    return iRangeType;
}

/** @inheritdoc */
bool StandardDateTimeFieldType::equals(const Object *obj) const {
    if (this == obj) {
        return true;
    }
    // TODO: implement instance of
    //            if (obj instanceof StandardDateTimeFieldType) {
    //                return iOrdinal == ((StandardDateTimeFieldType) obj).iOrdinal;
    //            }
    return false;
}

/** @inheritdoc */
int StandardDateTimeFieldType::hashCode() {
    return (1 << iOrdinal);
}

/** @inheritdoc */
const DateTimeField *StandardDateTimeFieldType::getField(Chronology *chronology) const {
    chronology = DateTimeUtils::getChronology(chronology);
    
    switch (iOrdinal) {
        case ERA:
            return chronology->era();
        case YEAR_OF_ERA:
            return chronology->yearOfEra();
        case CENTURY_OF_ERA:
            return chronology->centuryOfEra();
        case YEAR_OF_CENTURY:
            return chronology->yearOfCentury();
        case YEAR:
            return chronology->year();
        case DAY_OF_YEAR:
            return chronology->dayOfYear();
        case MONTH_OF_YEAR:
            return chronology->monthOfYear();
        case DAY_OF_MONTH:
            return chronology->dayOfMonth();
        case WEEKYEAR_OF_CENTURY:
            return chronology->weekyearOfCentury();
        case WEEKYEAR:
            return chronology->weekyear();
        case WEEK_OF_WEEKYEAR:
            return chronology->weekOfWeekyear();
        case DAY_OF_WEEK:
            return chronology->dayOfWeek();
        case HALFDAY_OF_DAY:
            return chronology->halfdayOfDay();
        case HOUR_OF_HALFDAY:
            return chronology->hourOfHalfday();
        case CLOCKHOUR_OF_HALFDAY:
            return chronology->clockhourOfHalfday();
        case CLOCKHOUR_OF_DAY:
            return chronology->clockhourOfDay();
        case HOUR_OF_DAY:
            return chronology->hourOfDay();
        case MINUTE_OF_DAY:
            return chronology->minuteOfDay();
        case MINUTE_OF_HOUR:
            return chronology->minuteOfHour();
        case SECOND_OF_DAY:
            return chronology->secondOfDay();
        case SECOND_OF_MINUTE:
            return chronology->secondOfMinute();
        case MILLIS_OF_DAY:
            return chronology->millisOfDay();
        case MILLIS_OF_SECOND:
            return chronology->millisOfSecond();
        default:
            // Shouldn't happen.
            throw InternalError();
    }
}

/**
 * Ensure a singleton is returned.
 *
 * @return the singleton type
 */
const void *StandardDateTimeFieldType::readResolve() {
    switch (iOrdinal) {
        case ERA:
            return ERA_TYPE;
        case YEAR_OF_ERA:
            return YEAR_OF_ERA_TYPE;
        case CENTURY_OF_ERA:
            return CENTURY_OF_ERA_TYPE;
        case YEAR_OF_CENTURY:
            return YEAR_OF_CENTURY_TYPE;
        case YEAR:
            return YEAR_TYPE;
        case DAY_OF_YEAR:
            return DAY_OF_YEAR_TYPE;
        case MONTH_OF_YEAR:
            return MONTH_OF_YEAR_TYPE;
        case DAY_OF_MONTH:
            return DAY_OF_MONTH_TYPE;
        case WEEKYEAR_OF_CENTURY:
            return WEEKYEAR_OF_CENTURY_TYPE;
        case WEEKYEAR:
            return WEEKYEAR_TYPE;
        case WEEK_OF_WEEKYEAR:
            return WEEK_OF_WEEKYEAR_TYPE;
        case DAY_OF_WEEK:
            return DAY_OF_WEEK_TYPE;
        case HALFDAY_OF_DAY:
            return HALFDAY_OF_DAY_TYPE;
        case HOUR_OF_HALFDAY:
            return HOUR_OF_HALFDAY_TYPE;
        case CLOCKHOUR_OF_HALFDAY:
            return CLOCKHOUR_OF_HALFDAY_TYPE;
        case CLOCKHOUR_OF_DAY:
            return CLOCKHOUR_OF_DAY_TYPE;
        case HOUR_OF_DAY:
            return HOUR_OF_DAY_TYPE;
        case MINUTE_OF_DAY:
            return MINUTE_OF_DAY_TYPE;
        case MINUTE_OF_HOUR:
            return MINUTE_OF_HOUR_TYPE;
        case SECOND_OF_DAY:
            return SECOND_OF_DAY_TYPE;
        case SECOND_OF_MINUTE:
            return SECOND_OF_MINUTE_TYPE;
        case MILLIS_OF_DAY:
            return MILLIS_OF_DAY_TYPE;
        case MILLIS_OF_SECOND:
            return MILLIS_OF_SECOND_TYPE;
        default:
            // Shouldn't happen.
            return this;
    }
}

CODATIME_END
