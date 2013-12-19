//
//  BaseInterval.h
//  CodaTime
//
//  Created by Saul Howard on 12/14/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BaseInterval_h
#define CodaTime_BaseInterval_h

#include "CodaTimeMacros.h"

#include "base/AbstractInterval.h"
#include "ReadableInterval.h"

CODATIME_BEGIN

class ReadableInstant;
class ReadableDuration;
class ReadablePeriod;

/**
 * BaseInterval is an abstract implementation of ReadableInterval that stores
 * data in two <code>uint64_t</code> millisecond fields.
 * <p>
 * This class should generally not be used directly by API users.
 * The {@link ReadableInterval} interface should be used when different
 * kinds of interval objects are to be referenced.
 * <p>
 * BaseInterval subclasses may be mutable and not thread-safe.
 *
 * @author Brian S O'Neill
 * @author Sean Geoghegan
 * @author Stephen Colebourne
 * @since 1.0
 */
class BaseInterval : public AbstractInterval, public virtual ReadableInterval {
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = 576586928732749278L;
    
    /** The chronology of the interval */
    Chronology *iChronology;
    /** The start of the interval */
    int64_t iStartMillis;
    /** The end of the interval */
    int64_t iEndMillis;
    
protected:
    
    /**
     * Constructs an interval from a start and end instant.
     *
     * @param startInstant  start of this interval, as milliseconds from 1970-01-01T00:00:00Z.
     * @param endInstant  end of this interval, as milliseconds from 1970-01-01T00:00:00Z.
     * @param chrono  the chronology to use, NULL is ISO default
     * @throws IllegalArgumentException if the end is before the start
     */
    BaseInterval(int64_t startInstant, int64_t endInstant, Chronology *chrono);
    
    /**
     * Constructs an interval from a start and end instant.
     *
     * @param start  start of this interval, NULL means now
     * @param end  end of this interval, NULL means now
     * @throws IllegalArgumentException if the end is before the start
     */
    BaseInterval(ReadableInstant *start, ReadableInstant *end);
    
    /**
     * Constructs an interval from a start instant and a duration.
     *
     * @param start  start of this interval, NULL means now
     * @param duration  the duration of this interval, NULL means zero length
     * @throws IllegalArgumentException if the end is before the start
     * @throws ArithmeticException if the end instant exceeds the capacity of a uint64_t
     */
    BaseInterval(ReadableInstant *start, ReadableDuration *duration);
    
    /**
     * Constructs an interval from a millisecond duration and an end instant.
     *
     * @param duration  the duration of this interval, NULL means zero length
     * @param end  end of this interval, NULL means now
     * @throws IllegalArgumentException if the end is before the start
     * @throws ArithmeticException if the start instant exceeds the capacity of a uint64_t
     */
    BaseInterval(ReadableDuration *duration, ReadableInstant *end);
    
    /**
     * Constructs an interval from a start instant and a time period.
     * <p>
     * When forming the interval, the chronology from the instant is used
     * if present, otherwise the chronology of the period is used.
     *
     * @param start  start of this interval, NULL means now
     * @param period  the period of this interval, NULL means zero length
     * @throws IllegalArgumentException if the end is before the start
     * @throws ArithmeticException if the end instant exceeds the capacity of a uint64_t
     */
    BaseInterval(ReadableInstant *start, ReadablePeriod *period);
    
    /**
     * Constructs an interval from a time period and an end instant.
     * <p>
     * When forming the interval, the chronology from the instant is used
     * if present, otherwise the chronology of the period is used.
     *
     * @param period  the period of this interval, NULL means zero length
     * @param end  end of this interval, NULL means now
     * @throws IllegalArgumentException if the end is before the start
     * @throws ArithmeticException if the start instant exceeds the capacity of a uint64_t
     */
    BaseInterval(ReadablePeriod *period, ReadableInstant *end);
    
    /**
     * Constructs a time interval converting or copying from another object
     * that describes an interval.
     *
     * @param interval  the time interval to copy
     * @param chrono  the chronology to use, NULL means let converter decide
     * @throws IllegalArgumentException if the interval is invalid
     */
    BaseInterval(Object *interval, Chronology *chrono);
    
    //-----------------------------------------------------------------------
    /**
     * Sets this interval from two millisecond instants and a chronology.
     *
     * @param startInstant  the start of the time interval
     * @param endInstant  the start of the time interval
     * @param chrono  the chronology, not NULL
     * @throws IllegalArgumentException if the end is before the start
     */
    void setInterval(int64_t startInstant, int64_t endInstant, Chronology *chrono);
    
public:
    
    //-----------------------------------------------------------------------
    /**
     * Gets the chronology of this interval.
     *
     * @return the chronology
     */
    Chronology *getChronology() const {
        return iChronology;
    }
    
    /**
     * Gets the start of this time interval which is inclusive.
     *
     * @return the start of the time interval,
     *  millisecond instant from 1970-01-01T00:00:00Z
     */
    int64_t getStartMillis() const {
        return iStartMillis;
    }
    
    /**
     * Gets the end of this time interval which is exclusive.
     *
     * @return the end of the time interval,
     *  millisecond instant from 1970-01-01T00:00:00Z
     */
    int64_t getEndMillis() const {
        return iEndMillis;
    }
    
};

CODATIME_END

#endif