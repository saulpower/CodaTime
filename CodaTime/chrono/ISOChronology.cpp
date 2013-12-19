//
//  ISOChronology.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "ISOChronology.h"

CODATIME_BEGIN

/**
 * Gets an instance of the ISOChronology.
 * The time zone of the returned instance is UTC.
 *
 * @return a singleton UTC instance of the chronology
 */
ISOChronology *ISOChronology::getInstanceUTC() {
    return INSTANCE_UTC;
}

/**
 * Gets an instance of the ISOChronology in the default time zone->
 *
 * @return a chronology in the default time zone
 */
ISOChronology *ISOChronology::getInstance() {
    return getInstance(DateTimeZone::getDefault());
}

/**
 * Gets an instance of the ISOChronology in the given time zone->
 *
 * @param zone  the time zone to get the chronology in, NULL is default
 * @return a chronology in the specified time zone
 */
ISOChronology *ISOChronology::getInstance(DateTimeZone *zone) {
    if (zone == NULL) {
        zone = DateTimeZone::getDefault();
    }
    int index = 0;//System.identityHashCode(zone) & (FAST_CACHE_SIZE - 1);
    ISOChronology *chrono = cFastCache[index];
    if (chrono != NULL && chrono->getZone() == zone) {
        return chrono;
    }
    // TODO: synchronized
//    synchronized (cCache) {
        chrono = cCache[zone];
        if (chrono == NULL) {
            chrono = new ISOChronology(ZonedChronology::getInstance(INSTANCE_UTC, zone));
            cCache[zone] = chrono;
        }
//    }
    cFastCache[index] = chrono;
    return chrono;
}

// Conversion
//-----------------------------------------------------------------------

/**
 * Gets the Chronology in a specific time zone->
 *
 * @param zone  the zone to get the chronology in, NULL is default
 * @return the chronology
 */
Chronology *ISOChronology::withZone(DateTimeZone *zone) {
    if (zone == NULL) {
        zone = DateTimeZone::getDefault();
    }
    if (zone == getZone()) {
        return this;
    }
    return getInstance(zone);
}

// Output
//-----------------------------------------------------------------------
/**
 * Gets a debugging toString.
 *
 * @return a debugging string
 */
string ISOChronology::toString() {
    string str("ISOChronology");
    DateTimeZone *zone = getZone();
    if (zone != NULL) {
        str.append(1, '[');
        str.append(zone->getID());
        str.append(1, ']');
    }
    return str;
}

void ISOChronology::assemble(Fields *fields) {
    if (getBase()->getZone() == DateTimeZone::UTC) {
        // Use zero based century and year of century.
        fields->centuryOfEra = new DividedDateTimeField(ISOYearOfEraDateTimeField::INSTANCE, DateTimeFieldType::centuryOfEra(), 100);
        fields->centuries = fields->centuryOfEra->getDurationField();
        
        fields->yearOfCentury = new RemainderDateTimeField((DividedDateTimeField*) fields->centuryOfEra, DateTimeFieldType::yearOfCentury());
        fields->weekyearOfCentury = new RemainderDateTimeField((DividedDateTimeField*) fields->centuryOfEra, fields->weekyears, DateTimeFieldType::weekyearOfCentury());
    }
}

//-----------------------------------------------------------------------
/**
 * Checks if this chronology instance equals another.
 *
 * @param obj  the object to compare to
 * @return true if equal
 * @since 1.6
 */
bool ISOChronology::equals(const Object *obj) const {
    if (this == obj) {
        return true;
    }
    const ISOChronology *chrono = dynamic_cast<const ISOChronology*>(obj);
    if (chrono != 0) {
        return getZone()->equals(chrono->getZone());
    }
    return false;
}

/**
 * A suitable hash code for the chronology.
 *
 * @return the hash code
 * @since 1.6
 */
int ISOChronology::hashCode() {
    string iso("ISO");
    hash<string> hash;
    return hash(iso) * 11 + getZone()->hashCode();
}

//-----------------------------------------------------------------------
/**
 * Serialize ISOChronology instances using a small stub. This reduces the
 * serialized size, and deserialized instances come from the cache.
 */
//private void writeReplace() {
//    return new Stub(getZone());
//}
//
//private static final class Stub implements Serializable {
//    private static final long serialVersionUID = -6212696554273812441L;
//    
//    private transient DateTimeZone iZone;
//    
//    Stub(DateTimeZone zone) {
//        iZone = zone;
//    }
//    
//    private Object readResolve() {
//        return ISOChronology.getInstance(iZone);
//    }
//    
//    private void writeObject(ObjectOutputStream out) throws IOException {
//        out.writeObject(iZone);
//    }
//    
//    private void readObject(ObjectInputStream in)
//    throws IOException, ClassNotFoundException
//    {
//        iZone = (DateTimeZone)in.readObject();
//    }
//}

CODATIME_END