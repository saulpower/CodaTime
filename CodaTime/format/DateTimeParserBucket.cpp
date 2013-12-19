//
//  DateTimeParserBucket.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/19/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "DateTimeParserBucket.h"

#include "Chronology.h"
#include "DateTimeField.h"
#include "DateTimeFieldType.h"
#include "DateTimeUtils.h"
#include "DateTimeZone.h"
#include "DurationField.h"
#include "DurationFieldType.h"
#include "Exceptions.h"
#include "Locale.h"

#include <algorithm>

CODATIME_BEGIN

DateTimeParserBucket::SavedState::SavedState(DateTimeParserBucket *bucket) :
    mBucket(bucket), iZone(bucket->iZone), iOffset(bucket->iOffset),
    iSavedFields(bucket->iSavedFields), iSavedFieldsCount(bucket->iSavedFieldsCount)
{
}

bool DateTimeParserBucket::SavedState::restoreState(DateTimeParserBucket *enclosing) {
    if (enclosing != mBucket) {
        return false;
    }
    enclosing->iZone = iZone;
    enclosing->iOffset = iOffset;
    enclosing->iSavedFields = iSavedFields;
    if (iSavedFieldsCount < enclosing->iSavedFieldsCount) {
        // Since count is being restored to a lower count, the
        // potential exists for new saved fields to destroy data being
        // shared by another state. Set this flag such that the array
        // of saved fields is cloned prior to modification.
        enclosing->iSavedFieldsShared = true;
    }
    enclosing->iSavedFieldsCount = iSavedFieldsCount;
    return true;
}

DateTimeParserBucket::SavedField::SavedField(const DateTimeField *field, int value) :
    iValue(value), iText(NULL)
{
    iField = field;
    iLocale = NULL;
}

DateTimeParserBucket::SavedField::SavedField(const DateTimeField *field, string text, Locale *locale) :
    iValue(0), iText(text)
{
    iField = field;
    iLocale = locale;
}

int64_t DateTimeParserBucket::SavedField::set(int64_t millis, bool reset) {
    if (iText.empty()) {
        millis = iField->set(millis, iValue);
    } else {
        millis = iField->set(millis, iText, iLocale);
    }
    if (reset) {
        millis = iField->roundFloor(millis);
    }
    return millis;
}

/**
 * The field with the longer range duration is ordered first, where
 * NULL is considered infinite. If the ranges match, then the field
 * with the longer duration is ordered first.
 */
//int DateTimeParserBucket::SavedField::compareTo(SavedField *obj) {
//    DateTimeField *other = obj->iField;
//    int result = compareReverse(iField->getRangeDurationField(), other->getRangeDurationField());
//    if (result != 0) {
//        return result;
//    }
//    return compareReverse(iField->getDurationField(), other->getDurationField());
//}

bool DateTimeParserBucket::SavedField::operator < (const SavedField &obj) const {
    const DateTimeField *other = obj.iField;
    int result = compareReverse(iField->getRangeDurationField(), other->getRangeDurationField());
    if (result != 0) {
        return result;
    }
    return compareReverse(iField->getDurationField(), other->getDurationField()) < 0;
}

void DateTimeParserBucket::sort(vector<SavedField*> &array, int high) {
    if (high > 10) {
        std::sort(array.begin(), array.begin() + high);
    } else {
        for (int i=0; i<high; i++) {
            for (int j = i; j > 0 && (array[j-1]) > array[j]; j--) {
                SavedField *t = array[j];
                array[j] = array[j-1];
                array[j-1] = t;
            }
        }
    }
}

// TODO: Verify this is working correctly
int DateTimeParserBucket::compareReverse(const DurationField *a, const DurationField *b) {
    if (a == NULL || !a->isSupported()) {
        if (b == NULL || !b->isSupported()) {
            return 0;
        }
        return -1;
    }
    if (b == NULL || !b->isSupported()) {
        return 1;
    }
    return -a->compareTo(b);
}

void DateTimeParserBucket::saveField(SavedField *field) {
    int &savedFieldsCount = iSavedFieldsCount;
    
    if (savedFieldsCount == iSavedFields.size() || iSavedFieldsShared) {
        // Expand capacity or merely copy if saved fields are shared.
        vector<SavedField*> newArray = vector<SavedField*>(savedFieldsCount == iSavedFields.size() ? savedFieldsCount * 2 : iSavedFields.size());
        iSavedFields = newArray;
        iSavedFieldsShared = false;
    }
    
    iSavedState = NULL;
    iSavedFields[savedFieldsCount] = field;
    iSavedFieldsCount = savedFieldsCount + 1;
}

