//
//  DateTimePrinter.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_DateTimePrinter_h
#define CodaTime_DateTimePrinter_h

#include "CodaTimeMacros.h"

#include <sstream>

using namespace std;

CODATIME_BEGIN

class ReadablePartial;
class Locale;
class Chronology;
class DateTimeZone;

/**
 * Internal interface for creating textual representations of datetimes.
 * <p>
 * Application users will rarely use this class directly. Instead, you
 * will use one of the factory classes to create a {@link DateTimeFormatter}.
 * <p>
 * The factory classes are:<br />
 * - {@link DateTimeFormatterBuilder}<br />
 * - {@link DateTimeFormat}<br />
 * - {@link ISODateTimeFormat}<br />
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @see DateTimeFormatterBuilder
 * @see DateTimeFormat
 * @see ISODateTimeFormat
 * @since 1.0
 */
class DateTimePrinter {
    
public:
    
    /**
     * Returns the expected maximum number of characters produced.
     * The actual amount should rarely exceed this estimate.
     *
     * @return the estimated length
     */
    virtual int estimatePrintedLength() = 0;
    
    /**
     * Prints an instant from milliseconds since 1970-01-01T00:00:00Z,
     * using the given Chronology.
     *
     * @param buf  formatted instant is appended to this buffer, not null
     * @param instant  millis since 1970-01-01T00:00:00Z
     * @param chrono  the chronology to use, not null
     * @param displayOffset  if a time zone offset is printed, force it to use
     * this millisecond value
     * @param displayZone  the time zone to use, null means local time
     * @param locale  the locale to use, null means default locale
     */
    virtual void printTo(string &buf, int64_t instant, Chronology *chrono,
                 int displayOffset, DateTimeZone *displayZone, Locale *locale) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Prints an instant from milliseconds since 1970-01-01T00:00:00Z,
     * using the given Chronology.
     *
     * @param buf  formatted instant is appended to this buffer, not null
     * @param instant  millis since 1970-01-01T00:00:00Z
     * @param chrono  the chronology to use, not null
     * @param displayOffset  if a time zone offset is printed, force it to use
     * this millisecond value
     * @param displayZone  the time zone to use, null means local time
     * @param locale  the locale to use, null means default locale
     */
    virtual void printTo(stringstream &buf, int64_t instant, Chronology *chrono,
                 int displayOffset, DateTimeZone *displayZone, Locale *locale) = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Prints a ReadablePartial.
     *
     * @param buf  formatted partial is appended to this buffer, not null
     * @param partial  partial to format, not null
     * @param locale  the locale to use, null means default locale
     */
    virtual void printTo(string &buf, ReadablePartial *partial, Locale *locale) = 0;
    
    /**
     * Prints a ReadablePartial.
     *
     * @param out  formatted partial is written out, not null
     * @param partial  partial to format, not null
     * @param locale  the locale to use, null means default locale
     */
    void printTo(stringstream &out, ReadablePartial *partial, Locale *locale);
    
};

CODATIME_END

#endif
