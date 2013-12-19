//
//  DateTimeFormatter.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "DateTimeFormatter.h"

#include "Chronology.h"
#include "ReadableInstant.h"
#include "ReadWritableInstant.h"
#include "format/DateTimeParser.h"
#include "format/DateTimePrinter.h"
#include "Exceptions.h"
#include "LocalDateTime.h"
#include "LocalTime.h"
#include "DateTime.h"
#include "DateTimeUtils.h"
#include "format/FormatUtils.h"
#include "format/DateTimeParserBucket.h"

CODATIME_BEGIN

/**
 * Creates a new formatter, however you will normally use the factory
 * or the builder.
 *
 * @param printer  the internal printer, NULL if cannot print
 * @param parser  the internal parser, NULL if cannot parse
 */
DateTimeFormatter::DateTimeFormatter(DateTimePrinter *printer, DateTimeParser *parser) {
    iPrinter = printer;
    iParser = parser;
    iLocale = NULL;
    iOffsetParsed = false;
    iChrono = NULL;
    iZone = NULL;
    iPivotYear = 0;
    iDefaultYear = 2000;
}

/**
 * Constructor.
 */
DateTimeFormatter::DateTimeFormatter(DateTimePrinter *printer, DateTimeParser *parser,
                          Locale *locale, bool offsetParsed,
                          Chronology *chrono, DateTimeZone *zone,
                          int pivotYear, int defaultYear) {
    iPrinter = printer;
    iParser = parser;
    iLocale = locale;
    iOffsetParsed = offsetParsed;
    iChrono = chrono;
    iZone = zone;
    iPivotYear = pivotYear;
    iDefaultYear = defaultYear;
}

//-----------------------------------------------------------------------
/**
 * Is this formatter capable of printing.
 *
 * @return true if this is a printer
 */
bool DateTimeFormatter::isPrinter() {
    return (iPrinter != NULL);
}

/**
 * Gets the internal printer object that performs the real printing work.
 *
 * @return the internal printer; is NULL if printing not supported
 */
DateTimePrinter *DateTimeFormatter::getPrinter() {
    return iPrinter;
}

/**
 * Is this formatter capable of parsing.
 *
 * @return true if this is a parser
 */
bool DateTimeFormatter::isParser() {
    return (iParser != NULL);
}

/**
 * Gets the internal parser object that performs the real parsing work.
 *
 * @return the internal parser; is NULL if parsing not supported
 */
DateTimeParser *DateTimeFormatter::getParser() {
    return iParser;
}

//-----------------------------------------------------------------------
/**
 * Returns a new formatter with a different locale that will be used
 * for printing and parsing.
 * <p>
 * A DateTimeFormatter *is immutable, so a new instance is returned,
 * and the original is unaltered and still usable.
 *
 * @param locale the locale to use; if NULL, formatter uses default locale
 * at invocation time
 * @return the new formatter
 */
DateTimeFormatter *DateTimeFormatter::withLocale(Locale *locale) {
    if (locale == getLocale() || (locale != NULL && locale->equals(getLocale()))) {
        return this;
    }
    return new DateTimeFormatter(iPrinter, iParser, locale,
                                 iOffsetParsed, iChrono, iZone, iPivotYear, iDefaultYear);
}

/**
 * Gets the locale that will be used for printing and parsing.
 *
 * @return the locale to use; if NULL, formatter uses default locale at
 * invocation time
 */
Locale *DateTimeFormatter::getLocale() {
    return iLocale;
}

//-----------------------------------------------------------------------
/**
 * Returns a new formatter that will create a datetime with a time zone
 * equal to that of the offset of the parsed string.
 * <p>
 * After calling this method, a string '2004-06-09T10:20:30-08:00' will
 * create a datetime with a zone of -08:00 (a fixed zone, with no daylight
 * savings rules). If the parsed string represents a local time (no zone
 * offset) the parsed datetime will be in the default zone.
 * <p>
 * Calling this method sets the override zone to NULL.
 * Calling the override zone method sets this flag off.
 *
 * @return the new formatter
 */
