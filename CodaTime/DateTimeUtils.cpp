//
//  DateTimeUtils.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "DateTimeUtils.h"

#include "Interval.h"
#include "Exceptions.h"
#include "chrono/ISOChronology.h"
#include "PeriodType.h"

#include <math.h>

CODATIME_BEGIN

SystemMillisProvider *DateTimeUtils::SYSTEM_MILLIS_PROVIDER = new SystemMillisProvider();
MillisProvider *DateTimeUtils::cMillisProvider = SYSTEM_MILLIS_PROVIDER;

//-----------------------------------------------------------------------
/**
 * Gets the current time in milliseconds.
 * <p>
 * By default this returns <code>System.currentTimeMillis()</code>.
 * This may be changed using other methods in this class.
 *
 * @return the current time in milliseconds from 1970-01-01T00:00:00Z
 */
const int64_t DateTimeUtils::currentTimeMillis() {
    return cMillisProvider->getMillis();
}

/**
 * Resets the current time to return the system time.
 * <p>
 * This method changes the behaviour of {@link #currentTimeMillis()}.
 * Whenever the current time is queried, {@link System#currentTimeMillis()} is used.
 *
 * @throws SecurityException if the application does not have sufficient security rights
 */
//public static const void setCurrentMillisSystem() throws SecurityException {
//    checkPermission();
//    cMillisProvider = SYSTEM_MILLIS_PROVIDER;
//}

/**
 * Sets the current time to return a fixed millisecond time.
 * <p>
 * This method changes the behaviour of {@link #currentTimeMillis()}.
 * Whenever the current time is queried, the same millisecond time will be returned.
 *
 * @param fixedMillis  the fixed millisecond time to use
 * @throws SecurityException if the application does not have sufficient security rights
 */
//public static const void setCurrentMillisFixed(int64_t fixedMillis) throws SecurityException {
//    checkPermission();
//    cMillisProvider = new FixedMillisProvider(fixedMillis);
//}

/**
 * Sets the current time to return the system time plus an offset.
 * <p>
 * This method changes the behaviour of {@link #currentTimeMillis()}.
 * Whenever the current time is queried, {@link System#currentTimeMillis()} is used
 * and then offset by adding the millisecond value specified here.
 *
 * @param offsetMillis  the fixed millisecond time to use
 * @throws SecurityException if the application does not have sufficient security rights
 */
//public static const void setCurrentMillisOffset(int64_t offsetMillis) throws SecurityException {
//    checkPermission();
//    if (offsetMillis == 0) {
//        cMillisProvider = SYSTEM_MILLIS_PROVIDER;
//    } else {
//        cMillisProvider = new OffsetMillisProvider(offsetMillis);
//    }
//}

/**
 * Sets the provider of the current time to class specified.
 * <p>
 * This method changes the behaviour of {@link #currentTimeMillis()}.
 * Whenever the current time is queried, the specified class will be called.
 *
 * @param millisProvider  the provider of the current time to use, not NULL
 * @throws SecurityException if the application does not have sufficient security rights
 * @since 2.0
 */
//public static const void setCurrentMillisProvider(MillisProvider millisProvider) throws SecurityException {
//    if (millisProvider == NULL) {
//        throw new IllegalArgumentException("The MillisProvider must not be NULL");
//    }
//    checkPermission();
//    cMillisProvider = millisProvider;
//}

/**
 * Checks whether the provider may be changed using permission 'CurrentTime.setProvider'.
 *
 * @throws SecurityException if the provider may not be changed
 */
//private static void checkPermission() throws SecurityException {
//    SecurityManager sm = System.getSecurityManager();
//    if (sm != NULL) {
//        sm.checkPermission(new JodaTimePermission("CurrentTime.setProvider"));
//    }
//}

//-----------------------------------------------------------------------
/**
 * Gets the millisecond instant from the specified instant object handling NULL.
 * <p>
 * If the instant object is <code>NULL</code>, the {@link #currentTimeMillis()}
 * will be returned. Otherwise, the millis from the object are returned.
 *
 * @param instant  the instant to examine, NULL means now
 * @return the time in milliseconds from 1970-01-01T00:00:00Z
 */
