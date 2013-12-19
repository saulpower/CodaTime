//
//  DateTimeZone.c
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "DateTimeZone.h"

#include "DateTimeConstants.h"
#include "DateTimeUtils.h"
#include "Exceptions.h"
#include "format/DateTimeFormatter.h"
#include "format/FormatUtils.h"
#include "LocalDateTime.h"
#include "tz/FixedDateTimeZone.h"
#include "tz/NameProvider.h"
#include "tz/Provider.h"

CODATIME_BEGIN

DateTimeZone *DateTimeZone::UTC = new FixedDateTimeZone("UTC", "UTC", 0, 0);
Provider *DateTimeZone::cProvider = NULL;
NameProvider *DateTimeZone::cNameProvider = NULL;

// TODO: synchronized
DateTimeZone *DateTimeZone::fixedOffsetZone(string id, int offset) {
    if (offset == 0) {
        return DateTimeZone::UTC;
    }
    DateTimeZone *zone = NULL;
    DateTimeZone *ref = iFixedOffsetCache[id];
    if (ref != NULL) {
        zone = ref;
        if (zone != NULL) {
            return zone;
        }
    }
    zone = new FixedDateTimeZone(id, NULL, offset, offset);
    iFixedOffsetCache[id] = zone;
    return zone;
}

/**
 * Sets the zone provider factory without performing the security check.
 *
 * @param provider  provider to use, or NULL for default
 * @throws IllegalArgumentException if the provider is invalid
 */
void DateTimeZone::setProvider0(Provider *provider) {
    if (provider == NULL) {
        provider = getDefaultProvider();
    }
    set<string> ids = provider->getAvailableIDs();
    if (ids.empty() || ids.size() == 0) {
        throw IllegalArgumentException
        ("The provider doesn't have any available ids");
    }
    
    if (ids.find(string("UTC")) == ids.end()) {
        throw new IllegalArgumentException("The provider doesn't support UTC");
    }
    if (!UTC->equals(provider->getZone("UTC"))) {
        throw new IllegalArgumentException("Invalid UTC zone provided");
    }
    cProvider = provider;
    cAvailableIDs = ids;
}

/**
 * Gets the default zone provider.
 * <p>
 * Tries the system property <code>org.joda.time.DateTimeZone::Provider</code>.
 * Then tries a <code>ZoneInfoProvider</code> using the data in <code>org/joda/time/tz/data</code>.
 * Then uses <code>UTCProvider</code>.
 *
 * @return the default name provider
 */
Provider *DateTimeZone::getDefaultProvider() {
    
    Provider *provider = NULL;
    
    if (provider == NULL) {
        try {
            provider = new ZoneInfoProvider("org/joda/time/tz/data");
        } catch (exception ex) {
        }
    }
    
    if (provider == NULL) {
        provider = new UTCProvider();
    }
    
    return provider;
}

/**
 * Sets the name provider factory without performing the security check.
 *
 * @param nameProvider  provider to use, or NULL for default
 * @throws IllegalArgumentException if the provider is invalid
 */
void DateTimeZone::setNameProvider0(NameProvider *nameProvider) {
    if (nameProvider == NULL) {
        nameProvider = getDefaultNameProvider();
    }
    cNameProvider = nameProvider;
}

/**
 * Gets the default name provider.
 * <p>
 * Tries the system property <code>org.joda.time.DateTimeZone::NameProvider</code>.
 * Then uses <code>DefaultNameProvider</code>.
 *
 * @return the default name provider
 */
NameProvider *DateTimeZone::getDefaultNameProvider() {
    
    NameProvider *nameProvider = NULL;
    
    if (nameProvider == NULL) {
        nameProvider = new DefaultNameProvider();
    }
    
    return nameProvider;
}

//-----------------------------------------------------------------------
/**
 * Converts an old style id to a new style id.
 *
 * @param id  the old style id
 * @return the new style id, NULL if not found
 */
