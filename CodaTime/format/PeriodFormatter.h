//
//  PeriodFormatter.h
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_PeriodFormatter_h
#define CodaTime_PeriodFormatter_h

#include "CodaTimeMacros.h"

#include "Exceptions.h"
#include "format/FormatUtils.h"
#include "format/PeriodParser.h"
#include "format/PeriodPrinter.h"
#include "Locale.h"
#include "MutablePeriod.h"
#include "ReadWritablePeriod.h"

#include <string>
#include <sstream>

using namespace std;

CODATIME_BEGIN

class PeriodType;
class ReadablePeriod;

/**
 * Controls the printing and parsing of a time period to and from a string.
 * <p>
 * This class is the main API for printing and parsing used by most applications.
 * Instances of this class are created via one of three factory classes:
 * <ul>
 * <li>{@link PeriodFormat} - formats by pattern and style</li>
 * <li>{@link ISOPeriodFormat} - ISO8601 formats</li>
 * <li>{@link PeriodFormatterBuilder} - complex formats created via method calls</li>
 * </ul>
 * <p>
 * An instance of this class holds a reference internally to one printer and
 * one parser. It is possible that one of these may be NULL, in which case the
 * formatter cannot print/parse. This can be checked via the {@link #isPrinter()}
 * and {@link #isParser()} methods.
 * <p>
 * The underlying printer/parser can be altered to behave exactly as required
 * by using a decorator modifier:
 * <ul>
 * <li>{@link #withLocale(Locale)} - returns a new formatter that uses the specified locale</li>
 * </ul>
 * This returns a new formatter (instances of this class are immutable).
 * <p>
 * The main methods of the class are the <code>printXxx</code> and
 * <code>parseXxx</code> methods. These are used as follows:
 * <pre>
 * // print using the default locale
 * String periodStr = formatter.print(period);
 * // print using the French locale
 * String periodStr = formatter.withLocale(Locale.FRENCH).print(period);
 *
 * // parse using the French locale
 * Period date = formatter.withLocale(Locale.FRENCH).parsePeriod(str);
 * </pre>
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
class PeriodFormatter {
    
private:
    
    /** The internal printer used to output the datetime. */
    PeriodPrinter *iPrinter;
    /** The internal parser used to output the datetime. */
    PeriodParser *iParser;
    /** The locale to use for printing and parsing. */
    const Locale *iLocale;
    /** The period type used in parsing. */
    const PeriodType *iParseType;
    
    /**
     * Constructor.
     *
     * @param printer  the internal printer, NULL if cannot print
     * @param parser  the internal parser, NULL if cannot parse
     * @param locale  the locale to use
     * @param type  the parse period type
     */
    PeriodFormatter(PeriodPrinter *printer, PeriodParser *parser,
                            const Locale *locale, const PeriodType *type) {
        iPrinter = printer;
        iParser = parser;
        iLocale = locale;
        iParseType = type;
    }
    
    /**
     * Checks whether printing is supported.
     *
     * @throws UnsupportedOperationException if printing is not supported
     */
    void checkPrinter() {
        if (iPrinter == NULL) {
            throw UnsupportedOperationException("Printing not supported");
        }
    }
    
    /**
     * Checks whether the period is non-NULL.
     *
     * @throws IllegalArgumentException if the period is NULL
     */
    void checkPeriod(ReadablePeriod *period) {
        if (period == NULL) {
            throw IllegalArgumentException("Period must not be NULL");
        }
    }
    
    /**
     * Checks whether parsing is supported.
     *
     * @throws UnsupportedOperationException if parsing is not supported
     */
    void checkParser() {
        if (iParser == NULL) {
            throw UnsupportedOperationException("Parsing not supported");
        }
    }
    
