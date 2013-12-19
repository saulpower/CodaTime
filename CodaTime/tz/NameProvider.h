//
//  NameProvider.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_NameProvider_h
#define CodaTime_NameProvider_h

#include "CodaTimeMacros.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Locale;

/**
 * Service provider factory for localized time zone names.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class NameProvider {
    
public:
    
    /**
     * Returns a localized short name, or null if not found.
     *
     * @param locale locale to use for selecting name set
     * @param id time zone id
     * @param nameKey time zone name key
     */
    virtual string getShortName(Locale *locale, string id, string nameKey) = 0;
    
    /**
     * Returns a localized name, or null if not found.
     *
     * @param locale locale to use for selecting name set
     * @param id time zone id
     * @param nameKey time zone name key
     */
    virtual string getName(Locale *locale, string id, string nameKey) = 0;
};

CODATIME_END

#endif
