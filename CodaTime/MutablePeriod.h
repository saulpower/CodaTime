//
//  MutablePeriod.h
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__MutablePeriod__
#define __CodaTime__MutablePeriod__

#include "CodaTimeMacros.h"

#include "base/BasePeriod.h"
#include "ReadWritablePeriod.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Chronology;
class DurationFieldType;
class PeriodType;
class PeriodFormatter;
class ReadableDuration;
class ReadableInstant;
class ReadablePeriod;

/**
 * Standard mutable time period implementation.
 * <p>
 * A time period is divided into a number of fields, such as hours and seconds.
 * Which fields are supported is defined by the PeriodType class.
 * The default is the standard period type, which supports years, months, weeks, days,
 * hours, minutes, seconds and millis.
 * <p>
 * When this time period is added to an instant, the effect is of adding each field in turn.
 * As a result, this takes into account daylight savings time.
 * Adding a time period of 1 day to the day before daylight savings starts will only add
 * 23 hours rather than 24 to ensure that the time remains the same.
 * If this is not the behaviour you want, then see {@link Duration}.
 * <p>
 * The definition of a period also affects the equals method. A period of 1
 * day is not equal to a period of 24 hours, nor 1 hour equal to 60 minutes.
 * This is because periods represent an abstracted definition of a time period
 * (eg. a day may not actually be 24 hours, it might be 23 or 25 at daylight
 * savings boundary). To compare the actual duration of two periods, convert
 * both to durations using toDuration, an operation that emphasises that the
 * result may differ according to the date you choose.
 * <p>
 * MutablePeriod is mutable and not thread-safe, unless concurrent threads
 * are not invoking mutator methods.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 * @see Period
 */
class MutablePeriod : public BasePeriod, public virtual ReadWritablePeriod {
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = 3436451121567212165L;
    
public:
    
    //-----------------------------------------------------------------------
    /**
     * Parses a {@code MutablePeriod} from the specified string.
     * <p>
     * This uses {@link ISOPeriodFormat#standard()}.
     *
     * @param str  the string to parse, not null
     * @since 2.0
     */
    static MutablePeriod *parse(string str);
    
    /**
     * Parses a {@code MutablePeriod} from the specified string using a formatter.
     *
     * @param str  the string to parse, not null
     * @param formatter  the formatter to use, not null
     * @since 2.0
     */
    static MutablePeriod *parse(string str, PeriodFormatter *formatter);
    
    //-----------------------------------------------------------------------
    /**
     * Creates a zero-length period using the standard period type.
     */
    MutablePeriod();
    
    /**
     * Creates a zero-length period using the specified period type.
     *
     * @param type  which set of fields this period supports
     */
    MutablePeriod(const PeriodType *type);
    
    /**
     * Create a period from a set of field values using the standard set of fields.
     *
     * @param hours  amount of hours in this period
     * @param minutes  amount of minutes in this period
     * @param seconds  amount of seconds in this period
     * @param millis  amount of milliseconds in this period
     */
    MutablePeriod(int hours, int minutes, int seconds, int millis);
    
    /**
     * Create a period from a set of field values using the standard set of fields.
     *
     * @param years  amount of years in this period
     * @param months  amount of months in this period
     * @param weeks  amount of weeks in this period
     * @param days  amount of days in this period
     * @param hours  amount of hours in this period
     * @param minutes  amount of minutes in this period
     * @param seconds  amount of seconds in this period
     * @param millis  amount of milliseconds in this period
     */
    MutablePeriod(int years, int months, int weeks, int days,
                         int hours, int minutes, int seconds, int millis);
    
