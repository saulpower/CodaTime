//
//  DateTimeFormat.h
//  CodaTime
//
//  Created by Saul Howard on 12/19/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__DateTimeFormat__
#define __CodaTime__DateTimeFormat__

#include "CodaTimeMacros.h"

#include "format/DateTimePrinter.h"
#include "format/DateTimeParser.h"

#include <map>
#include <vector>
#include <sstream>

using namespace std;

CODATIME_BEGIN

class DateTimeFormatter;
class DateTimeFormatterBuilder;

/**
 * Factory that creates instances of DateTimeFormatter from patterns and styles.
 * <p>
 * Datetime formatting is performed by the {@link DateTimeFormatter} class.
 * Three classes provide factory methods to create formatters, and this is one.
 * The others are {@link ISODateTimeFormat} and {@link DateTimeFormatterBuilder}.
 * <p>
 * This class provides two types of factory:
 * <ul>
 * <li>{@link #forPattern(string) Pattern} provides a DateTimeFormatter based on
 * a pattern string that is mostly compatible with the JDK date patterns.
 * <li>{@link #forStyle(string) Style} provides a DateTimeFormatter based on a
 * two character style, representing short, medium, int64_t and full.
 * </ul>
 * <p>
 * For example, to use a patterm:
 * <pre>
 * DateTime dt = new DateTime();
 * DateTimeFormatter fmt = DateTimeFormat.forPattern("MMMM, yyyy");
 * string str = fmt.print(dt);
 * </pre>
 *
 * The pattern syntax is mostly compatible with java.text.SimpleDateFormat -
 * time zone names cannot be parsed and a few more symbols are supported.
 * All ASCII letters are reserved as pattern letters, which are defined as follows:
 * <blockquote>
 * <pre>
 * Symbol  Meaning                      Presentation  Examples
 * ------  -------                      ------------  -------
 * G       era                          text          AD
 * C       century of era (&gt;=0)         number        20
 * Y       year of era (&gt;=0)            year          1996
 *
 * x       weekyear                     year          1996
 * w       week of weekyear             number        27
 * e       day of week                  number        2
 * E       day of week                  text          Tuesday; Tue
 *
 * y       year                         year          1996
 * D       day of year                  number        189
 * M       month of year                month         July; Jul; 07
 * d       day of month                 number        10
 *
 * a       halfday of day               text          PM
 * K       hour of halfday (0~11)       number        0
 * h       clockhour of halfday (1~12)  number        12
 *
 * H       hour of day (0~23)           number        0
 * k       clockhour of day (1~24)      number        24
 * m       minute of hour               number        30
 * s       second of minute             number        55
 * S       fraction of second           millis        978
 *
 * z       time zone                    text          Pacific Standard Time; PST
 * Z       time zone offset/id          zone          -0800; -08:00; America/Los_Angeles
 *
 * '       escape for text              delimiter
 * ''      single quote                 literal       '
 * </pre>
 * </blockquote>
 * The count of pattern letters determine the format.
 * <p>
 * <strong>Text</strong>: If the number of pattern letters is 4 or more,
 * the full form is used; otherwise a short or abbreviated form is used if
 * available.
 * <p>
 * <strong>Number</strong>: The minimum number of digits.
 * Shorter numbers are zero-padded to this amount.
 * When parsing, any number of digits are accepted.
 * <p>
 * <strong>Year</strong>: Numeric presentation for year and weekyear fields
 * are handled specially. For example, if the count of 'y' is 2, the year
 * will be displayed as the zero-based year of the century, which is two
 * digits.
 * <p>
 * <strong>Month</strong>: 3 or over, use text, otherwise use number.
 * <p>
 * <strong>Millis</strong>: The exact number of fractional digits.
 * If more millisecond digits are available then specified the number will be truncated,
 * if there are fewer than specified then the number will be zero-padded to the right.
 * When parsing, only the exact number of digits are accepted.
 * <p>
 * <strong>Zone</strong>: 'Z' outputs offset without a colon, 'ZZ' outputs
 * the offset with a colon, 'ZZZ' or more outputs the zone id.
 * <p>
 * <strong>Zone names</strong>: Time zone names ('z') cannot be parsed.
 * <p>
 * Any characters in the pattern that are not in the ranges of ['a'..'z']
 * and ['A'..'Z'] will be treated as quoted text. For instance, characters
 * like ':', '.', ' ', '#' and '?' will appear in the resulting time text
 * even they are not embraced within single quotes.
 * <p>
 * DateTimeFormat is thread-safe and immutable, and the formatters it returns
 * are as well.
 *
 * @author Brian S O'Neill
 * @author Maxim Zhao
 * @since 1.0
 * @see ISODateTimeFormat
 * @see DateTimeFormatterBuilder
 */