// TODO: synchronized
string DateTimeZone::getConvertedId(string id) {
    map<string, string> &map = cZoneIdConversion;
    if (map.empty()) {
        // Backwards compatibility with TimeZone.
        map["GMT"] = "UTC";
        map["WET"] = "WET";
        map["CET"] = "CET";
        map["MET"] = "CET";
        map["ECT"] = "CET";
        map["EET"] = "EET";
        map["MIT"] = "Pacific/Apia";
        map["HST"] = "Pacific/Honolulu";  // JDK 1.1 compatible
        map["AST"] = "America/Anchorage";
        map["PST"] = "America/Los_Angeles";
        map["MST"] = "America/Denver";  // JDK 1.1 compatible
        map["PNT"] = "America/Phoenix";
        map["CST"] = "America/Chicago";
        map["EST"] = "America/New_York";  // JDK 1.1 compatible
        map["IET"] = "America/Indiana/Indianapolis";
        map["PRT"] = "America/Puerto_Rico";
        map["CNT"] = "America/St_Johns";
        map["AGT"] = "America/Argentina/Buenos_Aires";
        map["BET"] = "America/Sao_Paulo";
        map["ART"] = "Africa/Cairo";
        map["CAT"] = "Africa/Harare";
        map["EAT"] = "Africa/Addis_Ababa";
        map["NET"] = "Asia/Yerevan";
        map["PLT"] = "Asia/Karachi";
        map["IST"] = "Asia/Kolkata";
        map["BST"] = "Asia/Dhaka";
        map["VST"] = "Asia/Ho_Chi_Minh";
        map["CTT"] = "Asia/Shanghai";
        map["JST"] = "Asia/Tokyo";
        map["ACT"] = "Australia/Darwin";
        map["AET"] = "Australia/Sydney";
        map["SST"] = "Pacific/Guadalcanal";
        map["NST"] = "Pacific/Auckland";
    }
    return map[id];
}

int DateTimeZone::parseOffset(string str) {
    // Can't use a real chronology if called during class
    // initialization. Offset parser doesn't need it anyhow.
    Chronology *chrono = new UnrealChrono();
    
    return -(int) offsetFormatter()->withChronology(chrono)->parseMillis(str);
}

/**
 * Formats a timezone offset string.
 * <p>
 * This method is kept separate from the formatting classes to speed and
 * simplify startup and classloading.
 *
 * @param offset  the offset in milliseconds
 * @return the time zone string
 */
string DateTimeZone::printOffset(int offset) {
    
    string buf;
    
    if (offset >= 0) {
        buf.append("+");
    } else {
        buf.append("-");
        offset = -offset;
    }
    
    int hours = offset / DateTimeConstants::MILLIS_PER_HOUR;
    FormatUtils::appendPaddedInteger(buf, hours, 2);
    offset -= hours * (int) DateTimeConstants::MILLIS_PER_HOUR;
    
    int minutes = offset / DateTimeConstants::MILLIS_PER_MINUTE;
    buf.append(1, ':');
    FormatUtils::appendPaddedInteger(buf, minutes, 2);
    offset -= minutes * DateTimeConstants::MILLIS_PER_MINUTE;
    if (offset == 0) {
        return buf;
    }
    
    int seconds = offset / DateTimeConstants::MILLIS_PER_SECOND;
    buf.append(1, ':');
    FormatUtils::appendPaddedInteger(buf, seconds, 2);
    offset -= seconds * DateTimeConstants::MILLIS_PER_SECOND;
    if (offset == 0) {
        return buf;
    }
    
    buf.append(1, '.');
    FormatUtils::appendPaddedInteger(buf, offset, 3);
    return buf;
}

/**
 * Gets a printer/parser for managing the offset id formatting.
 *
 * @return the formatter
 */
// TODO: synchronized
DateTimeFormatter *DateTimeZone::offsetFormatter() {
    if (cOffsetFormatter == NULL) {
        cOffsetFormatter = new DateTimeFormatterBuilder()
        .appendTimeZoneOffset(NULL, true, 2, 4)
        .toFormatter();
    }
    return cOffsetFormatter;
}

DateTimeZone *DateTimeZone::getDefault() {
    
    DateTimeZone *zone = cDefault;
    
    if (zone == NULL) {
        //synchronized(DateTimeZone::class) {
        zone = cDefault;
        if (zone == NULL) {
            DateTimeZone *temp = NULL;
            //                    try {
            //                        try {
            //                            string id = System.getProperty("user.timezone");
            //                            if (id != NULL) {  // NULL check avoids stack overflow
            //                                temp = forID(id);
            //                            }
            //                        } catch (RuntimeException ex) {
            //                            // ignored
            //                        }
            //                        if (temp == NULL) {
            //                            temp = forTimeZone(TimeZone::getDefault());
            //                        }
            //                    } catch (IllegalArgumentException ex) {
            //                        // ignored
            //                    }
            if (temp == NULL) {
                temp = UTC;
            }
            cDefault = zone = temp;
        }
        // }
    }
    
    return zone;
}

