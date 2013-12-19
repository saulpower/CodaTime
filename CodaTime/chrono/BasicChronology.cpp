//
//  BasicChronology.c
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "BasicChronology.h"

#include "chrono/BasicYearDateTimeField.h"
#include "chrono/GJLocaleSymbols.h"
#include "DateTimeFieldType.h"
#include "DateTimeZone.h"
#include "DurationFieldType.h"

CODATIME_BEGIN

string BasicChronology::HalfdayField::getAsText(int fieldValue, Locale *locale) const {
    return GJLocaleSymbols::forLocale(locale)->halfdayValueToText(fieldValue);
}

int64_t BasicChronology::HalfdayField::set(int64_t millis, string text, Locale *locale) {
    return PreciseDateTimeField::set(millis, GJLocaleSymbols::forLocale(locale)->halfdayTextToValue(text));
}

int BasicChronology::HalfdayField::getMaximumTextLength(Locale *locale) {
    return GJLocaleSymbols::forLocale(locale)->getHalfdayMaxTextLength();
}

BasicChronology::BasicChronology(Chronology *base, Object *param, int minDaysInFirstWeek) : AssembledChronology(base, param) {
    
    iYearInfoCache.resize(CACHE_SIZE);
    
    if (minDaysInFirstWeek < 1 || minDaysInFirstWeek > 7) {
        string str("Invalid min days in first week: ");
        str.append(to_string(minDaysInFirstWeek));
        throw new IllegalArgumentException(str);
    }
    
    iMinDaysInFirstWeek = minDaysInFirstWeek;
}

// Although accessed by multiple threads, this method doesn't need to be synchronized.
BasicChronology::YearInfo *BasicChronology::getYearInfo(int year) {
    YearInfo *info = iYearInfoCache[year & CACHE_MASK];
    if (info == NULL || info->iYear != year) {
        info = new YearInfo(year, calculateFirstDayOfYearMillis(year));
        iYearInfoCache[year & CACHE_MASK] = info;
    }
    return info;
}

/**
 * Get the number of weeks in the year.
 *
 * @param year  the year to use
 * @return number of weeks in the year
 */
int BasicChronology::getWeeksInYear(int year) {
    int64_t firstWeekMillis1 = getFirstWeekOfYearMillis(year);
    int64_t firstWeekMillis2 = getFirstWeekOfYearMillis(year + 1);
    return (int) ((firstWeekMillis2 - firstWeekMillis1) / DateTimeConstants::MILLIS_PER_WEEK);
}

/**
 * Get the millis for the first week of a year.
 *
 * @param year  the year to use
 * @return millis
 */
int64_t BasicChronology::getFirstWeekOfYearMillis(int year) {
    int64_t jan1millis = getYearMillis(year);
    int jan1dayOfWeek = getDayOfWeek(jan1millis);
    
    if (jan1dayOfWeek > (8 - iMinDaysInFirstWeek)) {
        // First week is end of previous year because it doesn't have enough days.
        return jan1millis + (8 - jan1dayOfWeek)
        * (int64_t)DateTimeConstants::MILLIS_PER_DAY;
    } else {
        // First week is start of this year because it has enough days.
        return jan1millis - (jan1dayOfWeek - 1)
        * (int64_t)DateTimeConstants::MILLIS_PER_DAY;
    }
}

/**
 * Get the milliseconds for the start of a year.
 *
 * @param year The year to use.
 * @return millis from 1970-01-01T00:00:00Z
 */
int64_t BasicChronology::getYearMillis(int year) {
    return getYearInfo(year)->iFirstDayMillis;
}

/**
 * Get the milliseconds for the start of a month.
 *
 * @param year The year to use.
 * @param month The month to use
 * @return millis from 1970-01-01T00:00:00Z
 */
int64_t BasicChronology::getYearMonthMillis(int year, int month) {
    int64_t millis = getYearMillis(year);
    millis += getTotalMillisByYearMonth(year, month);
    return millis;
}

/**
 * Get the milliseconds for a particular date.
 *
 * @param year The year to use.
 * @param month The month to use
 * @param dayOfMonth The day of the month to use
 * @return millis from 1970-01-01T00:00:00Z
 */
