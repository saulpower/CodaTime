//
//  ReadableDuration.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_ReadableDuration_h
#define CodaTime_ReadableDuration_h

#include "CodaTimeMacros.h"

#include "Comparable.h"
#include "Object.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Duration;
class Period;

/**
 * Defines an exact duration of time in milliseconds.
 * <p>
 * The implementation of this interface may be mutable or immutable. This
 * interface only gives access to retrieve data, never to change it.
 * <p>
 * Methods that are passed a duration as a parameter will treat <code>null</code>
 * as a zero length duration.
 * <p>
 * The {@code compareTo} method is no uint64_ter defined in this class in version 2.0.
 * Instead, the definition is simply inherited from the {@code Comparable} interface.
 * This approach is necessary to preserve binary compatibility.
 * The definition of the comparison is ascending order by millisecond duration.
 * Implementors are recommended to extend {@code AbstractInstant} instead of this interface.
 *
 * @see ReadableInterval
 * @see ReadablePeriod
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class ReadableDuration : public virtual Object, public Comparable<ReadableDuration> {
    
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
     * <p>
     * This will either typecast this instance, or create a new <code>Duration</code>.
     *
     * @return a Duration created using the millisecond duration from this instance
     */
    virtual Duration *toDuration() = 0;
    
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
    virtual Period *toPeriod();
    
    //-----------------------------------------------------------------------
    // Method is no uint64_ter defined here as that would break generic backwards compatibility
    //    /**
    //     * Compares this duration with the specified duration based on length.
    //     *
    //     * @param obj  a duration to check against
    //     * @return negative value if this is less, 0 if equal, or positive value if greater
    //     * @throws NullPointerException if the object is null
    //     * @throws ClassCastException if the given object is not supported
    //     */
    //    int compareTo(ReadableDuration obj);
    virtual int compareTo(const ReadableDuration *obj) const = 0;
    
    /**
     * Is the length of this duration equal to the duration passed in.
     *
     * @param duration  another duration to compare to, null means zero milliseconds
     * @return true if this duration is equal to than the duration passed in
     */
    virtual bool isEqual(ReadableDuration *duration) = 0;
    
    /**
     * Is the length of this duration uint64_ter than the duration passed in.
     *
     * @param duration  another duration to compare to, null means zero milliseconds
     * @return true if this duration is equal to than the duration passed in
     */
    virtual bool isLongerThan(ReadableDuration *duration) = 0;
    
    /**
     * Is the length of this duration shorter than the duration passed in.
     *
     * @param duration  another duration to compare to, null means zero milliseconds
     * @return true if this duration is equal to than the duration passed in
     */
    virtual bool isShorterThan(ReadableDuration *duration) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Compares this object with the specified object for equality based
     * on the millisecond length. All ReadableDuration instances are accepted.
     *
     * @param readableDuration  a readable duration to check against
     * @return true if the length of the duration is equal
     */
    virtual bool equals(const Object *readableDuration) const = 0;
    
    /**
     * Gets a hash code for the duration that is compatable with the
     * equals method.
     * The following formula must be used:
     * <pre>
     *  int64_t len = getMillis();
     *  return (int) (len ^ (len >>> 32));
     * </pre>
     *
     * @return a hash code
     */
    virtual int hashCode() = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the value as a String in the ISO8601 duration format using hours,
     * minutes and seconds (including fractional milliseconds).
     * <p>
     * For example, "PT6H3M7S" represents 6 hours, 3 minutes, 7 seconds.
     *
     * @return the value as an ISO8601 string
     */
    virtual string toString() = 0;
    
};

CODATIME_END

#endif
