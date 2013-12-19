//
//  DateTimeZone::h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_DateTimeZone_h
#define CodaTime_DateTimeZone_h

#include "CodaTimeMacros.h"

#include "chrono/BaseChronology.h"
#include "Object.h"

#include <map>
#include <set>
#include <string>
#include <cmath>
#include <functional>

using namespace std;

CODATIME_BEGIN

class LocalDateTime;
class DateTimeFormatter;
class NameProvider;
class Provider;
class Locale;
class ReadableInstant;

/**
 * DateTimeZone represents a time zone.
 * <p>
 * A time zone is a system of rules to convert time from one geographic
 * location to another. For example, Paris, France is one hour ahead of
 * London, England. Thus when it is 10:00 in London, it is 11:00 in Paris.
 * <p>
 * All time zone rules are expressed, for historical reasons, relative to
 * Greenwich, London. Local time in Greenwich is referred to as Greenwich Mean
 * Time (GMT).  This is similar, but not precisely identical, to Universal
 * Coordinated Time, or UTC. This library only uses the term UTC.
 * <p>
 * Using this system, America/Los_Angeles is expressed as UTC-08:00, or UTC-07:00
 * in the summer. The offset -08:00 indicates that America/Los_Angeles time is
 * obtained from UTC by adding -08:00, that is, by subtracting 8 hours.
 * <p>
 * The offset differs in the summer because of daylight saving time, or DST.
 * The following definitions of time are generally used:
 * <ul>
 * <li>UTC - The reference time.
 * <li>Standard Time - The local time without a daylight saving time offset.
 * For example, in Paris, standard time is UTC+01:00.
 * <li>Daylight Saving Time - The local time with a daylight saving time
 * offset. This offset is typically one hour, but not always. It is typically
 * used in most countries away from the equator.  In Paris, daylight saving
 * time is UTC+02:00.
 * <li>Wall Time - This is what a local clock on the wall reads. This will be
 * either Standard Time or Daylight Saving Time depending on the time of year
 * and whether the location uses Daylight Saving Time.
 * </ul>
 * <p>
 * Unlike the Java TimeZone class, DateTimeZone is immutable. It also only
 * supports int64_t format time zone ids. Thus EST and ECT are not accepted.
 * However, the factory that accepts a TimeZone will attempt to convert from
 * the old short id to a suitable int64_t id.
 * <p>
 * DateTimeZone is thread-safe and immutable, and all subclasses must be as
 * well.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class DateTimeZone : public virtual Object {
    
private:
    
    /** Serialization version. */
    static const long long serialVersionUID = 5546345482340108586L;
    
    /** Maximum offset. */
    static const int MAX_MILLIS = (86400 * 1000) - 1;
    
    /** The instance that is providing time zones. */
    static Provider *cProvider;
    /** The instance that is providing time zone names. */
    static NameProvider *cNameProvider;
    /** The set of ID strings. */
    static set<string> cAvailableIDs;
    /** The default time zone. */
    static DateTimeZone *cDefault;
    /** A formatter for printing and parsing zones. */
    static DateTimeFormatter *cOffsetFormatter;
    
    /** Cache that maps fixed offset strings to softly referenced DateTimeZones */
    static map<string, DateTimeZone*> iFixedOffsetCache;
    
    /** Cache of old zone IDs to new zone IDs */
    static map<string, string> cZoneIdConversion;
    
    string iID;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the zone using a fixed offset amount.
     *
     * @param id  the zone id
     * @param offset  the offset in millis
     * @return the zone
     */
    // TODO: synchronized
    static DateTimeZone *fixedOffsetZone(string id, int offset);
    
    /**
     * Sets the zone provider factory without performing the security check.
     *
     * @param provider  provider to use, or NULL for default
     * @throws IllegalArgumentException if the provider is invalid
     */
    static void setProvider0(Provider *provider);
    
    /**
     * Gets the default zone provider.
     * <p>
     * Tries the system property <code>org.joda.time.DateTimeZone::Provider</code>.
     * Then tries a <code>ZoneInfoProvider</code> using the data in <code>org/joda/time/tz/data</code>.
     * Then uses <code>UTCProvider</code>.
     *
     * @return the default name provider
     */
    static Provider *getDefaultProvider();
    
    /**
     * Sets the name provider factory without performing the security check.
     *
     * @param nameProvider  provider to use, or NULL for default
     * @throws IllegalArgumentException if the provider is invalid
     */
    static void setNameProvider0(NameProvider *nameProvider);
    
    /**
     * Gets the default name provider.
     * <p>
     * Tries the system property <code>org.joda.time.DateTimeZone::NameProvider</code>.
     * Then uses <code>DefaultNameProvider</code>.
     *
     * @return the default name provider
     */
    static NameProvider *getDefaultNameProvider();
    
    //-----------------------------------------------------------------------
    /**
     * Converts an old style id to a new style id.
     *
     * @param id  the old style id
     * @return the new style id, NULL if not found
     */
    // TODO: synchronized
    static string getConvertedId(string id);
    
    class UnrealChrono : public BaseChronology {
        
    private:
        
        static const long long serialVersionUID = -3128740902654445468L;
        
    public:
        
        DateTimeZone *getZone() {
            return NULL;
        }
        Chronology *withUTC() {
            return this;
        }
        Chronology *withZone(DateTimeZone *zone) {
            return this;
        }
        string toString() {
            return string("UnrealChrono");
        }
    };
    
    static int parseOffset(string str);
    
    /**
     * Formats a timezone offset string.
     * <p>
     * This method is kept separate from the formatting classes to speed and
     * simplify startup and classloading.
     *
     * @param offset  the offset in milliseconds
     * @return the time zone string
     */
    static string printOffset(int offset);
    
    /**
     * Gets a printer/parser for managing the offset id formatting.
     *
     * @return the formatter
     */
    // TODO: synchronized
    static DateTimeFormatter *offsetFormatter();
    
