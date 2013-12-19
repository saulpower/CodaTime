//
//  DateTimeFormat.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/19/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "DateTimeFormat.h"

#include "DateTime.h"
#include "format/DateTimeFormatter.h"
//#include "format/DateTimeFormatterBuilder.h"
#include "Locale.h"

CODATIME_BEGIN

const vector<DateTimeFormatter*> DateTimeFormat::STYLE_CACHE = vector<DateTimeFormatter*>(25);

void DateTimeFormat::appendPatternTo(DateTimeFormatterBuilder *builder, string pattern) {
    parsePatternTo(builder, pattern);
}

//-----------------------------------------------------------------------
void DateTimeFormat::parsePatternTo(DateTimeFormatterBuilder *builder, string pattern) {
    int length = pattern.size();
    vector<int> indexRef;
    
    for (int i = 0; i < length; i++) {
        indexRef[0] = i;
        string token = parseToken(pattern, indexRef);
        i = indexRef[0];
        
        int tokenLen = token.size();
        if (tokenLen == 0) {
            break;
        }
        char c = token.at(0);
        
        switch (c) {
            case 'G': // era designator (text)
                builder->appendEraText();
                break;
            case 'C': // century of era (number)
                builder->appendCenturyOfEra(tokenLen, tokenLen);
                break;
            case 'x': // weekyear (number)
            case 'y': // year (number)
            case 'Y': // year of era (number)
                if (tokenLen == 2) {
                    bool lenientParse = true;
                    
                    // Peek ahead to next token.
                    if (i + 1 < length) {
                        indexRef[0]++;
                        if (isNumericToken(parseToken(pattern, indexRef))) {
                            // If next token is a number, cannot support
                            // lenient parse, because it will consume digits
                            // that it should not.
                            lenientParse = false;
                        }
                        indexRef[0]--;
                    }
                    
                    // Use pivots which are compatible with SimpleDateFormat.
                    switch (c) {
                        case 'x':
                            DateTime dateTime = DateTime();
                            builder->appendTwoDigitWeekyear(dateTime.getWeekyear() - 30, lenientParse);
                            break;
                        case 'y':
                        case 'Y':
                        default:
                            DateTime *dateTime = DateTime();
                            builder->appendTwoDigitYear(dateTime.getYear() - 30, lenientParse);
                            break;
                    }
                } else {
                    // Try to support int64_t year values.
                    int maxDigits = 9;
                    
                    // Peek ahead to next token.
                    if (i + 1 < length) {
                        indexRef[0]++;
                        if (isNumericToken(parseToken(pattern, indexRef))) {
                            // If next token is a number, cannot support int64_t years.
                            maxDigits = tokenLen;
                        }
                        indexRef[0]--;
                    }
                    
                    switch (c) {
                        case 'x':
                            builder->appendWeekyear(tokenLen, maxDigits);
                            break;
                        case 'y':
                            builder->appendYear(tokenLen, maxDigits);
                            break;
                        case 'Y':
                            builder->appendYearOfEra(tokenLen, maxDigits);
                            break;
                    }
                }
                break;
            case 'M': // month of year (text and number)
                if (tokenLen >= 3) {
                    if (tokenLen >= 4) {
                        builder->appendMonthOfYearText();
                    } else {
                        builder->appendMonthOfYearShortText();
                    }
                } else {
                    builder->appendMonthOfYear(tokenLen);
                }
                break;
            case 'd': // day of month (number)
                builder->appendDayOfMonth(tokenLen);
                break;
            case 'a': // am/pm marker (text)
                builder->appendHalfdayOfDayText();
                break;
            case 'h': // clockhour of halfday (number, 1..12)
                builder->appendClockhourOfHalfday(tokenLen);
                break;
            case 'H': // hour of day (number, 0..23)
                builder->appendHourOfDay(tokenLen);
                break;
            case 'k': // clockhour of day (1..24)
                builder->appendClockhourOfDay(tokenLen);
                break;
            case 'K': // hour of halfday (0..11)
                builder->appendHourOfHalfday(tokenLen);
                break;
            case 'm': // minute of hour (number)
                builder->appendMinuteOfHour(tokenLen);
                break;
            case 's': // second of minute (number)
                builder->appendSecondOfMinute(tokenLen);
                break;
            case 'S': // fraction of second (number)
                builder->appendFractionOfSecond(tokenLen, tokenLen);
                break;
            case 'e': // day of week (number)
                builder->appendDayOfWeek(tokenLen);
                break;
            case 'E': // dayOfWeek (text)
                if (tokenLen >= 4) {
                    builder->appendDayOfWeekText();
                } else {
                    builder->appendDayOfWeekShortText();
                }
                break;
            case 'D': // day of year (number)
                builder->appendDayOfYear(tokenLen);
                break;
            case 'w': // week of weekyear (number)
                builder->appendWeekOfWeekyear(tokenLen);
                break;
            case 'z': // time zone (text)
                if (tokenLen >= 4) {
                    builder->appendTimeZoneName();
                } else {
                    builder->appendTimeZoneShortName(NULL);
                }
                break;
            case 'Z': // time zone offset
                if (tokenLen == 1) {
                    builder->appendTimeZoneOffset(NULL, "Z", false, 2, 2);
                } else if (tokenLen == 2) {
                    builder->appendTimeZoneOffset(NULL, "Z", true, 2, 2);
                } else {
                    builder->appendTimeZoneId();
                }
                break;
            case '\'': // literal text
                string sub = token.substring(1);
                if (sub.size() == 1) {
                    builder->appendLiteral(sub.at(0));
                } else {
                    // Create copy of sub since otherwise the temporary quoted
                    // string would still be referenced internally.
                    builder->appendLiteral(new string(sub));
                }
                break;
            default:
                throw IllegalArgumentException("Illegal pattern component: " + token);
        }
    }
}

