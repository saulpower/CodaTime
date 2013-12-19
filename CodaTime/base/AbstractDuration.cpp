//
//  AbstractDuration.c
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "AbstractDuration.h"

#include "Duration.h"
#include "Period.h"
#include "Object.h"
#include "format/FormatUtils.h"

CODATIME_BEGIN

Duration *AbstractDuration::toDuration() {
    return new Duration(getMillis());
}

Period *AbstractDuration::toPeriod() {
    return new Period(getMillis());
}

int AbstractDuration::compareTo(const ReadableDuration *other) const {
    int64_t thisMillis = getMillis();
    int64_t otherMillis = other->getMillis();
    
    // cannot do (thisMillis - otherMillis) as it can overflow
    if (thisMillis < otherMillis) {
        return -1;
    }
    if (thisMillis > otherMillis) {
        return 1;
    }
    return 0;
}

bool AbstractDuration::isEqual(ReadableDuration *duration) {
    if (duration == NULL) {
        duration = Duration::ZERO;
    }
    return compareTo(duration) == 0;
}

bool AbstractDuration::isLongerThan(ReadableDuration *duration) {
    if (duration == NULL) {
        duration = Duration::ZERO;
    }
    return compareTo(duration) > 0;
}

bool AbstractDuration::isShorterThan(ReadableDuration *duration) {
    if (duration == NULL) {
        duration = Duration::ZERO;
    }
    return compareTo(duration) < 0;
}

bool AbstractDuration::equals(const Object *duration) const {
//    if (this == duration) {
//        return true;
//    }
    const ReadableDuration *readable = dynamic_cast<const ReadableDuration*>(duration);
    if (readable == 0) {
        return false;
    }
    return (getMillis() == readable->getMillis());
}

int AbstractDuration::hashCode() {
    int64_t len = getMillis();
    return (int) (len ^ (len >> 32));
}

string AbstractDuration::toString() {
    int64_t millis = getMillis();
    string buf;
    buf.append("PT");
    bool negative = (millis < 0);
    FormatUtils::appendUnpaddedInteger(buf, millis);
    while (buf.length() < (negative ? 7 : 6)) {
        buf.insert(negative ? 3 : 2, "0");
    }
    if ((millis / 1000) * 1000 == millis) {
        buf.resize(buf.length() - 3);
    } else {
        buf.insert(buf.length() - 3, ".");
    }
    buf.append(1, 'S');
    return buf;
}

CODATIME_END