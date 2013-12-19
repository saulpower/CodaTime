//
//  DateTimeFieldType.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_DateTimeFieldType_h
#define CodaTime_DateTimeFieldType_h

#include "CodaTimeMacros.h"

#include "Exceptions.h"
#include "Object.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class DateTimeField;
class DurationFieldType;
class StandardDateTimeFieldType;
class Chronology;

/**
 * Identifies a field, such as year or minuteOfHour, in a chronology-neutral way.
 * <p>
 * A field type defines the type of the field, such as hourOfDay.
 * If does not directly enable any calculations, however it does provide a
 * {@link #getField(Chronology)} method that returns the actual calculation engine
 * for a particular chronology->
 * It also provides access to the related {@link DurationFieldType}s.
 * <p>
 * Instances of <code>DateTimeFieldType</code> are singletons.
 * They can be compared using <code>==</code>.
 * <p>
 * If required, you can create your own field, for example a quarterOfYear.
 * You must create a subclass of <code>DateTimeFieldType</code> that defines the field type.
 * This class returns the actual calculation engine from {@link #getField(Chronology)}.
 * The subclass should implement equals and hashCode.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
class DateTimeFieldType : public virtual Object {
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = -42615285973990L;
    
    /** The name of the field. */
    string iName;
    
protected:
    
    /** The era field type. */
    static const DateTimeFieldType *ERA_TYPE;
    /** The yearOfEra field type. */
    static const DateTimeFieldType *YEAR_OF_ERA_TYPE;
    /** The centuryOfEra field type. */
    static const DateTimeFieldType *CENTURY_OF_ERA_TYPE;
    /** The yearOfCentury field type. */
    static const DateTimeFieldType *YEAR_OF_CENTURY_TYPE;
    /** The year field type. */
    static const DateTimeFieldType *YEAR_TYPE;
    /** The dayOfYear field type. */
    static const DateTimeFieldType *DAY_OF_YEAR_TYPE;
    /** The monthOfYear field type. */
    static const DateTimeFieldType *MONTH_OF_YEAR_TYPE;
    /** The dayOfMonth field type. */
    static const DateTimeFieldType *DAY_OF_MONTH_TYPE;
    /** The weekyearOfCentury field type. */
    static const DateTimeFieldType *WEEKYEAR_OF_CENTURY_TYPE;
    /** The weekyear field type. */
    static const DateTimeFieldType *WEEKYEAR_TYPE;
    /** The weekOfWeekyear field type. */
    static const DateTimeFieldType *WEEK_OF_WEEKYEAR_TYPE;
    /** The dayOfWeek field type. */
    static const DateTimeFieldType *DAY_OF_WEEK_TYPE;
    
    /** The halfday field type. */
    static const DateTimeFieldType *HALFDAY_OF_DAY_TYPE;
    /** The hourOfHalfday field type. */
    static const DateTimeFieldType *HOUR_OF_HALFDAY_TYPE;
    /** The clockhourOfHalfday field type. */
    static const DateTimeFieldType *CLOCKHOUR_OF_HALFDAY_TYPE;
    /** The clockhourOfDay field type. */
    static const DateTimeFieldType *CLOCKHOUR_OF_DAY_TYPE;
    /** The hourOfDay field type. */
    static const DateTimeFieldType *HOUR_OF_DAY_TYPE;
    /** The minuteOfDay field type. */
    static const DateTimeFieldType *MINUTE_OF_DAY_TYPE;
    /** The minuteOfHour field type. */
    static const DateTimeFieldType *MINUTE_OF_HOUR_TYPE;
    /** The secondOfDay field type. */
    static const DateTimeFieldType *SECOND_OF_DAY_TYPE;
    /** The secondOfMinute field type. */
    static const DateTimeFieldType *SECOND_OF_MINUTE_TYPE;
    /** The millisOfDay field type. */
    static const DateTimeFieldType *MILLIS_OF_DAY_TYPE;
    /** The millisOfSecond field type. */
    static const DateTimeFieldType *MILLIS_OF_SECOND_TYPE;
    
    //-----------------------------------------------------------------------
    /**
     * Constructor.
     *
     * @param name  the name to use
     */
    DateTimeFieldType(string name) {
        iName = name;
    }
    