string DateTimeFormat::parseToken(string pattern, vector<int> indexRef) {
    string buf;
    
    int i = indexRef[0];
    int length = pattern.size();
    
    char c = pattern.at(i);
    if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z') {
        // Scan a run of the same character, which indicates a time
        // pattern.
        buf.append(1, c);
        
        while (i + 1 < length) {
            char peek = pattern.at(i + 1);
            if (peek == c) {
                buf.append(1, c);
                i++;
            } else {
                break;
            }
        }
    } else {
        // This will identify token as text.
        buf.append(1, '\'');
        
        bool inLiteral = false;
        
        for (; i < length; i++) {
            c = pattern.at(i);
            
            if (c == '\'') {
                if (i + 1 < length && pattern.at(i + 1) == '\'') {
                    // '' is treated as escaped '
                    i++;
                    buf.append(1, c);
                } else {
                    inLiteral = !inLiteral;
                }
            } else if (!inLiteral &&
                       (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')) {
                i--;
                break;
            } else {
                buf.append(1, c);
            }
        }
    }
    
    indexRef[0] = i;
    return buf;
}

bool DateTimeFormat::isNumericToken(string token) {
    int tokenLen = token.size();
    if (tokenLen > 0) {
        char c = token.at(0);
        switch (c) {
            case 'c': // century (number)
            case 'C': // century of era (number)
            case 'x': // weekyear (number)
            case 'y': // year (number)
            case 'Y': // year of era (number)
            case 'd': // day of month (number)
            case 'h': // hour of day (number, 1..12)
            case 'H': // hour of day (number, 0..23)
            case 'm': // minute of hour (number)
            case 's': // second of minute (number)
            case 'S': // fraction of second (number)
            case 'e': // day of week (number)
            case 'D': // day of year (number)
            case 'F': // day of week in month (number)
            case 'w': // week of year (number)
            case 'W': // week of month (number)
            case 'k': // hour of day (1..24)
            case 'K': // hour of day (0..11)
                return true;
            case 'M': // month of year (text and number)
                if (tokenLen <= 2) {
                    return true;
                }
        }
    }
    
    return false;
}