/**
 * Sets the default time zone.
 * <p>
 * NOTE: Calling this method does <i>not</i> set the {@code java.util.TimeZone} default.
 *
 * @param zone  the default datetime zone object, must not be NULL
 * @throws IllegalArgumentException if the zone is NULL
 * @throws SecurityException if the application has insufficient security rights
 */
//    void setDefault(DateTimeZone *zone) throws SecurityException {
//        SecurityManager sm = System.getSecurityManager();
//        if (sm != NULL) {
//            sm.checkPermission(new JodaTimePermission("DateTimeZone::setDefault"));
//        }
//        if (zone == NULL) {
//            throw new IllegalArgumentException("The datetime zone must not be NULL");
//        }
//        synchronized(DateTimeZone::class) {
//            cDefault = zone;
//        }
//    }

//-----------------------------------------------------------------------
/**
 * Gets a time zone instance for the specified time zone id.
 * <p>
 * The time zone id may be one of those returned by getAvailableIDs.
 * Short ids, as accepted by {@link java.util.TimeZone}, are not accepted.
 * All IDs must be specified in the int64_t format.
 * The exception is UTC, which is an acceptable id.
 * <p>
 * Alternatively a locale independent, fixed offset, datetime zone can
 * be specified. The form <code>[+-]hh:mm</code> can be used.
 *
 * @param id  the ID of the datetime zone, NULL means default
 * @return the DateTimeZone object for the ID
 * @throws IllegalArgumentException if the ID is not recognised
 */
DateTimeZone *DateTimeZone::forID(string id) {
    if (id.empty()) {
        return getDefault();
    }
    if (id.compare("UTC") == 0) {
        return DateTimeZone::UTC;
    }
    DateTimeZone *zone = cProvider->getZone(id);
    if (zone != NULL) {
        return zone;
    }
    if (id.at(0) == '+' || id.at(0) == '-') {
        int offset = parseOffset(id);
        if (offset == 0L) {
            return DateTimeZone::UTC;
        } else {
            id = printOffset(offset);
            return fixedOffsetZone(id, offset);
        }
    }
    
    string err("The datetime zone id '");
    err.append(id);
    err.append("' is not recognized");
    
    throw IllegalArgumentException(err);
}

/**
 * Gets a time zone instance for the specified offset to UTC in hours.
 * This method assumes standard length hours.
 * <p>
 * This factory is a convenient way of constructing zones with a fixed offset.
 *
 * @param hoursOffset  the offset in hours from UTC, from -23 to +23
 * @return the DateTimeZone object for the offset
 * @throws IllegalArgumentException if the offset is too large or too small
 */
DateTimeZone *DateTimeZone::forOffsetHours(int hoursOffset) {
    return forOffsetHoursMinutes(hoursOffset, 0);
}

/**
 * Gets a time zone instance for the specified offset to UTC in hours and minutes.
 * This method assumes 60 minutes in an hour, and standard length minutes.
 * <p>
 * This factory is a convenient way of constructing zones with a fixed offset.
 * The hours value must be in the range -23 to +23.
 * The minutes value must be in the range -59 to +59.
 * The following combinations of sign for the hour and minute are possible:
 * <pre>
 *  Hour    Minute    Example    Result
 *
 *  +ve     +ve       (2, 15)    +02:15
 *  +ve     zero      (2, 0)     +02:00
 *  +ve     -ve       (2, -15)   IllegalArgumentException
 *
 *  zero    +ve       (0, 15)    +00:15
 *  zero    zero      (0, 0)     +00:00
 *  zero    -ve       (0, -15)   -00:15
 *
 *  -ve     +ve       (-2, 15)   -02:15
 *  -ve     zero      (-2, 0)    -02:00
 *  -ve     -ve       (-2, -15)  -02:15
 * </pre>
 * Note that in versions before 2.3, the minutes had to be zero or positive.
 *
 * @param hoursOffset  the offset in hours from UTC, from -23 to +23
 * @param minutesOffset  the offset in minutes from UTC, from -59 to +59
 * @return the DateTimeZone object for the offset
 * @throws IllegalArgumentException if any value is out of range, the minutes are negative
 *  when the hours are positive, or the resulting offset exceeds +/- 23:59:59.000
 */
