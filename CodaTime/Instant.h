//
//  Instant.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__Instant__
#define __CodaTime__Instant__

#include "CodaTimeMacros.h"

#include "AbstractInstant.h"
#include "Chronology.h"
#include "DateTime.h"
#include "format/DateTimeFormatter.h"
#include "ReadableInstant.h"
#include "ReadableDuration.h"
#include "ReadableDuration.h"

#include <string>

using namespace std;

CODATIME_BEGIN

/**
 * Instant is the standard implementation of a fully immutable instant in time.
 * <p>
 * <code>Instant</code> is an implementation of {@link ReadableInstant}.
 * As with all instants, it represents an exact point on the time-line,
 * but limited to the precision of milliseconds. An <code>Instant</code>
 * should be used to represent a point in time irrespective of any other
 * factor, such as chronology or time zone.
 * <p>
 * Internally, the class holds one piece of data, the instant as milliseconds
 * from the Java epoch of 1970-01-01T00:00:00Z.
 * <p>
 * For example, an Instant can be used to compare two <code>DateTime</code>
 * objects irrespective of chronology or time zone.
 * <pre>
 * boolean sameInstant = dt1.toInstant().equals(dt2.toInstant());
 * </pre>
 * Note that the following code will also perform the same check:
 * <pre>
 * boolean sameInstant = dt1.isEqual(dt2);
 * </pre>
 * <p>
 * Instant is thread-safe and immutable.
 *
 * @author Stephen Colebourne
 * @since 1.0
 */
class Instant : public AbstractInstant, public virtual ReadableInstant {
  
private:
    
    /** Serialization lock */
    static const long long serialVersionUID = 3299096530934209741L;
    
    /** The millis from 1970-01-01T00:00:00Z */
    int64_t iMillis;
    
public:
    
    Instant();
    Instant(int64_t instant);
    Instant(void *instant);
    
    static Instant *now();
    static Instant *parse(string str);
    static Instant *parse(string str, DateTimeFormatter *formatter);
    Instant *toInstant();
    Instant *withMillis(int64_t newMillis);
    Instant *withDurationAdded(int64_t durationToAdd, int scalar);
    Instant *withDurationAdded(ReadableDuration *durationToAdd, int scalar);
    Instant *plus(int64_t duration);
    Instant *plus(ReadableDuration *duration);
    Instant *minus(int64_t duration);
    Instant *minus(ReadableDuration *duration);
    int64_t getMillis() const;
    Chronology *getChronology() const;
    DateTime *toDateTime();
    MutableDateTime *toMutableDateTime();
};

CODATIME_END

#endif /* defined(__CodaTime__Instant__) */