DateTimeFormatter *DateTimeFormatter::withOffsetParsed() {
    if (iOffsetParsed == true) {
        return this;
    }
    return new DateTimeFormatter(iPrinter, iParser, iLocale,
                                 true, iChrono, NULL, iPivotYear, iDefaultYear);
}

/**
 * Checks whether the offset from the string is used as the zone of
 * the parsed datetime.
 *
 * @return true if the offset from the string is used as the zone
 */
bool DateTimeFormatter::isOffsetParsed() {
    return iOffsetParsed;
}

//-----------------------------------------------------------------------
/**
 * Returns a new formatter that will use the specified Chronology *in
 * preference to that of the printed object, or ISO on a parse.
 * <p>
 * When printing, this chronolgy will be used in preference to the chronology
 * from the datetime that would otherwise be used.
 * <p>
 * When parsing, this Chronology *will be set on the parsed datetime.
 * <p>
 * A NULL Chronology *means no-override.
 * If both an override Chronology *and an override zone are set, the
 * override zone will take precedence over the zone in the chronology.
 *
 * @param chrono  the Chronology *to use as an override
 * @return the new formatter
 */
DateTimeFormatter *DateTimeFormatter::withChronology(Chronology *chrono) {
    if (iChrono == chrono) {
        return this;
    }
    return new DateTimeFormatter(iPrinter, iParser, iLocale,
                                 iOffsetParsed, chrono, iZone, iPivotYear, iDefaultYear);
}

/**
 * Gets the Chronology *to use as an override.
 *
 * @return the Chronology *to use as an override
 */
Chronology *DateTimeFormatter::getChronology() const {
    return iChrono;
}

//-----------------------------------------------------------------------
/**
 * Returns a new formatter that will use the UTC zone in preference
 * to the zone of the printed object, or default zone on a parse.
 * <p>
 * When printing, UTC will be used in preference to the zone
 * from the datetime that would otherwise be used.
 * <p>
 * When parsing, UTC will be set on the parsed datetime.
 * <p>
 * If both an override Chronology *and an override zone are set, the
 * override zone will take precedence over the zone in the chronology.
 *
 * @return the new formatter, never NULL
 * @since 2.0
 */
DateTimeFormatter *DateTimeFormatter::withZoneUTC() {
    return withZone(DateTimeZone::UTC);
}

/**
 * Returns a new formatter that will use the specified zone in preference
 * to the zone of the printed object, or default zone on a parse.
 * <p>
 * When printing, this zone will be used in preference to the zone
 * from the datetime that would otherwise be used.
 * <p>
 * When parsing, this zone will be set on the parsed datetime.
 * <p>
 * A NULL zone means of no-override.
 * If both an override Chronology *and an override zone are set, the
 * override zone will take precedence over the zone in the chronology.
 *
 * @param zone  the zone to use as an override
 * @return the new formatter
 */
DateTimeFormatter *DateTimeFormatter::withZone(DateTimeZone *zone) {
    if (iZone == zone) {
        return this;
    }
    return new DateTimeFormatter(iPrinter, iParser, iLocale,
                                 false, iChrono, zone, iPivotYear, iDefaultYear);
}

/**
 * Gets the zone to use as an override.
 *
 * @return the zone to use as an override
 */
DateTimeZone *DateTimeFormatter::getZone() {
    return iZone;
}

//-----------------------------------------------------------------------
/**
 * Returns a new formatter that will use the specified pivot year for two
 * digit year parsing in preference to that stored in the parser.
 * <p>
 * This setting is useful for changing the pivot year of formats built
 * using a pattern - {@link DateTimeFormat#forPattern(String)}.
 * <p>
 * When parsing, this pivot year is used. NULL means no-override.
 * There is no effect when printing.
 * <p>
 * The pivot year enables a two digit year to be converted to a four
 * digit year. The pivot represents the year in the middle of the
 * supported range of years. Thus the full range of years that will
 * be built is <code>(pivot - 50) .. (pivot + 49)</code>.
 *
 * <pre>
 * pivot   supported range   00 is   20 is   40 is   60 is   80 is
 * ---------------------------------------------------------------
 * 1950      1900..1999      1900    1920    1940    1960    1980
 * 1975      1925..2024      2000    2020    1940    1960    1980
 * 2000      1950..2049      2000    2020    2040    1960    1980
 * 2025      1975..2074      2000    2020    2040    2060    1980
 * 2050      2000..2099      2000    2020    2040    2060    2080
 * </pre>
 *
 * @param pivotYear  the pivot year to use as an override when parsing
 * @return the new formatter
 * @since 1.1
 */