DateTimeZone *DateTimeZone::forOffsetHoursMinutes(int hoursOffset, int minutesOffset) {
    if (hoursOffset == 0 && minutesOffset == 0) {
        return DateTimeZone::UTC;
    }
    if (hoursOffset < -23 || hoursOffset > 23) {
        string err("Hours out of range: ");
        err.append(to_string(hoursOffset));
        throw IllegalArgumentException(err);
    }
    if (minutesOffset < -59 || minutesOffset > 59) {
        string err("Minutes out of range: ");
        err.append(to_string(minutesOffset));
        throw IllegalArgumentException(err);
    }
    if (hoursOffset > 0 && minutesOffset < 0) {
        string err("Positive hours must not have negative minutes: ");
        err.append(to_string(minutesOffset));
        throw IllegalArgumentException(err);
    }
    int offset = 0;
    try {
        int hoursInMinutes = hoursOffset * 60;
        if (hoursInMinutes < 0) {
            minutesOffset = hoursInMinutes - abs((double) minutesOffset);
        } else {
            minutesOffset = hoursInMinutes + minutesOffset;
        }
        offset = FieldUtils::safeMultiply(minutesOffset, DateTimeConstants::MILLIS_PER_MINUTE);
    } catch (ArithmeticException ex) {
        throw IllegalArgumentException("Offset is too large");
    }
    return forOffsetMillis(offset);
}

/**
 * Gets a time zone instance for the specified offset to UTC in milliseconds.
 *
 * @param millisOffset  the offset in millis from UTC, from -23:59:59.999 to +23:59:59.999
 * @return the DateTimeZone object for the offset
 */
DateTimeZone *DateTimeZone::forOffsetMillis(int millisOffset) {
    if (millisOffset < -MAX_MILLIS || millisOffset > MAX_MILLIS) {
        string err("Millis out of range: ");
        err.append(to_string(millisOffset));
        throw IllegalArgumentException(err);
    }
    string id = printOffset(millisOffset);
    return fixedOffsetZone(id, millisOffset);
}

/**
 * Gets a time zone instance for a JDK TimeZone.
 * <p>
 * DateTimeZone only accepts a subset of the IDs from TimeZone. The
 * excluded IDs are the short three letter form (except UTC). This
 * method will attempt to convert between time zones created using the
 * short IDs and the full version.
 * <p>
 * This method is not designed to parse time zones with rules created by
 * applications using <code>SimpleTimeZone</code> directly.
 *
 * @param zone  the zone to convert, NULL means default
 * @return the DateTimeZone object for the zone
 * @throws IllegalArgumentException if the zone is not recognised
 */
//    DateTimeZone *forTimeZone(TimeZone *zone) {
//        if (zone == NULL) {
//            return getDefault();
//        }
//        const string id = zone.getID();
//        if (id.compare("UTC") == 0) {
//            return DateTimeZone::UTC;
//        }
//
//        // Convert from old alias before consulting provider since they may differ.
//        DateTimeZone *dtz = NULL;
//        string convId = getConvertedId(id);
//        if (!convId.empty()) {
//            dtz = cProvider->getZone(convId);
//        }
//        if (dtz == NULL) {
//            dtz = cProvider->getZone(id);
//        }
//        if (dtz != NULL) {
//            return dtz;
//        }
//
//        // Support GMT+/-hh:mm formats
//        if (convId.empty()) {
//            convId = zone->getID();
//            if (convId.substr(0, 4).compare("GMT+") == 0 || convId.substr(0, 4).compare("GMT-") == 0) {
//                convId = convId.substr(3);
//                int offset = parseOffset(convId);
//                if (offset == 0L) {
//                    return DateTimeZone::UTC;
//                } else {
//                    convId = printOffset(offset);
//                    return fixedOffsetZone(convId, offset);
//                }
//            }
//        }
//        string err("The datetime zone id '");
//        err.append(id);
//        err.append("' is not recognised");
//        throw IllegalArgumentException(err);
//    }

