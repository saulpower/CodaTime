//
//  ReadWritableInstant.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_ReadWritableInstant_h
#define CodaTime_ReadWritableInstant_h

#include "CodaTimeMacros.h"

#include "ReadableInstant.h"

CODATIME_BEGIN

class Chronology;
class DateTimeZone;
class ReadableDuration;
class ReadablePeriod;
class DateTimeFieldType;
class DurationFieldType;

/**
 * Defines an instant in the datetime continuum that can be queried and modified.
 * This interface expresses the datetime as milliseconds from 1970-01-01T00:00:00Z.
 * <p>
 * The implementation of this interface will be mutable.
 * It may provide more advanced methods than those in the interface.
 *
 * @author Stephen Colebourne
 * @since 1.0
 */
class ReadWritableInstant : public ReadableInstant {
    
public:
    
    /**
     * Sets the value as the number of milliseconds since
     * the epoch, 1970-01-01T00:00:00Z.
     *
     * @param instant  the milliseconds since 1970-01-01T00:00:00Z to set the
     * instant to
     * @throws IllegalArgumentException if the value is invalid
     */
    virtual void setMillis(int64_t instant) = 0;
    
    /**
     * Sets the millisecond instant of this instant from another.
     * <p>
     * This method does not change the chronology of this instant, just the
     * millisecond instant.
     *
     * @param instant  the instant to use, null means now
     */
    virtual void setMillis(ReadableInstant *instant) = 0;
    
    /**
     * Sets the chronology of the datetime, which has no effect if not applicable.
     *
     * @param chronology  the chronology to use, null means ISOChronology in default zone
     * @throws IllegalArgumentException if the value is invalid
     */
    virtual void setChronology(Chronology *chronology) = 0;
    
    /**
     * Sets the time zone of the datetime, changing the chronology and field values.
     * <p>
     * Changing the zone using this method retains the millisecond instant.
     * The millisecond instant is adjusted in the new zone to compensate.
     *
     * chronology. Setting the time zone does not affect the millisecond value
     * of this instant.
     * <p>
     * If the chronology already has this time zone, no change occurs.
     *
     * @param zone  the time zone to use, null means default zone
     * @see #setZoneRetainFields
     */
    virtual void setZone(DateTimeZone *zone) = 0;
    
    /**
     * Sets the time zone of the datetime, changing the chronology and millisecond.
     * <p>
     * Changing the zone using this method retains the field values.
     * The millisecond instant is adjusted in the new zone to compensate.
     * <p>
     * If the chronology already has this time zone, no change occurs.
     *
     * @param zone  the time zone to use, null means default zone
     * @see #setZone
     */
    virtual void setZoneRetainFields(DateTimeZone *zone) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Adds a millisecond duration to this instant.
     * <p>
     * This will typically change the value of ost fields.
     *
     * @param duration  the millis to add
     * @throws IllegalArgumentException if the value is invalid
     */
    virtual void add(int64_t duration) = 0;
    
    /**
     * Adds a duration to this instant.
     * <p>
     * This will typically change the value of most fields.
     *
     * @param duration  the duration to add, null means add zero
     * @throws ArithmeticException if the result exceeds the capacity of the instant
     */
    virtual void add(ReadableDuration *duration) = 0;
    
    /**
     * Adds a duration to this instant specifying how many times to add.
     * <p>
     * This will typically change the value of most fields.
     *
     * @param duration  the duration to add, null means add zero
     * @param scalar  direction and amount to add, which may be negative
     * @throws ArithmeticException if the result exceeds the capacity of the instant
     */
    virtual void add(ReadableDuration *duration, int scalar) = 0;
    
    /**
     * Adds a period to this instant.
     * <p>
     * This will typically change the value of most fields.
     *
     * @param period  the period to add, null means add zero
     * @throws ArithmeticException if the result exceeds the capacity of the instant
     */
    virtual void add(ReadablePeriod *period) = 0;
    
    /**
     * Adds a period to this instant specifying how many times to add.
     * <p>
     * This will typically change the value of most fields.
     *
     * @param period  the period to add, null means add zero
     * @param scalar  direction and amount to add, which may be negative
     * @throws ArithmeticException if the result exceeds the capacity of the instant
     */
    virtual void add(ReadablePeriod *period, int scalar) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Sets the value of one of the fields of the instant, such as hourOfDay.
     *
     * @param type  a field type, usually obtained from DateTimeFieldType, null ignored
     * @param value  the value to set the field to
     * @throws IllegalArgumentException if the value is invalid
     */
    virtual void set(DateTimeFieldType *type, int value) = 0;
    
    /**
     * Adds to the instant specifying the duration and multiple to add.
     *
     * @param type  a field type, usually obtained from DateTimeFieldType, null ignored
     * @param amount  the amount to add of this duration
     * @throws ArithmeticException if the result exceeds the capacity of the instant
     */
    virtual void add(DurationFieldType *type, int amount) = 0;
    
};

CODATIME_END

#endif
