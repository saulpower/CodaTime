//
//  Duration.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__Duration__
#define __CodaTime__Duration__

#include "CodaTimeMacros.h"

#include "base/BaseDuration.h"
#include "ReadableDuration.h"

#include <string>

using namespace std;

CODATIME_BEGIN

/**
 * An immutable duration specifying a length of time in milliseconds.
 * <p>
 * A duration is defined by a fixed number of milliseconds.
 * There is no concept of fields, such as days or seconds, as these fields can vary in length.
 * <p>
 * A duration may be converted to a {@link Period} to obtain field values.
 * This conversion will typically cause a loss of precision.
 * <p>
 * Duration is thread-safe and immutable.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class Duration : public BaseDuration, public virtual ReadableDuration {
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = 2471658376918L;
    
public:
    
    int64_t getMillis() const { return BaseDuration::getMillis(); }
    
    Period *toPeriod() { return BaseDuration::toPeriod(); }
    
    int compareTo(const ReadableDuration *other) const { return BaseDuration::compareTo(other); }
    
    bool isEqual(ReadableDuration *duration) { return BaseDuration::isEqual(duration); }
    
    bool isLongerThan(ReadableDuration *duration) { return BaseDuration::isLongerThan(duration); }
    
    bool isShorterThan(ReadableDuration *duration) { return BaseDuration::isShorterThan(duration); }
    
    bool equals(const Object *duration) const { return BaseDuration::equals(duration); }
    
    int hashCode() { return BaseDuration::hashCode(); }
    
    string toString() { return BaseDuration::toString(); }
    
    /** Constant representing zero millisecond duration */
    static Duration *ZERO;
    
    //-----------------------------------------------------------------------
    /**
     * Parses a {@code Duration} from the specified string.
     * <p>
     * This parses the format {@code PTa.bS}, as per {@link #toString()}.
     *
     * @param str  the string to parse, not null
     * @since 2.0
     */
    static Duration *parse(string str);
    
    //-----------------------------------------------------------------------
    /**
     * Create a duration with the specified number of days assuming that
     * there are the standard number of milliseconds in a day.
     * <p>
     * This method assumes that there are 24 hours in a day,
     * 60 minutes in an hour, 60 seconds in a minute and 1000 milliseconds in
     * a second. This will be true for most days, however days with Daylight
     * Savings changes will not have 24 hours, so use this method with care.
     * <p>
     * A Duration is a representation of an amount of time. If you want to express
     * the concept of 'days' you should consider using the {@link Days} class.
     *
     * @param days  the number of standard days in this duration
     * @return the duration, never null
     * @throws ArithmeticException if the days value is too large
     * @since 1.6
     */
    static Duration *standardDays(int64_t days);
    
    /**
     * Create a duration with the specified number of hours assuming that
     * there are the standard number of milliseconds in an hour.
     * <p>
     * This method assumes that there are 60 minutes in an hour,
     * 60 seconds in a minute and 1000 milliseconds in a second.
     * All currently supplied chronologies use this definition.
     * <p>
     * A Duration is a representation of an amount of time. If you want to express
     * the concept of 'hours' you should consider using the {@link Hours} class.
     *
     * @param hours  the number of standard hours in this duration
     * @return the duration, never null
     * @throws ArithmeticException if the hours value is too large
     * @since 1.6
     */
    static Duration *standardHours(int64_t hours);
    
    /**
     * Create a duration with the specified number of minutes assuming that
     * there are the standard number of milliseconds in a minute.
     * <p>
     * This method assumes that there are 60 seconds in a minute and
     * 1000 milliseconds in a second.
     * All currently supplied chronologies use this definition.
     * <p>
     * A Duration is a representation of an amount of time. If you want to express
     * the concept of 'minutes' you should consider using the {@link Minutes} class.
     *
     * @param minutes  the number of standard minutes in this duration
     * @return the duration, never null
     * @throws ArithmeticException if the minutes value is too large
     * @since 1.6
     */
    static Duration *standardMinutes(int64_t minutes);
    
    /**
     * Create a duration with the specified number of seconds assuming that
     * there are the standard number of milliseconds in a second.
     * <p>
     * This method assumes that there are 1000 milliseconds in a second.
     * All currently supplied chronologies use this definition.
     * <p>
     * A Duration is a representation of an amount of time. If you want to express
     * the concept of 'seconds' you should consider using the {@link Seconds} class.
     *
     * @param seconds  the number of standard seconds in this duration
     * @return the duration, never null
     * @throws ArithmeticException if the seconds value is too large
     * @since 1.6
     */
    static Duration *standardSeconds(int64_t seconds);
    
    /**
     * Create a duration with the specified number of milliseconds.
     *
     * @param millis  the number of standard milliseconds in this duration
     * @return the duration, never null
     * @since 2.0
     */
    static Duration *millis(int64_t millis);
    
    //-----------------------------------------------------------------------
    /**
     * Creates a duration from the given millisecond duration.
     *
     * @param duration  the duration, in milliseconds
     */
    Duration(int64_t duration);
    
    /**
     * Creates a duration from the given interval endpoints.
     *
     * @param startInstant  interval start, in milliseconds
     * @param endInstant  interval end, in milliseconds
     * @throws ArithmeticException if the duration exceeds a 64-bit long
     */
    Duration(int64_t startInstant, int64_t endInstant);
    
    /**
     * Creates a duration from the given interval endpoints.
     *
     * @param start  interval start, null means now
     * @param end  interval end, null means now
     * @throws ArithmeticException if the duration exceeds a 64-bit long
     */
    Duration(ReadableInstant *start, ReadableInstant *end);
    
    /**
     * Creates a duration from the specified object using the
     * {@link org.joda.time.convert.ConverterManager ConverterManager}.
     *
     * @param duration  duration to convert
     * @throws IllegalArgumentException if duration is invalid
     */
    Duration(string duration);
    
    //-----------------------------------------------------------------------
    /**
     * Gets the length of this duration in days assuming that there are the
     * standard number of milliseconds in a day.
     * <p>
     * This method assumes that there are 24 hours in a day,
     * 60 minutes in an hour, 60 seconds in a minute and 1000 milliseconds in
     * a second. This will be true for most days, however days with Daylight
     * Savings changes will not have 24 hours, so use this method with care.
     * <p>
     * This returns <code>getMillis() / MILLIS_PER_DAY</code>.
     * The result is an integer division, thus excess milliseconds are truncated.
     *
     * @return the length of the duration in standard seconds
     * @since 2.0
     */
    int64_t getStandardDays();
    
    /**
     * Gets the length of this duration in hours assuming that there are the
     * standard number of milliseconds in an hour.
     * <p>
     * This method assumes that there are 60 minutes in an hour,
     * 60 seconds in a minute and 1000 milliseconds in a second.
     * All currently supplied chronologies use this definition.
     * <p>
     * This returns <code>getMillis() / MILLIS_PER_HOUR</code>.
     * The result is an integer division, thus excess milliseconds are truncated.
     *
     * @return the length of the duration in standard seconds
     * @since 2.0
     */
    int64_t getStandardHours();
    
    /**
     * Gets the length of this duration in minutes assuming that there are the
     * standard number of milliseconds in a minute.
     * <p>
     * This method assumes that there are 60 seconds in a minute and
     * 1000 milliseconds in a second.
     * All currently supplied chronologies use this definition.
     * <p>
     * This returns <code>getMillis() / 60000</code>.
     * The result is an integer division, thus excess milliseconds are truncated.
     *
     * @return the length of the duration in standard seconds
     * @since 2.0
     */
    int64_t getStandardMinutes();
    
    /**
     * Gets the length of this duration in seconds assuming that there are the
     * standard number of milliseconds in a second.
     * <p>
     * This method assumes that there are 1000 milliseconds in a second.
     * All currently supplied chronologies use this definition.
     * <p>
     * This returns <code>getMillis() / 1000</code>.
     * The result is an integer division, so 2999 millis returns 2 seconds.
     *
     * @return the length of the duration in standard seconds
     * @since 1.6
     */
    int64_t getStandardSeconds();
    
    //-----------------------------------------------------------------------
    /**
     * Get this duration as an immutable <code>Duration</code> object
     * by returning <code>this</code>.
     *
     * @return <code>this</code>
     */
    Duration *toDuration();
    
    /**
     * Converts this duration to a period in days assuming that there are the
     * standard number of milliseconds in a day.
     * <p>
     * This method assumes that there are 24 hours in a day,
     * 60 minutes in an hour, 60 seconds in a minute and 1000 milliseconds in
     * a second. This will be true for most days, however days with Daylight
     * Savings changes will not have 24 hours, so use this method with care.
     *
     * @return a period representing the number of standard days in this period, never null
     * @throws ArithmeticException if the number of days is too large to be represented
     * @since 2.0
     */
    Days *toStandardDays();
    
    /**
     * Converts this duration to a period in hours assuming that there are the
     * standard number of milliseconds in an hour.
     * <p>
     * This method assumes that there are 60 minutes in an hour,
     * 60 seconds in a minute and 1000 milliseconds in a second.
     * All currently supplied chronologies use this definition.
     *
     * @return a period representing the number of standard hours in this period, never null
     * @throws ArithmeticException if the number of hours is too large to be represented
     * @since 2.0
     */
    Hours *toStandardHours();
    
    /**
     * Converts this duration to a period in minutes assuming that there are the
     * standard number of milliseconds in a minute.
     * <p>
     * This method assumes that there are 60 seconds in a minute and
     * 1000 milliseconds in a second.
     * All currently supplied chronologies use this definition.
     *
     * @return a period representing the number of standard minutes in this period, never null
     * @throws ArithmeticException if the number of minutes is too large to be represented
     * @since 2.0
     */
    Minutes *toStandardMinutes();
    
    /**
     * Converts this duration to a period in seconds assuming that there are the
     * standard number of milliseconds in a second.
     * <p>
     * This method assumes that there are 1000 milliseconds in a second.
     * All currently supplied chronologies use this definition.
     *
     * @return a period representing the number of standard seconds in this period, never null
     * @throws ArithmeticException if the number of seconds is too large to be represented
     * @since 1.6
     */
    Seconds *toStandardSeconds();
    
    //-----------------------------------------------------------------------
    /**
     * Creates a new Duration instance with a different millisecond length.
     *
     * @param duration  the new length of the duration
     * @return the new duration instance
     */
    Duration *withMillis(int64_t duration);
    
    /**
     * Returns a new duration with this length plus that specified multiplied by the scalar.
     * This instance is immutable and is not altered.
     * <p>
     * If the addition is zero, this instance is returned.
     *
     * @param durationToAdd  the duration to add to this one
     * @param scalar  the amount of times to add, such as -1 to subtract once
     * @return the new duration instance
     */
    Duration *withDurationAdded(int64_t durationToAdd, int scalar);
    
    /**
     * Returns a new duration with this length plus that specified multiplied by the scalar.
     * This instance is immutable and is not altered.
     * <p>
     * If the addition is zero, this instance is returned.
     *
     * @param durationToAdd  the duration to add to this one, null means zero
     * @param scalar  the amount of times to add, such as -1 to subtract once
     * @return the new duration instance
     */
    Duration *withDurationAdded(ReadableDuration *durationToAdd, int scalar);
    
    //-----------------------------------------------------------------------
    /**
     * Returns a new duration with this length plus that specified.
     * This instance is immutable and is not altered.
     * <p>
     * If the addition is zero, this instance is returned.
     *
     * @param amount  the duration to add to this one
     * @return the new duration instance
     */
    Duration *plus(int64_t amount);
    
    /**
     * Returns a new duration with this length plus that specified.
     * This instance is immutable and is not altered.
     * <p>
     * If the amount is zero, this instance is returned.
     *
     * @param amount  the duration to add to this one, null means zero
     * @return the new duration instance
     */
    Duration *plus(ReadableDuration *amount);
    
    /**
     * Returns a new duration with this length minus that specified.
     * This instance is immutable and is not altered.
     * <p>
     * If the addition is zero, this instance is returned.
     *
     * @param amount  the duration to take away from this one
     * @return the new duration instance
     */
    Duration *minus(int64_t amount);
    
    /**
     * Returns a new duration with this length minus that specified.
     * This instance is immutable and is not altered.
     * <p>
     * If the amount is zero, this instance is returned.
     *
     * @param amount  the duration to take away from this one, null means zero
     * @return the new duration instance
     */
    Duration *minus(ReadableDuration *amount);
    
    /**
     * Returns a new duration with this length multiplied by the
     * specified multiplicand.
     * This instance is immutable and is not altered.
     * <p>
     * If the multiplicand is one, this instance is returned.
     *
     * @param multiplicand  the multiplicand to multiply this one by
     * @return the new duration instance
     */
    Duration *multipliedBy(int64_t multiplicand);
    
    /**
     * Returns a new duration with this length divided by the
     * specified divisor.
     * This instance is immutable and is not altered.
     * <p>
     * If the divisor is one, this instance is returned.
     *
     * @param divisor  the divisor to divide this one by
     * @return the new duration instance
     */
    Duration *dividedBy(int64_t divisor);
    
    /**
     * Returns a new duration with this length negated.
     * This instance is immutable and is not altered.
     *
     * @return the new duration instance
     */
    Duration *negated();
    
};

CODATIME_END

#endif /* defined(__CodaTime__Duration__) */