DateTimeFormatter *DateTimeFormatter::withPivotYear(int pivotYear) {
    if (iPivotYear == pivotYear || (iPivotYear != 0 && iPivotYear == pivotYear)) {
        return this;
    }
    return new DateTimeFormatter(iPrinter, iParser, iLocale,
                                 iOffsetParsed, iChrono, iZone, pivotYear, iDefaultYear);
}

/**
 * Gets the pivot year to use as an override.
 *
 * @return the pivot year to use as an override
 * @since 1.1
 */
int DateTimeFormatter::getPivotYear() {
    return iPivotYear;
}

//-----------------------------------------------------------------------
/**
 * Returns a new formatter that will use the specified default year.
 * <p>
 * The default year is used when parsing in the case where there is a
 * month or a day but not a year. Specifically, it is used if there is
 * a field parsed with a duration between the length of a month and the
 * length of a day inclusive.
 * <p>
 * This value is typically used to move the year from 1970 to a leap year
 * to enable February 29th to be parsed.
 * Unless customised, the year 2000 is used.
 * <p>
 * This setting has no effect when printing.
 *
 * @param defaultYear  the default year to use
 * @return the new formatter, not NULL
 * @since 2.0
 */
DateTimeFormatter *DateTimeFormatter::withDefaultYear(int defaultYear) {
    return new DateTimeFormatter(iPrinter, iParser, iLocale,
                                 iOffsetParsed, iChrono, iZone, iPivotYear, defaultYear);
}

/**
 * Gets the default year for parsing months and days.
 *
 * @return the default year for parsing months and days
 * @since 2.0
 */
int DateTimeFormatter::getDefaultYear() {
    return iDefaultYear;
}

//-----------------------------------------------------------------------
/**
 * Prints a ReadableInstant, using the Chronology *supplied by the instant.
 *
 * @param buf  the destination to format to, not NULL
 * @param instant  instant to format, NULL means now
 */
void DateTimeFormatter::printTo(string &buf, ReadableInstant *instant) {
    int64_t millis = DateTimeUtils::getInstantMillis(instant);
    Chronology *chrono = DateTimeUtils::getInstantChronology(instant);
    printTo(buf, millis, chrono);
}

/**
 * Prints a ReadableInstant, using the Chronology *supplied by the instant.
 *
 * @param out  the destination to format to, not NULL
 * @param instant  instant to format, NULL means now
 */
void DateTimeFormatter::printTo(stringstream &out, ReadableInstant *instant) {
    int64_t millis = DateTimeUtils::getInstantMillis(instant);
    Chronology *chrono = DateTimeUtils::getInstantChronology(instant);
    printTo(out, millis, chrono);
}

/**
 * Prints a ReadableInstant, using the Chronology *supplied by the instant.
 *
 * @param appendable  the destination to format to, not NULL
 * @param instant  instant to format, NULL means now
 * @since 2.0
 */
//void printTo(Appendable appendable, ReadableInstant instant) throws IOException {
//    appendable.append(print(instant));
//}

//-----------------------------------------------------------------------
/**
 * Prints an instant from milliseconds since 1970-01-01T00:00:00Z,
 * using ISO Chronology *in the default DateTimeZone.
 *
 * @param buf  the destination to format to, not NULL
 * @param instant  millis since 1970-01-01T00:00:00Z
 */
void DateTimeFormatter::printTo(string &buf, int64_t instant) {
    printTo(buf, instant, NULL);
}

/**
 * Prints an instant from milliseconds since 1970-01-01T00:00:00Z,
 * using ISO Chronology *in the default DateTimeZone.
 *
 * @param out  the destination to format to, not NULL
 * @param instant  millis since 1970-01-01T00:00:00Z
 */
void DateTimeFormatter::printTo(stringstream &out, int64_t instant) {
    printTo(out, instant, NULL);
}

