//
//  DurationFieldType.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_DurationFieldType_h
#define CodaTime_DurationFieldType_h

#include "CodaTimeMacros.h"

#include "Object.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class Chronology;
class DurationField;
class DurationFieldType;
class StandardDurationFieldType;

/**
 * Identifies a duration field, such as years or minutes, in a chronology-neutral way.
 * <p>
 * A duration field type defines the type of the field, such as hours.
 * If does not directly enable any calculations, however it does provide a
 * {@link #getField(Chronology)} method that returns the actual calculation engine
 * for a particular chronology->
 * <p>
 * Instances of <code>DurationFieldType</code> are singletons.
 * They can be compared using <code>==</code>.
 * <p>
 * If required, you can create your own field, for example a quarters.
 * You must create a subclass of <code>DurationFieldType</code> that defines the field type.
 * This class returns the actual calculation engine from {@link #getField(Chronology)}.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
class DurationFieldType : public virtual Object {
    
    friend class Period;
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = 8765135187319L;
    
    /** The name of the field type. */
    const string iName;
    
protected:
    
    /** The eras field type. */
    static const DurationFieldType *ERAS_TYPE;
    /** The centuries field type. */
    static const DurationFieldType *CENTURIES_TYPE;
    /** The weekyears field type. */
    static const DurationFieldType *WEEKYEARS_TYPE;
    /** The years field type. */
    static const DurationFieldType *YEARS_TYPE;
    /** The months field type. */
    static const DurationFieldType *MONTHS_TYPE;
    /** The weeks field type. */
    static const DurationFieldType *WEEKS_TYPE;
    /** The days field type. */
    static const DurationFieldType *DAYS_TYPE;
    /** The halfdays field type. */
    static const DurationFieldType *HALFDAYS_TYPE;
    /** The hours field type. */
    static const DurationFieldType *HOURS_TYPE;
    /** The minutes field type. */
    static const DurationFieldType *MINUTES_TYPE;
    /** The seconds field type. */
    static const DurationFieldType *SECONDS_TYPE;
    /** The millis field type. */
    static const DurationFieldType *MILLIS_TYPE;
    
    // Ordinals for standard field types.
    static const unsigned char
    ERAS = 1,
    CENTURIES = 2,
    WEEKYEARS = 3,
    YEARS = 4,
    MONTHS = 5,
    WEEKS = 6,
    DAYS = 7,
    HALFDAYS = 8,
    HOURS = 9,
    MINUTES = 10,
    SECONDS = 11,
    MILLIS = 12;
    
    //-----------------------------------------------------------------------
    /**
     * Constructor.
     *
     * @param name  the name to use, which by convention, are plural.
     */
    DurationFieldType(string name) : iName(name) {
    }
    
public:
    
    //-----------------------------------------------------------------------
    /**
     * Get the millis field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *millis() {
        return MILLIS_TYPE;
    }
    
    /**
     * Get the seconds field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *seconds() {
        return SECONDS_TYPE;
    }
    
    /**
     * Get the minutes field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *minutes() {
        return MINUTES_TYPE;
    }
    
    /**
     * Get the hours field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *hours() {
        return HOURS_TYPE;
    }
    
    /**
     * Get the halfdays field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *halfdays() {
        return HALFDAYS_TYPE;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Get the days field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *days() {
        return DAYS_TYPE;
    }
    
    /**
     * Get the weeks field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *weeks() {
        return WEEKS_TYPE;
    }
    
    /**
     * Get the weekyears field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *weekyears() {
        return WEEKYEARS_TYPE;
    }
    
    /**
     * Get the months field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *months() {
        return MONTHS_TYPE;
    }
    
    /**
     * Get the years field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *years() {
        return YEARS_TYPE;
    }
    
    /**
     * Get the centuries field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *centuries() {
        return CENTURIES_TYPE;
    }
    
    /**
     * Get the eras field type.
     *
     * @return the DateTimeFieldType constant
     */
    static const DurationFieldType *eras() { return ERAS_TYPE; }
    
    //-----------------------------------------------------------------------
    /**
     * Get the name of the field.
     * By convention, names are plural.
     *
     * @return field name
     */
    const string getName() const { return iName; }
    
    /**
     * Gets a suitable field for this type from the given chronology->
     *
     * @param chronology  the chronology to use, null means ISOChronology in default zone
     * @return a suitable field
     */
    virtual const DurationField *getField(Chronology *chronology) const = 0;
    
    /**
     * Checks whether this field supported in the given chronology->
     *
     * @param chronology  the chronology to use, null means ISOChronology in default zone
     * @return true if supported
     */
    bool isSupported(Chronology *chronology);
    
    /**
     * Get a suitable debug string.
     *
     * @return debug string
     */
    string toString() const { return getName(); }

};

class StandardDurationFieldType : public DurationFieldType {
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = 31156755687123L;
    
    /** The ordinal of the standard field type, for switch statements */
    unsigned char iOrdinal;
    
public:
    
    /**
     * Constructor.
     *
     * @param name  the name to use
     */
    StandardDurationFieldType(string name, unsigned char ordinal) : DurationFieldType(name) {
        iOrdinal = ordinal;
    }
    
    /** @inheritdoc */
    bool equals(const Object *obj) const;
    
    /** @inheritdoc */
    int hashCode() { return (1 << iOrdinal); }
    
    const DurationField *getField(Chronology *chronology) const;
    
    /**
     * Ensure a singleton is returned.
     *
     * @return the singleton type
     */
    const DurationFieldType *readResolve();
};

CODATIME_END

#endif