    /**
     * Create a period from a set of field values.
     *
     * @param years  amount of years in this period, which must be zero if unsupported
     * @param months  amount of months in this period, which must be zero if unsupported
     * @param weeks  amount of weeks in this period, which must be zero if unsupported
     * @param days  amount of days in this period, which must be zero if unsupported
     * @param hours  amount of hours in this period, which must be zero if unsupported
     * @param minutes  amount of minutes in this period, which must be zero if unsupported
     * @param seconds  amount of seconds in this period, which must be zero if unsupported
     * @param millis  amount of milliseconds in this period, which must be zero if unsupported
     * @param type  which set of fields this period supports, null means AllType
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    MutablePeriod(int years, int months, int weeks, int days,
                         int hours, int minutes, int seconds, int millis, const PeriodType *type);
    
    /**
     * Creates a period from the given millisecond duration using the standard
     * set of fields.
     * <p>
     * Only precise fields in the period type will be used.
     * For the standard period type this is the time fields only.
     * Thus the year, month, week and day fields will not be populated.
     * <p>
     * If the duration is small, less than one day, then this method will perform
     * as you might expect and split the fields evenly.
     * <p>
     * If the duration is larger than one day then all the remaining duration will
     * be stored in the largest available precise field, hours in this case.
     * <p>
     * For example, a duration equal to (365 + 60 + 5) days will be converted to
     * ((365 + 60 + 5) * 24) hours by this constructor.
     * <p>
     * For more control over the conversion process, you have two options:
     * <ul>
     * <li>convert the duration to an {@link Interval}, and from there obtain the period
     * <li>specify a period type that contains precise definitions of the day and larger
     * fields, such as the UTC or precise types.
     * </ul>
     *
     * @param duration  the duration, in milliseconds
     */
    MutablePeriod(int64_t duration);
    
    /**
     * Creates a period from the given millisecond duration.
     * <p>
     * Only precise fields in the period type will be used.
     * Imprecise fields will not be populated.
     * <p>
     * If the duration is small then this method will perform
     * as you might expect and split the fields evenly.
     * <p>
     * If the duration is large then all the remaining duration will
     * be stored in the largest available precise field.
     * For details as to which fields are precise, review the period type javadoc.
     *
     * @param duration  the duration, in milliseconds
     * @param type  which set of fields this period supports, null means standard
     */
    MutablePeriod(int64_t duration, const PeriodType *type);
    
    /**
     * Creates a period from the given millisecond duration using the standard
     * set of fields.
     * <p>
     * Only precise fields in the period type will be used.
     * Imprecise fields will not be populated.
     * <p>
     * If the duration is small then this method will perform
     * as you might expect and split the fields evenly.
     * <p>
     * If the duration is large then all the remaining duration will
     * be stored in the largest available precise field.
     * For details as to which fields are precise, review the period type javadoc.
     *
     * @param duration  the duration, in milliseconds
     * @param chronology  the chronology to use to split the duration, null means ISO default
     */
    MutablePeriod(int64_t duration, Chronology *chronology);
    
    /**
     * Creates a period from the given millisecond duration.
     * <p>
     * Only precise fields in the period type will be used.
     * Imprecise fields will not be populated.
     * <p>
     * If the duration is small then this method will perform
     * as you might expect and split the fields evenly.
     * <p>
     * If the duration is large then all the remaining duration will
     * be stored in the largest available precise field.
     * For details as to which fields are precise, review the period type javadoc.
     *
     * @param duration  the duration, in milliseconds
     * @param type  which set of fields this period supports, null means standard
     * @param chronology  the chronology to use to split the duration, null means ISO default
     */
    MutablePeriod(int64_t duration, const PeriodType *type, Chronology *chronology);
    
    /**
     * Creates a period from the given interval endpoints using the standard
     * set of fields.
     *
     * @param startInstant  interval start, in milliseconds
     * @param endInstant  interval end, in milliseconds
     */
    MutablePeriod(int64_t startInstant, int64_t endInstant);
    
    /**
     * Creates a period from the given interval endpoints.
     *
     * @param startInstant  interval start, in milliseconds
     * @param endInstant  interval end, in milliseconds
     * @param type  which set of fields this period supports, null means standard
     */
    MutablePeriod(int64_t startInstant, int64_t endInstant, const PeriodType *type);
    