int64_t BasicChronology::getYearMonthDayMillis(int year, int month, int dayOfMonth) {
    int64_t millis = getYearMillis(year);
    millis += getTotalMillisByYearMonth(year, month);
    return millis + (dayOfMonth - 1) * (int64_t)DateTimeConstants::MILLIS_PER_DAY;
}

/**
 * @param instant millis from 1970-01-01T00:00:00Z
 */
int BasicChronology::getYear(int64_t instant) {
    // Get an initial estimate of the year, and the millis value that
    // represents the start of that year. Then verify estimate and fix if
    // necessary.
    
    // Initial estimate uses values divided by two to avoid overflow.
    int64_t unitMillis = getAverageMillisPerYearDividedByTwo();
    int64_t i2 = (instant >> 1) + getApproxMillisAtEpochDividedByTwo();
    if (i2 < 0) {
        i2 = i2 - unitMillis + 1;
    }
    int year = (int) (i2 / unitMillis);
    
    int64_t yearStart = getYearMillis(year);
    int64_t diff = instant - yearStart;
    
    if (diff < 0) {
        year--;
    } else if (diff >= DateTimeConstants::MILLIS_PER_DAY * 365L) {
        // One year may need to be added to fix estimate.
        int64_t oneYear;
        if (isLeapYear(year)) {
            oneYear = DateTimeConstants::MILLIS_PER_DAY * 366L;
        } else {
            oneYear = DateTimeConstants::MILLIS_PER_DAY * 365L;
        }
        
        yearStart += oneYear;
        
        if (yearStart <= instant) {
            // Didn't go too far, so actually add one year.
            year++;
        }
    }
    
    return year;
}

/**
 * @param millis from 1970-01-01T00:00:00Z
 */
int BasicChronology::getDayOfMonth(int64_t millis) {
    int year = getYear(millis);
    int month = getMonthOfYear(millis, year);
    return getDayOfMonth(millis, year, month);
}

/**
 * @param millis from 1970-01-01T00:00:00Z
 * @param year precalculated year of millis
 */
int BasicChronology::getDayOfMonth(int64_t millis, int year) {
    int month = getMonthOfYear(millis, year);
    return getDayOfMonth(millis, year, month);
}

/**
 * @param millis from 1970-01-01T00:00:00Z
 * @param year precalculated year of millis
 * @param month precalculated month of millis
 */
int BasicChronology::getDayOfMonth(int64_t millis, int year, int month) {
    int64_t dateMillis = getYearMillis(year);
    dateMillis += getTotalMillisByYearMonth(year, month);
    return (int) ((millis - dateMillis) / DateTimeConstants::MILLIS_PER_DAY) + 1;
}

/**
 * @param instant millis from 1970-01-01T00:00:00Z
 * @param year precalculated year of millis
 */
int BasicChronology::getDayOfYear(int64_t instant, int year) {
    int64_t yearStart = getYearMillis(year);
    return (int) ((instant - yearStart) / DateTimeConstants::MILLIS_PER_DAY) + 1;
}

/**
 * @param instant millis from 1970-01-01T00:00:00Z
 */
int BasicChronology::getWeekyear(int64_t instant) {
    int year = getYear(instant);
    int week = getWeekOfWeekyear(instant, year);
    if (week == 1) {
        return getYear(instant + DateTimeConstants::MILLIS_PER_WEEK);
    } else if (week > 51) {
        return getYear(instant - (2 * DateTimeConstants::MILLIS_PER_WEEK));
    } else {
        return year;
    }
}

/**
 * @param instant millis from 1970-01-01T00:00:00Z
 * @param year precalculated year of millis
 */
int BasicChronology::getWeekOfWeekyear(int64_t instant, int year) {
    int64_t firstWeekMillis1 = getFirstWeekOfYearMillis(year);
    if (instant < firstWeekMillis1) {
        return getWeeksInYear(year - 1);
    }
    int64_t firstWeekMillis2 = getFirstWeekOfYearMillis(year + 1);
    if (instant >= firstWeekMillis2) {
        return 1;
    }
    return (int) ((instant - firstWeekMillis1) / DateTimeConstants::MILLIS_PER_WEEK) + 1;
}

/**
 * @param instant millis from 1970-01-01T00:00:00Z
 */
