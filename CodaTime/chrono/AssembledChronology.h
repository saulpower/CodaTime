//
//  AssembledChronology.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_AssembledChronology_h
#define CodaTime_AssembledChronology_h

#include "CodaTimeMacros.h"

#include "BaseChronology.h"
#include "DurationField.h"
#include "DateTimeField.h"

CODATIME_BEGIN

/**
 * Abstract Chronology that enables chronologies to be assembled from
 * a container of fields->
 * <p>
 * AssembledChronology is thread-safe and immutable.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class AssembledChronology : public BaseChronology {
    
public:
    
    DateTimeZone *getZone() const {
        Chronology *base;
        if ((base = iBase) != NULL) {
            return base->getZone();
        }
        return NULL;
    }
    
    int64_t getDateTimeMillis(int year, int monthOfYear, int dayOfMonth,
                               int millisOfDay) {
        Chronology *base;
        if ((base = iBase) != NULL && (iBaseFlags & 6) == 6) {
            // Only call specialized implementation if applicable fields are the same.
            return base->getDateTimeMillis(year, monthOfYear, dayOfMonth, millisOfDay);
        }
        return BaseChronology::getDateTimeMillis(year, monthOfYear, dayOfMonth, millisOfDay);
    }
    
    int64_t getDateTimeMillis(int year, int monthOfYear, int dayOfMonth,
                               int hourOfDay, int minuteOfHour,
                               int secondOfMinute, int millisOfSecond) {
        Chronology *base;
        if ((base = iBase) != NULL && (iBaseFlags & 5) == 5) {
            // Only call specialized implementation if applicable fields are the same.
            return base->getDateTimeMillis(year, monthOfYear, dayOfMonth,
                                          hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond);
        }
        return BaseChronology::getDateTimeMillis(year, monthOfYear, dayOfMonth,
                                       hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond);
    }
    
    int64_t getDateTimeMillis(int64_t instant,
                               int hourOfDay, int minuteOfHour,
                               int secondOfMinute, int millisOfSecond) {
        Chronology *base;
        if ((base = iBase) != NULL && (iBaseFlags & 1) == 1) {
            // Only call specialized implementation if applicable fields are the same.
            return base->getDateTimeMillis
            (instant, hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond);
        }
        return BaseChronology::getDateTimeMillis
        (instant, hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond);
    }
    
    const DurationField *millis() {
        return iMillis;
    }
    
    const DateTimeField *millisOfSecond() {
        return iMillisOfSecond;
    }
    
    const DateTimeField *millisOfDay() {
        return iMillisOfDay;
    }
    
    const DurationField *seconds() {
        return iSeconds;
    }
    
    const DateTimeField *secondOfMinute() {
        return iSecondOfMinute;
    }
    
    const DateTimeField *secondOfDay() {
        return iSecondOfDay;
    }
    
    const DurationField *minutes() {
        return iMinutes;
    }
    
    const DateTimeField *minuteOfHour() {
        return iMinuteOfHour;
    }
    
    const DateTimeField *minuteOfDay() {
        return iMinuteOfDay;
    }
    
    const DurationField *hours() {
        return iHours;
    }
    
    const DateTimeField *hourOfDay() {
        return iHourOfDay;
    }
    
    const DateTimeField *clockhourOfDay() {
        return iClockhourOfDay;
    }
    
    const DurationField *halfdays() {
        return iHalfdays;
    }
    
    const DateTimeField *hourOfHalfday() {
        return iHourOfHalfday;
    }
    
    const DateTimeField *clockhourOfHalfday() {
        return iClockhourOfHalfday;
    }
    
    const DateTimeField *halfdayOfDay() {
        return iHalfdayOfDay;
    }
    
    const DurationField *days() {
        return iDays;
    }
    
    const DateTimeField *dayOfWeek() {
        return iDayOfWeek;
    }
    
    const DateTimeField *dayOfMonth() {
        return iDayOfMonth;
    }
    
    const DateTimeField *dayOfYear() {
        return iDayOfYear;
    }
    
    const DurationField *weeks() {
        return iWeeks;
    }
    
    const DateTimeField *weekOfWeekyear() {
        return iWeekOfWeekyear;
    }
    
    const DurationField *weekyears() {
        return iWeekyears;
    }
    
    const DateTimeField *weekyear() {
        return iWeekyear;
    }
    
    const DateTimeField *weekyearOfCentury() {
        return iWeekyearOfCentury;
    }
    
    const DurationField *months() {
        return iMonths;
    }
    
    const DateTimeField *monthOfYear() {
        return iMonthOfYear;
    }
    
    const DurationField *years() {
        return iYears;
    }
    
    const DateTimeField *year() {
        return iYear;
    }
    
    const DateTimeField *yearOfEra() {
        return iYearOfEra;
    }
    
    const DateTimeField *yearOfCentury() {
        return iYearOfCentury;
    }
    
    const DurationField *centuries() {
        return iCenturies;
    }
    
    const DateTimeField *centuryOfEra() {
        return iCenturyOfEra;
    }
    
    const DurationField *eras() {
        return iEras;
    }
    
    const DateTimeField *era() {
        return iEra;
    }
    
    /**
     * A container of fields used for assembling a chronology.
     */
    class Fields {
        
    public:
        
        const DurationField *millis;
        const DurationField *seconds;
        const DurationField *minutes;
        const DurationField *hours;
        const DurationField *halfdays;
        
        const DurationField *days;
        const DurationField *weeks;
        const DurationField *weekyears;
        const DurationField *months;
        const DurationField *years;
        const DurationField *centuries;
        const DurationField *eras;
        
        const DateTimeField *millisOfSecond;
        const DateTimeField *millisOfDay;
        const DateTimeField *secondOfMinute;
        const DateTimeField *secondOfDay;
        const DateTimeField *minuteOfHour;
        const DateTimeField *minuteOfDay;
        const DateTimeField *hourOfDay;
        const DateTimeField *clockhourOfDay;
        const DateTimeField *hourOfHalfday;
        const DateTimeField *clockhourOfHalfday;
        const DateTimeField *halfdayOfDay;
        
        const DateTimeField *dayOfWeek;
        const DateTimeField *dayOfMonth;
        const DateTimeField *dayOfYear;
        const DateTimeField *weekOfWeekyear;
        const DateTimeField *weekyear;
        const DateTimeField *weekyearOfCentury;
        const DateTimeField *monthOfYear;
        const DateTimeField *year;
        const DateTimeField *yearOfEra;
        const DateTimeField *yearOfCentury;
        const DateTimeField *centuryOfEra;
        const DateTimeField *era;
        
        Fields() {
        }
        
        /**
         * Copy the supported fields from a chronology into this container.
         */
        void copyFieldsFrom(Chronology *chrono) {
            {
                const DurationField *f;
                if (isSupported(f = chrono->millis())) {
                    millis = f;
                }
                if (isSupported(f = chrono->seconds())) {
                    seconds = f;
                }
                if (isSupported(f = chrono->minutes())) {
                    minutes = f;
                }
                if (isSupported(f = chrono->hours())) {
                    hours = f;
                }
                if (isSupported(f = chrono->halfdays())) {
                    halfdays = f;
                }
                if (isSupported(f = chrono->days())) {
                    days = f;
                }
                if (isSupported(f = chrono->weeks())) {
                    weeks = f;
                }
                if (isSupported(f = chrono->weekyears())) {
                    weekyears = f;
                }
                if (isSupported(f = chrono->months())) {
                    months = f;
                }
                if (isSupported(f = chrono->years())) {
                    years = f;
                }
                if (isSupported(f = chrono->centuries())) {
                    centuries = f;
                }
                if (isSupported(f = chrono->eras())) {
                    eras = f;
                }
            }
            
            {
                const DateTimeField *f;
                if (isSupported(f = chrono->millisOfSecond())) {
                    millisOfSecond = f;
                }
                if (isSupported(f = chrono->millisOfDay())) {
                    millisOfDay = f;
                }
                if (isSupported(f = chrono->secondOfMinute())) {
                    secondOfMinute = f;
                }
                if (isSupported(f = chrono->secondOfDay())) {
                    secondOfDay = f;
                }
                if (isSupported(f = chrono->minuteOfHour())) {
                    minuteOfHour = f;
                }
                if (isSupported(f = chrono->minuteOfDay())) {
                    minuteOfDay = f;
                }
                if (isSupported(f = chrono->hourOfDay())) {
                    hourOfDay = f;
                }
                if (isSupported(f = chrono->clockhourOfDay())) {
                    clockhourOfDay = f;
                }
                if (isSupported(f = chrono->hourOfHalfday())) {
                    hourOfHalfday = f;
                }
                if (isSupported(f = chrono->clockhourOfHalfday())) {
                    clockhourOfHalfday = f;
                }
                if (isSupported(f = chrono->halfdayOfDay())) {
                    halfdayOfDay = f;
                }
                if (isSupported(f = chrono->dayOfWeek())) {
                    dayOfWeek = f;
                }
                if (isSupported(f = chrono->dayOfMonth())) {
                    dayOfMonth = f;
                }
                if (isSupported(f = chrono->dayOfYear())) {
                    dayOfYear = f;
                }
                if (isSupported(f = chrono->weekOfWeekyear())) {
                    weekOfWeekyear = f;
                }
                if (isSupported(f = chrono->weekyear())) {
                    weekyear = f;
                }
                if (isSupported(f = chrono->weekyearOfCentury())) {
                    weekyearOfCentury = f;
                }
                if (isSupported(f = chrono->monthOfYear())) {
                    monthOfYear = f;
                }
                if (isSupported(f = chrono->year())) {
                    year = f;
                }
                if (isSupported(f = chrono->yearOfEra())) {
                    yearOfEra = f;
                }
                if (isSupported(f = chrono->yearOfCentury())) {
                    yearOfCentury = f;
                }
                if (isSupported(f = chrono->centuryOfEra())) {
                    centuryOfEra = f;
                }
                if (isSupported(f = chrono->era())) {
                    era = f;
                }
            }
        }
        
    private:
        
        static bool isSupported(const DurationField *field) {
            return field == NULL ? false : field->isSupported();
        }
        
        static bool isSupported(const DateTimeField *field) {
            return field == NULL ? false : field->isSupported();
        }
    };
    