public:
    
    /**
     * Creates a new formatter, however you will normally use the factory
     * or the builder.
     *
     * @param printer  the internal printer, NULL if cannot print
     * @param parser  the internal parser, NULL if cannot parse
     */
    PeriodFormatter(PeriodPrinter *printer, PeriodParser *parser) {
        iPrinter = printer;
        iParser = parser;
        iLocale = NULL;
        iParseType = NULL;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Is this formatter capable of printing.
     *
     * @return true if this is a printer
     */
    bool isPrinter() {
        return (iPrinter != NULL);
    }
    
    /**
     * Gets the internal printer object that performs the real printing work.
     *
     * @return the internal printer
     */
    PeriodPrinter *getPrinter() {
        return iPrinter;
    }
    
    /**
     * Is this formatter capable of parsing.
     *
     * @return true if this is a parser
     */
    bool isParser() {
        return (iParser != NULL);
    }
    
    /**
     * Gets the internal parser object that performs the real parsing work.
     *
     * @return the internal parser
     */
    PeriodParser *getParser() {
        return iParser;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Returns a new formatter with a different locale that will be used
     * for printing and parsing.
     * <p>
     * A PeriodFormatter is immutable, so a new instance is returned,
     * and the original is unaltered and still usable.
     *
     * @param locale  the locale to use
     * @return the new formatter
     */
    PeriodFormatter *withLocale(Locale *locale) {
        if (locale == getLocale() || (locale != NULL && locale->equals(getLocale()))) {
            return this;
        }
        return new PeriodFormatter(iPrinter, iParser, locale, iParseType);
    }
    
    /**
     * Gets the locale that will be used for printing and parsing.
     *
     * @return the locale to use
     */
    const Locale *getLocale() {
        return iLocale;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Returns a new formatter with a different PeriodType for parsing.
     * <p>
     * A PeriodFormatter is immutable, so a new instance is returned,
     * and the original is unaltered and still usable.
     *
     * @param type  the type to use in parsing
     * @return the new formatter
     */
    PeriodFormatter *withParseType(PeriodType *type) {
        if (type == iParseType) {
            return this;
        }
        return new PeriodFormatter(iPrinter, iParser, iLocale, type);
    }
    
    /**
     * Gets the PeriodType that will be used for parsing.
     *
     * @return the parse type to use
     */
    const PeriodType *getParseType() {
        return iParseType;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Prints a ReadablePeriod to a StringBuffer.
     *
     * @param buf  the formatted period is appended to this buffer
     * @param period  the period to format, not NULL
     */
    void printTo(string buf, ReadablePeriod *period) {
        checkPrinter();
        checkPeriod(period);
        
        getPrinter()->printTo(buf, period, iLocale);
    }
    
    /**
     * Prints a ReadablePeriod to a Writer.
     *
     * @param out  the formatted period is written out
     * @param period  the period to format, not NULL
     */
    void printTo(stringstream out, ReadablePeriod *period) {
        checkPrinter();
        checkPeriod(period);
        
        getPrinter()->printTo(out, period, iLocale);
    }
    
    /**
     * Prints a ReadablePeriod to a new String.
     *
     * @param period  the period to format, not NULL
     * @return the printed result
     */
    string print(ReadablePeriod *period) {
        checkPrinter();
        checkPeriod(period);
        
        const PeriodPrinter *printer = getPrinter();
        string buf;
        printer->printTo(buf, period, iLocale);
        return buf;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Parses a period from the given text, at the given position, saving the
     * result into the fields of the given ReadWritablePeriod. If the parse
     * succeeds, the return value is the new text position. Note that the parse
     * may succeed without fully reading the text.
     * <p>
     * The parse type of the formatter is not used by this method.
     * <p>
     * If it fails, the return value is negative, but the period may still be
     * modified. To determine the position where the parse failed, apply the
     * one's complement operator (~) on the return value.
     *
     * @param period  a period that will be modified
     * @param text  text to parse
     * @param position position to start parsing from
     * @return new position, if negative, parse failed. Apply complement
     * operator (~) to get position of failure
     * @throws IllegalArgumentException if any field is out of range
     */
    int parseInto(ReadWritablePeriod *period, string text, int position) {
        checkParser();
        checkPeriod(period);
        
        return getParser()->parseInto(period, text, position, iLocale);
    }
    
    /**
     * Parses a period from the given text, returning a new Period.
     *
     * @param text  text to parse
     * @return parsed value in a Period object
     * @throws IllegalArgumentException if any field is out of range
     */
    Period *parsePeriod(string text) {
        checkParser();
        
        return parseMutablePeriod(text)->toPeriod();
    }
    
    /**
     * Parses a period from the given text, returning a new MutablePeriod.
     *
     * @param text  text to parse
     * @return parsed value in a MutablePeriod object
     * @throws IllegalArgumentException if any field is out of range
     */
    MutablePeriod *parseMutablePeriod(string text) {
        checkParser();
        
        MutablePeriod *period = new MutablePeriod(0LL, iParseType);
        int newPos = getParser()->parseInto(period, text, 0, iLocale);
        if (newPos >= 0) {
            if (newPos >= text.size()) {
                return period;
            }
        } else {
            newPos = ~newPos;
        }
        throw IllegalArgumentException(FormatUtils::createErrorMessage(text, newPos));
    }
    
};

CODATIME_END

#endif
