//
//  PeriodType.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__PeriodType__
#define __CodaTime__PeriodType__

#include "CodaTimeMacros.h"

#include "Object.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

CODATIME_BEGIN

class Object;
class DurationFieldType;
class ReadablePeriod;

/**
 * Controls a period implementation by specifying which duration fields are to be used.
 * <p>
 * The following implementations are provided:
 * <ul>
 * <li>Standard - years, months, weeks, days, hours, minutes, seconds, millis
 * <li>YearMonthDayTime - years, months, days, hours, minutes, seconds, millis
 * <li>YearMonthDay - years, months, days
 * <li>YearWeekDayTime - years, weeks, days, hours, minutes, seconds, millis
 * <li>YearWeekDay - years, weeks, days
 * <li>YearDayTime - years, days, hours, minutes, seconds, millis
 * <li>YearDay - years, days, hours
 * <li>DayTime - days, hours, minutes, seconds, millis
 * <li>Time - hours, minutes, seconds, millis
 * <li>plus one for each single type
 * </ul>
 *
 * <p>
 * PeriodType is thread-safe and immutable, and all subclasses must be as well.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class PeriodType : public virtual Object {
    
    friend class Period;
    friend class MutablePeriod;
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = 2274324892792009998L;
    
    /** Cache of all the known types. */
    static map<PeriodType*, Object*> cTypes;
    
    static const int YEAR_INDEX = 0;
    static const int MONTH_INDEX = 1;
    static const int WEEK_INDEX = 2;
    static const int DAY_INDEX = 3;
    static const int HOUR_INDEX = 4;
    static const int MINUTE_INDEX = 5;
    static const int SECOND_INDEX = 6;
    static const int MILLI_INDEX = 7;
    
    static PeriodType *cStandard;
    static PeriodType *cYMDTime;
    static PeriodType *cYMD;
    static PeriodType *cYWDTime;
    static PeriodType *cYWD;
    static PeriodType *cYDTime;
    static PeriodType *cYD;
    static PeriodType *cDTime;
    static PeriodType *cTime;
    
    static PeriodType *cYears;
    static PeriodType *cMonths;
    static PeriodType *cWeeks;
    static PeriodType *cDays;
    static PeriodType *cHours;
    static PeriodType *cMinutes;
    static PeriodType *cSeconds;
    static PeriodType *cMillis;
    
    //-----------------------------------------------------------------------
    /** The name of the type */
    string iName;
    /** The array of types */
    vector<const DurationFieldType*> iTypes;
    /** The array of indices */
    vector<int> iIndices;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the indexed field part of the period.
     *
     * @param period  the period to query
     * @param index  the index to use
     * @return the value of the field, zero if unsupported
     */
    int getIndexedField(ReadablePeriod *period, int index) const;
    
    /**
     * Sets the indexed field part of the period.
     *
     * @param period  the period to query
     * @param index  the index to use
     * @param values  the array to populate
     * @param newValue  the value to set
     * @throws UnsupportedOperationException if not supported
     */
    bool setIndexedField(ReadablePeriod *period, int index, vector<int> values, int newValue) const;
    
    /**
     * Adds to the indexed field part of the period.
     *
     * @param period  the period to query
     * @param index  the index to use
     * @param values  the array to populate
     * @param valueToAdd  the value to add
     * @return true if the array is updated
     * @throws UnsupportedOperationException if not supported
     */
    bool addIndexedField(ReadablePeriod *period, int index, vector<int> values, int valueToAdd) const;
    
    /**
     * Removes the field specified by indices index.
     *
     * @param indicesIndex  the index to remove
     * @param name  the name addition
     * @return the new type
     */
    PeriodType *withFieldRemoved(int indicesIndex, string name);
    
protected:
    
    /**
     * Constructor.
     *
     * @param name  the name
     * @param types  the types
     * @param indices  the indices
     */
    PeriodType(string name, vector<const DurationFieldType*> types, vector<int> indices);
    
