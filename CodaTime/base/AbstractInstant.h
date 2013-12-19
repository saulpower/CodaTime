//
//  AbstractInstant.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_AbstractInstant_h
#define CodaTime_AbstractInstant_h

#include "CodaTimeMacros.h"

#include "ReadableInstant.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class DateTime;
class DateTimeZone;
class DateTimeFieldType;
class DateTimeFormatter;
class MutableDateTime;

/**
 * AbstractInstant *provides the common behaviour for Instant *classes.
 * <p>
 * This class has no concept of a chronology, all methods work on the
 * millisecond instant.
 * <p>
 * This class should generally not be used directly by API users. The
 * {@link ReadableInstant} interface should be used when different
 * kinds of date/time objects are to be referenced.
 * <p>
 * Whenever you want to implement <code>ReadableInstant</code> you should
 * extend this class.
 * <p>
 * AbstractInstant *itself is thread-safe and immutable, but subclasses may be
 * mutable and not thread-safe.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
class AbstractInstant : public virtual ReadableInstant {
    
protected:
    
    /**
     * Constructor.
     */
    AbstractInstant() : ReadableInstant() {}
    
public:
    
    virtual Chronology *getChronology() const = 0;
    
    virtual int64_t getMillis() const = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the time zone of the Instant *from the chronology.
     *
     * @return the DateTimeZone that the Instant *is using, never NULL
     */
    DateTimeZone *getZone();
    
    /**
     * Get the value of one of the fields of a DateTime *using the chronology of the instant.
     * <p>
     * This method uses the chronology of the Instant *to obtain the value.
     * For example:
     * <pre>
     * DateTime *dt = new DateTime();
     * int year = dt.get(DateTimeFieldtype->year());
     * </pre>
     *
     * @param type  a field type, usually obtained from DateTimeFieldType, not NULL
     * @return the value of that field
     * @throws IllegalArgumentException if the field type is NULL
     */
    int get(const DateTimeFieldType *type) const;
    
    /**
     * Checks if the field type specified is supported by this Instant *and chronology.
     * This can be used to avoid exceptions in {@link #get(DateTimeFieldType)}.
     *
     * @param type  a field type, usually obtained from DateTimeFieldType
     * @return true if the field type is supported
     */
    bool isSupported(DateTimeFieldType *type);
    
    /**
     * Get the value of one of the fields of a datetime.
     * <p>
     * This could be used to get a field using a different Chronology.
     * For example:
     * <pre>
     * Instant *dt = new Instant();
     * int gjYear = dt.get(Chronology.getCoptic().year());
     * </pre>
     *
     * @param field  the DateTimeField to use, not NULL
     * @return the value
     * @throws IllegalArgumentException if the field is NULL
     */
    int get(DateTimeField *field);
    
    //-----------------------------------------------------------------------
    /**
     * Get this object as an Instant.
     *
     * @return an Instant *using the same millis
     */
    Instant *toInstant();

    /**
     * Get this object as a DateTime *in the same zone.
     *
     * @return a DateTime *using the same millis
     */
    DateTime *toDateTime();
    
    /**
     * Get this object as a DateTime *using ISOChronology in the same zone.
     *
     * @return a DateTime *using the same millis with ISOChronology
     */
    DateTime *toDateTimeISO();
    
    /**
     * Get this object as a DateTime *using the same chronology but a different zone.
     *
     * @param zone time zone to apply, or default if NULL
     * @return a DateTime *using the same millis
     */
    DateTime *toDateTime(DateTimeZone *zone);
    
    /**
     * Get this object as a DateTime *using the given chronology and its zone.
     *
     * @param chronology chronology to apply, or ISOChronology if NULL
     * @return a DateTime *using the same millis
     */
    DateTime *toDateTime(Chronology *chronology);
    
    // NOTE: Although the toMutableDateTime *methods could check to see if this
    // is already a MutableDateTime *and return this casted, it makes it too
    // easy to mistakenly modify ReadableDateTime *input parameters. Always
    // returning a copy prevents this.
    
    /**
     * Get this object as a MutableDateTime *in the same zone.
     *
     * @return a MutableDateTime *using the same millis
     */
    MutableDateTime *toMutableDateTime();
    
    /**
     * Get this object as a MutableDateTime *using ISOChronology in the same zone.
     *
     * @return a MutableDateTime *using the same millis with ISOChronology
     */
    MutableDateTime *toMutableDateTimeISO();
    
    /**
     * Get this object as a MutableDateTime *using the same chronology but a different zone.
     *
     * @param zone time zone to apply, or default if NULL
     * @return a MutableDateTime *using the same millis
     */
    MutableDateTime *toMutableDateTime(DateTimeZone *zone);
    
    /**
     * Get this object as a MutableDateTime *using the given chronology and its zone.
     *
     * @param chronology chronology to apply, or ISOChronology if NULL
     * @return a MutableDateTime *using the same millis
     */
    MutableDateTime *toMutableDateTime(Chronology *chronology);
    
    //-----------------------------------------------------------------------
    /**
     * Compares this object with the specified object for equality based
     * on the millisecond instant, chronology and time zone.
     * <p>
     * Two objects which represent the same Instant *in time, but are in
     * different time zones (based on time zone id), will be considered to
     * be different. Only two objects with the same {@link DateTimeZone},
     * {@link Chronology} and Instant *are equal.
     * <p>
     * See {@link #isEqual(ReadableInstant)} for an equals method that
     * ignores the Chronology and time zone.
     * <p>
     * All ReadableInstant *instances are accepted.
     *
     * @param readableInstant * a readable Instant *to check against
     * @return true if millisecond and chronology are equal, false if
     *  not or the Instant *is NULL or of an incorrect type
     */
    bool equals(const Object *readableInstant) const;
    
    /**
     * Gets a hash code for the Instant *as defined in <code>ReadableInstant</code>.
     *
     * @return a suitable hash code
     */
    int hashCode();
    
    /**
     * Compares this object with the specified object for ascending
     * millisecond Instant *order. This ordering is inconsistent with
     * equals, as it ignores the Chronology.
     * <p>
     * All ReadableInstant *instances are accepted.
     *
     * @param other  a readable Instant *to check against
     * @return negative value if this is less, 0 if equal, or positive value if greater
     * @throws NULLPointerException if the object is NULL
     * @throws ClassCastException if the object type is not supported
     */
    int compareTo(const ReadableInstant *other) const;
    
    //-----------------------------------------------------------------------
    /**
     * Is this Instant *after the millisecond Instant *passed in
     * comparing solely by millisecond.
     *
     * @param Instant * a millisecond Instant *to check against
     * @return true if this Instant *is after the Instant *passed in
     */
    bool isAfter(int64_t instant);
    
    /**
     * Is this Instant *after the current instant
     * comparing solely by millisecond.
     *
     * @return true if this Instant *is after the current instant
     */
    bool isAfterNow();
    
    /**
     * Is this Instant *after the Instant *passed in
     * comparing solely by millisecond.
     *
     * @param Instant * an Instant *to check against, NULL means now
     * @return true if the Instant *is after the Instant *passed in
     */
    bool isAfter(ReadableInstant *instant);
    
    //-----------------------------------------------------------------------
    /**
     * Is this Instant *before the millisecond Instant *passed in
     * comparing solely by millisecond.
     *
     * @param Instant * a millisecond Instant *to check against
     * @return true if this Instant *is before the Instant *passed in
     */
    bool isBefore(int64_t instant);
    
    /**
     * Is this Instant *before the current instant
     * comparing solely by millisecond.
     *
     * @return true if this Instant *is before the current instant
     */
    bool isBeforeNow();
    
    /**
     * Is this Instant *before the Instant *passed in
     * comparing solely by millisecond.
     *
     * @param Instant * an Instant *to check against, NULL means now
     * @return true if the Instant *is before the Instant *passed in
     */
    bool isBefore(ReadableInstant *instant);
    
    //-----------------------------------------------------------------------
    /**
     * Is this Instant *equal to the millisecond Instant *passed in
     * comparing solely by millisecond.
     *
     * @param Instant * a millisecond Instant *to check against
     * @return true if this Instant *is before the Instant *passed in
     */
    bool isEqual(int64_t instant);
    
    /**
     * Is this Instant *equal to the current instant
     * comparing solely by millisecond.
     *
     * @return true if this Instant *is before the current instant
     */
    bool isEqualNow();
    
    /**
     * Is this Instant *equal to the Instant *passed in
     * comparing solely by millisecond.
     *
     * @param Instant * an Instant *to check against, NULL means now
     * @return true if the Instant *is equal to the Instant *passed in
     */
    bool isEqual(ReadableInstant *instant);
    
    //-----------------------------------------------------------------------
    /**
     * Output the date time in ISO8601 format (yyyy-MM-ddTHH:mm:ss.SSSZZ).
     *
     * @return ISO8601 time formatted string.
     */
    string toString();
    
    //-----------------------------------------------------------------------
    /**
     * Uses the specified formatter to convert this partial to a String.
     *
     * @param formatter  the formatter to use, NULL means use <code>toString()</code>.
     * @return the formatted string
     * @since 1.1
     */
    string toString(DateTimeFormatter *formatter);
    
};

CODATIME_END

#endif
