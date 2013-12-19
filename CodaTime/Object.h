//
//  Object.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_Object_h
#define CodaTime_Object_h

#include "CodaTimeMacros.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Object {
    
public:
    
    virtual int hashCode() const {
        return 0;
    }
    
    virtual string toString() const {
        return string();
    }
    
    virtual bool equals(const Object *object) const {
        if (this == object) {
            return true;
        }
        if (object == NULL) {
            return false;
        }
        return false;
    }
};

CODATIME_END

#endif