const int64_t DateTimeUtils::getInstantMillis(ReadableInstant *instant) {
    if (instant == NULL) {
        return DateTimeUtils::currentTimeMillis();
    }
    return instant->getMillis();
}

//-----------------------------------------------------------------------
/**
 * Gets the chronology from the specified instant object handling NULL.
 * <p>
 * If the instant object is <code>NULL</code>, or the instant's chronology is
 * <code>NULL</code>, {@link ISOChronology#getInstance()} will be returned.
 * Otherwise, the chronology from the object is returned.
 *
 * @param instant  the instant to examine, NULL means ISO in the default zone
 * @return the chronology, never NULL
 */
Chronology *DateTimeUtils::getInstantChronology(ReadableInstant *instant) {
    if (instant == NULL) {
        return ISOChronology::getInstance();
    }
    Chronology *chrono = instant->getChronology();
    if (chrono == NULL) {
        return ISOChronology::getInstance();
    }
    return chrono;
}

//-----------------------------------------------------------------------
/**
 * Gets the chronology from the specified instant based interval handling NULL.
 * <p>
 * The chronology is obtained from the start if that is not NULL, or from the
 * end if the start is NULL. The result is additionally checked, and if still
 * NULL then {@link ISOChronology#getInstance()} will be returned.
 *
 * @param start  the instant to examine and use as the primary source of the chronology
 * @param end  the instant to examine and use as the secondary source of the chronology
 * @return the chronology, never NULL
 */
Chronology *DateTimeUtils::getIntervalChronology(ReadableInstant *start, ReadableInstant *end) {
    Chronology *chrono = NULL;
    if (start != NULL) {
        chrono = start->getChronology();
    } else if (end != NULL) {
        chrono = end->getChronology();
    }
    if (chrono == NULL) {
        chrono = ISOChronology::getInstance();
    }
    return chrono;
}

//-----------------------------------------------------------------------
/**
 * Gets the chronology from the specified interval object handling NULL.
 * <p>
 * If the interval object is <code>NULL</code>, or the interval's chronology is
 * <code>NULL</code>, {@link ISOChronology#getInstance()} will be returned.
 * Otherwise, the chronology from the object is returned.
 *
 * @param interval  the interval to examine, NULL means ISO in the default zone
 * @return the chronology, never NULL
 */
Chronology *DateTimeUtils::getIntervalChronology(ReadableInterval *interval) {
    if (interval == NULL) {
        return ISOChronology::getInstance();
    }
    Chronology *chrono = interval->getChronology();
    if (chrono == NULL) {
        return ISOChronology::getInstance();
    }
    return chrono;
}

//-----------------------------------------------------------------------
/**
 * Gets the interval handling NULL.
 * <p>
 * If the interval is <code>NULL</code>, an interval representing now
 * to now in the {@link ISOChronology#getInstance() ISOChronology}
 * will be returned. Otherwise, the interval specified is returned.
 *
 * @param interval  the interval to use, NULL means now to now
 * @return the interval, never NULL
 * @since 1.1
 */
ReadableInterval *DateTimeUtils::getReadableInterval(ReadableInterval *interval) {
    if (interval == NULL) {
        int64_t now = DateTimeUtils::currentTimeMillis();
        interval = new Interval(now, now);
    }
    return interval;
}

//-----------------------------------------------------------------------
/**
 * Gets the chronology handling NULL.
 * <p>
 * If the chronology is <code>NULL</code>, {@link ISOChronology#getInstance()}
 * will be returned. Otherwise, the chronology is returned.
 *
 * @param chrono  the chronology to use, NULL means ISO in the default zone
 * @return the chronology, never NULL
 */
Chronology *DateTimeUtils::getChronology(Chronology *chrono) {
    if (chrono == NULL) {
        return ISOChronology::getInstance();
    }
    return chrono;
}