/**
 * Gets all the available IDs supported.
 *
 * @return an unmodifiable Set of string IDs
 */
set<string> DateTimeZone::getAvailableIDs() {
    return cAvailableIDs;
}

//-----------------------------------------------------------------------
/**
 * Gets the zone provider factory.
 * <p>
 * The zone provider is a pluggable instance factory that supplies the
 * actual instances of DateTimeZone::
 *
 * @return the provider
 */
Provider *DateTimeZone::getProvider() {
    return cProvider;
}

/**
 * Sets the zone provider factory.
 * <p>
 * The zone provider is a pluggable instance factory that supplies the
 * actual instances of DateTimeZone::
 *
 * @param provider  provider to use, or NULL for default
 * @throws SecurityException if you do not have the permission DateTimeZone::setProvider
 * @throws IllegalArgumentException if the provider is invalid
 */
void DateTimeZone::setProvider(Provider *provider) {
    //        SecurityManager sm = System.getSecurityManager();
    //        if (sm != NULL) {
    //            sm.checkPermission(new JodaTimePermission("DateTimeZone::setProvider"));
    //        }
    setProvider0(provider);
}

//-----------------------------------------------------------------------
/**
 * Gets the name provider factory.
 * <p>
 * The name provider is a pluggable instance factory that supplies the
 * names of each DateTimeZone::
 *
 * @return the provider
 */
NameProvider *DateTimeZone::getNameProvider() {
    return cNameProvider;
}

/**
 * Sets the name provider factory.
 * <p>
 * The name provider is a pluggable instance factory that supplies the
 * names of each DateTimeZone::
 *
 * @param nameProvider  provider to use, or NULL for default
 * @throws SecurityException if you do not have the permission DateTimeZone::setNameProvider
 * @throws IllegalArgumentException if the provider is invalid
 */
void DateTimeZone::setNameProvider(NameProvider *nameProvider) {
    //        SecurityManager sm = System.getSecurityManager();
    //        if (sm != NULL) {
    //            sm.checkPermission(new JodaTimePermission("DateTimeZone::setNameProvider"));
    //        }
    setNameProvider0(nameProvider);
}

// Principal methods
//--------------------------------------------------------------------

/**
 * Gets the ID of this datetime zone.
 *
 * @return the ID of this datetime zone
 */
string DateTimeZone::getID() const {
    return iID;
}

/**
 * Gets the short name of this datetime zone suitable for display using
 * the default locale.
 * <p>
 * If the name is not available for the locale, then this method returns a
 * string in the format <code>[+-]hh:mm</code>.
 *
 * @param instant  milliseconds from 1970-01-01T00:00:00Z to get the name for
 * @return the human-readable short name in the default locale
 */
const string DateTimeZone::getShortName(int64_t instant) {
    return getShortName(instant, NULL);
}

/**
 * Gets the short name of this datetime zone suitable for display using
 * the specified locale.
 * <p>
 * If the name is not available for the locale, then this method returns a
 * string in the format <code>[+-]hh:mm</code>.
 *
 * @param instant  milliseconds from 1970-01-01T00:00:00Z to get the name for
 * @param locale  the locale to get the name for
 * @return the human-readable short name in the specified locale
 */
string DateTimeZone::getShortName(int64_t instant, Locale *locale) {
    if (locale == NULL) {
        locale = Locale::getDefault();
    }
    string nameKey = getNameKey(instant);
    if (nameKey.empty()) {
        return iID;
    }
    string name = cNameProvider->getShortName(locale, iID, nameKey);
    if (!name.empty()) {
        return name;
    }
    return printOffset(getOffset(instant));
}

/**
 * Gets the int64_t name of this datetime zone suitable for display using
 * the default locale.
 * <p>
 * If the name is not available for the locale, then this method returns a
 * string in the format <code>[+-]hh:mm</code>.
 *
 * @param instant  milliseconds from 1970-01-01T00:00:00Z to get the name for
 * @return the human-readable int64_t name in the default locale
 */
const string DateTimeZone::getName(int64_t instant) {
    return getName(instant, NULL);
}

