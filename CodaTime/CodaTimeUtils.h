//
//  CodaUtils.h
//  CodaTime
//
//  Created by Saul Howard on 12/19/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_CodaUtils_h
#define CodaTime_CodaUtils_h

#include "CodaTimeMacros.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class CodaTimeUtils {

public:
    
    struct InsensitiveCompare {
        bool operator() (const std::string& a, const std::string& b) const {
            return iequals(a, b);
        }
        
        bool iequals(const string& a, const string& b) const
        {
            size_t sz = a.size();
            if (b.size() != sz)
                return false;
            for (unsigned int i = 0; i < sz; ++i)
                if (tolower(a[i]) != tolower(b[i]))
                    return false;
            return true;
        }
    };
    
};

CODATIME_END

#endif