//-----------------------------------------------------------------------
/**
 * Gets the zone handling NULL.
 * <p>
 * If the zone is <code>NULL</code>, {@link DateTimeZone#getDefault()}
 * will be returned. Otherwise, the zone specified is returned.
 *
 * @param zone  the time zone to use, NULL means the default zone
 * @return the time zone, never NULL
 */
DateTimeZone *DateTimeUtils::getZone(DateTimeZone *zone) {
    if (zone == NULL) {
        return DateTimeZone::getDefault();
    }
    return zone;
}

//-----------------------------------------------------------------------
/**
 * Gets the period type handling NULL.
 * <p>
 * If the zone is <code>NULL</code>, {@link PeriodType#standard()}
 * will be returned. Otherwise, the type specified is returned.
 *
 * @param type  the time zone to use, NULL means the standard type
 * @return the type to use, never NULL
 */
const PeriodType *DateTimeUtils::getPeriodType(const PeriodType *type) {
    if (type == NULL) {
        return PeriodType::standard();
    }
    return type;
}

//-----------------------------------------------------------------------
/**
 * Gets the millisecond duration from the specified duration object handling NULL.
 * <p>
 * If the duration object is <code>NULL</code>, zero will be returned.
 * Otherwise, the millis from the object are returned.
 *
 * @param duration  the duration to examine, NULL means zero
 * @return the duration in milliseconds
 */
const int64_t DateTimeUtils::getDurationMillis(ReadableDuration *duration) {
    if (duration == NULL) {
        return 0L;
    }
    return duration->getMillis();
}

//-----------------------------------------------------------------------
/**
 * Checks whether the partial is contiguous.
 * <p>
 * A partial is contiguous if one field starts where another ends.
 * <p>
 * For example <code>LocalDate</code> is contiguous because DayOfMonth has
 * the same range (Month) as the unit of the next field (MonthOfYear), and
 * MonthOfYear has the same range (Year) as the unit of the next field (Year).
 * <p>
 * Similarly, <code>LocalTime</code> is contiguous, as it consists of
 * MillisOfSecond, SecondOfMinute, MinuteOfHour and HourOfDay (note how
 * the names of each field 'join up').
 * <p>
 * However, a Year/HourOfDay partial is not contiguous because the range
 * field Day is not equal to the next field Year.
 * Similarly, a DayOfWeek/DayOfMonth partial is not contiguous because
 * the range Month is not equal to the next field Day.
 *
 * @param partial  the partial to check
 * @return true if the partial is contiguous
 * @throws IllegalArgumentException if the partial is NULL
 * @since 1.1
 */
const bool DateTimeUtils::isContiguous(ReadablePartial *partial) {
    if (partial == NULL) {
        throw IllegalArgumentException("Partial must not be NULL");
    }
    const DurationFieldType *lastType = NULL;
    for (int i = 0; i < partial->size(); i++) {
        const DateTimeField *loopField = partial->getField(i);
        if (i > 0) {
            if (loopField->getRangeDurationField() == NULL || loopField->getRangeDurationField()->getType() != lastType) {
                return false;
            }
        }
        lastType = loopField->getDurationField()->getType();
    }
    return true;
}

//-----------------------------------------------------------------------
/**
 * Gets the {@link DateFormatSymbols} based on the given locale.
 * <p>
 * If JDK 6 or newer is being used, DateFormatSymbols.getInstance(locale) will
 * be used in order to allow the use of locales defined as extensions.
 * Otherwise, new DateFormatSymbols(locale) will be used.
 * See JDK 6 {@link DateFormatSymbols} for further information.
 *
 * @param locale  the {@link Locale} used to get the correct {@link DateFormatSymbols}
 * @return the symbols
 * @since 2.0
 */
//public static const DateFormatSymbols getDateFormatSymbols(Locale locale) {
//    try {
//        Method method = DateFormatSymbols.class.getMethod("getInstance", new Class[] {Locale.class});
//        return (DateFormatSymbols) method.invoke(NULL, new Object[] {locale});
//    } catch (Exception ex) {
//        return new DateFormatSymbols(locale);
//    }
//}

