//
//  DateTimeFormatter.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__DateTimeFormatter__
#define __CodaTime__DateTimeFormatter__

#include "CodaTimeMacros.h"

#include <sstream>

using namespace std;

CODATIME_BEGIN

class DateTimePrinter;
class DateTimeParser;
class DateTimeZone;
class Chronology;
class Locale;
class LocalTime;
class LocalDate;
class LocalDateTime;
class DateTime;
class MutableDateTime;
class ReadableInstant;
class ReadablePartial;
class ReadWritableInstant;

/**
 * Controls the printing and parsing of a datetime to and from a string.
 * <p>
 * This class is the main API for printing and parsing used by most applications.
 * Instances of this class are created via one of three factory classes:
 * <ul>
 * <li>{@link DateTimeFormat} - formats by pattern and style</li>
 * <li>{@link ISODateTimeFormat} - ISO8601 formats</li>
 * <li>{@link DateTimeFormatterBuilder} - complex formats created via method calls</li>
 * </ul>
 * <p>
 * An instance of this class holds a reference internally to one printer and
 * one parser. It is possible that one of these may be null, in which case the
 * formatter cannot print/parse. This can be checked via the {@link #isPrinter()}
 * and {@link #isParser()} methods.
 * <p>
 * The underlying printer/parser can be altered to behave exactly as required
 * by using one of the decorator modifiers:
 * <ul>
 * <li>{@link #withLocale(Locale)} - returns a new formatter that uses the specified locale</li>
 * <li>{@link #withZone(DateTimeZone)} - returns a new formatter that uses the specified time zone</li>
 * <li>{@link #withChronology(Chronology)} - returns a new formatter that uses the specified chronology</li>
 * <li>{@link #withOffsetParsed()} - returns a new formatter that returns the parsed time zone offset</li>
 * <li>{@link #withPivotYear(int)} - returns a new formatter with the specified pivot year</li>
 * <li>{@link #withDefaultYear(int)} - returns a new formatter with the specified default year</li>
 * </ul>
 * Each of these returns a new formatter (instances of this class are immutable).
 * <p>
 * The main methods of the class are the <code>printXxx</code> and
 * <code>parseXxx</code> methods. These are used as follows:
 * <pre>
 * // print using the defaults (default locale, chronology/zone of the datetime)
 * String dateStr = formatter.print(dt);
 * // print using the French locale
 * String dateStr = formatter.withLocale(Locale.FRENCH).print(dt);
 * // print using the UTC zone
 * String dateStr = formatter.withZone(DateTimeZone.UTC).print(dt);
 *
 * // parse using the Paris zone
 * DateTime date = formatter.withZone(DateTimeZone.forID("Europe/Paris")).parseDateTime(str);
 * </pre>
 * <p>
 * Parsing builds up the resultant instant by 'setting' the value of each parsed field
 * from largest to smallest onto an initial instant, typically 1970-01-01T00:00Z.
 * This design means that day-of-month is set before day-of-week.
 * As such, if both the day-of-month and day-of-week are parsed, and the day-of-week
 * is incorrect, then the day-of-week overrides the day-of-month.
 *
 * This has a side effect if the input is not consistent.
 *
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @author Fredrik Borgh
 * @since 1.0
 */
class DateTimeFormatter {
    
private:
    
    /** The internal printer used to output the datetime. */
    DateTimePrinter *iPrinter;
    /** The internal parser used to output the datetime. */
    DateTimeParser *iParser;
    /** The locale to use for printing and parsing. */
    Locale *iLocale;
    /** Whether the offset is parsed. */
    bool iOffsetParsed;
    /** The chronology to use as an override. */
    Chronology *iChrono;
    /** The zone to use as an override. */
    DateTimeZone *iZone;
    /** The pivot year to use for two-digit year parsing. */
    int iPivotYear;
    /** The default year for parsing month/day without year. */
    int iDefaultYear;
    
    DateTimeFormatter(DateTimePrinter *printer, DateTimeParser *parser,
                      Locale *locale, bool offsetParsed,
                      Chronology *chrono, DateTimeZone *zone,
                      int pivotYear, int defaultYear);
    
    void printTo(string &buf, int64_t instant, Chronology *chrono);
    void printTo(stringstream &buf, int64_t instant, Chronology *chrono);
    DateTimePrinter *requirePrinter();
    DateTimeParser *requireParser();
    Chronology *selectChronology(Chronology *chrono);
    
public:
    
    DateTimeFormatter(DateTimePrinter *printer, DateTimeParser *parser);
    
    bool isPrinter();
    DateTimePrinter *getPrinter();
    bool isParser();
    DateTimeParser *getParser();
    DateTimeFormatter *withLocale(Locale *locale);
    Locale *getLocale();
    DateTimeFormatter *withOffsetParsed();
    bool isOffsetParsed();
    DateTimeFormatter *withChronology(Chronology *chrono);
    Chronology *getChronology() const;
    DateTimeFormatter *withZoneUTC();
    DateTimeFormatter *withZone(DateTimeZone *zone);
    DateTimeZone *getZone();
    DateTimeFormatter *withPivotYear(int pivotYear);
    int getPivotYear();
    DateTimeFormatter *withDefaultYear(int defaultYear);
    int getDefaultYear();
    
    void printTo(string &buf, ReadableInstant *instant);
    void printTo(stringstream &out, ReadableInstant *instant);
    void printTo(string &buf, int64_t instant);
    void printTo(stringstream &out, int64_t instant);
    void printTo(string &buf, ReadablePartial *partial);
    void printTo(stringstream &out, ReadablePartial *partial);
    
    string print(ReadableInstant *instant);
    string print(int64_t instant);
    string print(ReadablePartial *partial);
    
    int parseInto(ReadWritableInstant *instant, string text, int position);
    int64_t parseMillis(string text);
    LocalDate *parseLocalDate(string text);
    LocalTime *parseLocalTime(string text);
    LocalDateTime *parseLocalDateTime(string text);
    DateTime *parseDateTime(string text);
    MutableDateTime *parseMutableDateTime(string text);
};

CODATIME_END

#endif /* defined(__CodaTime__DateTimeFormatter__) */