class DateTimeFormat {
    
private:
    
    /** Style constant for FULL. */
    static const int FULL = 0;  // DateFormat.FULL
    /** Style constant for LONG. */
    static const int LONG = 1;  // DateFormat.LONG
    /** Style constant for MEDIUM. */
    static const int MEDIUM = 2;  // DateFormat.MEDIUM
    /** Style constant for SHORT. */
    static const int SHORT = 3;  // DateFormat.SHORT
    /** Style constant for NONE. */
    static const int NONE = 4;
    
    /** Type constant for DATE only. */
    static const int DATE = 0;
    /** Type constant for TIME only. */
    static const int TIME = 1;
    /** Type constant for DATETIME. */
    static const int DATETIME = 2;
    
    /** Maximum size of the pattern cache. */
    static const int PATTERN_CACHE_SIZE = 500;
    
    /** maps patterns to formatters via LRU, patterns don't vary by locale. */
    static const map<string, DateTimeFormatter*> PATTERN_CACHE;// = new LinkedHashmap<string, DateTimeFormatter*>(7) {
//        static const int64_t serialVersionUID = 23L;
//        @Override
//        protected bool removeEldestEntry(const map.Entry<string, DateTimeFormatter*> eldest) {
//            return size() > PATTERN_CACHE_SIZE;
//        };
//    };
    
    /** maps patterns to formatters, patterns don't vary by locale. */
    static const vector<DateTimeFormatter*> STYLE_CACHE;
    
    //-----------------------------------------------------------------------
    /**
     * Parses the given pattern and appends the rules to the given
     * DateTimeFormatterBuilder.
     *
     * @param pattern  pattern specification
     * @throws IllegalArgumentException if the pattern is invalid
     */
    static void appendPatternTo(DateTimeFormatterBuilder *builder, string pattern);
    
    //-----------------------------------------------------------------------
    /**
     * Parses the given pattern and appends the rules to the given
     * DateTimeFormatterBuilder.
     *
     * @param pattern  pattern specification
     * @throws IllegalArgumentException if the pattern is invalid
     * @see #forPattern
     */
    static void parsePatternTo(DateTimeFormatterBuilder *builder, string pattern);
    
    /**
     * Parses an individual token.
     *
     * @param pattern  the pattern string
     * @param indexRef  a single element array, where the input is the start
     *  location and the output is the location after parsing the token
     * @return the parsed token
     */
    static string parseToken(string pattern, vector<int> indexRef);
    
    /**
     * Returns true if token should be parsed as a numeric field.
     *
     * @param token  the token to parse
     * @return true if numeric field
     */
    static bool isNumericToken(string token);
    
    //-----------------------------------------------------------------------
    /**
     * Select a format from a custom pattern.
     *
     * @param pattern  pattern specification
     * @throws IllegalArgumentException if the pattern is invalid
     * @see #appendPatternTo
     */
    static DateTimeFormatter *createFormatterForPattern(string pattern);
    
    /**
     * Select a format from a two character style pattern. The first character
     * is the date style, and the second character is the time style. Specify a
     * character of 'S' for short style, 'M' for medium, 'L' for long, and 'F'
     * for full. A date or time may be ommitted by specifying a style character '-'.
     *
     * @param style  two characters from the set {"S", "M", "L", "F", "-"}
     * @throws IllegalArgumentException if the style is invalid
     */
    static DateTimeFormatter *createFormatterForStyle(string style);
    
    /**
     * Gets the formatter for the specified style.
     *
     * @param dateStyle  the date style
     * @param timeStyle  the time style
     * @return the formatter
     */
    static DateTimeFormatter *createFormatterForStyleIndex(int dateStyle, int timeStyle);
    
    /**
     * Creates a formatter for the specified style.
     * @param dateStyle  the date style
     * @param timeStyle  the time style
     * @return the formatter
     */
    static DateTimeFormatter *createDateTimeFormatter(int dateStyle, int timeStyle);
    
    /**
     * Gets the JDK style code from the Joda code.
     *
     * @param ch  the Joda style code
     * @return the JDK style code
     */
    static int selectStyle(char ch);
    
    //-----------------------------------------------------------------------
    class StyleFormatter : public DateTimePrinter, public DateTimeParser {
        
    public:
        
        static const map<string, DateTimeFormatter*> cCache;// = new Hashmap<string, DateTimeFormatter*>();  // manual sync
        
        const int iDateStyle;
        const int iTimeStyle;
        const int iType;
        