/**
 * Gets the int64_t name of this datetime zone suitable for display using
 * the specified locale.
 * <p>
 * If the name is not available for the locale, then this method returns a
 * string in the format <code>[+-]hh:mm</code>.
 *
 * @param instant  milliseconds from 1970-01-01T00:00:00Z to get the name for
 * @param locale  the locale to get the name for
 * @return the human-readable int64_t name in the specified locale
 */
string DateTimeZone::getName(int64_t instant, Locale *locale) {
    if (locale == NULL) {
        locale = Locale::getDefault();
    }
    string nameKey = getNameKey(instant);
    if (nameKey.empty()) {
        return iID;
    }
    string name = cNameProvider->getName(locale, iID, nameKey);
    if (!name.empty()) {
        return name;
    }
    return printOffset(getOffset(instant));
}

/**
 * Gets the millisecond offset to add to UTC to get local time.
 *
 * @param instant  instant to get the offset for, NULL means now
 * @return the millisecond offset to add to UTC to get local time
 */
const int DateTimeZone::getOffset(ReadableInstant *instant) {
    if (instant == NULL) {
        return getOffset(DateTimeUtils::currentTimeMillis());
    }
    return getOffset(instant->getMillis());
}

/**
 * Checks whether, at a particular instant, the offset is standard or not.
 * <p>
 * This method can be used to determine whether Summer Time (DST) applies.
 * As a general rule, if the offset at the specified instant is standard,
 * then either Winter time applies, or there is no Summer Time. If the
 * instant is not standard, then Summer Time applies.
 * <p>
 * The implementation of the method is simply whether {@link #getOffset(int64_t)}
 * equals {@link #getStandardOffset(int64_t)} at the specified instant.
 *
 * @param instant  milliseconds from 1970-01-01T00:00:00Z to get the offset for
 * @return true if the offset at the given instant is the standard offset
 * @since 1.5
 */
bool DateTimeZone::isStandardOffset(int64_t instant) {
    return getOffset(instant) == getStandardOffset(instant);
}

/**
 * Gets the millisecond offset to subtract from local time to get UTC time.
 * This offset can be used to undo adding the offset obtained by getOffset.
 *
 * <pre>
 * millisLocal == millisUTC   + getOffset(millisUTC)
 * millisUTC   == millisLocal - getOffsetFromLocal(millisLocal)
 * </pre>
 *
 * NOTE: After calculating millisLocal, some error may be introduced. At
 * offset transitions (due to DST or other historical changes), ranges of
 * local times may map to different UTC times.
 * <p>
 * This method will return an offset suitable for calculating an instant
 * after any DST gap. For example, consider a zone with a cutover
 * from 01:00 to 01:59:<br />
 * Input: 00:00  Output: 00:00<br />
 * Input: 00:30  Output: 00:30<br />
 * Input: 01:00  Output: 02:00<br />
 * Input: 01:30  Output: 02:30<br />
 * Input: 02:00  Output: 02:00<br />
 * Input: 02:30  Output: 02:30<br />
 * <p>
 * During a DST overlap (where the local time is ambiguous) this method will return
 * the earlier instant. The combination of these two rules is to always favour
 * daylight (summer) time over standard (winter) time.
 * <p>
 * NOTE: Prior to v2.0, the DST overlap behaviour was not defined and varied by hemisphere.
 * Prior to v1.5, the DST gap behaviour was also not defined.
 *
 * @param instantLocal  the millisecond instant, relative to this time zone, to get the offset for
 * @return the millisecond offset to subtract from local time to get UTC time
 */
