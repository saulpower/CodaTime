//
//  Provider.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_Provider_h
#define CodaTime_Provider_h

#include "CodaTimeMacros.h"

#include <string>
#include <set>

using namespace std;

CODATIME_BEGIN

class DateTimeZone;

/**
 * Service provider factory for time zones.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class Provider {
    
public:
    
    /**
     * Retrieves a DateTimeZone for the given id. All providers must at
     * least support id "UTC".
     *
     * @return null if not found
     */
    virtual DateTimeZone *getZone(string id) = 0;
    
    /**
     * Returns an unmodifiable set of ids. All providers must at least
     * support id "UTC".
     */
    virtual set<string> getAvailableIDs() = 0;
    
};

CODATIME_END

#endif
