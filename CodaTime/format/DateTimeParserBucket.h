//
//  DateTimeParserBucket.h
//  CodaTime
//
//  Created by Saul Howard on 12/19/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__DateTimeParserBucket__
#define __CodaTime__DateTimeParserBucket__

#include "CodaTimeMacros.h"

#include "Object.h"
#include "Comparable.h"

#include <vector>
#include <string>

using namespace std;

CODATIME_BEGIN

class Chronology;
class DateTimeZone;
class Locale;
class DateTimeFieldType;
class DateTimeField;
class DurationField;

/**
 * DateTimeParserBucket is an advanced class, intended mainly for parser
 * implementations. It can also be used during normal parsing operations to
 * capture more information about the parse.
 * <p>
 * This class allows fields to be saved in any order, but be physically set in
 * a consistent order. This is useful for parsing against formats that allow
 * field values to contradict each other.
 * <p>
 * Field values are applied in an order where the "larger" fields are set
 * first, making their value less likely to stick.  A field is larger than
 * another when it's range duration is longer. If both ranges are the same,
 * then the larger field has the longer duration. If it cannot be determined
 * which field is larger, then the fields are set in the order they were saved.
 * <p>
 * For example, these fields were saved in this order: dayOfWeek, monthOfYear,
 * dayOfMonth, dayOfYear. When computeMillis is called, the fields are set in
 * this order: monthOfYear, dayOfYear, dayOfMonth, dayOfWeek.
 * <p>
 * DateTimeParserBucket is mutable and not thread-safe.
 *
 * @author Brian S O'Neill
 * @author Fredrik Borgh
 * @since 1.0
 */
class DateTimeParserBucket : public virtual Object {
    
private:
    
    /** The chronology to use for parsing. */
    Chronology *iChrono;
    const int64_t iMillis;
    
    /** The parsed zone, initialised to formatter zone. */
    DateTimeZone *iZone;
    /** The parsed offset. */
    int iOffset = -1;
    /** The locale to use for parsing. */
    Locale *iLocale;
    /** Used for parsing two-digit years. */
    int iPivotYear;
    /** Used for parsing month/day without year. */
    int iDefaultYear;
    
    int iSavedFieldsCount;
    bool iSavedFieldsShared;
    
    Object *iSavedState;
    
    class SavedField : public virtual Object {
        
    public:
        
        const DateTimeField *iField;
        const int iValue;
        const string iText;
        Locale *iLocale;
        
        SavedField(const DateTimeField *field, int value);
        
        SavedField(const DateTimeField *field, string text, Locale *locale);
        
        int64_t set(int64_t millis, bool reset);
        
        /**
         * The field with the longer range duration is ordered first, where
         * NULL is considered infinite. If the ranges match, then the field
         * with the longer duration is ordered first.
         */
//        int compareTo(SavedField *obj);
        
        bool operator < (const SavedField &obj) const;
    };
    
    class SavedState : public virtual Object {
        
    public:
        
        DateTimeParserBucket *mBucket;
        DateTimeZone *iZone;
        const int iOffset;
        const vector<SavedField*> iSavedFields;
        const int iSavedFieldsCount;
        
        SavedState(DateTimeParserBucket *bucket);
        
        bool restoreState(DateTimeParserBucket *enclosing);
    };
    
    vector<SavedField*> iSavedFields;
    
    /**
     * Sorts elements [0,high). Calling java.util.Arrays isn't always the right
     * choice since it always creates an internal copy of the array, even if it
     * doesn't need to. If the array slice is small enough, an insertion sort
     * is chosen instead, but it doesn't need a copy!
     * <p>
     * This method has a modified version of that insertion sort, except it
     * doesn't create an unnecessary array copy. If high is over 10, then
     * java.util.Arrays is called, which will perform a merge sort, which is
     * faster than insertion sort on large lists.
     * <p>
     * The end result is much greater performance when computeMillis is called.
     * Since the amount of saved fields is small, the insertion sort is a
     * better choice. Additional performance is gained since there is no extra
     * array allocation and copying. Also, the insertion sort here does not
     * perform any casting operations. The version in java.util.Arrays performs
     * casts within the insertion sort loop.
     */
    static void sort(vector<SavedField*> &array, int high);
    
    static int compareReverse(const DurationField *a, const DurationField *b);
    
    void saveField(SavedField *field);
    
public:
    
    /**
     * Constructs a bucket.
     *
     * @param instantLocal  the initial millis from 1970-01-01T00:00:00, local time
     * @param chrono  the chronology to use
     * @param locale  the locale to use
     * @deprecated Use longer constructor
     */
//    @Deprecated
//    DateTimeParserBucket(int64_t instantLocal, Chronology chrono, Locale locale) {
//        this(instantLocal, chrono, locale, NULL, 2000);
//    }
    
    /**
     * Constructs a bucket, with the option of specifying the pivot year for
     * two-digit year parsing.
     *
     * @param instantLocal  the initial millis from 1970-01-01T00:00:00, local time
     * @param chrono  the chronology to use
     * @param locale  the locale to use
     * @param pivotYear  the pivot year to use when parsing two-digit years
     * @since 1.1
     * @deprecated Use longer constructor
     */
//    @Deprecated
//    DateTimeParserBucket(int64_t instantLocal, Chronology chrono, Locale locale, int pivotYear) {
//        this(instantLocal, chrono, locale, pivotYear, 2000);
//    }
    
