//
//  BaseDuration.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BaseDuration_h
#define CodaTime_BaseDuration_h

#include "CodaTimeMacros.h"

#include "base/AbstractDuration.h"
#include "DateTimeUtils.h"
#include "field/FieldUtils.h"
#include "Period.h"
#include "ReadableDuration.h"
#include "Interval.h"

CODATIME_BEGIN

class ReadableInstant;
class PeriodType;
class Chronology;

/**
 * BaseDuration is an abstract implementation of ReadableDuration that stores
 * data in a <code>long</code> duration milliseconds field.
 * <p>
 * This class should generally not be used directly by API users.
 * The {@link ReadableDuration} interface should be used when different
 * kinds of duration objects are to be referenced.
 * <p>
 * BaseDuration subclasses may be mutable and not thread-safe.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class BaseDuration : public AbstractDuration, public virtual ReadableDuration {
    
private:
    
    /** The duration length */
    int64_t iMillis;
    
protected:
    
    /**
     * Creates a duration from the given millisecond duration.
     *
     * @param duration  the duration, in milliseconds
     */
    BaseDuration(int64_t duration) : AbstractDuration() {
        iMillis = duration;
    }
    
    /**
     * Creates a duration from the given interval endpoints.
     *
     * @param startInstant  interval start, in milliseconds
     * @param endInstant  interval end, in milliseconds
     * @throws ArithmeticException if the duration exceeds a 64-bit long
     */
    BaseDuration(int64_t startInstant, int64_t endInstant) : AbstractDuration() {
        iMillis = FieldUtils::safeAdd(endInstant, -startInstant);
    }
    
    /**
     * Creates a duration from the given interval endpoints.
     *
     * @param start  interval start, null means now
     * @param end  interval end, null means now
     * @throws ArithmeticException if the duration exceeds a 64-bit long
     */
    BaseDuration(ReadableInstant *start, ReadableInstant *end) : AbstractDuration() {
        if (start == end) {
            iMillis = 0L;
        } else {
            int64_t startMillis = DateTimeUtils::getInstantMillis(start);
            int64_t endMillis = DateTimeUtils::getInstantMillis(end);
            iMillis = FieldUtils::safeAdd(endMillis, -startMillis);
        }
    }
    
    /**
     * Creates a duration from the specified object using the
     * {@link org.joda.time.convert.ConverterManager ConverterManager}.
     *
     * @param duration  duration to convert
     * @throws IllegalArgumentException if duration is invalid
     */
    BaseDuration(string duration) : AbstractDuration() {
        DurationConverter *converter = ConverterManager::getInstance()->getDurationConverter(duration);
        iMillis = converter->getDurationMillis(duration);
    }
    
    //-----------------------------------------------------------------------
    /**
     * Sets the length of this duration in milliseconds.
     *
     * @param duration  the new length of the duration
     */
    void setMillis(int64_t duration) {
        iMillis = duration;
    }
    
