//
//  DurationFieldType.c
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "DurationFieldType.h"

#include "DateTimeUtils.h"
#include "DurationField.h"
#include "Exceptions.h"
#include "Chronology.h"

CODATIME_BEGIN

const DurationFieldType *DurationFieldType::ERAS_TYPE = new StandardDurationFieldType("eras", ERAS);
const DurationFieldType *DurationFieldType::CENTURIES_TYPE = new StandardDurationFieldType("centuries", CENTURIES);
/** The weekyears field type. */
const DurationFieldType *DurationFieldType::WEEKYEARS_TYPE = new StandardDurationFieldType("weekyears", WEEKYEARS);
/** The years field type. */
const DurationFieldType *DurationFieldType::YEARS_TYPE = new StandardDurationFieldType("years", YEARS);
/** The months field type. */
const DurationFieldType *DurationFieldType::MONTHS_TYPE = new StandardDurationFieldType("months", MONTHS);
/** The weeks field type. */
const DurationFieldType *DurationFieldType::WEEKS_TYPE = new StandardDurationFieldType("weeks", WEEKS);
/** The days field type. */
const DurationFieldType *DurationFieldType::DAYS_TYPE = new StandardDurationFieldType("days", DAYS);
/** The halfdays field type. */
const DurationFieldType *DurationFieldType::HALFDAYS_TYPE = new StandardDurationFieldType("halfdays", HALFDAYS);
/** The hours field type. */
const DurationFieldType *DurationFieldType::HOURS_TYPE = new StandardDurationFieldType("hours", HOURS);
/** The minutes field type. */
const DurationFieldType *DurationFieldType::MINUTES_TYPE = new StandardDurationFieldType("minutes", MINUTES);
/** The seconds field type. */
const DurationFieldType *DurationFieldType::SECONDS_TYPE = new StandardDurationFieldType("seconds", SECONDS);
/** The millis field type. */
const DurationFieldType *DurationFieldType::MILLIS_TYPE = new StandardDurationFieldType("millis", MILLIS);

bool DurationFieldType::isSupported(Chronology *chronology) {
    return getField(chronology)->isSupported();
}

bool StandardDurationFieldType::equals(const Object *obj) const {
    if (this == obj) {
        return true;
    }
    // TODO: implement instanceof
    //            if (obj instanceof StandardDurationFieldType) {
    //                return iOrdinal == ((StandardDurationFieldType) obj).iOrdinal;
    //            }
    return false;
}

const DurationField *StandardDurationFieldType::getField(Chronology *chronology) const {
    chronology = DateTimeUtils::getChronology(chronology);
    
    switch (iOrdinal) {
        case ERAS:
            return chronology->eras();
        case CENTURIES:
            return chronology->centuries();
        case WEEKYEARS:
            return chronology->weekyears();
        case YEARS:
            return chronology->years();
        case MONTHS:
            return chronology->months();
        case WEEKS:
            return chronology->weeks();
        case DAYS:
            return chronology->days();
        case HALFDAYS:
            return chronology->halfdays();
        case HOURS:
            return chronology->hours();
        case MINUTES:
            return chronology->minutes();
        case SECONDS:
            return chronology->seconds();
        case MILLIS:
            return chronology->millis();
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
const DurationFieldType *StandardDurationFieldType::readResolve() {
    switch (iOrdinal) {
        case ERAS:
            return ERAS_TYPE;
        case CENTURIES:
            return CENTURIES_TYPE;
        case WEEKYEARS:
            return WEEKYEARS_TYPE;
        case YEARS:
            return YEARS_TYPE;
        case MONTHS:
            return MONTHS_TYPE;
        case WEEKS:
            return WEEKS_TYPE;
        case DAYS:
            return DAYS_TYPE;
        case HALFDAYS:
            return HALFDAYS_TYPE;
        case HOURS:
            return HOURS_TYPE;
        case MINUTES:
            return MINUTES_TYPE;
        case SECONDS:
            return SECONDS_TYPE;
        case MILLIS:
            return MILLIS_TYPE;
        default:
            // Shouldn't happen.
            return this;
    }
}

CODATIME_END