int BasicChronology::getDayOfWeek(int64_t instant) {
    // 1970-01-01 is day of week 4, Thursday.
    
    int64_t daysSince19700101;
    if (instant >= 0) {
        daysSince19700101 = instant / DateTimeConstants::MILLIS_PER_DAY;
    } else {
        daysSince19700101 = (instant - (DateTimeConstants::MILLIS_PER_DAY - 1))
        / DateTimeConstants::MILLIS_PER_DAY;
        if (daysSince19700101 < -3) {
            return 7 + (int) ((daysSince19700101 + 4) % 7);
        }
    }
    
    return 1 + (int) ((daysSince19700101 + 3) % 7);
}

int BasicChronology::getMillisOfDay(int64_t instant) {
    if (instant >= 0) {
        return (int) (instant % DateTimeConstants::MILLIS_PER_DAY);
    } else {
        return (DateTimeConstants::MILLIS_PER_DAY - 1)
        + (int) ((instant + 1) % DateTimeConstants::MILLIS_PER_DAY);
    }
}

int BasicChronology::getDaysInMonthMax(int64_t instant) {
    int thisYear = getYear(instant);
    int thisMonth = getMonthOfYear(instant, thisYear);
    return getDaysInYearMonth(thisYear, thisMonth);
}

int64_t BasicChronology::getDateMidnightMillis(int year, int monthOfYear, int dayOfMonth) {
    FieldUtils::verifyValueBounds(DateTimeFieldType::year(), year, getMinYear(), getMaxYear());
    FieldUtils::verifyValueBounds(DateTimeFieldType::monthOfYear(), monthOfYear, 1, getMaxMonth(year));
    FieldUtils::verifyValueBounds(DateTimeFieldType::dayOfMonth(), dayOfMonth, 1, getDaysInYearMonth(year, monthOfYear));
    return getYearMonthDayMillis(year, monthOfYear, dayOfMonth);
}

DateTimeZone *BasicChronology::getZone() const {
    Chronology *base;
    if ((base = getBase()) != NULL) {
        return base->getZone();
    }
    return DateTimeZone::UTC;
}

int64_t BasicChronology::getDateTimeMillis(int year, int monthOfYear, int dayOfMonth, int millisOfDay) {
    Chronology *base;
    if ((base = getBase()) != NULL) {
        return base->getDateTimeMillis(year, monthOfYear, dayOfMonth, millisOfDay);
    }
    
    FieldUtils::verifyValueBounds
    (DateTimeFieldType::millisOfDay(), millisOfDay, 0, DateTimeConstants::MILLIS_PER_DAY - 1);
    return getDateMidnightMillis(year, monthOfYear, dayOfMonth) + millisOfDay;
}

int64_t BasicChronology::getDateTimeMillis(int year, int monthOfYear, int dayOfMonth,
                          int hourOfDay, int minuteOfHour, int secondOfMinute, int millisOfSecond) {
    Chronology *base;
    if ((base = getBase()) != NULL) {
        return base->getDateTimeMillis(year, monthOfYear, dayOfMonth,
                                       hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond);
    }
    
    FieldUtils::verifyValueBounds(DateTimeFieldType::hourOfDay(), hourOfDay, 0, 23);
    FieldUtils::verifyValueBounds(DateTimeFieldType::minuteOfHour(), minuteOfHour, 0, 59);
    FieldUtils::verifyValueBounds(DateTimeFieldType::secondOfMinute(), secondOfMinute, 0, 59);
    FieldUtils::verifyValueBounds(DateTimeFieldType::millisOfSecond(), millisOfSecond, 0, 999);
    
    return getDateMidnightMillis(year, monthOfYear, dayOfMonth)
    + hourOfDay * DateTimeConstants::MILLIS_PER_HOUR
    + minuteOfHour * DateTimeConstants::MILLIS_PER_MINUTE
    + secondOfMinute * DateTimeConstants::MILLIS_PER_SECOND
    + millisOfSecond;
}

//-----------------------------------------------------------------------
/**
 * Checks if this chronology instance equals another.
 *
 * @param obj  the object to compare to
 * @return true if equal
 * @since 1.6
 */
bool BasicChronology::equals(const Object *obj) const {
//    if (this == obj) {
//        return true;
//    }
    
    const BasicChronology *chrono = dynamic_cast<const BasicChronology*>(obj);
    
    if (chrono != 0) {
        return getMinimumDaysInFirstWeek() == chrono->getMinimumDaysInFirstWeek() &&
        getZone()->equals(chrono->getZone());
    }
    return false;
}