    /**
     * Constructs a bucket, with the option of specifying the pivot year for
     * two-digit year parsing.
     *
     * @param instantLocal  the initial millis from 1970-01-01T00:00:00, local time
     * @param chrono  the chronology to use
     * @param locale  the locale to use
     * @param pivotYear  the pivot year to use when parsing two-digit years
     * @since 2.0
     */
    DateTimeParserBucket(int64_t instantLocal, Chronology *chrono,
                         Locale *locale, int pivotYear, int defaultYear);
    
    //-----------------------------------------------------------------------
    /**
     * Gets the chronology of the bucket, which will be a local (UTC) chronology.
     */
    const Chronology *getChronology() const;
    
    //-----------------------------------------------------------------------
    /**
     * Returns the locale to be used during parsing.
     *
     * @return the locale to use
     */
    Locale *getLocale();
    
    //-----------------------------------------------------------------------
    /**
     * Returns the time zone used by computeMillis.
     */
    DateTimeZone *getZone();
    
    /**
     * Set a time zone to be used when computeMillis is called.
     */
    void setZone(DateTimeZone *zone);
    
    //-----------------------------------------------------------------------
    /**
     * Returns the time zone offset in milliseconds used by computeMillis.
     * @deprecated use int version
     */
//    @Deprecated
//    int getOffset() {
//        return (iOffset != NULL ? iOffset : 0);
//    }
    
    /**
     * Returns the time zone offset in milliseconds used by computeMillis.
     */
    int getOffsetint();
    
    /**
     * Set a time zone offset to be used when computeMillis is called.
     * @deprecated use int version
     */
//    @Deprecated
//    void setOffset(int offset) {
//        iSavedState = NULL;
//        iOffset = offset;
//    }
    
    /**
     * Set a time zone offset to be used when computeMillis is called.
     */
    void setOffset(int offset);
    
    //-----------------------------------------------------------------------
    /**
     * Returns the default year used when information is incomplete.
     * <p>
     * This is used for two-digit years and when the largest parsed field is
     * months or days.
     * <p>
     * A NULL value for two-digit years means to use the value from DateTimeFormatterBuilder.
     * A NULL value for month/day only parsing will cause the default of 2000 to be used.
     *
     * @return int value of the pivot year, NULL if not set
     * @since 1.1
     */
    int getPivotYear();
    
    /**
     * Sets the pivot year to use when parsing two digit years.
     * <p>
     * If the value is set to NULL, this will indicate that default
     * behaviour should be used.
     *
     * @param pivotYear  the pivot year to use
     * @since 1.1
     */
    void setPivotYear(int pivotYear);
    
    //-----------------------------------------------------------------------
    /**
     * Saves a datetime field value.
     *
     * @param field  the field, whose chronology must match that of this bucket
     * @param value  the value
     */
    void saveField(DateTimeField *field, int value);
    
    /**
     * Saves a datetime field value.
     *
     * @param fieldType  the field type
     * @param value  the value
     */
    void saveField(const DateTimeFieldType *fieldType, int value);
    
    /**
     * Saves a datetime field text value.
     *
     * @param fieldType  the field type
     * @param text  the text value
     * @param locale  the locale to use
     */
    void saveField(const DateTimeFieldType *fieldType, string text, Locale *locale);
    
    /**
     * Saves the state of this bucket, returning it in an opaque object. Call
     * restoreState to undo any changes that were made since the state was
     * saved. Calls to saveState may be nested.
     *
     * @return opaque saved state, which may be passed to restoreState
     */
    Object *saveState();
    
    /**
     * Restores the state of this bucket from a previously saved state. The
     * state object passed into this method is not consumed, and it can be used
     * later to restore to that state again.
     *
     * @param savedState opaque saved state, returned from saveState
     * @return true state object is valid and state restored
     */
    bool restoreState(Object *savedState);
    
    /**
     * Computes the parsed datetime by setting the saved fields.
     * This method is idempotent, but it is not thread-safe.
     *
     * @return milliseconds since 1970-01-01T00:00:00Z
     * @throws IllegalArgumentException if any field is out of range
     */
    int64_t computeMillis();
    
    /**
     * Computes the parsed datetime by setting the saved fields.
     * This method is idempotent, but it is not thread-safe.
     *
     * @param resetFields false by default, but when true, unsaved field values are cleared
     * @return milliseconds since 1970-01-01T00:00:00Z
     * @throws IllegalArgumentException if any field is out of range
     */
    int64_t computeMillis(bool resetFields);
    
    /**
     * Computes the parsed datetime by setting the saved fields.
     * This method is idempotent, but it is not thread-safe.
     *
     * @param resetFields false by default, but when true, unsaved field values are cleared
     * @param text optional text being parsed, to be included in any error message
     * @return milliseconds since 1970-01-01T00:00:00Z
     * @throws IllegalArgumentException if any field is out of range
     * @since 1.3
     */
    int64_t computeMillis(bool resetFields, string text);
    
};

CODATIME_END

#endif /* defined(__CodaTime__DateTimeParserBucket__) */
