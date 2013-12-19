//
//  BasePeriod.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__BasePeriod__
#define __CodaTime__BasePeriod__

#include "CodaTimeMacros.h"

#include "base/AbstractPeriod.h"
#include "ReadablePeriod.h"
#include "PeriodType.h"

#include <vector>

using namespace std;

CODATIME_BEGIN

class ReadableInstant;
class ReadableDuration;
class ReadablePartial;
class Chronology;
class Duration;
class DurationFieldType;

/**
 * BasePeriod is an abstract implementation of ReadablePeriod that stores
 * data in a <code>PeriodType</code> and an <code>vector<int></code>.
 * <p>
 * This class should generally not be used directly by API users.
 * The {@link ReadablePeriod} interface should be used when different
 * kinds of period objects are to be referenced.
 * <p>
 * BasePeriod subclasses may be mutable and not thread-safe.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class BasePeriod : public AbstractPeriod, public virtual ReadablePeriod {
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = -2110953284060001145L;
    
    class DummyPeriod : public AbstractPeriod {
        
    public:
        int getValue(int index) const {
            return 0;
        }
        const PeriodType *getPeriodType() const {
            return PeriodType::time();
        }
    };
    
    /** Serialization version */
    static const DummyPeriod *DUMMY_PERIOD;
    
    /** The type of period */
    const PeriodType *iType;
    
    /** The values */
    vector<int> iValues;
    
    //-----------------------------------------------------------------------
    /**
     * Checks whether a field type is supported, and if so adds the new value
     * to the relevant index in the specified array.
     *
     * @param type  the field type
     * @param values  the array to update
     * @param newValue  the new value to store if successful
     */
    void checkAndUpdate(const DurationFieldType *type, vector<int> values, int newValue);
    
    /**
     * method called from constructor.
     */
    void setPeriodInternal(ReadablePeriod *period);
    
    /**
     * method called from constructor.
     */
    vector<int> setPeriodInternal(int years, int months, int weeks, int days,
                                  int hours, int minutes, int seconds, int millis);
    
