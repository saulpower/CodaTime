//
//  Comparable.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_Comparable_h
#define CodaTime_Comparable_h

#include "CodaTimeMacros.h"

CODATIME_BEGIN

template<class T>
class Comparable {
public:
    virtual int compareTo(const T *t) const = 0;
};

CODATIME_END

#endif