DateTimeParserBucket::DateTimeParserBucket(int64_t instantLocal, Chronology *chrono,
                                           Locale *locale, int pivotYear, int defaultYear) :
iMillis(instantLocal), iChrono(DateTimeUtils::getChronology(chrono)->withUTC())
{
    chrono = DateTimeUtils::getChronology(chrono);
    iZone = chrono->getZone();
    iLocale = (locale == NULL ? Locale::getDefault() : locale);
    iPivotYear = pivotYear;
    iDefaultYear = defaultYear;
    iSavedFields.resize(8);
}

const Chronology *DateTimeParserBucket::getChronology() const {
    return iChrono;
}

Locale *DateTimeParserBucket::getLocale() {
    return iLocale;
}

DateTimeZone *DateTimeParserBucket::getZone() {
    return iZone;
}

void DateTimeParserBucket::setZone(DateTimeZone *zone) {
    iSavedState = NULL;
    iZone = zone;
}

int DateTimeParserBucket::getOffsetint() {
    return iOffset;
}

void DateTimeParserBucket::setOffset(int offset) {
    iSavedState = NULL;
    iOffset = offset;
}

int DateTimeParserBucket::getPivotYear() {
    return iPivotYear;
}

void DateTimeParserBucket::setPivotYear(int pivotYear) {
    iPivotYear = pivotYear;
}

void DateTimeParserBucket::saveField(DateTimeField *field, int value) {
    saveField(new SavedField(field, value));
}

void DateTimeParserBucket::saveField(const DateTimeFieldType *fieldType, int value) {
    saveField(new SavedField(fieldType->getField(iChrono), value));
}

void DateTimeParserBucket::saveField(const DateTimeFieldType *fieldType, string text, Locale *locale) {
    saveField(new SavedField(fieldType->getField(iChrono), text, locale));
}

Object *DateTimeParserBucket::saveState() {
    if (iSavedState == NULL) {
        iSavedState = new SavedState(this);
    }
    return iSavedState;
}

bool DateTimeParserBucket::restoreState(Object *savedState) {
    
    SavedState* state = dynamic_cast<SavedState*>(savedState);
    
    if (state != 0) {
        if (state->restoreState(this)) {
            iSavedState = savedState;
            return true;
        }
    }
    return false;
}

int64_t DateTimeParserBucket::computeMillis() {
    return computeMillis(false, NULL);
}

int64_t DateTimeParserBucket::computeMillis(bool resetFields) {
    return computeMillis(resetFields, NULL);
}

int64_t DateTimeParserBucket::computeMillis(bool resetFields, string text) {
    int count = iSavedFieldsCount;
    if (iSavedFieldsShared) {
//        iSavedFields = savedFields = (vector<SavedField*>)iSavedFields.clone();
        iSavedFieldsShared = false;
    }
    sort(iSavedFields, count);
    if (count > 0) {
        // alter base year for parsing if first field is month or day
        const DurationField *months = DurationFieldType::months()->getField(iChrono);
        const DurationField *days = DurationFieldType::days()->getField(iChrono);
        const DurationField *first = iSavedFields[0]->iField->getDurationField();
        if (compareReverse(first, months) >= 0 && compareReverse(first, days) <= 0) {
            saveField(DateTimeFieldType::year(), iDefaultYear);
            return computeMillis(resetFields, text);
        }
    }
    
    int64_t millis = iMillis;
    try {
        for (int i = 0; i < count; i++) {
            millis = iSavedFields[i]->set(millis, resetFields);
        }
        if (resetFields) {
            for (int i = 0; i < count; i++) {
                millis = iSavedFields[i]->set(millis, i == (count - 1));
            }
        }
    } catch (IllegalFieldValueException e) {
        if (!text.empty()) {
//            e.prependMessage("Cannot parse \"" + text + '"');
        }
        throw e;
    }
    
    millis -= iOffset;
    if (iZone != NULL) {
        int offset = iZone->getOffsetFromLocal(millis);
        millis -= offset;
        if (offset != iZone->getOffset(millis)) {
            string message = "Illegal instant due to time zone offset transition (" + iZone->toString() + ")";
            if (!text.empty()) {
                message = "Cannot parse \"" + text + "\": " + message;
            }
            throw IllegalInstantException(message);
        }
    }
    
    return millis;
}

CODATIME_END