    /**
     * Creates a period from the given interval endpoints using the standard
     * set of fields.
     *
     * @param startInstant  interval start, in milliseconds
     * @param endInstant  interval end, in milliseconds
     * @param chrono  the chronology to use, null means ISO in default zone
     */
    MutablePeriod(int64_t startInstant, int64_t endInstant, Chronology *chrono);
    
    /**
     * Creates a period from the given interval endpoints.
     *
     * @param startInstant  interval start, in milliseconds
     * @param endInstant  interval end, in milliseconds
     * @param type  which set of fields this period supports, null means standard
     * @param chrono  the chronology to use, null means ISO in default zone
     */
    MutablePeriod(int64_t startInstant, int64_t endInstant, const PeriodType *type, Chronology *chrono);
    
    /**
     * Creates a period from the given interval endpoints using the standard
     * set of fields.
     * <p>
     * The chronology of the start instant is used, unless that is null when the
     * chronology of the end instant is used instead.
     *
     * @param startInstant  interval start, null means now
     * @param endInstant  interval end, null means now
     */
    MutablePeriod(ReadableInstant *startInstant, ReadableInstant *endInstant);
    
    /**
     * Creates a period from the given interval endpoints.
     * <p>
     * The chronology of the start instant is used, unless that is null when the
     * chronology of the end instant is used instead.
     *
     * @param startInstant  interval start, null means now
     * @param endInstant  interval end, null means now
     * @param type  which set of fields this period supports, null means AllType
     */
    MutablePeriod(ReadableInstant *startInstant, ReadableInstant *endInstant, const PeriodType *type);
    
    /**
     * Creates a period from the given start point and the duration.
     *
     * @param startInstant  the interval start, null means now
     * @param duration  the duration of the interval, null means zero-length
     */
    MutablePeriod(ReadableInstant *startInstant, ReadableDuration *duration);
    
    /**
     * Creates a period from the given start point and the duration.
     *
     * @param startInstant  the interval start, null means now
     * @param duration  the duration of the interval, null means zero-length
     * @param type  which set of fields this period supports, null means standard
     */
    MutablePeriod(ReadableInstant *startInstant, ReadableDuration *duration, const PeriodType *type);
    
    /**
     * Creates a period from the given duration and end point.
     *
     * @param duration  the duration of the interval, null means zero-length
     * @param endInstant  the interval end, null means now
     */
    MutablePeriod(ReadableDuration *duration, ReadableInstant *endInstant);
    
    /**
     * Creates a period from the given duration and end point.
     *
     * @param duration  the duration of the interval, null means zero-length
     * @param endInstant  the interval end, null means now
     * @param type  which set of fields this period supports, null means standard
     */
    MutablePeriod(ReadableDuration *duration, ReadableInstant *endInstant, const PeriodType *type);
    
    /**
     * Creates a period by converting or copying from another object.
     * <p>
     * The recognised object types are defined in
     * {@link org.joda.time.convert.ConverterManager ConverterManager} and
     * include ReadablePeriod, ReadableInterval and String.
     * The string formats are described by {@link ISOPeriodFormat#standard()}.
     *
     * @param period  period to convert
     * @throws IllegalArgumentException if period is invalid
     * @throws UnsupportedOperationException if an unsupported field's value is non-zero
     */
    MutablePeriod(Object *period);
    
    /**
     * Creates a period by converting or copying from another object.
     * <p>
     * The recognised object types are defined in
     * {@link org.joda.time.convert.ConverterManager ConverterManager} and
     * include ReadablePeriod, ReadableInterval and String.
     * The string formats are described by {@link ISOPeriodFormat#standard()}.
     *
     * @param period  period to convert
     * @param type  which set of fields this period supports, null means use converter
     * @throws IllegalArgumentException if period is invalid
     * @throws UnsupportedOperationException if an unsupported field's value is non-zero
     */
    MutablePeriod(Object *period, const PeriodType *type);
    