int DateTimeZone::getOffsetFromLocal(int64_t instantLocal) {
    // get the offset at instantLocal (first estimate)
    const int offsetLocal = getOffset(instantLocal);
    // adjust instantLocal using the estimate and recalc the offset
    const int64_t instantAdjusted = instantLocal - offsetLocal;
    const int offsetAdjusted = getOffset(instantAdjusted);
    // if the offsets differ, we must be near a DST boundary
    if (offsetLocal != offsetAdjusted) {
        // we need to ensure that time is always after the DST gap
        // this happens naturally for positive offsets, but not for negative
        if ((offsetLocal - offsetAdjusted) < 0) {
            // if we just return offsetAdjusted then the time is pushed
            // back before the transition, whereas it should be
            // on or after the transition
            int64_t nextLocal = nextTransition(instantAdjusted);
            int64_t nextAdjusted = nextTransition(instantLocal - offsetAdjusted);
            if (nextLocal != nextAdjusted) {
                return offsetLocal;
            }
        }
    } else if (offsetLocal >= 0) {
        int64_t prev = previousTransition(instantAdjusted);
        if (prev < instantAdjusted) {
            int offsetPrev = getOffset(prev);
            int diff = offsetPrev - offsetLocal;
            if (instantAdjusted - prev <= diff) {
                return offsetPrev;
            }
        }
    }
    return offsetAdjusted;
}

/**
 * Converts a standard UTC instant to a local instant with the same
 * local time. This conversion is used before performing a calculation
 * so that the calculation can be done using a simple local zone.
 *
 * @param instantUTC  the UTC instant to convert to local
 * @return the local instant with the same local time
 * @throws ArithmeticException if the result overflows a uint64_t
 * @since 1.5
 */
int64_t DateTimeZone::convertUTCToLocal(int64_t instantUTC) {
    int offset = getOffset(instantUTC);
    int64_t instantLocal = instantUTC + offset;
    // If there is a sign change, but the two values have the same sign...
    if ((instantUTC ^ instantLocal) < 0 && (instantUTC ^ offset) >= 0) {
        throw ArithmeticException("Adding time zone offset caused overflow");
    }
    return instantLocal;
}

/**
 * Converts a local instant to a standard UTC instant with the same
 * local time attempting to use the same offset as the original.
 * <p>
 * This conversion is used after performing a calculation
 * where the calculation was done using a simple local zone.
 * Whenever possible, the same offset as the original offset will be used.
 * This is most significant during a daylight savings overlap.
 *
 * @param instantLocal  the local instant to convert to UTC
 * @param strict  whether the conversion should reject non-existent local times
 * @param originalInstantUTC  the original instant that the calculation is based on
 * @return the UTC instant with the same local time,
 * @throws ArithmeticException if the result overflows a uint64_t
 * @throws IllegalArgumentException if the zone has no equivalent local time
 * @since 2.0
 */
int64_t DateTimeZone::convertLocalToUTC(int64_t instantLocal, bool strict, int64_t originalInstantUTC) {
    int offsetOriginal = getOffset(originalInstantUTC);
    int64_t instantUTC = instantLocal - offsetOriginal;
    int offsetLocalFromOriginal = getOffset(instantUTC);
    if (offsetLocalFromOriginal == offsetOriginal) {
        return instantUTC;
    }
    return convertLocalToUTC(instantLocal, strict);
}

/**
 * Converts a local instant to a standard UTC instant with the same
 * local time. This conversion is used after performing a calculation
 * where the calculation was done using a simple local zone.
 *
 * @param instantLocal  the local instant to convert to UTC
 * @param strict  whether the conversion should reject non-existent local times
 * @return the UTC instant with the same local time,
 * @throws ArithmeticException if the result overflows a uint64_t
 * @throws IllegalInstantException if the zone has no equivalent local time
 * @since 1.5
 */
int64_t DateTimeZone::convertLocalToUTC(int64_t instantLocal, bool strict) {
    // get the offset at instantLocal (first estimate)
    int offsetLocal = getOffset(instantLocal);
    // adjust instantLocal using the estimate and recalc the offset
    int offset = getOffset(instantLocal - offsetLocal);
    // if the offsets differ, we must be near a DST boundary
    if (offsetLocal != offset) {
        // if strict then always check if in DST gap
        // otherwise only check if zone in Western hemisphere (as the
        // value of offset is already correct for Eastern hemisphere)
        if (strict || offsetLocal < 0) {
            // determine if we are in the DST gap
            int64_t nextLocal = nextTransition(instantLocal - offsetLocal);
            if (nextLocal == (instantLocal - offsetLocal)) {
                nextLocal = LONG_MAX;
            }
            int64_t nextAdjusted = nextTransition(instantLocal - offset);
            if (nextAdjusted == (instantLocal - offset)) {
                nextAdjusted = LONG_MAX;
            }
            if (nextLocal != nextAdjusted) {
                // yes we are in the DST gap
                if (strict) {
                    // DST gap is not acceptable
                    throw IllegalInstantException(instantLocal, getID());
                } else {
                    // DST gap is acceptable, but for the Western hemisphere
                    // the offset is wrong and will result in local times
                    // before the cutover so use the offsetLocal instead
                    offset = offsetLocal;
                }
            }
        }
    }
    // check for overflow
    int64_t instantUTC = instantLocal - offset;
    // If there is a sign change, but the two values have different signs...
    if ((instantLocal ^ instantUTC) < 0 && (instantLocal ^ offset) < 0) {
        throw ArithmeticException("Subtracting time zone offset caused overflow");
    }
    return instantUTC;
}