//-----------------------------------------------------------------------
DateTimeFormatter *DateTimeFormat::createFormatterForPattern(string pattern) {
    if (pattern == NULL || pattern.size() == 0) {
        throw IllegalArgumentException("Invalid pattern specification");
    }
    DateTimeFormatter *formatter = NULL;
    // TODO: synchronized
//    synchronized (PATTERN_CACHE) {
        formatter = PATTERN_CACHE[pattern];
        if (formatter == NULL) {
            DateTimeFormatterBuilder *builder = new DateTimeFormatterBuilder();
            parsePatternTo(builder, pattern);
            formatter = builder->toFormatter();
            
            PATTERN_CACHE.insert(pair<string, DateTimeFormatter*>(pattern, formatter));
        }
//    }
    return formatter;
}

DateTimeFormatter *DateTimeFormat::createFormatterForStyle(string style) {
    if (style == NULL || style.size() != 2) {
        throw IllegalArgumentException("Invalid style specification: " + style);
    }
    int dateStyle = selectStyle(style.at(0));
    int timeStyle = selectStyle(style.at(1));
    if (dateStyle == NONE && timeStyle == NONE) {
        throw IllegalArgumentException("Style '--' is invalid");
    }
    return createFormatterForStyleIndex(dateStyle, timeStyle);
}

DateTimeFormatter *DateTimeFormat::createFormatterForStyleIndex(int dateStyle, int timeStyle) {
    int index = ((dateStyle << 2) + dateStyle) + timeStyle;
    // Should never happen but do a double check...
    if (index >= STYLE_CACHE.size()) {
        return createDateTimeFormatter(dateStyle, timeStyle);
    }
    DateTimeFormatter *f = NULL;
    // TODO synchronized
//    synchronized (STYLE_CACHE) {
        f = STYLE_CACHE[index];
        if (f == NULL) {
            f = createDateTimeFormatter(dateStyle, timeStyle);
            STYLE_CACHE[index] = f;
        }
//    }
    return f;
}

DateTimeFormatter *DateTimeFormat::createDateTimeFormatter(int dateStyle, int timeStyle) {
    int type = DATETIME;
    if (dateStyle == NONE) {
        type = TIME;
    } else if (timeStyle == NONE) {
        type = DATE;
    }
    StyleFormatter *llf = new StyleFormatter(dateStyle, timeStyle, type);
    return new DateTimeFormatter(llf, llf);
}

int DateTimeFormat::selectStyle(char ch) {
    switch (ch) {
        case 'S':
            return SHORT;
        case 'M':
            return MEDIUM;
        case 'L':
            return LONG;
        case 'F':
            return FULL;
        case '-':
            return NONE;
        default:
            throw IllegalArgumentException("Invalid style character: " + ch);
    }
}

DateTimeFormatter *DateTimeFormat::forPattern(string pattern) {
    return createFormatterForPattern(pattern);
}

DateTimeFormatter *DateTimeFormat::forStyle(string style) {
    return createFormatterForStyle(style);
}

string patternForStyle(string style, Locale *locale) {
    DateTimeFormatter *formatter = createFormatterForStyle(style);
    if (locale == NULL) {
        locale = Locale::getDefault();
    }
    // Not pretty, but it works.
    return ((StyleFormatter*) formatter->getPrinter())->getPattern(locale);
}

//-----------------------------------------------------------------------
DateTimeFormatter *DateTimeFormat::shortDate() {
    return createFormatterForStyleIndex(SHORT, NONE);
}

DateTimeFormatter *DateTimeFormat::shortTime() {
    return createFormatterForStyleIndex(NONE, SHORT);
}

DateTimeFormatter *DateTimeFormat::shortDateTime() {
    return createFormatterForStyleIndex(SHORT, SHORT);
}

//-----------------------------------------------------------------------
DateTimeFormatter *DateTimeFormat::mediumDate() {
    return createFormatterForStyleIndex(MEDIUM, NONE);
}

DateTimeFormatter *DateTimeFormat::mediumTime() {
    return createFormatterForStyleIndex(NONE, MEDIUM);
}