/**
 * Prints an instant from milliseconds since 1970-01-01T00:00:00Z,
 * using ISO Chronology *in the default DateTimeZone.
 *
 * @param appendable  the destination to format to, not NULL
 * @param instant  millis since 1970-01-01T00:00:00Z
 * @since 2.0
 */
//void printTo(Appendable appendable, int64_t instant) throws IOException {
//    appendable.append(print(instant));
//}

//-----------------------------------------------------------------------
/**
 * Prints a ReadablePartial.
 * <p>
 * Neither the override Chronology *nor the override zone are used
 * by this method.
 *
 * @param buf  the destination to format to, not NULL
 * @param partial  partial to format
 */
void DateTimeFormatter::printTo(string &buf, ReadablePartial *partial) {
    DateTimePrinter *printer = requirePrinter();
    if (partial == NULL) {
        throw IllegalArgumentException("The partial must not be NULL");
    }
    printer->printTo(buf, partial, iLocale);
}

/**
 * Prints a ReadablePartial.
 * <p>
 * Neither the override Chronology *nor the override zone are used
 * by this method.
 *
 * @param out  the destination to format to, not NULL
 * @param partial  partial to format
 */
void DateTimeFormatter::printTo(stringstream &out, ReadablePartial *partial) {
    DateTimePrinter *printer = requirePrinter();
    if (partial == NULL) {
        throw IllegalArgumentException("The partial must not be NULL");
    }
    printer->printTo(out, partial, iLocale);
}

/**
 * Prints a ReadablePartial.
 * <p>
 * Neither the override Chronology *nor the override zone are used
 * by this method.
 *
 * @param appendable  the destination to format to, not NULL
 * @param partial  partial to format
 * @since 2.0
 */
//void printTo(Appendable appendable, ReadablePartial partial) throws IOException {
//    appendable.append(print(partial));
//}

//-----------------------------------------------------------------------
/**
 * Prints a ReadableInstant to a String.
 * <p>
 * This method will use the override zone and the override chronololgy if
 * they are set. Otherwise it will use the Chronology *and zone of the instant.
 *
 * @param instant  instant to format, NULL means now
 * @return the printed result
 */
string DateTimeFormatter::print(ReadableInstant *instant) {
    string buf;
    printTo(buf, instant);
    return buf;
}

/**
 * Prints a millisecond instant to a String.
 * <p>
 * This method will use the override zone and the override chronololgy if
 * they are set. Otherwise it will use the ISO Chronology *and default zone.
 *
 * @param instant  millis since 1970-01-01T00:00:00Z
 * @return the printed result
 */
string DateTimeFormatter::print(int64_t instant) {
    string buf;
    printTo(buf, instant);
    return buf;
}

/**
 * Prints a ReadablePartial to a new String.
 * <p>
 * Neither the override Chronology *nor the override zone are used
 * by this method.
 *
 * @param partial  partial to format
 * @return the printed result
 */
string DateTimeFormatter::print(ReadablePartial *partial) {
    string buf;
    printTo(buf, partial);
    return buf;
}

void DateTimeFormatter::printTo(string &buf, int64_t instant, Chronology *chrono) {
    DateTimePrinter *printer = requirePrinter();
    chrono = selectChronology(chrono);
    // Shift instant into local time (UTC) to avoid excessive offset
    // calculations when printing multiple fields in a composite printer.
    DateTimeZone *zone = chrono->getZone();
    int offset = zone->getOffset(instant);
    int64_t adjustedInstant = instant + offset;
    if ((instant ^ adjustedInstant) < 0 && (instant ^ offset) >= 0) {
        // Time zone offset overflow, so revert to UTC.
        zone = DateTimeZone::UTC;
        offset = 0;
        adjustedInstant = instant;
    }
    printer->printTo(buf, adjustedInstant, chrono->withUTC(), offset, zone, iLocale);
}

void DateTimeFormatter::printTo(stringstream &buf, int64_t instant, Chronology *chrono) {
    DateTimePrinter *printer = requirePrinter();
    chrono = selectChronology(chrono);
    // Shift instant into local time (UTC) to avoid excessive offset
    // calculations when printing multiple fields in a composite printer.
    DateTimeZone *zone = chrono->getZone();
    int offset = zone->getOffset(instant);
    int64_t adjustedInstant = instant + offset;
    if ((instant ^ adjustedInstant) < 0 && (instant ^ offset) >= 0) {
        // Time zone offset overflow, so revert to UTC.
        zone = DateTimeZone::UTC;
        offset = 0;
        adjustedInstant = instant;
    }
    printer->printTo(buf, adjustedInstant, chrono->withUTC(), offset, zone, iLocale);
}