protected:
    
    //-----------------------------------------------------------------------
    /**
     * Creates a period from a set of field values.
     *
     * @param years  amount of years in this period, which must be zero if unsupported
     * @param months  amount of months in this period, which must be zero if unsupported
     * @param weeks  amount of weeks in this period, which must be zero if unsupported
     * @param days  amount of days in this period, which must be zero if unsupported
     * @param hours  amount of hours in this period, which must be zero if unsupported
     * @param minutes  amount of minutes in this period, which must be zero if unsupported
     * @param seconds  amount of seconds in this period, which must be zero if unsupported
     * @param millis  amount of milliseconds in this period, which must be zero if unsupported
     * @param type  which set of fields this period supports
     * @throws IllegalArgumentException if period type is invalid
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    BasePeriod(int years, int months, int weeks, int days,
                         int hours, int minutes, int seconds, int millis,
               const PeriodType *type);
    
    /**
     * Creates a period from the given interval endpoints.
     *
     * @param startInstant  interval start, in milliseconds
     * @param endInstant  interval end, in milliseconds
     * @param type  which set of fields this period supports, null means standard
     * @param chrono  the chronology to use, null means ISO default
     * @throws IllegalArgumentException if period type is invalid
     */
    BasePeriod(int64_t startInstant, int64_t endInstant, const PeriodType *type, Chronology *chrono);
    
    /**
     * Creates a period from the given interval endpoints.
     *
     * @param startInstant  interval start, null means now
     * @param endInstant  interval end, null means now
     * @param type  which set of fields this period supports, null means standard
     * @throws IllegalArgumentException if period type is invalid
     */
    BasePeriod(ReadableInstant *startInstant, ReadableInstant *endInstant, const PeriodType *type);
    
    /**
     * Creates a period from the given duration and end point.
     * <p>
     * The two partials must contain the same fields, thus you can
     * specify two <code>LocalDate</code> objects, or two <code>LocalTime</code>
     * objects, but not one of each.
     * As these are Partial objects, time zones have no effect on the result.
     * <p>
     * The two partials must also both be contiguous - see
     * {@link DateTimeUtils#isContiguous(ReadablePartial)} for a
     * definition. Both <code>LocalDate</code> and <code>LocalTime</code> are contiguous.
     *
     * @param start  the start of the period, must not be null
     * @param end  the end of the period, must not be null
     * @param type  which set of fields this period supports, null means standard
     * @throws IllegalArgumentException if the partials are null or invalid
     * @since 1.1
     */
    BasePeriod(ReadablePartial *start, ReadablePartial *end, const PeriodType *type);
    
    /**
     * Creates a period from the given start point and duration.
     *
     * @param startInstant  the interval start, null means now
     * @param duration  the duration of the interval, null means zero-length
     * @param type  which set of fields this period supports, null means standard
     */
    BasePeriod(ReadableInstant *startInstant, ReadableDuration *duration, const PeriodType *type);
    
    /**
     * Creates a period from the given duration and end point.
     *
     * @param duration  the duration of the interval, null means zero-length
     * @param endInstant  the interval end, null means now
     * @param type  which set of fields this period supports, null means standard
     */
    BasePeriod(ReadableDuration *duration, ReadableInstant *endInstant, const PeriodType *type);
    
    /**
     * Creates a period from the given millisecond duration with the standard period type
     * and ISO rules, ensuring that the calculation is performed with the time-only period type.
     * <p>
     * The calculation uses the hour, minute, second and millisecond fields.
     *
     * @param duration  the duration, in milliseconds
     */
    BasePeriod(int64_t duration);
    
    /**
     * Creates a period from the given millisecond duration, which is only really
     * suitable for durations less than one day.
     * <p>
     * Only fields that are precise will be used.
     * Thus the largest precise field may have a large value.
     *
     * @param duration  the duration, in milliseconds
     * @param type  which set of fields this period supports, null means standard
     * @param chrono  the chronology to use, null means ISO default
     * @throws IllegalArgumentException if period type is invalid
     */
    BasePeriod(int64_t duration, const PeriodType *type, Chronology *chrono);
    
    /**
     * Creates a new period based on another using the {@link ConverterManager}.
     *
     * @param period  the period to convert
     * @param type  which set of fields this period supports, null means use type from object
     * @param chrono  the chronology to use, null means ISO default
     * @throws IllegalArgumentException if period is invalid
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    BasePeriod(Object *period, const PeriodType *type, Chronology *chrono);
    
    /**
     * Constructor used when we trust ourselves.
     * Do not expose publically.
     *
     * @param values  the values to use, not null, not cloned
     * @param type  which set of fields this period supports, not null
     */
    BasePeriod(vector<int> values, const PeriodType *type);
    
    //-----------------------------------------------------------------------
    /**
     * Validates a period type, converting nulls to a default value and
     * checking the type is suitable for this instance.
     *
     * @param type  the type to check, may be null
     * @return the validated type to use, not null
     * @throws IllegalArgumentException if the period type is invalid
     */
    const PeriodType *checkPeriodType(const PeriodType *type);
    
    //-----------------------------------------------------------------------
    /**
     * Sets all the fields of this period from another.
     *
     * @param period  the period to copy from, not null
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    void setPeriod(ReadablePeriod *period);
    
    /**
     * Sets the eight standard the fields in one go.
     *
     * @param years  amount of years in this period, which must be zero if unsupported
     * @param months  amount of months in this period, which must be zero if unsupported
     * @param weeks  amount of weeks in this period, which must be zero if unsupported
     * @param days  amount of days in this period, which must be zero if unsupported
     * @param hours  amount of hours in this period, which must be zero if unsupported
     * @param minutes  amount of minutes in this period, which must be zero if unsupported
     * @param seconds  amount of seconds in this period, which must be zero if unsupported
     * @param millis  amount of milliseconds in this period, which must be zero if unsupported
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    void setPeriod(int years, int months, int weeks, int days,
                   int hours, int minutes, int seconds, int millis);
    
    //-----------------------------------------------------------------------
    /**
     * Sets the value of a field in this period.
     *
     * @param field  the field to set
     * @param value  the value to set
     * @throws IllegalArgumentException if field is is null or not supported.
     */
    void setField(const DurationFieldType *field, int value);
    
    /**
     * Sets the value of a field in this period.
     *
     * @param values  the array of values to update
     * @param field  the field to set
     * @param value  the value to set
     * @throws IllegalArgumentException if field is null or not supported.
     */
    void setFieldInto(vector<int> values, const DurationFieldType *field, int value);
    
    /**
     * Adds the value of a field in this period.
     *
     * @param field  the field to set
     * @param value  the value to set
     * @throws IllegalArgumentException if field is is null or not supported.
     */
    void addField(const DurationFieldType *field, int value);
    
    /**
     * Adds the value of a field in this period.
     *
     * @param values  the array of values to update
     * @param field  the field to set
     * @param value  the value to set
     * @throws IllegalArgumentException if field is is null or not supported.
     */
    void addFieldInto(vector<int> values, const DurationFieldType *field, int value);
    
    /**
     * Merges the fields from another period.
     *
     * @param period  the period to add from, not null
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    void mergePeriod(ReadablePeriod *period);
    
    /**
     * Merges the fields from another period.
     *
     * @param values  the array of values to update
     * @param period  the period to add from, not null
     * @return the updated values
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    vector<int> mergePeriodInto(vector<int> values, ReadablePeriod *period);
    
    /**
     * Adds the fields from another period.
     *
     * @param period  the period to add from, not null
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    void addPeriod(ReadablePeriod *period);
    
    /**
     * Adds the fields from another period.
     *
     * @param values  the array of values to update
     * @param period  the period to add from, not null
     * @return the updated values
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    vector<int> addPeriodInto(vector<int> values, ReadablePeriod *period);
    
    //-----------------------------------------------------------------------
    /**
     * Sets the value of the field at the specified index.
     *
     * @param index  the index
     * @param value  the value to set
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    void setValue(int index, int value);
    
    /**
     * Sets the values of all fields.
     * <p>
     * In version 2.0 and later, this method copies the array into the original.
     * This is because the instance variable has been changed to be const to satisfy the Java Memory Model.
     * This only impacts subclasses that are mutable.
     *
     * @param values  the array of values
     */
    void setValues(vector<int> values);
    