    /**
     * Creates a period by converting or copying from another object.
     * <p>
     * The recognised object types are defined in
     * {@link org.joda.time.convert.ConverterManager ConverterManager} and
     * include ReadablePeriod, ReadableInterval and String.
     * The string formats are described by {@link ISOPeriodFormat#standard()}.
     *
     * @param period  period to convert
     * @param chrono  the chronology to use, null means ISO in default zone
     * @throws IllegalArgumentException if period is invalid
     * @throws UnsupportedOperationException if an unsupported field's value is non-zero
     */
    MutablePeriod(Object *period, Chronology *chrono);
    
    /**
     * Creates a period by converting or copying from another object.
     * <p>
     * The recognised object types are defined in
     * {@link org.joda.time.convert.ConverterManager ConverterManager} and
     * include ReadablePeriod, ReadableInterval and String.
     * The string formats are described by {@link ISOPeriodFormat#standard()}.
     *
     * @param period  period to convert
     * @param type  which set of fields this period supports, null means use converter
     * @param chrono  the chronology to use, null means ISO in default zone
     * @throws IllegalArgumentException if period is invalid
     * @throws UnsupportedOperationException if an unsupported field's value is non-zero
     */
    MutablePeriod(Object *period, const PeriodType *type, Chronology *chrono);
    
    //-----------------------------------------------------------------------
    /**
     * Clears the period, setting all values back to zero.
     */
    void clear();
    
    /**
     * Sets the value of one of the fields by index.
     *
     * @param index  the field index
     * @param value  the new value for the field
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    void setValue(int index, int value);
    
    /**
     * Sets the value of one of the fields.
     * <p>
     * The field type specified must be one of those that is supported by the period.
     *
     * @param field  a DurationFieldType instance that is supported by this period, not null
     * @param value  the new value for the field
     * @throws IllegalArgumentException if the field is null or not supported
     */
    void set(DurationFieldType *field, int value);
    
    /**
     * Sets all the fields in one go from another ReadablePeriod.
     *
     * @param period  the period to set, null means zero length period
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    void setPeriod(ReadablePeriod *period);
    
    /**
     * Sets all the fields in one go.
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
    
    /**
     * Sets all the fields in one go from an interval using the ISO chronology
     * and dividing the fields using the period type.
     *
     * @param interval  the interval to set, null means zero length
     * @throws ArithmeticException if the set exceeds the capacity of the period
     */
    void setPeriod(ReadableInterval *interval);
    
    /**
     * Sets all the fields in one go from two instants representing an interval.
     * <p>
     * The chronology of the start instant is used, unless that is null when the
     * chronology of the end instant is used instead.
     *
     * @param start  the start instant, null means now
     * @param end  the end instant, null means now
     * @throws ArithmeticException if the set exceeds the capacity of the period
     */
    void setPeriod(ReadableInstant *start, ReadableInstant *end);
    
    /**
     * Sets all the fields in one go from a millisecond interval using ISOChronology
     * and dividing the fields using the period type.
     *
     * @param startInstant  interval start, in milliseconds
     * @param endInstant  interval end, in milliseconds
     * @throws ArithmeticException if the set exceeds the capacity of the period
     */
    void setPeriod(int64_t startInstant, int64_t endInstant);
    
    /**
     * Sets all the fields in one go from a millisecond interval.
     *
     * @param startInstant  interval start, in milliseconds
     * @param endInstant  interval end, in milliseconds
     * @param chrono  the chronology to use, null means ISO chronology
     * @throws ArithmeticException if the set exceeds the capacity of the period
     */
    void setPeriod(int64_t startInstant, int64_t endInstant, Chronology *chrono);
    
    /**
     * Sets all the fields in one go from a duration dividing the
     * fields using the period type.
     * <p>
     * When dividing the duration, only precise fields in the period type will be used.
     * For large durations, all the remaining duration will be stored in the largest
     * available precise field.
     *
     * @param duration  the duration to set, null means zero length
     * @throws ArithmeticException if the set exceeds the capacity of the period
     */
    void setPeriod(ReadableDuration *duration);
    