/**
 * Checks whether printing is supported.
 *
 * @throws UnsupportedOperationException if printing is not supported
 */
DateTimePrinter *DateTimeFormatter::requirePrinter() {
    DateTimePrinter *printer = iPrinter;
    if (printer == NULL) {
        throw UnsupportedOperationException("Printing not supported");
    }
    return printer;
}

//-----------------------------------------------------------------------
/**
 * Parses a datetime from the given text, at the given position, saving the
 * result into the fields of the given ReadWritableInstant. If the parse
 * succeeds, the return value is the new text position. Note that the parse
 * may succeed without fully reading the text and in this case those fields
 * that were read will be set.
 * <p>
 * Only those fields present in the string will be changed in the specified
 * instant. All other fields will remain unaltered. Thus if the string only
 * contains a year and a month, then the day and time will be retained from
 * the input instant. If this is not the behaviour you want, then reset the
 * fields before calling this method, or use {@link #parseDateTime(String)}
 * or {@link #parseMutableDateTime(String)}.
 * <p>
 * If it fails, the return value is negative, but the instant may still be
 * modified. To determine the position where the parse failed, apply the
 * one's complement operator (~) on the return value.
 * <p>
 * This parse method ignores the {@link #getDefaultYear() default year} and
 * parses using the year from the supplied instant based on the chronology
 * and time-zone of the supplied instant.
 * <p>
 * The parse will use the Chronology *of the instant.
 *
 * @param instant  an instant that will be modified, not NULL
 * @param text  the text to parse
 * @param position  position to start parsing from
 * @return new position, negative value means parse failed -
 *  apply complement operator (~) to get position of failure
 * @throws UnsupportedOperationException if parsing is not supported
 * @throws IllegalArgumentException if the instant is NULL
 * @throws IllegalArgumentException if any field is out of range
 */
int DateTimeFormatter::parseInto(ReadWritableInstant *instant, string text, int position) {
    DateTimeParser *parser = requireParser();
    if (instant == NULL) {
        throw IllegalArgumentException("Instant must not be NULL");
    }
    
    int64_t instantMillis = instant->getMillis();
    Chronology *chrono = instant->getChronology();
    int defaultYear = DateTimeUtils::getChronology(chrono)->year()->get(instantMillis);
    int64_t instantLocal = instantMillis + chrono->getZone()->getOffset(instantMillis);
    chrono = selectChronology(chrono);
    
    DateTimeParserBucket *bucket = new DateTimeParserBucket(instantLocal, chrono, iLocale, iPivotYear, defaultYear);
    int newPos = parser->parseInto(bucket, text, position);
    instant->setMillis(bucket->computeMillis(false, text));
    if (iOffsetParsed && bucket->getOffsetint() != -1) {
        int parsedOffset = bucket->getOffsetint();
        DateTimeZone *parsedZone = DateTimeZone::forOffsetMillis(parsedOffset);
        chrono = chrono->withZone(parsedZone);
    } else if (bucket->getZone() != NULL) {
        chrono = chrono->withZone(bucket->getZone());
    }
    instant->setChronology(chrono);
    if (iZone != NULL) {
        instant->setZone(iZone);
    }
    return newPos;
}

/**
 * Parses a datetime from the given text, returning the number of
 * milliseconds since the epoch, 1970-01-01T00:00:00Z.
 * <p>
 * The parse will use the ISO chronology, and the default time zone.
 * If the text contains a time zone string then that will be taken into account.
 *
 * @param text  text to parse
 * @return parsed value expressed in milliseconds since the epoch
 * @throws UnsupportedOperationException if parsing is not supported
 * @throws IllegalArgumentException if the text to parse is invalid
 */