/**
 * A suitable hash code for the chronology.
 *
 * @return the hash code
 * @since 1.6
 */
int BasicChronology::hashCode() {
    string className("BasicChronology");
    hash<string> hash;
    return hash(className) * 11 + getZone()->hashCode() + getMinimumDaysInFirstWeek();
}

// Output
//-----------------------------------------------------------------------
/**
 * Gets a debugging tostring.
 *
 * @return a debugging string
 */
string BasicChronology::toString() {
    string sb;
    string name("BasicChronology");
    //        int index = name.lastIndexOf('.');
    //        if (index >= 0) {
    //            name = name.substr(index + 1);
    //        }
    sb.append(name);
    sb.append(1, '[');
    DateTimeZone *zone = getZone();
    if (zone != NULL) {
        sb.append(zone->getID());
    }
    if (getMinimumDaysInFirstWeek() != 4) {
        sb.append(",mdfw=");
        sb.append(to_string(getMinimumDaysInFirstWeek()));
    }
    sb.append(1, ']');
    return sb;
}

void BasicChronology::assemble(Fields *fields) {
    // First copy fields that are the same for all Gregorian and Julian
    // chronologies.
    
    fields->millis = cMillisField;
    fields->seconds = cSecondsField;
    fields->minutes = cMinutesField;
    fields->hours = cHoursField;
    fields->halfdays = cHalfdaysField;
    fields->days = cDaysField;
    fields->weeks = cWeeksField;
    
    fields->millisOfSecond = cMillisOfSecondField;
    fields->millisOfDay = cMillisOfDayField;
    fields->secondOfMinute = cSecondOfMinuteField;
    fields->secondOfDay = cSecondOfDayField;
    fields->minuteOfHour = cMinuteOfHourField;
    fields->minuteOfDay = cMinuteOfDayField;
    fields->hourOfDay = cHourOfDayField;
    fields->hourOfHalfday = cHourOfHalfdayField;
    fields->clockhourOfDay = cClockhourOfDayField;
    fields->clockhourOfHalfday = cClockhourOfHalfdayField;
    fields->halfdayOfDay = cHalfdayOfDayField;
    
    // Now create fields that have unique behavior for Gregorian and Julian
    // chronologies.
    
    fields->year = new BasicYearDateTimeField(this);
    fields->yearOfEra = new GJYearOfEraDateTimeField(fields->year, this);
    
    // Define one-based centuryOfEra and yearOfCentury.
    DateTimeField *field = new OffsetDateTimeField(fields->yearOfEra, 99);
    fields->centuryOfEra = new DividedDateTimeField(field, DateTimeFieldType::centuryOfEra(), 100);
    fields->centuries = fields->centuryOfEra->getDurationField();
    
    field = new RemainderDateTimeField((DividedDateTimeField) fields->centuryOfEra);
    fields->yearOfCentury = new OffsetDateTimeField(field, DateTimeFieldType::yearOfCentury(), 1);
    
    fields->era = new GJEraDateTimeField(this);
    fields->dayOfWeek = new GJDayOfWeekDateTimeField(this, fields->days);
    fields->dayOfMonth = new BasicDayOfMonthDateTimeField(this, fields->days);
    fields->dayOfYear = new BasicDayOfYearDateTimeField(this, fields->days);
    fields->monthOfYear = new GJMonthOfYearDateTimeField(this);
    fields->weekyear = new BasicWeekyearDateTimeField(this);
    fields->weekOfWeekyear = new BasicWeekOfWeekyearDateTimeField(this, fields->weeks);
    
    field = new RemainderDateTimeField(fields->weekyear, fields->centuries, DateTimeFieldType::weekyearOfCentury(), 100);
    fields->weekyearOfCentury = new OffsetDateTimeField(field, DateTimeFieldType::weekyearOfCentury(), 1);
    
    // The remaining (imprecise) durations are available from the newly
    // created datetime fields->
    fields->years = fields->year->getDurationField();
    fields->months = fields->monthOfYear->getDurationField();
    fields->weekyears = fields->weekyear->getDurationField();
}

CODATIME_END