public:
    
    //-----------------------------------------------------------------------
    /**
     * Gets the length of this duration in milliseconds.
     *
     * @return the length of the duration in milliseconds.
     */
    int64_t getMillis() const {
        return iMillis;
    }
    
    Duration *toDuration() { return AbstractDuration::toDuration(); }
    
    Period *toPeriod() { return AbstractDuration::toPeriod(); }
    
    int compareTo(const ReadableDuration *other) const { return AbstractDuration::compareTo(other); }
    
    bool isEqual(ReadableDuration *duration) { return AbstractDuration::isEqual(duration); }
    
    bool isLongerThan(ReadableDuration *duration) { return AbstractDuration::isLongerThan(duration); }
    
    bool isShorterThan(ReadableDuration *duration) { return AbstractDuration::isShorterThan(duration); }
    
    bool equals(const Object *duration) const { return AbstractDuration::equals(duration); }
    
    int hashCode() { return AbstractDuration::hashCode(); }
    
    string toString() { return AbstractDuration::toString(); }
    
    //-----------------------------------------------------------------------
    /**
     * Converts this duration to a Period instance using the specified period type
     * and the ISO chronology.
     * <p>
     * Only precise fields in the period type will be used.
     * At most these are hours, minutes, seconds and millis - the period
     * type may restrict the selection further.
     * <p>
     * For more control over the conversion process, you must pair the duration with
     * an instant, see {@link #toPeriodFrom(ReadableInstant, PeriodType)}.
     *
     * @param type  the period type to use, null means standard
     * @return a Period created using the millisecond duration from this instance
     */
    Period *toPeriod(const PeriodType *type) {
        return new Period(getMillis(), type);
    }
    
    /**
     * Converts this duration to a Period instance using the standard period type
     * and the specified chronology.
     * <p>
     * Only precise fields in the period type will be used.
     * Exactly which fields are precise depends on the chronology.
     * Only the time fields are precise for ISO chronology with a time zone.
     * However, ISO UTC also has precise days and weeks.
     * <p>
     * For more control over the conversion process, you must pair the duration with
     * an instant, see {@link #toPeriodFrom(ReadableInstant)} and
     * {@link #toPeriodTo(ReadableInstant)}
     *
     * @param chrono  the chronology to use, null means ISO default
     * @return a Period created using the millisecond duration from this instance
     */
    Period *toPeriod(Chronology *chrono) {
        return new Period(getMillis(), chrono);
    }
    
    /**
     * Converts this duration to a Period instance using the specified period type
     * and chronology.
     * <p>
     * Only precise fields in the period type will be used.
     * Exactly which fields are precise depends on the chronology.
     * Only the time fields are precise for ISO chronology with a time zone.
     * However, ISO UTC also has precise days and weeks.
     * <p>
     * For more control over the conversion process, you must pair the duration with
     * an instant, see {@link #toPeriodFrom(ReadableInstant, PeriodType)} and
     * {@link #toPeriodTo(ReadableInstant, PeriodType)}
     *
     * @param type  the period type to use, null means standard
     * @param chrono  the chronology to use, null means ISO default
     * @return a Period created using the millisecond duration from this instance
     */
    Period *toPeriod(PeriodType *type, Chronology *chrono) {
        return new Period(getMillis(), type, chrono);
    }
    
    /**
     * Converts this duration to a Period instance by adding the duration to a start
     * instant to obtain an interval using the standard period type.
     * <p>
     * This conversion will determine the fields of a period accurately.
     * The results are based on the instant millis, the chronology of the instant,
     * the standard period type and the length of this duration.
     *
     * @param startInstant  the instant to calculate the period from, null means now
     * @return a Period created using the millisecond duration from this instance
     */
    Period *toPeriodFrom(ReadableInstant *startInstant) {
        return new Period(startInstant, this);
    }
    
    /**
     * Converts this duration to a Period instance by adding the duration to a start
     * instant to obtain an interval.
     * <p>
     * This conversion will determine the fields of a period accurately.
     * The results are based on the instant millis, the chronology of the instant,
     * the period type and the length of this duration.
     *
     * @param startInstant  the instant to calculate the period from, null means now
     * @param type  the period type determining how to split the duration into fields, null means All type
     * @return a Period created using the millisecond duration from this instance
     */
    Period *toPeriodFrom(ReadableInstant *startInstant, PeriodType *type) {
        return new Period(startInstant, this, type);
    }
    
    /**
     * Converts this duration to a Period instance by subtracting the duration
     * from an end instant to obtain an interval using the standard period
     * type.
     * <p>
     * This conversion will determine the fields of a period accurately.
     * The results are based on the instant millis, the chronology of the instant,
     * the standard period type and the length of this duration.
     *
     * @param endInstant  the instant to calculate the period to, null means now
     * @return a Period created using the millisecond duration from this instance
     */
    Period *toPeriodTo(ReadableInstant *endInstant) {
        return new Period(this, endInstant);
    }
    
    /**
     * Converts this duration to a Period instance by subtracting the duration
     * from an end instant to obtain an interval using the standard period
     * type.
     * <p>
     * This conversion will determine the fields of a period accurately.
     * The results are based on the instant millis, the chronology of the instant,
     * the period type and the length of this duration.
     *
     * @param endInstant  the instant to calculate the period to, null means now
     * @param type  the period type determining how to split the duration into fields, null means All type
     * @return a Period created using the millisecond duration from this instance
     */
    Period *toPeriodTo(ReadableInstant *endInstant, PeriodType *type) {
        return new Period(this, endInstant, type);
    }
    
    /**
     * Converts this duration to an Interval starting at the specified instant.
     *
     * @param startInstant  the instant to start the interval at, null means now
     * @return an Interval starting at the specified instant
     */
    Interval *toIntervalFrom(ReadableInstant *startInstant) {
        return new Interval(startInstant, this);
    }
    
    /**
     * Converts this duration to an Interval ending at the specified instant.
     *
     * @param endInstant  the instant to end the interval at, null means now
     * @return an Interval ending at the specified instant
     */
    Interval *toIntervalTo(ReadableInstant *endInstant) {
        return new Interval(this, endInstant);
    }
    
};

CODATIME_END

#endif