private:
    
    
    static const long long serialVersionUID = -6728465968995518215L;
    
    Chronology *iBase;
    void *iParam;
    
    const DurationField *iMillis;
    const DurationField *iSeconds;
    const DurationField *iMinutes;
    const DurationField *iHours;
    const DurationField *iHalfdays;
    
    const DurationField *iDays;
    const DurationField *iWeeks;
    const DurationField *iWeekyears;
    const DurationField *iMonths;
    const DurationField *iYears;
    const DurationField *iCenturies;
    const DurationField *iEras;
    
    const DateTimeField *iMillisOfSecond;
    const DateTimeField *iMillisOfDay;
    const DateTimeField *iSecondOfMinute;
    const DateTimeField *iSecondOfDay;
    const DateTimeField *iMinuteOfHour;
    const DateTimeField *iMinuteOfDay;
    const DateTimeField *iHourOfDay;
    const DateTimeField *iClockhourOfDay;
    const DateTimeField *iHourOfHalfday;
    const DateTimeField *iClockhourOfHalfday;
    const DateTimeField *iHalfdayOfDay;
    
    const DateTimeField *iDayOfWeek;
    const DateTimeField *iDayOfMonth;
    const DateTimeField *iDayOfYear;
    const DateTimeField *iWeekOfWeekyear;
    const DateTimeField *iWeekyear;
    const DateTimeField *iWeekyearOfCentury;
    const DateTimeField *iMonthOfYear;
    const DateTimeField *iYear;
    const DateTimeField *iYearOfEra;
    const DateTimeField *iYearOfCentury;
    const DateTimeField *iCenturyOfEra;
    const DateTimeField *iEra;
    
    // Bit set determines which base fields are used
    // bit 1 set: hourOfDay, minuteOfHour, secondOfMinute, and millisOfSecond fields
    // bit 2 set: millisOfDayField
    // bit 3 set: year, monthOfYear, and dayOfMonth fields
    int iBaseFlags = 0;
    
    void setFields() {
        Fields *fields = new Fields();
        if (iBase != NULL) {
            fields->copyFieldsFrom(iBase);
        }
        assemble(fields);
        
        {
            const DurationField *f;
            iMillis    = (f = fields->millis)    != NULL ? f : BaseChronology::millis();
            iSeconds   = (f = fields->seconds)   != NULL ? f : BaseChronology::seconds();
            iMinutes   = (f = fields->minutes)   != NULL ? f : BaseChronology::minutes();
            iHours     = (f = fields->hours)     != NULL ? f : BaseChronology::hours();
            iHalfdays  = (f = fields->halfdays)  != NULL ? f : BaseChronology::halfdays();
            iDays      = (f = fields->days)      != NULL ? f : BaseChronology::days();
            iWeeks     = (f = fields->weeks)     != NULL ? f : BaseChronology::weeks();
            iWeekyears = (f = fields->weekyears) != NULL ? f : BaseChronology::weekyears();
            iMonths    = (f = fields->months)    != NULL ? f : BaseChronology::months();
            iYears     = (f = fields->years)     != NULL ? f : BaseChronology::years();
            iCenturies = (f = fields->centuries) != NULL ? f : BaseChronology::centuries();
            iEras      = (f = fields->eras)      != NULL ? f : BaseChronology::eras();
        }
        
        {
            const DateTimeField *f;
            iMillisOfSecond     = (f = fields->millisOfSecond)     != NULL ? f : BaseChronology::millisOfSecond();
            iMillisOfDay        = (f = fields->millisOfDay)        != NULL ? f : BaseChronology::millisOfDay();
            iSecondOfMinute     = (f = fields->secondOfMinute)     != NULL ? f : BaseChronology::secondOfMinute();
            iSecondOfDay        = (f = fields->secondOfDay)        != NULL ? f : BaseChronology::secondOfDay();
            iMinuteOfHour       = (f = fields->minuteOfHour)       != NULL ? f : BaseChronology::minuteOfHour();
            iMinuteOfDay        = (f = fields->minuteOfDay)        != NULL ? f : BaseChronology::minuteOfDay();
            iHourOfDay          = (f = fields->hourOfDay)          != NULL ? f : BaseChronology::hourOfDay();
            iClockhourOfDay     = (f = fields->clockhourOfDay)     != NULL ? f : BaseChronology::clockhourOfDay();
            iHourOfHalfday      = (f = fields->hourOfHalfday)      != NULL ? f : BaseChronology::hourOfHalfday();
            iClockhourOfHalfday = (f = fields->clockhourOfHalfday) != NULL ? f : BaseChronology::clockhourOfHalfday();
            iHalfdayOfDay       = (f = fields->halfdayOfDay)       != NULL ? f : BaseChronology::halfdayOfDay();
            iDayOfWeek          = (f = fields->dayOfWeek)          != NULL ? f : BaseChronology::dayOfWeek();
            iDayOfMonth         = (f = fields->dayOfMonth)         != NULL ? f : BaseChronology::dayOfMonth();
            iDayOfYear          = (f = fields->dayOfYear)          != NULL ? f : BaseChronology::dayOfYear();
            iWeekOfWeekyear     = (f = fields->weekOfWeekyear)     != NULL ? f : BaseChronology::weekOfWeekyear();
            iWeekyear           = (f = fields->weekyear)           != NULL ? f : BaseChronology::weekyear();
            iWeekyearOfCentury  = (f = fields->weekyearOfCentury)  != NULL ? f : BaseChronology::weekyearOfCentury();
            iMonthOfYear        = (f = fields->monthOfYear)        != NULL ? f : BaseChronology::monthOfYear();
            iYear               = (f = fields->year)               != NULL ? f : BaseChronology::year();
            iYearOfEra          = (f = fields->yearOfEra)          != NULL ? f : BaseChronology::yearOfEra();
            iYearOfCentury      = (f = fields->yearOfCentury)      != NULL ? f : BaseChronology::yearOfCentury();
            iCenturyOfEra       = (f = fields->centuryOfEra)       != NULL ? f : BaseChronology::centuryOfEra();
            iEra                = (f = fields->era)                != NULL ? f : BaseChronology::era();
        }
        
        int flags;
        if (iBase == NULL) {
            flags = 0;
        } else {
            flags =
            ((iHourOfDay      == iBase->hourOfDay()      &&
              iMinuteOfHour   == iBase->minuteOfHour()   &&
              iSecondOfMinute == iBase->secondOfMinute() &&
              iMillisOfSecond == iBase->millisOfSecond()   ) ? 1 : 0) |
            
            ((iMillisOfDay == iBase->millisOfDay()) ? 2 : 0) |
            
            ((iYear        == iBase->year()        &&
              iMonthOfYear == iBase->monthOfYear() &&
              iDayOfMonth  == iBase->dayOfMonth()    ) ? 4 : 0);
        }
        
        iBaseFlags = flags;
    }
    
//    void readObject(ObjectInputStream in) {
//        in.defaultReadObject();
//        setFields();
//    }
    
protected:
    
    /**
     * Constructor calls the assemble method, enabling subclasses to define its
     * supported fields-> If a base chronology is supplied, the field set
     * initially contains references to each base chronology field.
     * <p>
     * Other methods in this class will delegate to the base chronology, if it
     * can be determined that the base chronology will produce the same results
     * as AbstractChronology.
     *
     * @param base optional base chronology to copy initial fields from
     * @param param optional param object avalable for assemble method
     */
    AssembledChronology(Chronology *base, void *param) {
        iBase = base;
        iParam = param;
        setFields();
    }
    
    /**
     * Invoked by the constructor and after deserialization to allow subclasses
     * to define all of its supported fields-> All unset fields default to
     * unsupported instances.
     *
     * @param fields container of fields
     */
    virtual void assemble(Fields *fields) = 0;
    
    /**
     * Returns the same base chronology as passed into the constructor.
     */
    Chronology *getBase() const {
        return iBase;
    }
    
    /**
     * Returns the same param object as passed into the constructor.
     */
    void *getParam() {
        return iParam;
    }
};

CODATIME_END

#endif
