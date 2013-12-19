//
//  DateTimeUtils.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__DateTimeUtils__
#define __CodaTime__DateTimeUtils__

#include "CodaTimeMacros.h"

#include "Chronology.h"
#include "ReadableInstant.h"
#include "ReadableInterval.h"
#include "ReadableDuration.h"

#include <map>
#include <string>
#include <chrono>

using namespace std;

CODATIME_BEGIN

//-----------------------------------------------------------------------
/**
 * A millisecond provider, allowing control of the system clock.
 *
 * @author Stephen Colebourne
 * @since 2.0 (previously private)
 */
class MillisProvider {
    
public:
    /**
     * Gets the current time.
     * <p>
     * Implementations of this method must be thread-safe.
     *
     * @return the current time in milliseconds
     */
    virtual int64_t getMillis() const = 0;
};

/**
 * System millis provider.
 */
class SystemMillisProvider : public MillisProvider {
    
public:
    
    /**
     * Gets the current time.
     * @return the current time in millis
     */
    virtual int64_t getMillis() const {
        return chrono::system_clock::now().time_since_epoch() / chrono::milliseconds(1);
    }
};

/**
 * Fixed millisecond provider.
 */
class FixedMillisProvider : public MillisProvider {
    
private:
    
    /** The fixed millis value. */
    int64_t iMillis;
    
public:
    
    /**
     * Constructor.
     * @param offsetMillis  the millis offset
     */
    FixedMillisProvider(int64_t fixedMillis) {
        iMillis = fixedMillis;
    }
    
    /**
     * Gets the current time.
     * @return the current time in millis
     */
    virtual int64_t getMillis() const {
        return iMillis;
    }
};

/**
 * Offset from system millis provider.
 */
class OffsetMillisProvider : public MillisProvider {
    
private:
    
    /** The millis offset. */
    int64_t iMillis;
    
public:
    
    /**
     * Constructor.
     * @param offsetMillis  the millis offset
     */
    OffsetMillisProvider(int64_t offsetMillis) {
        iMillis = offsetMillis;
    }
    
    /**
     * Gets the current time.
     * @return the current time in millis
     */
    virtual int64_t getMillis() const {
        return chrono::system_clock::now().time_since_epoch() / chrono::milliseconds(1) + iMillis;
    }
};

class DateTimeUtils {
    
public:
    
    static const int64_t currentTimeMillis();
    static const int64_t getInstantMillis(ReadableInstant *instant);
    static Chronology *getInstantChronology(ReadableInstant *instant);
    static Chronology *getIntervalChronology(ReadableInstant *start, ReadableInstant *end);
    static Chronology *getIntervalChronology(ReadableInterval *interval);
    
    static ReadableInterval *getReadableInterval(ReadableInterval *interval);
    static Chronology *getChronology(Chronology *chrono);
    static DateTimeZone *getZone(DateTimeZone *zone);
    static const PeriodType *getPeriodType(const PeriodType *type);
    static const int64_t getDurationMillis(ReadableDuration *duration);
    static const bool isContiguous(ReadablePartial *partial);
    
    static const map<string, DateTimeZone*> getDefaultTimeZoneNames();
    static const void setDefaultTimeZoneNames(map<string, DateTimeZone*> names);
    
    static const double toJulianDay(int64_t epochMillis);
    static const int64_t toJulianDayNumber(int64_t epochMillis);
    static const int64_t fromJulianDay(double julianDay);
    
protected:
    
    DateTimeUtils() {
    }
    
private:
    
    /** The singleton instance of the system millisecond provider. */
    static SystemMillisProvider *SYSTEM_MILLIS_PROVIDER;
    /** The millisecond provider currently in use. */
    static MillisProvider *cMillisProvider;
    /** The millisecond provider currently in use. */
    static map<string, DateTimeZone*> cZoneNames;
    
    struct StaticBlock {
        StaticBlock() {
            
        }
    };
    
    static StaticBlock staticBlock;
    
};

CODATIME_END

#endif /* defined(__CodaTime__DateTimeUtils__) */
