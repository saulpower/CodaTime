//
//  Abstractinterval->h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_AbstractInterval_h
#define CodaTime_AbstractInterval_h

#include "CodaTimeMacros.h"

#include "Readableinterval.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class PeriodType;

/**
 * AbstractInterval provides the common behaviour for time intervals.
 * <p>
 * This class should generally not be used directly by API users. The
 * {@link ReadableInterval} interface should be used when different
 * kinds of intervals are to be referenced.
 * <p>
 * AbstractInterval subclasses may be mutable and not thread-safe.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class AbstractInterval : public virtual ReadableInterval {
    
protected:
    
    /**
     * Constructor.
     */
    AbstractInterval() {}
    
    //-----------------------------------------------------------------------
    /**
     * Validates an interval->
     *
     * @param start  the start instant in milliseconds
     * @param end  the end instant in milliseconds
     * @throws IllegalArgumentException if the interval is invalid
     */
    void checkInterval(int64_t start, int64_t end);
    
public:
    
    virtual Chronology *getChronology() const = 0;
    
    virtual int64_t getStartMillis() const = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the start of this time interval, which is inclusive, as a DateTime.
     *
     * @return the start of the time interval
     */
    DateTime *getStart();
    
    virtual int64_t getEndMillis() const = 0;
    
    /**
     * Gets the end of this time interval, which is exclusive, as a DateTime.
     *
     * @return the end of the time interval
     */
    DateTime *getEnd();
    
    //-----------------------------------------------------------------------
    /**
     * Does this time interval contain the specified millisecond instant.
     * <p>
     * Non-zero duration intervals are inclusive of the start instant and
     * exclusive of the end. A zero duration interval cannot contain anything.
     *
     * @param millisInstant  the instant to compare to,
     *  millisecond instant from 1970-01-01T00:00:00Z
     * @return true if this time interval contains the millisecond
     */
    bool contains(int64_t millisInstant);
    
    /**
     * Does this time interval contain the current instant.
     * <p>
     * Non-zero duration intervals are inclusive of the start instant and
     * exclusive of the end. A zero duration interval cannot contain anything.
     *
     * @return true if this time interval contains the current instant
     */
    bool containsNow();
    
    /**
     * Does this time interval contain the specified instant.
     * <p>
     * Non-zero duration intervals are inclusive of the start instant and
     * exclusive of the end. A zero duration interval cannot contain anything.
     * <p>
     * For example:
     * <pre>
     * [09:00 to 10:00) contains 08:59  = false (before start)
     * [09:00 to 10:00) contains 09:00  = true
     * [09:00 to 10:00) contains 09:59  = true
     * [09:00 to 10:00) contains 10:00  = false (equals end)
     * [09:00 to 10:00) contains 10:01  = false (after end)
     *
     * [14:00 to 14:00) contains 14:00  = false (zero duration contains nothing)
     * </pre>
     * Passing in a <code>NULL</code> parameter will have the same effect as
     * calling {@link #containsNow()}.
     *
     * @param instant  the instant, NULL means now
     * @return true if this time interval contains the instant
     */
    bool contains(ReadableInstant *instant);
    
    /**
     * Does this time interval contain the specified time interval->
     * <p>
     * Non-zero duration intervals are inclusive of the start instant and
     * exclusive of the end. The other interval is contained if this interval
     * wholly contains, starts, finishes or equals it.
     * A zero duration interval cannot contain anything.
     * <p>
     * When two intervals are compared the result is one of three states:
     * (a) they abut, (b) there is a gap between them, (c) they overlap.
     * The <code>contains</code> method is not related to these states.
     * In particular, a zero duration interval is contained at the start of
     * a larger interval, but does not overlap (it abuts instead).
     * <p>
     * For example:
     * <pre>
     * [09:00 to 10:00) contains [09:00 to 10:00)  = true
     * [09:00 to 10:00) contains [09:00 to 09:30)  = true
     * [09:00 to 10:00) contains [09:30 to 10:00)  = true
     * [09:00 to 10:00) contains [09:15 to 09:45)  = true
     * [09:00 to 10:00) contains [09:00 to 09:00)  = true
     *
     * [09:00 to 10:00) contains [08:59 to 10:00)  = false (otherStart before thisStart)
     * [09:00 to 10:00) contains [09:00 to 10:01)  = false (otherEnd after thisEnd)
     * [09:00 to 10:00) contains [10:00 to 10:00)  = false (otherStart equals thisEnd)
     *
     * [14:00 to 14:00) contains [14:00 to 14:00)  = false (zero duration contains nothing)
     * </pre>
     * Passing in a <code>NULL</code> parameter will have the same effect as
     * calling {@link #containsNow()}.
     *
     * @param interval  the time interval to compare to, NULL means a zero duration interval now
     * @return true if this time interval contains the time interval
     */
    bool contains(ReadableInterval *interval);
    
    /**
     * Does this time interval overlap the specified time interval->
     * <p>
     * Intervals are inclusive of the start instant and exclusive of the end.
     * An interval overlaps another if it shares some common part of the
     * datetime continuum.
     * <p>
     * When two intervals are compared the result is one of three states:
     * (a) they abut, (b) there is a gap between them, (c) they overlap.
     * The abuts state takes precedence over the other two, thus a zero duration
     * interval at the start of a larger interval abuts and does not overlap.
     * <p>
     * For example:
     * <pre>
     * [09:00 to 10:00) overlaps [08:00 to 08:30)  = false (completely before)
     * [09:00 to 10:00) overlaps [08:00 to 09:00)  = false (abuts before)
     * [09:00 to 10:00) overlaps [08:00 to 09:30)  = true
     * [09:00 to 10:00) overlaps [08:00 to 10:00)  = true
     * [09:00 to 10:00) overlaps [08:00 to 11:00)  = true
     *
     * [09:00 to 10:00) overlaps [09:00 to 09:00)  = false (abuts before)
     * [09:00 to 10:00) overlaps [09:00 to 09:30)  = true
     * [09:00 to 10:00) overlaps [09:00 to 10:00)  = true
     * [09:00 to 10:00) overlaps [09:00 to 11:00)  = true
     *
     * [09:00 to 10:00) overlaps [09:30 to 09:30)  = true
     * [09:00 to 10:00) overlaps [09:30 to 10:00)  = true
     * [09:00 to 10:00) overlaps [09:30 to 11:00)  = true
     *
     * [09:00 to 10:00) overlaps [10:00 to 10:00)  = false (abuts after)
     * [09:00 to 10:00) overlaps [10:00 to 11:00)  = false (abuts after)
     *
     * [09:00 to 10:00) overlaps [10:30 to 11:00)  = false (completely after)
     *
     * [14:00 to 14:00) overlaps [14:00 to 14:00)  = false (abuts before and after)
     * [14:00 to 14:00) overlaps [13:00 to 15:00)  = true
     * </pre>
     *
     * @param interval  the time interval to compare to, NULL means a zero length interval now
     * @return true if the time intervals overlap
     */
    bool overlaps(ReadableInterval *interval);
    
    //-----------------------------------------------------------------------
    /**
     * Is this interval equal to the specified interval ignoring the chronology.
     * <p>
     * This compares the underlying instants, ignoring the chronology.
     *
     * @param other  a readable interval to check against
     * @return true if the intervals are equal comparing the start and end millis
     * @since 2.3
     */
    bool isEqual(ReadableInterval *other);
    
    /**
     * Is this time interval before the specified millisecond instant.
     * <p>
     * Intervals are inclusive of the start instant and exclusive of the end.
     *
     * @param millisInstant  the instant to compare to,
     *  millisecond instant from 1970-01-01T00:00:00Z
     * @return true if this time interval is before the instant
     */
    bool isBefore(int64_t millisInstant);
    
    /**
     * Is this time interval before the current instant.
     * <p>
     * Intervals are inclusive of the start instant and exclusive of the end.
     *
     * @return true if this time interval is before the current instant
     */
    bool isBeforeNow();
    
    /**
     * Is this time interval before the specified instant.
     * <p>
     * Intervals are inclusive of the start instant and exclusive of the end.
     *
     * @param instant  the instant to compare to, NULL means now
     * @return true if this time interval is before the instant
     */
    bool isBefore(ReadableInstant *instant);
    
    /**
     * Is this time interval entirely before the specified instant.
     * <p>
     * Intervals are inclusive of the start instant and exclusive of the end.
     *
     * @param interval  the interval to compare to, NULL means now
     * @return true if this time interval is before the interval specified
     */
    bool isBefore(ReadableInterval *interval);
    
    //-----------------------------------------------------------------------
    /**
     * Is this time interval after the specified millisecond instant.
     * <p>
     * Intervals are inclusive of the start instant and exclusive of the end.
     *
     * @param millisInstant  the instant to compare to,
     *  millisecond instant from 1970-01-01T00:00:00Z
     * @return true if this time interval is after the instant
     */
    bool isAfter(int64_t millisInstant);
    
    /**
     * Is this time interval after the current instant.
     * <p>
     * Intervals are inclusive of the start instant and exclusive of the end.
     *
     * @return true if this time interval is after the current instant
     */
    bool isAfterNow();
    
    /**
     * Is this time interval after the specified instant.
     * <p>
     * Intervals are inclusive of the start instant and exclusive of the end.
     *
     * @param instant  the instant to compare to, NULL means now
     * @return true if this time interval is after the instant
     */
    bool isAfter(ReadableInstant *instant);
    
    /**
     * Is this time interval entirely after the specified interval->
     * <p>
     * Intervals are inclusive of the start instant and exclusive of the end.
     * Only the end time of the specified interval is used in the comparison.
     *
     * @param interval  the interval to compare to, NULL means now
     * @return true if this time interval is after the interval specified
     */
    bool isAfter(ReadableInterval *interval);
    
    //-----------------------------------------------------------------------
    /**
     * Get this interval as an immutable <code>Interval</code> object.
     *
     * @return the interval as an Interval object
     */
    Interval *toInterval();
    
    /**
     * Get this time interval as a <code>MutableInterval</code>.
     * <p>
     * This will always return a new <code>MutableInterval</code> with the same interval->
     *
     * @return the time interval as a MutableInterval object
     */
    MutableInterval *toMutableInterval();

    //-----------------------------------------------------------------------
    /**
     * Gets the duration of this time interval in milliseconds.
     * <p>
     * The duration is equal to the end millis minus the start millis.
     *
     * @return the duration of the time interval in milliseconds
     * @throws ArithmeticException if the duration exceeds the capacity of a uint64_t
     */
    int64_t toDurationMillis();
    
    /**
     * Gets the duration of this time interval->
     * <p>
     * The duration is equal to the end millis minus the start millis.
     *
     * @return the duration of the time interval
     * @throws ArithmeticException if the duration exceeds the capacity of a uint64_t
     */
    Duration *toDuration();
    
    //-----------------------------------------------------------------------
    /**
     * Converts the duration of the interval to a <code>Period</code> using the
     * All period type.
     * <p>
     * This method should be used to exract the field values describing the
     * difference between the start and end instants.
     *
     * @return a time period derived from the interval
     */
    Period *toPeriod();
    
    /**
     * Converts the duration of the interval to a <code>Period</code> using the
     * specified period type.
     * <p>
     * This method should be used to exract the field values describing the
     * difference between the start and end instants.
     *
     * @param type  the requested type of the duration, NULL means AllType
     * @return a time period derived from the interval
     */
    Period *toPeriod(const PeriodType *type);
    
    //-----------------------------------------------------------------------
    /**
     * Compares this object with the specified object for equality based
     * on start and end millis plus the chronology.
     * All ReadableInterval instances are accepted.
     * <p>
     * To compare the duration of two time intervals, use {@link #toDuration()}
     * to get the durations and compare those.
     *
     * @param readableInterval  a readable interval to check against
     * @return true if the intervals are equal comparing the start millis,
     *  end millis and chronology
     */
    bool equals(const Object *readableInterval) const;
    
    /**
     * Hashcode compatible with equals method.
     *
     * @return suitable hashcode
     */
    int hashCode();
    
    /**
     * Output a string in ISO8601 interval format.
     * <p>
     * From version 2.1, the string includes the time zone offset.
     *
     * @return re-parsable string (in the default zone)
     */
    string toString();
    
};

CODATIME_END

#endif
