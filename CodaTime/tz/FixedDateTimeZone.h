//
//  FixedDateTimeZone.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__FixedDateTimeZone__
#define __CodaTime__FixedDateTimeZone__

#include "CodaTimeMacros.h"

#include "Object.h"

#include <string>
#include <functional>

using namespace std;

CODATIME_BEGIN

class DateTimeZone;

/**
 * Basic DateTimeZone implementation that has a fixed name key and offsets.
 * <p>
 * FixedDateTimeZone is thread-safe and immutable.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class FixedDateTimeZone : public DateTimeZone {
    
private:
    
    static const long long serialVersionUID = -3513011772763289092L;
    
    const string iNameKey;
    const int iWallOffset;
    const int iStandardOffset;
    
public:
    
    FixedDateTimeZone(string id, string nameKey,
                             int wallOffset, int standardOffset) : DateTimeZone(id),
    iNameKey(nameKey), iWallOffset(wallOffset), iStandardOffset(standardOffset) {
    }
    
    string getNameKey(int64_t instant) {
        return iNameKey;
    }
    
    int getOffset(int64_t instant) {
        return iWallOffset;
    }
    
    int getStandardOffset(int64_t instant) {
        return iStandardOffset;
    }
    
    int getOffsetFromLocal(int64_t instantLocal) {
        return iWallOffset;
    }
    
    bool isFixed() {
        return true;
    }
    
    int64_t nextTransition(int64_t instant) {
        return instant;
    }
    
    int64_t previousTransition(int64_t instant) {
        return instant;
    }
    
    /**
     * Override to return the correct timzone instance.
     * @since 1.5
     */
//    java.util.TimeZone toTimeZone() {
//        string id = getID();
//        if (id.length() == 6 && (id.startsWith("+") || id.startsWith("-"))) {
//            // standard format offset [+-]hh:mm
//            // our ID is without any prefix, so we need to add the GMT back
//            return java.util.TimeZone.getTimeZone("GMT" + getID());
//        }
//        // unusual offset, so setup a SimpleTimeZone as best we can
//        return new java.util.SimpleTimeZone(iWallOffset, getID());
//    }
    
    bool equals(const Object *obj) const {
//        if (this == obj) {
//            return true;
//        }
        
        const FixedDateTimeZone *other = dynamic_cast<const FixedDateTimeZone*>(obj);
        
        if (other != 0) {
            return
            (getID().compare(other->getID()) == 0) &&
            iStandardOffset == other->iStandardOffset &&
            iWallOffset == other->iWallOffset;
        }
        return false;
    }
    
    int hashCode() {
        hash<string> hash;
        return hash(getID()) + 37 * iStandardOffset + 31 * iWallOffset;
    }
    
};

CODATIME_END

#endif /* defined(__CodaTime__FixedDateTimeZone__) */