DateTimeFormatter *DateTimeFormat::mediumDateTime() {
    return createFormatterForStyleIndex(MEDIUM, MEDIUM);
}

//-----------------------------------------------------------------------
DateTimeFormatter *DateTimeFormat::longDate() {
    return createFormatterForStyleIndex(LONG, NONE);
}

DateTimeFormatter *DateTimeFormat::longTime() {
    return createFormatterForStyleIndex(NONE, LONG);
}

DateTimeFormatter *DateTimeFormat::longDateTime() {
    return createFormatterForStyleIndex(LONG, LONG);
}

//-----------------------------------------------------------------------
DateTimeFormatter *DateTimeFormat::fullDate() {
    return createFormatterForStyleIndex(FULL, NONE);
}

DateTimeFormatter *DateTimeFormat::fullTime() {
    return createFormatterForStyleIndex(NONE, FULL);
}

DateTimeFormatter *DateTimeFormat::fullDateTime() {
    return createFormatterForStyleIndex(FULL, FULL);
}

int DateTimeFormat::StyleFormatter::estimatePrintedLength() {
    return 40;  // guess
}

void DateTimeFormat::StyleFormatter::printTo(string &buf, int64_t instant, Chronology *chrono,
             int displayOffset, DateTimeZone *displayZone, Locale *locale) {
    DateTimePrinter *p = getFormatter(locale)->getPrinter();
    p->printTo(buf, instant, chrono, displayOffset, displayZone, locale);
}

void DateTimeFormat::StyleFormatter::printTo(stringstream &out, int64_t instant, Chronology *chrono,
             int displayOffset, DateTimeZone *displayZone, Locale *locale) {
    DateTimePrinter *p = getFormatter(locale)->getPrinter();
    p->printTo(out, instant, chrono, displayOffset, displayZone, locale);
}

void DateTimeFormat::StyleFormatter::printTo(string &buf, ReadablePartial *partial, Locale *locale) {
    DateTimePrinter *p = getFormatter(locale)->getPrinter();
    p->printTo(buf, partial, locale);
}

void DateTimeFormat::StyleFormatter::printTo(stringstream &out, ReadablePartial *partial, Locale *locale) {
    DateTimePrinter *p = getFormatter(locale)->getPrinter();
    p->printTo(out, partial, locale);
}

int DateTimeFormat::StyleFormatter::estimateParsedLength() {
    return 40;  // guess
}

int DateTimeFormat::StyleFormatter::parseInto(DateTimeParserBucket *bucket, string text, int position) {
    DateTimeParser p = getFormatter(bucket->getLocale())->getParser();
    return p.parseInto(bucket, text, position);
}

DateTimeFormatter *DateTimeFormat::StyleFormatter::getFormatter(Locale *locale) {
    locale = (locale == NULL ? Locale::getDefault() : locale);
    string key = to_string(iType + (iDateStyle << 4) + (iTimeStyle << 8)) + locale->tostring();
    DateTimeFormatter *f = NULL;
//    synchronized (cCache) {
        f = cCache[key];
        if (f == NULL) {
            string pattern = getPattern(locale);
            f = DateTimeFormat::forPattern(pattern);
            cCache.insert(pair<string, DateTimeFormatter*>(key, f));
        }
//    }
    return f;
}

string DateTimeFormat::StyleFormatter::getPattern(Locale *locale) {
    DateFormat *f = NULL;
    switch (iType) {
        case DATE:
            f = DateFormat::getDateInstance(iDateStyle, locale);
            break;
        case TIME:
            f = DateFormat::getTimeInstance(iTimeStyle, locale);
            break;
        case DATETIME:
            f = DateFormat::getDateTimeInstance(iDateStyle, iTimeStyle, locale);
            break;
    }
    
    SimpleDateFormat *sdf = dynamic_cast<SimpleDateFormat*>(f);
    
    if (sdf != 0) {
        throw IllegalArgumentException("No datetime pattern for locale: " + locale);
    }
    return sdf->toPattern();
}

CODATIME_END