public:
    
    /**
     * Gets a type that defines all standard fields.
     * <ul>
     * <li>years
     * <li>months
     * <li>weeks
     * <li>days
     * <li>hours
     * <li>minutes
     * <li>seconds
     * <li>milliseconds
     * </ul>
     *
     * @return the period type
     */
    static PeriodType *standard();
    
    /**
     * Gets a type that defines all standard fields except weeks.
     * <ul>
     * <li>years
     * <li>months
     * <li>days
     * <li>hours
     * <li>minutes
     * <li>seconds
     * <li>milliseconds
     * </ul>
     *
     * @return the period type
     */
    static PeriodType *yearMonthDayTime();
    
    /**
     * Gets a type that defines the year, month and day fields.
     * <ul>
     * <li>years
     * <li>months
     * <li>days
     * </ul>
     *
     * @return the period type
     * @since 1.1
     */
    static PeriodType *yearMonthDay();
    
    /**
     * Gets a type that defines all standard fields except months.
     * <ul>
     * <li>years
     * <li>weeks
     * <li>days
     * <li>hours
     * <li>minutes
     * <li>seconds
     * <li>milliseconds
     * </ul>
     *
     * @return the period type
     */
    static PeriodType *yearWeekDayTime();
    
    /**
     * Gets a type that defines year, week and day fields.
     * <ul>
     * <li>years
     * <li>weeks
     * <li>days
     * </ul>
     *
     * @return the period type
     * @since 1.1
     */
    static PeriodType *yearWeekDay();
    
    /**
     * Gets a type that defines all standard fields except months and weeks.
     * <ul>
     * <li>years
     * <li>days
     * <li>hours
     * <li>minutes
     * <li>seconds
     * <li>milliseconds
     * </ul>
     *
     * @return the period type
     */
    static PeriodType *yearDayTime();
    
    /**
     * Gets a type that defines the year and day fields.
     * <ul>
     * <li>years
     * <li>days
     * </ul>
     *
     * @return the period type
     * @since 1.1
     */
    static PeriodType *yearDay();
    
    /**
     * Gets a type that defines all standard fields from days downwards.
     * <ul>
     * <li>days
     * <li>hours
     * <li>minutes
     * <li>seconds
     * <li>milliseconds
     * </ul>
     *
     * @return the period type
     */
    static PeriodType *dayTime();
    
    /**
     * Gets a type that defines all standard time fields.
     * <ul>
     * <li>hours
     * <li>minutes
     * <li>seconds
     * <li>milliseconds
     * </ul>
     *
     * @return the period type
     */
    static PeriodType *time();
    
    /**
     * Gets a type that defines just the years field.
     *
     * @return the period type
     */
    static PeriodType *years();
    
    /**
     * Gets a type that defines just the months field.
     *
     * @return the period type
     */
    static PeriodType *months();
    
    /**
     * Gets a type that defines just the weeks field.
     *
     * @return the period type
     */
    static PeriodType *weeks();
    
    /**
     * Gets a type that defines just the days field.
     *
     * @return the period type
     */
    static PeriodType *days();
    
    /**
     * Gets a type that defines just the hours field.
     *
     * @return the period type
     */
    static PeriodType *hours();
    
    /**
     * Gets a type that defines just the minutes field.
     *
     * @return the period type
     */
    static PeriodType *minutes();
    
    /**
     * Gets a type that defines just the seconds field.
     *
     * @return the period type
     */
    static PeriodType *seconds();
    
    /**
     * Gets a type that defines just the millis field.
     *
     * @return the period type
     */
    static PeriodType *millis();
    
    /**
     * Gets a period type that contains the duration types of the array.
     * <p>
     * Only the 8 standard duration field types are supported.
     *
     * @param types  the types to include in the array.
     * @return the period type
     * @since 1.1
     */
    // TODO: synchronized
    static PeriodType *forFields(vector<const DurationFieldType*> types);
    
    //-----------------------------------------------------------------------
    /**
     * Gets the name of the period type.
     *
     * @return the name
     */
    string getName() const;
    
    /**
     * Gets the number of fields in the period type.
     *
     * @return the number of fields
     */
    int size() const;
    
    /**
     * Gets the field type by index.
     *
     * @param index  the index to retrieve
     * @return the field type
     * @throws IndexOutOfBoundsException if the index is invalid
     */
    const DurationFieldType *getFieldType(int index) const;
    
    /**
     * Checks whether the field specified is supported by this period.
     *
     * @param type  the type to check, may be null which returns false
     * @return true if the field is supported
     */
    bool isSupported(const DurationFieldType *type) const;
    
    /**
     * Gets the index of the field in this period.
     *
     * @param type  the type to check, may be null which returns -1
     * @return the index of -1 if not supported
     */
    int indexOf(const DurationFieldType *type) const;
    
    /**
     * Gets a debugging to string.
     *
     * @return a string
     */
    string toString();
    
    //-----------------------------------------------------------------------
    /**
     * Returns a version of this PeriodType instance that does not support years.
     *
     * @return a new period type that supports the original set of fields except years
     */
    PeriodType *withYearsRemoved();
    
    /**
     * Returns a version of this PeriodType instance that does not support months.
     *
     * @return a new period type that supports the original set of fields except months
     */
    PeriodType *withMonthsRemoved();
    
    /**
     * Returns a version of this PeriodType instance that does not support weeks.
     *
     * @return a new period type that supports the original set of fields except weeks
     */
    PeriodType *withWeeksRemoved();
    
    /**
     * Returns a version of this PeriodType instance that does not support days.
     *
     * @return a new period type that supports the original set of fields except days
     */
    PeriodType *withDaysRemoved();
    
    /**
     * Returns a version of this PeriodType instance that does not support hours.
     *
     * @return a new period type that supports the original set of fields except hours
     */
    PeriodType *withHoursRemoved();
    
    /**
     * Returns a version of this PeriodType instance that does not support minutes.
     *
     * @return a new period type that supports the original set of fields except minutes
     */
    PeriodType *withMinutesRemoved();
    
    /**
     * Returns a version of this PeriodType instance that does not support seconds.
     *
     * @return a new period type that supports the original set of fields except seconds
     */
    PeriodType *withSecondsRemoved();
    
    /**
     * Returns a version of this PeriodType instance that does not support milliseconds.
     *
     * @return a new period type that supports the original set of fields except milliseconds
     */
    PeriodType *withMillisRemoved();
    
    //-----------------------------------------------------------------------
    /**
     * Compares this type to another object.
     * To be equal, the object must be a PeriodType with the same set of fields.
     * 
     * @param obj  the object to compare to
     * @return true if equal
     */
    bool equals(const Object *obj) const;
    
    /**
     * Returns a hashcode based on the field types.
     * 
     * @return a suitable hashcode
     */
    int hashCode();
    
};

CODATIME_END

#endif /* defined(__CodaTime__PeriodType__) */