public:
    
    /** Ordinal values for standard field types. */
    static const unsigned char
    ERA = 1,
    YEAR_OF_ERA = 2,
    CENTURY_OF_ERA = 3,
    YEAR_OF_CENTURY = 4,
    YEAR = 5,
    DAY_OF_YEAR = 6,
    MONTH_OF_YEAR = 7,
    DAY_OF_MONTH = 8,
    WEEKYEAR_OF_CENTURY = 9,
    WEEKYEAR = 10,
    WEEK_OF_WEEKYEAR = 11,
    DAY_OF_WEEK = 12,
    HALFDAY_OF_DAY = 13,
    HOUR_OF_HALFDAY = 14,
    CLOCKHOUR_OF_HALFDAY = 15,
    CLOCKHOUR_OF_DAY = 16,
    HOUR_OF_DAY = 17,
    MINUTE_OF_DAY = 18,
    MINUTE_OF_HOUR = 19,
    SECOND_OF_DAY = 20,
    SECOND_OF_MINUTE = 21,
    MILLIS_OF_DAY = 22,
    MILLIS_OF_SECOND = 23;
    
    //-----------------------------------------------------------------------
    /**
     * Get the millis of second field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DateTimeFieldType *millisOfSecond() {
        return MILLIS_OF_SECOND_TYPE;
    }
    
    /**
     * Get the millis of day field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *millisOfDay() {
        return MILLIS_OF_DAY_TYPE;
    }
    
    /**
     * Get the second of minute field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *secondOfMinute() {
        return SECOND_OF_MINUTE_TYPE;
    }
    
    /**
     * Get the second of day field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *secondOfDay() {
        return SECOND_OF_DAY_TYPE;
    }
    
    /**
     * Get the minute of hour field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *minuteOfHour() {
        return MINUTE_OF_HOUR_TYPE;
    }
    
    /**
     * Get the minute of day field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *minuteOfDay() {
        return MINUTE_OF_DAY_TYPE;
    }
    
    /**
     * Get the hour of day (0-23) field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *hourOfDay() {
        return HOUR_OF_DAY_TYPE;
    }
    
    /**
     * Get the hour of day (offset to 1-24) field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *clockhourOfDay() {
        return CLOCKHOUR_OF_DAY_TYPE;
    }
    
    /**
     * Get the hour of am/pm (0-11) field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *hourOfHalfday() {
        return HOUR_OF_HALFDAY_TYPE;
    }
    
    /**
     * Get the hour of am/pm (offset to 1-12) field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *clockhourOfHalfday() {
        return CLOCKHOUR_OF_HALFDAY_TYPE;
    }
    
    /**
     * Get the AM(0) PM(1) field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *halfdayOfDay() {
        return HALFDAY_OF_DAY_TYPE;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Get the day of week field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *dayOfWeek() {
        return DAY_OF_WEEK_TYPE;
    }
    
    /**
     * Get the day of month field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *dayOfMonth() {
        return DAY_OF_MONTH_TYPE;
    }
    
    /**
     * Get the day of year field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *dayOfYear() {
        return DAY_OF_YEAR_TYPE;
    }
    
    /**
     * Get the week of a week based year field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *weekOfWeekyear() {
        return WEEK_OF_WEEKYEAR_TYPE;
    }
    
    /**
     * Get the year of a week based year field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *weekyear() {
        return WEEKYEAR_TYPE;
    }
    
    /**
     * Get the year of a week based year within a century field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *weekyearOfCentury() {
        return WEEKYEAR_OF_CENTURY_TYPE;
    }
    
    /**
     * Get the month of year field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *monthOfYear() {
        return MONTH_OF_YEAR_TYPE;
    }
    
    /**
     * Get the year field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *year() {
        return YEAR_TYPE;
    }
    
    /**
     * Get the year of era field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *yearOfEra() {
        return YEAR_OF_ERA_TYPE;
    }
    
    /**
     * Get the year of century field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *yearOfCentury() {
        return YEAR_OF_CENTURY_TYPE;
    }
    
    /**
     * Get the century of era field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *centuryOfEra() {
        return CENTURY_OF_ERA_TYPE;
    }
    
    /**
     * Get the era field type.
     *
     * @return the DateTimeFieldType *constant
     */
    static const DateTimeFieldType *era() {
        return ERA_TYPE;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Get the name of the field.
     * <p>
     * By convention, names follow a pattern of "dddOfRrr", where "ddd" represents
     * the (singular) duration unit field name and "Rrr" represents the (singular)
     * duration range field name. If the range field is not applicable, then
     * the name of the field is simply the (singular) duration field name.
     *
     * @return field name
     */
    string getName() const { return iName; }
    
    /**
     * Get the duration unit of the field.
     *
     * @return duration unit of the field, never NULL
     */
    virtual const DurationFieldType *getDurationType() const = 0;
    
    /**
     * Get the duration range of the field.
     *
     * @return duration range of the field, NULL if unbounded
     */
    virtual const DurationFieldType *getRangeDurationType() const = 0;
    
    /**
     * Gets a suitable field for this type from the given chronology->
     *
     * @param chronology  the chronology to use, NULL means ISOChronology in default zone
     * @return a suitable field
     */
    virtual const DateTimeField *getField(Chronology *chronology) const = 0;
    
    /**
     * Checks whether this field supported in the given chronology->
     *
     * @param chronology  the chronology to use, NULL means ISOChronology in default zone
     * @return true if supported
     */
    bool isSupported(Chronology *chronology);
    
    /**
     * Get a suitable debug string.
     *
     * @return debug string
     */
    string toString() const;
    
};

class StandardDateTimeFieldType : public DateTimeFieldType {
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = -9937958251642L;
    
    /** The ordinal of the standard field type, for switch statements */
    unsigned char iOrdinal;
    
    /** The unit duration of the field. */
    const DurationFieldType *iUnitType;
    /** The range duration of the field. */
    const DurationFieldType *iRangeType;
    
public:
    
    /**
     * Constructor.
     *
     * @param name  the name to use
     * @param ordinal  the unsigned char value for the oridinal index
     * @param unitType  the unit duration type
     * @param rangeType  the range duration type
     */
    StandardDateTimeFieldType(string name, unsigned char ordinal,
                              const DurationFieldType *unitType, const DurationFieldType *rangeType);
    
    /** @inheritdoc */
    const DurationFieldType *getDurationType() const;
    
    /** @inheritdoc */
    const DurationFieldType *getRangeDurationType() const;
    
    /** @inheritdoc */
    bool equals(const Object *obj) const;
    
    /** @inheritdoc */
    int hashCode();
    
    /** @inheritdoc */
    const DateTimeField *getField(Chronology *chronology) const;
    
    /**
     * Ensure a singleton is returned.
     *
     * @return the singleton type
     */
    const void *readResolve();
    
};

CODATIME_END

#endif