    /**
     * Sets all the fields in one go from a duration dividing the
     * fields using the period type.
     * <p>
     * When dividing the duration, only precise fields in the period type will be used.
     * For large durations, all the remaining duration will be stored in the largest
     * available precise field.
     *
     * @param duration  the duration to set, null means zero length
     * @param chrono  the chronology to use, null means ISO default
     * @throws ArithmeticException if the set exceeds the capacity of the period
     */
    void setPeriod(ReadableDuration *duration, Chronology *chrono);
    
    /**
     * Sets all the fields in one go from a millisecond duration dividing the
     * fields using the period type.
     * <p>
     * When dividing the duration, only precise fields in the period type will be used.
     * For large durations, all the remaining duration will be stored in the largest
     * available precise field.
     *
     * @param duration  the duration, in milliseconds
     * @throws ArithmeticException if the set exceeds the capacity of the period
     */
    void setPeriod(int64_t duration);
    
    /**
     * Sets all the fields in one go from a millisecond duration.
     * <p>
     * When dividing the duration, only precise fields in the period type will be used.
     * For large durations, all the remaining duration will be stored in the largest
     * available precise field.
     *
     * @param duration  the duration, in milliseconds
     * @param chrono  the chronology to use, null means ISO chronology
     * @throws ArithmeticException if the set exceeds the capacity of the period
     */
    void setPeriod(int64_t duration, Chronology *chrono);
    
    //-----------------------------------------------------------------------
    /**
     * Adds to the value of one of the fields.
     * <p>
     * The field type specified must be one of those that is supported by the period.
     *
     * @param field  a DurationFieldType instance that is supported by this period, not null
     * @param value  the value to add to the field
     * @throws IllegalArgumentException if the field is null or not supported
     */
    void add(DurationFieldType *field, int value);
    
    /**
     * Adds a period to this one by adding each field in turn.
     *
     * @param period  the period to add, null means add nothing
     * @throws IllegalArgumentException if the period being added contains a field
     * not supported by this period
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void add(ReadablePeriod *period);
    
    /**
     * Adds to each field of this period.
     *
     * @param years  amount of years to add to this period, which must be zero if unsupported
     * @param months  amount of months to add to this period, which must be zero if unsupported
     * @param weeks  amount of weeks to add to this period, which must be zero if unsupported
     * @param days  amount of days to add to this period, which must be zero if unsupported
     * @param hours  amount of hours to add to this period, which must be zero if unsupported
     * @param minutes  amount of minutes to add to this period, which must be zero if unsupported
     * @param seconds  amount of seconds to add to this period, which must be zero if unsupported
     * @param millis  amount of milliseconds to add to this period, which must be zero if unsupported
     * @throws IllegalArgumentException if the period being added contains a field
     * not supported by this period
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void add(int years, int months, int weeks, int days,
                    int hours, int minutes, int seconds, int millis);
    
    /**
     * Adds an interval to this one by dividing the interval into
     * fields and calling {@link #add(ReadablePeriod)}.
     *
     * @param interval  the interval to add, null means add nothing
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void add(ReadableInterval *interval);
    
    /**
     * Adds a duration to this one by dividing the duration into
     * fields and calling {@link #add(ReadablePeriod)}.
     *
     * @param duration  the duration to add, null means add nothing
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void add(ReadableDuration *duration);
    
    /**
     * Adds a millisecond duration to this one by dividing the duration into
     * fields and calling {@link #add(ReadablePeriod)}.
     * <p>
     * When dividing the duration, only precise fields in the period type will be used.
     * For large durations, all the remaining duration will be stored in the largest
     * available precise field.
     *
     * @param duration  the duration, in milliseconds
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void add(int64_t duration);
    
    /**
     * Adds a millisecond duration to this one by dividing the duration into
     * fields and calling {@link #add(ReadablePeriod)}.
     * <p>
     * When dividing the duration, only precise fields in the period type will be used.
     * For large durations, all the remaining duration will be stored in the largest
     * available precise field.
     *
     * @param duration  the duration, in milliseconds
     * @param chrono  the chronology to use, null means ISO default
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void add(int64_t duration, Chronology *chrono);
    
    //-----------------------------------------------------------------------
    /**
     * Merges all the fields from the specified period into this one.
     * <p>
     * Fields that are not present in the specified period are left unaltered.
     *
     * @param period  the period to set, null ignored
     * @throws IllegalArgumentException if an unsupported field's value is non-zero
     */
    void mergePeriod(ReadablePeriod *period);
    