int64_t DateTimeFormatter::parseMillis(string text) {
    DateTimeParser *parser = requireParser();
    
    Chronology *chrono = selectChronology(iChrono);
    DateTimeParserBucket *bucket = new DateTimeParserBucket(0, chrono, iLocale, iPivotYear, iDefaultYear);
    int newPos = parser->parseInto(bucket, text, 0);
    if (newPos >= 0) {
        if (newPos >= text.size()) {
            return bucket->computeMillis(true, text);
        }
    } else {
        newPos = ~newPos;
    }
    throw IllegalArgumentException(FormatUtils::createErrorMessage(text, newPos));
}

/**
 * Parses only the local date from the given text, returning a new LocalDate.
 * <p>
 * This will parse the text fully according to the formatter, using the UTC zone.
 * Once parsed, only the local date will be used.
 * This means that any parsed time, time-zone or offset field is completely ignored.
 * It also means that the zone and offset-parsed settings are ignored.
 *
 * @param text  the text to parse, not NULL
 * @return the parsed date, never NULL
 * @throws UnsupportedOperationException if parsing is not supported
 * @throws IllegalArgumentException if the text to parse is invalid
 * @since 2.0
 */
LocalDate *DateTimeFormatter::parseLocalDate(string text) {
    return parseLocalDateTime(text)->toLocalDate();
}

/**
 * Parses only the local time from the given text, returning a new LocalTime.
 * <p>
 * This will parse the text fully according to the formatter, using the UTC zone.
 * Once parsed, only the local time will be used.
 * This means that any parsed date, time-zone or offset field is completely ignored.
 * It also means that the zone and offset-parsed settings are ignored.
 *
 * @param text  the text to parse, not NULL
 * @return the parsed time, never NULL
 * @throws UnsupportedOperationException if parsing is not supported
 * @throws IllegalArgumentException if the text to parse is invalid
 * @since 2.0
 */
LocalTime *DateTimeFormatter::parseLocalTime(string text) {
    return parseLocalDateTime(text)->toLocalTime();
}

/**
 * Parses only the local date-time from the given text, returning a new LocalDateTime.
 * <p>
 * This will parse the text fully according to the formatter, using the UTC zone.
 * Once parsed, only the local date-time will be used.
 * This means that any parsed time-zone or offset field is completely ignored.
 * It also means that the zone and offset-parsed settings are ignored.
 *
 * @param text  the text to parse, not NULL
 * @return the parsed date-time, never NULL
 * @throws UnsupportedOperationException if parsing is not supported
 * @throws IllegalArgumentException if the text to parse is invalid
 * @since 2.0
 */
LocalDateTime *DateTimeFormatter::parseLocalDateTime(string text) {
    DateTimeParser *parser = requireParser();
    
    Chronology *chrono = selectChronology(NULL)->withUTC();  // always use UTC, avoiding DST gaps
    DateTimeParserBucket *bucket = new DateTimeParserBucket(0, chrono, iLocale, iPivotYear, iDefaultYear);
    int newPos = parser->parseInto(bucket, text, 0);
    if (newPos >= 0) {
        if (newPos >= text.size()) {
            int64_t millis = bucket->computeMillis(true, text);
            if (bucket->getOffsetint() != -1) {  // treat withOffsetParsed() as being true
                int parsedOffset = bucket->getOffsetint();
                DateTimeZone *parsedZone = DateTimeZone::forOffsetMillis(parsedOffset);
                chrono = chrono->withZone(parsedZone);
            } else if (bucket->getZone() != NULL) {
                chrono = chrono->withZone(bucket->getZone());
            }
            return new LocalDateTime(millis, chrono);
        }
    } else {
        newPos = ~newPos;
    }
    throw IllegalArgumentException(FormatUtils::createErrorMessage(text, newPos));
}

/**
 * Parses a date-time from the given text, returning a new DateTime.
 * <p>
 * The parse will use the zone and Chronology *specified on this formatter.
 * <p>
 * If the text contains a time zone string then that will be taken into
 * account in adjusting the time of day as follows.
 * If the {@link #withOffsetParsed()} has been called, then the resulting
 * DateTime will have a fixed offset based on the parsed time zone.
 * Otherwise the resulting DateTime will have the zone of this formatter,
 * but the parsed zone may have caused the time to be adjusted.
 *
 * @param text  the text to parse, not NULL
 * @return the parsed date-time, never NULL
 * @throws UnsupportedOperationException if parsing is not supported
 * @throws IllegalArgumentException if the text to parse is invalid
 */