//-----------------------------------------------------------------------
/**
 * Gets the default map of time zone names.
 * <p>
 * This can be changed by {@link #setDefaultTimeZoneNames}.
 * <p>
 * The default set of short time zone names is as follows:
 * <ul>
 * <li>UT - UTC
 * <li>UTC - UTC
 * <li>GMT - UTC
 * <li>EST - America/New_York
 * <li>EDT - America/New_York
 * <li>CST - America/Chicago
 * <li>CDT - America/Chicago
 * <li>MST - America/Denver
 * <li>MDT - America/Denver
 * <li>PST - America/Los_Angeles
 * <li>PDT - America/Los_Angeles
 * </ul>
 *
 * @return the unmodifiable map of abbreviations to zones, not NULL
 * @since 2.2
 */
const map<string, DateTimeZone*> DateTimeUtils::getDefaultTimeZoneNames() {
    return cZoneNames;
}

/**
 * Sets the default map of time zone names.
 * <p>
 * The map is copied before storage.
 *
 * @param names  the map of abbreviations to zones, not NULL
 * @since 2.2
 */
const void DateTimeUtils::setDefaultTimeZoneNames(map<string, DateTimeZone*> names) {
    cZoneNames = names;
}

//-------------------------------------------------------------------------
/**
 * Calculates the astronomical Julian Day for an instant.
 * <p>
 * The <a href="http://en.wikipedia.org/wiki/Julian_day">Julian day</a> is a well-known
 * system of time measurement for scientific use by the astronomy community.
 * It expresses the interval of time in days and fractions of a day since
 * January 1, 4713 BC (Julian) Greenwich noon.
 * <p>
 * Each day starts at midday (not midnight) and time is expressed as a fraction.
 * Thus the fraction 0.25 is 18:00. equal to one quarter of the day from midday to midday.
 * <p>
 * Note that this method has nothing to do with the day-of-year.
 *
 * @param epochMillis  the epoch millis from 1970-01-01Z
 * @return the astronomical Julian Day represented by the specified instant
 * @since 2.2
 */
const double DateTimeUtils::toJulianDay(int64_t epochMillis) {
    // useful links
    // http://en.wikipedia.org/wiki/Julian_day#cite_note-13 - Wikipedia
    // http://aa.usno.navy.mil/data/docs/JulianDate.php" - USNO
    // http://users.zoominternet.net/~matto/Java/Julian%20Date%20Converter.htm - Julian Date Converter by Matt Oltersdorf
    // http://ssd.jpl.nasa.gov/tc.cgi#top - CalTech
    double epochDay = epochMillis / 86400000.0;
    return epochDay + 2440587.5;
}

/**
 * Calculates the astronomical Julian Day Number for an instant.
 * <p>
 * The {@link #toJulianDay(int64_t)} method calculates the astronomical Julian Day
 * with a fraction based on days starting at midday.
 * This method calculates the variant where days start at midnight.
 * JDN 0 is used for the date equivalent to Monday January 1, 4713 BC (Julian).
 * Thus these days start 12 hours before those of the fractional Julian Day.
 * <p>
 * Note that this method has nothing to do with the day-of-year.
 *
 * @param epochMillis  the epoch millis from 1970-01-01Z
 * @return the astronomical Julian Day represented by the specified instant
 * @since 2.2
 */
const int64_t DateTimeUtils::toJulianDayNumber(int64_t epochMillis) {
    return (int64_t) floor(toJulianDay(epochMillis) + 0.5);
}

/**
 * Creates a date-time from a Julian Day.
 * <p>
 * Returns the {@code DateTime} object equal to the specified Julian Day.
 *
 * @param julianDay  the Julian Day
 * @return the epoch millis from 1970-01-01Z
 * @since 2.2
 */
const int64_t DateTimeUtils::fromJulianDay(double julianDay) {
    double epochDay = julianDay - 2440587.5;
    return (int64_t) (epochDay * 86400000.0);
}

CODATIME_END