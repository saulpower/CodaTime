//
//  ISOChronology.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__ISOChronology__
#define __CodaTime__ISOChronology__

#include "CodaTimeMacros.h"

#include "chrono/AssembledChronology.h"
#include "chrono/GregorianChronology.h"
#include "DateTimeZone.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

CODATIME_BEGIN

class ISOChronology : public AssembledChronology {
    
private:
    
    /** Serialization lock */
    static const long long serialVersionUID = -6212696554273812441L;
    
    /** Singleton instance of a UTC ISOChronology */
    static ISOChronology *INSTANCE_UTC;
    
    static const int FAST_CACHE_SIZE = 64;
    
    /** Fast cache of zone to chronology */
    static vector<ISOChronology*> cFastCache;
    
    /** Cache of zone to chronology */
    static map<DateTimeZone*, ISOChronology*> cCache;
    
    struct StaticBlock {
        StaticBlock() {
            cFastCache.resize(FAST_CACHE_SIZE);
            INSTANCE_UTC = new ISOChronology(GregorianChronology::getInstanceUTC());
            cCache[DateTimeZone::UTC] = INSTANCE_UTC;
        }
    };
    
    static StaticBlock staticBlock;
    
    /**
     * Restricted constructor
     */
    ISOChronology(Chronology *base) : AssembledChronology(base, NULL) {
    }
    
public:
    
    static ISOChronology *getInstanceUTC();
    static ISOChronology *getInstance();
    static ISOChronology *getInstance(DateTimeZone *zone);
    
    /**
     * Gets the Chronology in the UTC time zone.
     *
     * @return the chronology in UTC
     */
    Chronology *withUTC() {
        return INSTANCE_UTC;
    }
    
    Chronology *withZone(DateTimeZone *zone);
    string toString();
    bool equals(const Object *obj) const;
    int hashCode();
    
protected:
    
    void assemble(Fields *fields);
};

CODATIME_END

#endif /* defined(__CodaTime__ISOChronology__) */