public:
    
    /** The time zone for Universal Coordinated Time */
    static DateTimeZone *UTC;
    
    //-----------------------------------------------------------------------
    /**
     * Gets the default time zone.
     * <p>
     * The default time zone is derived from the system property {@code user.timezone}.
     * If that is {@code NULL} or is not a valid identifier, then the value of the
     * JDK {@code TimeZone} default is converted. If that fails, {@code UTC} is used.
     * <p>
     * NOTE: If the {@code java.util.TimeZone} default is updated <i>after</i> calling this
     * method, then the change will not be picked up here.
     *
     * @return the default datetime zone object
     */
    static DateTimeZone *getDefault();
    
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
    static DateTimeZone *forID(string id);
    
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
    static DateTimeZone *forOffsetHours(int hoursOffset);
    
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
    static DateTimeZone *forOffsetHoursMinutes(int hoursOffset, int minutesOffset);
    
    /**
     * Gets a time zone instance for the specified offset to UTC in milliseconds.
     *
     * @param millisOffset  the offset in millis from UTC, from -23:59:59.999 to +23:59:59.999
     * @return the DateTimeZone object for the offset
     */
    static DateTimeZone *forOffsetMillis(int millisOffset);
    
    /**
     * Gets all the available IDs supported.
     *
     * @return an unmodifiable Set of string IDs
     */
    static set<string> getAvailableIDs();
    
    //-----------------------------------------------------------------------
    /**
     * Gets the zone provider factory.
     * <p>
     * The zone provider is a pluggable instance factory that supplies the
     * actual instances of DateTimeZone::
     *
     * @return the provider
     */
    static Provider *getProvider();
    
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
    static void setProvider(Provider *provider);
    
    //-----------------------------------------------------------------------
    /**
     * Gets the name provider factory.
     * <p>
     * The name provider is a pluggable instance factory that supplies the
     * names of each DateTimeZone::
     *
     * @return the provider
     */
    static NameProvider *getNameProvider();
    
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
    static void setNameProvider(NameProvider *nameProvider);
    
    // Principal methods
    //--------------------------------------------------------------------
    
    /**
     * Gets the ID of this datetime zone.
     *
     * @return the ID of this datetime zone
     */
    string getID() const;
    
    /**
     * Returns a non-localized name that is unique to this time zone. It can be
     * combined with id to form a unique key for fetching localized names.
     *
     * @param instant  milliseconds from 1970-01-01T00:00:00Z to get the name for
     * @return name key or NULL if id should be used for names
     */
    virtual string getNameKey(int64_t instant) = 0;
    
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
    const string getShortName(int64_t instant);
    
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
    string getShortName(int64_t instant, Locale *locale);
    
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
    const string getName(int64_t instant);
    
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
    string getName(int64_t instant, Locale *locale);
    
    /**
     * Gets the millisecond offset to add to UTC to get local time.
     *
     * @param instant  milliseconds from 1970-01-01T00:00:00Z to get the offset for
     * @return the millisecond offset to add to UTC to get local time
     */
    virtual int getOffset(int64_t instant) = 0;
    
    /**
     * Gets the millisecond offset to add to UTC to get local time.
     *
     * @param instant  instant to get the offset for, NULL means now
     * @return the millisecond offset to add to UTC to get local time
     */
    const int getOffset(ReadableInstant *instant);
    
    /**
     * Gets the standard millisecond offset to add to UTC to get local time,
     * when standard time is in effect.
     *
     * @param instant  milliseconds from 1970-01-01T00:00:00Z to get the offset for
     * @return the millisecond offset to add to UTC to get local time
     */
    virtual int getStandardOffset(int64_t instant) = 0;
    
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
    bool isStandardOffset(int64_t instant);
    
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
    int getOffsetFromLocal(int64_t instantLocal);
    
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
    int64_t convertUTCToLocal(int64_t instantUTC);
    
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
    int64_t convertLocalToUTC(int64_t instantLocal, bool strict, int64_t originalInstantUTC);
    
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
    int64_t convertLocalToUTC(int64_t instantLocal, bool strict);
    
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
    int64_t getMillisKeepLocal(DateTimeZone *newZone, int64_t oldInstant);
    
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
    bool isLocalDateTimeGap(LocalDateTime *localDateTime);
    
    /**
     * Adjusts the offset to be the earlier or later one during an overlap.
     *
     * @param instant  the instant to adjust
     * @param earlierOrLater  false for earlier, true for later
     * @return the adjusted instant millis
     */
    int64_t adjustOffset(int64_t instant, bool earlierOrLater);
    
    //-----------------------------------------------------------------------
    /**
     * Returns true if this time zone has no transitions.
     *
     * @return true if no transitions
     */
    virtual bool isFixed() = 0;
    
    /**
     * Advances the given instant to where the time zone offset or name changes.
     * If the instant returned is exactly the same as passed in, then
     * no changes occur after the given instant.
     *
     * @param instant  milliseconds from 1970-01-01T00:00:00Z
     * @return milliseconds from 1970-01-01T00:00:00Z
     */
    virtual int64_t nextTransition(int64_t instant) = 0;
    
    /**
     * Retreats the given instant to where the time zone offset or name changes.
     * If the instant returned is exactly the same as passed in, then
     * no changes occur before the given instant.
     *
     * @param instant  milliseconds from 1970-01-01T00:00:00Z
     * @return milliseconds from 1970-01-01T00:00:00Z
     */
    virtual int64_t previousTransition(int64_t instant) = 0;
    
    // Basic methods
    //--------------------------------------------------------------------
    
    /**
     * Get the datetime zone as a {@link java.util.TimeZone}.
     *
     * @return the closest matching TimeZone object
     */
//    java.util.TimeZone toTimeZone() {
//        return java.util.TimeZone.getTimeZone(iID);
//    }
    
    /**
     * Compare this datetime zone with another.
     *
     * @param object the object to compare with
     * @return true if equal, based on the ID and all internal rules
     */
    virtual bool equals(const Object *object) const = 0;
    
    /**
     * Gets a hash code compatable with equals.
     *
     * @return suitable hashcode
     */
    int hashCode();
    
    /**
     * Gets the datetime zone as a string, which is simply its ID.
     * @return the id of the zone
     */
    string toString();
    
protected:
    
    /**
     * Constructor.
     *
     * @param id  the id to use
     * @throws IllegalArgumentException if the id is NULL
     */
    DateTimeZone(string id);
    
    /**
     * By default, when DateTimeZones are serialized, only a "stub" object
     * referring to the id is written out. When the stub is read in, it
     * replaces itself with a DateTimeZone object.
     * @return a stub object to go in the stream
     */
//    protected Object writeReplace() throws ObjectStreamException {
//        return new Stub(iID);
//    }
    
};

CODATIME_END

#endif
