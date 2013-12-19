//
//  AbstractInstant.c
//  CodaTime
//
//  Created by Saul Howard on 12/19/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "AbstractInstant.h"

#include "Chronology.h"
#include "chrono/ISOChronology.h"
#include "DateTime.h"
#include "DateTime.h"
#include "DateTimeField.h"
#include "DateTimeFieldType.h"
#include "DateTimeFormatter.h"
#include "DateTimeUtils.h"
#include "Exceptions.h"
#include "field/FieldUtils.h"
#include "Instant.h"
//#include "format/ISODateTimeFormat.h"

CODATIME_BEGIN

DateTimeZone *AbstractInstant::getZone() {
    return getChronology()->getZone();
}

int AbstractInstant::get(const DateTimeFieldType *type) const {
    if (type == NULL) {
        throw IllegalArgumentException("The DateTimeFieldType *must not be NULL");
    }
    return type->getField(getChronology())->get(getMillis());
}

bool AbstractInstant::isSupported(DateTimeFieldType *type) {
    if (type == NULL) {
        return false;
    }
    return type->getField(getChronology())->isSupported();
}

int AbstractInstant::get(DateTimeField *field) {
    if (field == NULL) {
        throw IllegalArgumentException("The DateTimeField must not be NULL");
    }
    return field->get(getMillis());
}

Instant *AbstractInstant::toInstant() {
    return new Instant(getMillis());
}

/**
 * Get this object as a DateTime in the same zone.
 *
 * @return a DateTime using the same millis
 */
DateTime *AbstractInstant::toDateTime() {
    return new DateTime(getMillis(), getZone());
}

/**
 * Get this object as a DateTime using ISOChronology in the same zone.
 *
 * @return a DateTime using the same millis with ISOChronology
 */
DateTime *AbstractInstant::toDateTimeISO() {
    return new DateTime(getMillis(), ISOChronology::getInstance(getZone()));
}

/**
 * Get this object as a DateTime using the same chronology but a different zone.
 *
 * @param zone time zone to apply, or default if null
 * @return a DateTime using the same millis
 */
DateTime *AbstractInstant::toDateTime(DateTimeZone *zone) {
    Chronology *chrono = DateTimeUtils::getChronology(getChronology());
    chrono = chrono->withZone(zone);
    return new DateTime(getMillis(), chrono);
}

/**
 * Get this object as a DateTime using the given chronology and its zone.
 *
 * @param chronology chronology to apply, or ISOChronology if null
 * @return a DateTime using the same millis
 */
DateTime *AbstractInstant::toDateTime(Chronology *chronology) {
    return new DateTime(getMillis(), chronology);
}

MutableDateTime *AbstractInstant::toMutableDateTime() {
    return new MutableDateTime(getMillis(), getZone());
}

/**
 * Get this object as a MutableDateTime using ISOChronology in the same zone.
 *
 * @return a MutableDateTime using the same millis with ISOChronology
 */
MutableDateTime *AbstractInstant::toMutableDateTimeISO() {
    return new MutableDateTime(getMillis(), ISOChronology::getInstance(getZone()));
}

/**
 * Get this object as a MutableDateTime using the same chronology but a different zone.
 *
 * @param zone time zone to apply, or default if null
 * @return a MutableDateTime using the same millis
 */
MutableDateTime *AbstractInstant::toMutableDateTime(DateTimeZone *zone) {
    Chronology *chrono = DateTimeUtils::getChronology(getChronology());
    chrono = chrono->withZone(zone);
    return new MutableDateTime(getMillis(), chrono);
}

/**
 * Get this object as a MutableDateTime using the given chronology and its zone.
 *
 * @param chronology chronology to apply, or ISOChronology if null
 * @return a MutableDateTime using the same millis
 */
MutableDateTime *AbstractInstant::toMutableDateTime(Chronology *chronology) {
    return new MutableDateTime(getMillis(), chronology);
}

bool AbstractInstant::equals(const Object *readableInstant) const {
    // must be to fulfil ReadableInstant *contract
    //        if (this == readableInstant) {
    //            return true;
    //        }
    const ReadableInstant *otherInstant = dynamic_cast<const ReadableInstant*>(readableInstant);
    if (otherInstant == 0) {
        return false;
    }
    return getMillis() == otherInstant->getMillis() && FieldUtils::equals(getChronology(), otherInstant->getChronology());
}

int AbstractInstant::hashCode() {
    // must be to fulfil ReadableInstant *contract
    return ((int) (getMillis() ^ (getMillis() >> 32))) + (getChronology()->hashCode());
}

int AbstractInstant::compareTo(const ReadableInstant *other) const {
    if (this->equals(other)) {
        return 0;
    }
    
    int64_t otherMillis = other->getMillis();
    int64_t thisMillis = getMillis();
    
    // cannot do (thisMillis - otherMillis) as can overflow
    if (thisMillis == otherMillis) {
        return 0;
    }
    if (thisMillis < otherMillis) {
        return -1;
    } else {
        return 1;
    }
}

//-----------------------------------------------------------------------
bool AbstractInstant::isAfter(int64_t instant) {
    return (getMillis() > instant);
}

bool AbstractInstant::isAfterNow() {
    return isAfter(DateTimeUtils::currentTimeMillis());
}

bool AbstractInstant::isAfter(ReadableInstant *instant) {
    int64_t instantMillis = DateTimeUtils::getInstantMillis(instant);
    return isAfter(instantMillis);
}

//-----------------------------------------------------------------------
bool AbstractInstant::isBefore(int64_t instant) {
    return (getMillis() < instant);
}

bool AbstractInstant::isBeforeNow() {
    return isBefore(DateTimeUtils::currentTimeMillis());
}

bool AbstractInstant::isBefore(ReadableInstant *instant) {
    int64_t instantMillis = DateTimeUtils::getInstantMillis(instant);
    return isBefore(instantMillis);
}

//-----------------------------------------------------------------------
bool AbstractInstant::isEqual(int64_t instant) {
    return (getMillis() == instant);
}

bool AbstractInstant::isEqualNow() {
    return isEqual(DateTimeUtils::currentTimeMillis());
}

bool AbstractInstant::isEqual(ReadableInstant *instant) {
    int64_t instantMillis = DateTimeUtils::getInstantMillis(instant);
    return isEqual(instantMillis);
}

//-----------------------------------------------------------------------
string AbstractInstant::toString() {
    return ISODateTimeFormat::dateTime()->print(this);
}

//-----------------------------------------------------------------------
string AbstractInstant::toString(DateTimeFormatter *formatter) {
    if (formatter == NULL) {
        return toString();
    }
    return formatter->print(this);
}

CODATIME_END
