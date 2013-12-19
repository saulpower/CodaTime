//
//  Exceptions.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_Exceptions_h
#define CodaTime_Exceptions_h

#include "CodaTimeMacros.h"

#include <string>
#include <exception>

using namespace std;

CODATIME_BEGIN

class DateTimeFieldType;

struct IllegalArgumentException : public exception {
    
    string s;
    
    IllegalArgumentException(string ss) : s(ss) {}
    IllegalArgumentException() throw () {}
    
    const char* what() const throw() { return s.c_str(); }
};

struct IllegalStateException : public IllegalArgumentException {
    IllegalStateException(string ss) : IllegalArgumentException(ss) {}
};

struct InternalError : public IllegalArgumentException {
    InternalError() : IllegalArgumentException("") {}
};

struct ArithmeticException : public IllegalArgumentException {
    ArithmeticException(string ss) : IllegalArgumentException(ss) {}
};


struct ClassCastException : public IllegalArgumentException {
    ClassCastException(string ss) : IllegalArgumentException(ss) {}
};

struct UnsupportedOperationException : public IllegalArgumentException {
    UnsupportedOperationException(string ss) : IllegalArgumentException(ss) {}
};

struct NullPointerException : public IllegalArgumentException {
    NullPointerException(string ss) : IllegalArgumentException(ss) {}
};

struct IndexOutOfBoundsException : public IllegalArgumentException {
    IndexOutOfBoundsException(string ss) : IllegalArgumentException(ss) {}
};

struct IllegalFieldValueException : public IllegalArgumentException {
    
    IllegalFieldValueException(const DateTimeFieldType *type, string ss) : IllegalArgumentException(ss) {}
    
    IllegalFieldValueException(string type, string value, string lowerBound, string upperBound) : IllegalArgumentException(createMessage(type, value, lowerBound, upperBound, string())) {}
    
    string createMessage(string type, string value, string lowerBound, string upperBound, string explain) {
        
        string buf("Value");
        buf.append(value);
        buf.append(" for ");
        buf.append(type);
        buf.append(" ");
        
        if (lowerBound.empty()) {
            if (upperBound.empty()) {
                buf.append("is not supported");
            } else {
                buf.append("must not be larger than ");
                buf.append(upperBound);
            }
        } else if (upperBound.empty()) {
            buf.append("must not be smaller than ");
            buf.append(lowerBound);
        } else {
            buf.append("must be in the range [");
            buf.append(lowerBound);
            buf.append(",");
            buf.append(upperBound);
            buf.append("]");
        }
        if (explain.empty()) {
            buf.append(": ");
            buf.append(explain);
        }
        
        return buf;
    }
};

struct IllegalInstantException : public IllegalArgumentException {
    IllegalInstantException(int64_t time, string ss) : IllegalArgumentException(ss) {}
    IllegalInstantException(string ss) : IllegalArgumentException(ss) {}
};

struct ArrayIndexOutOfBoundsException : public IllegalArgumentException {
    ArrayIndexOutOfBoundsException(string ss) : IllegalArgumentException(ss) {}
};

CODATIME_END

#endif