DateTime *DateTimeFormatter::parseDateTime(string text) {
    DateTimeParser *parser = requireParser();
    
    Chronology *chrono = selectChronology(NULL);
    DateTimeParserBucket *bucket = new DateTimeParserBucket(0, chrono, iLocale, iPivotYear, iDefaultYear);
    int newPos = parser->parseInto(bucket, text, 0);
    if (newPos >= 0) {
        if (newPos >= text.size()) {
            int64_t millis = bucket->computeMillis(true, text);
            if (iOffsetParsed && bucket->getOffsetint() != -1) {
                int parsedOffset = bucket->getOffsetint();
                DateTimeZone *parsedZone = DateTimeZone::forOffsetMillis(parsedOffset);
                chrono = chrono->withZone(parsedZone);
            } else if (bucket->getZone() != NULL) {
                chrono = chrono->withZone(bucket->getZone());
            }
            DateTime *dt = new DateTime(millis, chrono);
            if (iZone != NULL) {
                dt = dt->withZone(iZone);
            }
            return dt;
        }
    } else {
        newPos = ~newPos;
    }
    throw IllegalArgumentException(FormatUtils::createErrorMessage(text, newPos));
}

/**
 * Parses a date-time from the given text, returning a new MutableDateTime.
 * <p>
 * The parse will use the zone and Chronology *specified on this formatter.
 * <p>
 * If the text contains a time zone string then that will be taken into
 * account in adjusting the time of day as follows.
 * If the {@link #withOffsetParsed()} has been called, then the resulting
 * DateTime will have a fixed offset based on the parsed time zone.
 * Otherwise the resulting DateTime will have the zone of this formatter,
 * but the parsed zone may have caused the time to be adjusted.
 *
 * @param text  the text to parse, not NULL
 * @return the parsed date-time, never NULL
 * @throws UnsupportedOperationException if parsing is not supported
 * @throws IllegalArgumentException if the text to parse is invalid
 */
MutableDateTime *DateTimeFormatter::parseMutableDateTime(string text) {
    DateTimeParser *parser = requireParser();
    
    Chronology *chrono = selectChronology(NULL);
    DateTimeParserBucket *bucket = new DateTimeParserBucket(0, chrono, iLocale, iPivotYear, iDefaultYear);
    int newPos = parser->parseInto(bucket, text, 0);
    if (newPos >= 0) {
        if (newPos >= text.size()) {
            int64_t millis = bucket->computeMillis(true, text);
            if (iOffsetParsed && bucket->getOffsetint() != -1) {
                int parsedOffset = bucket->getOffsetint();
                DateTimeZone *parsedZone = DateTimeZone::forOffsetMillis(parsedOffset);
                chrono = chrono->withZone(parsedZone);
            } else if (bucket->getZone() != NULL) {
                chrono = chrono->withZone(bucket->getZone());
            }
            MutableDateTime *dt = new MutableDateTime(millis, chrono);
            if (iZone != NULL) {
                dt->setZone(iZone);
            }
            return dt;
        }
    } else {
        newPos = ~newPos;
    }
    throw IllegalArgumentException(FormatUtils::createErrorMessage(text, newPos));
}

/**
 * Checks whether parsing is supported.
 *
 * @throws UnsupportedOperationException if parsing is not supported
 */
DateTimeParser *DateTimeFormatter::requireParser() {
    DateTimeParser *parser = iParser;
    if (parser == NULL) {
        throw UnsupportedOperationException("Parsing not supported");
    }
    return parser;
}

//-----------------------------------------------------------------------
/**
 * Determines the correct Chronology *to use.
 *
 * @param chrono  the proposed chronology
 * @return the actual chronology
 */
Chronology *DateTimeFormatter::selectChronology(Chronology *chrono) {
    chrono = DateTimeUtils::getChronology(chrono);
    if (iChrono != NULL) {
        chrono = iChrono;
    }
    if (iZone != NULL) {
        chrono = chrono->withZone(iZone);
    }
    return chrono;
}

CODATIME_END