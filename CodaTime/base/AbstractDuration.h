//
//  AbstractDuration.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_AbstractDuration_h
#define CodaTime_AbstractDuration_h

#include "CodaTimeMacros.h"

#include "ReadableDuration.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Duration;
class Period;

/**
 * AbstractDuration provides the common behaviour for duration classes.
 * <p>
 * This class should generally not be used directly by API users. The
 * {@link ReadableDuration} interface should be used when different
 * kinds of durations are to be referenced.
 * <p>
 * AbstractDuration subclasses may be mutable and not thread-safe.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class AbstractDuration : public virtual ReadableDuration {
    
protected:
    
    /**
     * Constructor.
     */
    AbstractDuration() : ReadableDuration() {
    }
    
public:
    
    /**
     * Gets the total length of this duration in milliseconds.
     *
     * @return the total length of the time duration in milliseconds.
     */
    virtual int64_t getMillis() const = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Get this duration as an immutable <code>Duration</code> object.
     *
     * @return a Duration created using the millisecond duration from this instance
     */
    Duration *toDuration();
    
    //-----------------------------------------------------------------------
    /**
     * Converts this duration to a Period instance using the standard period type
     * and the ISO chronology.
     * <p>
     * Only precise fields in the period type will be used. Thus, only the hour,
     * minute, second and millisecond fields on the period will be used.
     * The year, month, week and day fields will not be populated.
     * <p>
     * If the duration is small, less than one day, then this method will perform
     * as you might expect and split the fields evenly.
     * If the duration is larger than one day then all the remaining duration will
     * be stored in the largest available field, hours in this case.
     * <p>
     * For example, a duration effectively equal to (365 + 60 + 5) days will be
     * converted to ((365 + 60 + 5) * 24) hours by this constructor.
     * <p>
     * For more control over the conversion process, you must pair the duration with
     * an instant, see {@link Period#Period(ReadableInstant,ReadableDuration)}.
     *
     * @return a Period created using the millisecond duration from this instance
     */
    Period *toPeriod();
    
    //-----------------------------------------------------------------------
    /**
     * Compares this duration with the specified duration based on length.
     *
     * @param other  a duration to check against
     * @return negative value if this is less, 0 if equal, or positive value if greater
     * @throws NullPointerException if the object is null
     * @throws ClassCastException if the given object is not supported
     */
    int compareTo(const ReadableDuration *other) const;
    
    /**
     * Is the length of this duration equal to the duration passed in.
     *
     * @param duration  another duration to compare to, null means zero milliseconds
     * @return true if this duration is equal to than the duration passed in
     */
    bool isEqual(ReadableDuration *duration) ;
    
    /**
     * Is the length of this duration longer than the duration passed in.
     *
     * @param duration  another duration to compare to, null means zero milliseconds
     * @return true if this duration is equal to than the duration passed in
     */
    bool isLongerThan(ReadableDuration *duration);
    
    /**
     * Is the length of this duration shorter than the duration passed in.
     *
     * @param duration  another duration to compare to, null means zero milliseconds
     * @return true if this duration is equal to than the duration passed in
     */
    bool isShorterThan(ReadableDuration *duration);
    
    //-----------------------------------------------------------------------
    /**
     * Compares this object with the specified object for equality based
     * on the millisecond length. All ReadableDuration instances are accepted.
     *
     * @param duration  a readable duration to check against
     * @return true if the length of the duration is equal
     */
    bool equals(const Object *duration) const;
    
    /**
     * Gets a hash code for the duration that is compatible with the
     * equals method.
     *
     * @return a hash code
     */
    int hashCode();
    
    //-----------------------------------------------------------------------
    /**
     * Gets the value as a string in the ISO8601 duration format including
     * only seconds and milliseconds.
     * <p>
     * For example, "PT72.345S" represents 1 minute, 12 seconds and 345 milliseconds.
     * <p>
     * For more control over the output, see
     * {@link org.joda.time.format.PeriodFormatterBuilder PeriodFormatterBuilder}.
     *
     * @return the value as an ISO8601 string
     */
    string toString();
    
};

CODATIME_END

#endif