        StyleFormatter(int dateStyle, int timeStyle, int type) : iDateStyle(dateStyle), iTimeStyle(timeStyle), iType(type) {
        }
        
        int estimatePrintedLength();
        
        void printTo(string &buf, int64_t instant, Chronology *chrono,
                     int displayOffset, DateTimeZone *displayZone, Locale *locale);
        
        void printTo(stringstream &out, int64_t instant, Chronology *chrono,
                     int displayOffset, DateTimeZone *displayZone, Locale *locale);
        
        void printTo(string &buf, ReadablePartial *partial, Locale *locale);
        
        void printTo(stringstream &out, ReadablePartial *partial, Locale *locale);
        
        int estimateParsedLength();
        
        int parseInto(DateTimeParserBucket *bucket, string text, int position);
        
        DateTimeFormatter *getFormatter(Locale *locale);
        
        string getPattern(Locale *locale);
    };
    
public:
    
    //-----------------------------------------------------------------------
    /**
     * Factory to create a formatter from a pattern string.
     * The pattern string is described above in the class level javadoc.
     * It is very similar to SimpleDateFormat patterns.
     * <p>
     * The format may contain locale specific output, and this will change as
     * you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     * For example:
     * <pre>
     * DateTimeFormat.forPattern(pattern).withLocale(Locale.FRANCE).print(dt);
     * </pre>
     *
     * @param pattern  pattern specification
     * @return the formatter
     * @throws IllegalArgumentException if the pattern is invalid
     */
    static DateTimeFormatter *forPattern(string pattern);
    
    /**
     * Factory to create a format from a two character style pattern.
     * <p>
     * The first character is the date style, and the second character is the
     * time style. Specify a character of 'S' for short style, 'M' for medium,
     * 'L' for long, and 'F' for full.
     * A date or time may be ommitted by specifying a style character '-'.
     * <p>
     * The returned formatter will dynamically adjust to the locale that
     * the print/parse takes place in. Thus you just call
     * {@link DateTimeFormatter#withLocale(Locale)} and the Short/Medium/Long/Full
     * style for that locale will be output. For example:
     * <pre>
     * DateTimeFormat.forStyle(style).withLocale(Locale.FRANCE).print(dt);
     * </pre>
     *
     * @param style  two characters from the set {"S", "M", "L", "F", "-"}
     * @return the formatter
     * @throws IllegalArgumentException if the style is invalid
     */
    static DateTimeFormatter *forStyle(string style);
    
    /**
     * Returns the pattern used by a particular style and locale.
     * <p>
     * The first character is the date style, and the second character is the
     * time style. Specify a character of 'S' for short style, 'M' for medium,
     * 'L' for long, and 'F' for full.
     * A date or time may be ommitted by specifying a style character '-'.
     *
     * @param style  two characters from the set {"S", "M", "L", "F", "-"}
     * @param locale  locale to use, null means default
     * @return the formatter
     * @throws IllegalArgumentException if the style is invalid
     * @since 1.3
     */
    static string patternForStyle(string style, Locale *locale);
    
    //-----------------------------------------------------------------------
    /**
     * Creates a format that outputs a short date format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *shortDate();
    
    /**
     * Creates a format that outputs a short time format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *shortTime();
    
    /**
     * Creates a format that outputs a short datetime format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *shortDateTime();
    
    //-----------------------------------------------------------------------
    /**
     * Creates a format that outputs a medium date format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *mediumDate();
    
    /**
     * Creates a format that outputs a medium time format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *mediumTime();
    
    /**
     * Creates a format that outputs a medium datetime format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *mediumDateTime();
    
    //-----------------------------------------------------------------------
    /**
     * Creates a format that outputs a int64_t date format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *longDate();
    
    /**
     * Creates a format that outputs a int64_t time format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *longTime();
    
    /**
     * Creates a format that outputs a int64_t datetime format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *longDateTime();
    
    //-----------------------------------------------------------------------
    /**
     * Creates a format that outputs a full date format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *fullDate();
    
    /**
     * Creates a format that outputs a full time format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *fullTime();
    
    /**
     * Creates a format that outputs a full datetime format.
     * <p>
     * The format will change as you change the locale of the formatter.
     * Call {@link DateTimeFormatter#withLocale(Locale)} to switch the locale.
     *
     * @return the formatter
     */
    static DateTimeFormatter *fullDateTime();
    
protected:
    
    //-----------------------------------------------------------------------
    /**
     * Constructor.
     *
     * @since 1.1 (previously private)
     */
    DateTimeFormat() {
    }
    
};

CODATIME_END

#endif /* defined(__CodaTime__DateTimeFormat__) */