/**
 * Gets the millisecond instant in another zone keeping the same local time.
 * <p>
 * The conversion is performed by converting the specified UTC millis to local
 * millis in this zone, then converting back to UTC millis in the new zone.
 *
 * @param newZone  the new zone, NULL means default
 * @param oldInstant  the UTC millisecond instant to convert
 * @return the UTC millisecond instant with the same local time in the new zone
 */
int64_t DateTimeZone::getMillisKeepLocal(DateTimeZone *newZone, int64_t oldInstant) {
    if (newZone == NULL) {
        newZone = DateTimeZone::getDefault();
    }
    if (newZone == this) {
        return oldInstant;
    }
    int64_t instantLocal = convertUTCToLocal(oldInstant);
    return newZone->convertLocalToUTC(instantLocal, false, oldInstant);
}

/**
 * Checks if the given {@link LocalDateTime} is within a gap.
 * <p>
 * When switching from standard time to Daylight Savings Time there is
 * typically a gap where a clock hour is missing. This method identifies
 * whether the local datetime refers to such a gap.
 *
 * @param localDateTime  the time to check, not NULL
 * @return true if the given datetime refers to a gap
 * @since 1.6
 */
bool DateTimeZone::isLocalDateTimeGap(LocalDateTime *localDateTime) {
    if (isFixed()) {
        return false;
    }
    try {
        localDateTime->toDateTime(this);
        return false;
    } catch (IllegalInstantException ex) {
        return true;
    }
}

/**
 * Adjusts the offset to be the earlier or later one during an overlap.
 *
 * @param instant  the instant to adjust
 * @param earlierOrLater  false for earlier, true for later
 * @return the adjusted instant millis
 */
int64_t DateTimeZone::adjustOffset(int64_t instant, bool earlierOrLater) {
    // a bit messy, but will work in all non-pathological cases
    
    // evaluate 3 hours before and after to work out if anything is happening
    int64_t instantBefore = instant - 3 * DateTimeConstants::MILLIS_PER_HOUR;
    int64_t instantAfter = instant + 3 * DateTimeConstants::MILLIS_PER_HOUR;
    int64_t offsetBefore = getOffset(instantBefore);
    int64_t offsetAfter = getOffset(instantAfter);
    if (offsetBefore <= offsetAfter) {
        return instant;  // not an overlap (less than is a gap, equal is normal case)
    }
    
    // work out range of instants that have duplicate local times
    int64_t diff = offsetBefore - offsetAfter;
    int64_t transition = nextTransition(instantBefore);
    int64_t overlapStart = transition - diff;
    int64_t overlapEnd = transition + diff;
    if (instant < overlapStart || instant >= overlapEnd) {
        return instant;  // not an overlap
    }
    
    // calculate result
    int64_t afterStart = instant - overlapStart;
    if (afterStart >= diff) {
        // currently in later offset
        return earlierOrLater ? instant : instant - diff;
    } else {
        // currently in earlier offset
        return earlierOrLater ? instant + diff : instant;
    }
}

/**
 * Gets a hash code compatable with equals.
 *
 * @return suitable hashcode
 */
int DateTimeZone::hashCode() {
    hash<string> hash;
    return 57 + (int) hash(getID());
}

/**
 * Gets the datetime zone as a string, which is simply its ID.
 * @return the id of the zone
 */
string DateTimeZone::toString() {
    return getID();
}

DateTimeZone::DateTimeZone(string id) {
    if (id.empty()) {
        throw IllegalArgumentException("Id must not be NULL");
    }
    iID = id;
}

CODATIME_END