    //-----------------------------------------------------------------------
    /**
     * Gets the years field part of the period.
     *
     * @return the number of years in the period, zero if unsupported
     */
    int getYears();
    
    /**
     * Gets the months field part of the period.
     *
     * @return the number of months in the period, zero if unsupported
     */
    int getMonths();
    
    /**
     * Gets the weeks field part of the period.
     *
     * @return the number of weeks in the period, zero if unsupported
     */
    int getWeeks();
    
    /**
     * Gets the days field part of the period.
     *
     * @return the number of days in the period, zero if unsupported
     */
    int getDays();
    
    //-----------------------------------------------------------------------
    /**
     * Gets the hours field part of the period.
     *
     * @return the number of hours in the period, zero if unsupported
     */
    int getHours();
    
    /**
     * Gets the minutes field part of the period.
     *
     * @return the number of minutes in the period, zero if unsupported
     */
    int getMinutes();
    
    /**
     * Gets the seconds field part of the period.
     *
     * @return the number of seconds in the period, zero if unsupported
     */
    int getSeconds();
    
    /**
     * Gets the millis field part of the period.
     *
     * @return the number of millis in the period, zero if unsupported
     */
    int getMillis();
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of years of the period.
     *
     * @param years  the number of years
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    void setYears(int years);
    
    /**
     * Adds the specified years to the number of years in the period.
     *
     * @param years  the number of years
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void addYears(int years);
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of months of the period.
     *
     * @param months  the number of months
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    void setMonths(int months);
    
    /**
     * Adds the specified months to the number of months in the period.
     *
     * @param months  the number of months
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void addMonths(int months);
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of weeks of the period.
     *
     * @param weeks  the number of weeks
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    void setWeeks(int weeks);
    
    /**
     * Adds the specified weeks to the number of weeks in the period.
     *
     * @param weeks  the number of weeks
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void addWeeks(int weeks);
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of days of the period.
     *
     * @param days  the number of days
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    void setDays(int days);
    
    /**
     * Adds the specified days to the number of days in the period.
     *
     * @param days  the number of days
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void addDays(int days);
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of hours of the period.
     *
     * @param hours  the number of hours
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    void setHours(int hours);
    
    /**
     * Adds the specified hours to the number of hours in the period.
     *
     * @param hours  the number of hours
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void addHours(int hours);
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of minutes of the period.
     *
     * @param minutes  the number of minutes
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    void setMinutes(int minutes);
    
    /**
     * Adds the specified minutes to the number of minutes in the period.
     *
     * @param minutes  the number of minutes
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void addMinutes(int minutes);
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of seconds of the period.
     *
     * @param seconds  the number of seconds
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    void setSeconds(int seconds);
    
    /**
     * Adds the specified seconds to the number of seconds in the period.
     *
     * @param seconds  the number of seconds
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void addSeconds(int seconds);
    
    //-----------------------------------------------------------------------
    /**
     * Sets the number of millis of the period.
     *
     * @param millis  the number of millis
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     */
    void setMillis(int millis);
    
    /**
     * Adds the specified millis to the number of millis in the period.
     *
     * @param millis  the number of millis
     * @throws IllegalArgumentException if field is not supported and the value is non-zero
     * @throws ArithmeticException if the addition exceeds the capacity of the period
     */
    void addMillis(int millis);
    
    // Misc
    //-----------------------------------------------------------------------
    /**
     * Clone this object without having to cast the returned object.
     *
     * @return a clone of the this object.
     */
//    MutablePeriod *copy();

    /**
     * Clone this object.
     *
     * @return a clone of this object.
     */
//    Object *clone(); catch (CloneNotSupportedException ex);

};

CODATIME_END

#endif /* defined(__CodaTime__MutablePeriod__) */