public:
    
    int size() { return AbstractPeriod::size(); }
    
    const DurationFieldType *getFieldType(int index) { return AbstractPeriod::getFieldType(index); }
    
    int get(const DurationFieldType *type) { return AbstractPeriod::get(type); }
    
    bool isSupported(const DurationFieldType *type) { return AbstractPeriod::isSupported(type); }
    
    bool equals(const Object *period) const { return AbstractPeriod::equals(period); }

    int hashCode() { return AbstractPeriod::hashCode(); }

    string toString() { return AbstractPeriod::toString(); }
    
    int indexOf(const DurationFieldType *type) { return AbstractPeriod::indexOf(type); }
    
    //-----------------------------------------------------------------------
    /**
     * Gets the period type.
     *
     * @return the period type
     */
    const PeriodType *getPeriodType() const;
    
    /**
     * Gets the value at the specified index.
     *
     * @param index  the index to retrieve
     * @return the value of the field at the specified index
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    int getValue(int index) const;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the total millisecond duration of this period relative to a start instant.
     * <p>
     * This method adds the period to the specified instant in order to
     * calculate the duration.
     * <p>
     * An instant must be supplied as the duration of a period varies.
     * For example, a period of 1 month could vary between the equivalent of
     * 28 and 31 days in milliseconds due to different length months.
     * Similarly, a day can vary at Daylight Savings cutover, typically between
     * 23 and 25 hours.
     *
     * @param startInstant  the instant to add the period to, thus obtaining the duration
     * @return the total length of the period as a duration relative to the start instant
     * @throws ArithmeticException if the millis exceeds the capacity of the duration
     */
    Duration *toDurationFrom(ReadableInstant *startInstant);
    
    /**
     * Gets the total millisecond duration of this period relative to an
     * end instant.
     * <p>
     * This method subtracts the period from the specified instant in order
     * to calculate the duration.
     * <p>
     * An instant must be supplied as the duration of a period varies.
     * For example, a period of 1 month could vary between the equivalent of
     * 28 and 31 days in milliseconds due to different length months.
     * Similarly, a day can vary at Daylight Savings cutover, typically between
     * 23 and 25 hours.
     *
     * @param endInstant  the instant to subtract the period from, thus obtaining the duration
     * @return the total length of the period as a duration relative to the end instant
     * @throws ArithmeticException if the millis exceeds the capacity of the duration
     */
    Duration *toDurationTo(ReadableInstant *endInstant);
    
};

CODATIME_END

#endif /* defined(__CodaTime__BasePeriod__) */
