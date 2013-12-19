//
//  Locale.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_Locale_h
#define CodaTime_Locale_h

#include "CodaTimeMacros.h"

#include "Object.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Locale : public virtual Object {
    
public:
    
    static Locale *getDefault() {
        return NULL;
    }
    
    string getLanguage() {
        return string("en");
    }
};

CODATIME_END

#endif
