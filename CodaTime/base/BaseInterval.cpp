//
//  BaseInterval.c
//  CodaTime
//
//  Created by Saul Howard on 12/19/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "BaseInterval.h"

#include "chrono/ISOChronology.h"
#include "Chronology.h"
#include "DateTimeUtils.h"
#include "field/FieldUtils.h"
//#include "ReadWritableInterval.h"

CODATIME_BEGIN

BaseInterval::BaseInterval(int64_t startInstant, int64_t endInstant, Chronology *chrono) {
    iChronology = DateTimeUtils::getChronology(chrono);
    checkInterval(startInstant, endInstant);
    iStartMillis = startInstant;
    iEndMillis = endInstant;
}

/**
 * Constructs an interval from a start and end instant.
 *
 * @param start  start of this interval, NULL means now
 * @param end  end of this interval, NULL means now
 * @throws IllegalArgumentException if the end is before the start
 */
BaseInterval::BaseInterval(ReadableInstant *start, ReadableInstant *end) {
    if (start == NULL && end == NULL) {
        iStartMillis = iEndMillis = DateTimeUtils::currentTimeMillis();
        iChronology = ISOChronology::getInstance();
    } else {
        iChronology = DateTimeUtils::getInstantChronology(start);
        iStartMillis = DateTimeUtils::getInstantMillis(start);
        iEndMillis = DateTimeUtils::getInstantMillis(end);
        checkInterval(iStartMillis, iEndMillis);
    }
}

/**
 * Constructs an interval from a start instant and a duration.
 *
 * @param start  start of this interval, NULL means now
 * @param duration  the duration of this interval, NULL means zero length
 * @throws IllegalArgumentException if the end is before the start
 * @throws ArithmeticException if the end instant exceeds the capacity of a uint64_t
 */
BaseInterval::BaseInterval(ReadableInstant *start, ReadableDuration *duration) {
    iChronology = DateTimeUtils::getInstantChronology(start);
    iStartMillis = DateTimeUtils::getInstantMillis(start);
    int64_t durationMillis = DateTimeUtils::getDurationMillis(duration);
    iEndMillis = FieldUtils::safeAdd(iStartMillis, durationMillis);
    checkInterval(iStartMillis, iEndMillis);
}

/**
 * Constructs an interval from a millisecond duration and an end instant.
 *
 * @param duration  the duration of this interval, NULL means zero length
 * @param end  end of this interval, NULL means now
 * @throws IllegalArgumentException if the end is before the start
 * @throws ArithmeticException if the start instant exceeds the capacity of a uint64_t
 */
BaseInterval::BaseInterval(ReadableDuration *duration, ReadableInstant *end) {
    iChronology = DateTimeUtils::getInstantChronology(end);
    iEndMillis = DateTimeUtils::getInstantMillis(end);
    int64_t durationMillis = DateTimeUtils::getDurationMillis(duration);
    iStartMillis = FieldUtils::safeAdd(iEndMillis, -durationMillis);
    checkInterval(iStartMillis, iEndMillis);
}

/**
 * Constructs an interval from a start instant and a time period.
 * <p>
 * When forming the interval, the chronology from the instant is used
 * if present, otherwise the chronology of the period is used.
 *
 * @param start  start of this interval, NULL means now
 * @param period  the period of this interval, NULL means zero length
 * @throws IllegalArgumentException if the end is before the start
 * @throws ArithmeticException if the end instant exceeds the capacity of a uint64_t
 */
BaseInterval::BaseInterval(ReadableInstant *start, ReadablePeriod *period) {
    Chronology *chrono = DateTimeUtils::getInstantChronology(start);
    iChronology = chrono;
    iStartMillis = DateTimeUtils::getInstantMillis(start);
    if (period == NULL) {
        iEndMillis = iStartMillis;
    } else {
        iEndMillis = chrono->add(period, iStartMillis, 1);
    }
    checkInterval(iStartMillis, iEndMillis);
}

/**
 * Constructs an interval from a time period and an end instant.
 * <p>
 * When forming the interval, the chronology from the instant is used
 * if present, otherwise the chronology of the period is used.
 *
 * @param period  the period of this interval, NULL means zero length
 * @param end  end of this interval, NULL means now
 * @throws IllegalArgumentException if the end is before the start
 * @throws ArithmeticException if the start instant exceeds the capacity of a uint64_t
 */
BaseInterval::BaseInterval(ReadablePeriod *period, ReadableInstant *end) {
    Chronology *chrono = DateTimeUtils::getInstantChronology(end);
    iChronology = chrono;
    iEndMillis = DateTimeUtils::getInstantMillis(end);
    if (period == NULL) {
        iStartMillis = iEndMillis;
    } else {
        iStartMillis = chrono->add(period, iEndMillis, -1);
    }
    checkInterval(iStartMillis, iEndMillis);
}

/**
 * Constructs a time interval converting or copying from another object
 * that describes an interval.
 *
 * @param interval  the time interval to copy
 * @param chrono  the chronology to use, NULL means let converter decide
 * @throws IllegalArgumentException if the interval is invalid
 */
BaseInterval::BaseInterval(Object *interval, Chronology *chrono) {
    IntervalConverter *converter = ConverterManager::getInstance()->getIntervalConverter(interval);
    if (converter->isReadableInterval(interval, chrono)) {
        ReadableInterval *input = dynamic_cast<ReadableInterval*>(interval);
        iChronology = (chrono != NULL ? chrono : input->getChronology());
        iStartMillis = input->getStartMillis();
        iEndMillis = input->getEndMillis();
    } else if (dynamic_cast<ReadWritableInterval*>(this) != 0) {
        converter->setInto(dynamic_cast<ReadWritableInterval*>(this), interval, chrono);
    } else {
        MutableInterval *mi = new MutableInterval();
        converter->setInto(mi, interval, chrono);
        iChronology = mi->getChronology();
        iStartMillis = mi->getStartMillis();
        iEndMillis = mi->getEndMillis();
    }
    checkInterval(iStartMillis, iEndMillis);
}

//-----------------------------------------------------------------------
/**
 * Sets this interval from two millisecond instants and a chronology.
 *
 * @param startInstant  the start of the time interval
 * @param endInstant  the start of the time interval
 * @param chrono  the chronology, not NULL
 * @throws IllegalArgumentException if the end is before the start
 */
void BaseInterval::setInterval(int64_t startInstant, int64_t endInstant, Chronology *chrono) {
    checkInterval(startInstant, endInstant);
    iStartMillis = startInstant;
    iEndMillis = endInstant;
    iChronology = DateTimeUtils::getChronology(chrono);
}

CODATIME_END