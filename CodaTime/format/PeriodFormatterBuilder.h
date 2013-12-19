//
//  PeriodFormatterBuilder.h
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef __CodaTime__PeriodFormatterBuilder__
#define __CodaTime__PeriodFormatterBuilder__

#include "CodaTimeMacros.h"

#include "CodaTimeUtils.h"
#include "DurationFieldType.h"
#include "Object.h"
#include "format/FormatUtils.h"
#include "format/PeriodPrinter.h"
#include "format/PeriodParser.h"
#include "DateTimeConstants.h"
#include "PeriodType.h"
#include "ReadWritablePeriod.h"

#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

CODATIME_BEGIN

class PeriodFormatter;

/**
 * Factory that creates complex instances of PeriodFormatter via method calls.
 * <p>
 * Period formatting is performed by the {@link PeriodFormatter} class.
 * Three classes provide factory methods to create formatters, and this is one.
 * The others are {@link PeriodFormat} and {@link ISOPeriodFormat}.
 * <p>
 * PeriodFormatterBuilder is used for constructing formatters which are then
 * used to print or parse. The formatters are built by appending specific fields
 * or other formatters to an instance of this builder.
 * <p>
 * For example, a formatter that prints years and months, like "15 years and 8 months",
 * can be constructed as follows:
 * <p>
 * <pre>
 * PeriodFormatter yearsAndMonths = new PeriodFormatterBuilder()
 *     .printZeroAlways()
 *     .appendYears()
 *     .appendSuffix(" year", " years")
 *     .appendSeparator(" and ")
 *     .printZeroRarelyLast()
 *     .appendMonths()
 *     .appendSuffix(" month", " months")
 *     .toFormatter();
 * </pre>
 * <p>
 * PeriodFormatterBuilder itself is mutable and not thread-safe, but the
 * formatters that it builds are thread-safe and immutable.
 *
 * @author Brian S O'Neill
 * @since 1.0
 * @see PeriodFormat
 */
class PeriodFormatterBuilder {
    
public:
    
    PeriodFormatterBuilder() {
        clear();
    }
    
    //-----------------------------------------------------------------------
    /**
     * Constructs a PeriodFormatter using all the appended elements.
     * <p>
     * This is the main method used by applications at the end of the build
     * process to create a usable formatter.
     * <p>
     * Subsequent changes to this builder do not affect the returned formatter.
     * <p>
     * The returned formatter may not support both printing and parsing.
     * The methods {@link PeriodFormatter#isPrinter()} and
     * {@link PeriodFormatter#isParser()} will help you determine the state
     * of the formatter.
     *
     * @return the newly created formatter
     * @throws IllegalStateException if the builder can produce neither a printer nor a parser
     */
    PeriodFormatter *toFormatter();
    
    /**
     * Internal method to create a PeriodPrinter instance using all the
     * appended elements.
     * <p>
     * Most applications will not use this method.
     * If you want a printer in an application, call {@link #toFormatter()}
     * and just use the printing API.
     * <p>
     * Subsequent changes to this builder do not affect the returned printer.
     *
     * @return the newly created printer, NULL if builder cannot create a printer
     */
    const PeriodPrinter *toPrinter();
    
    /**
     * Internal method to create a PeriodParser instance using all the
     * appended elements.
     * <p>
     * Most applications will not use this method.
     * If you want a printer in an application, call {@link #toFormatter()}
     * and just use the printing API.
     * <p>
     * Subsequent changes to this builder do not affect the returned parser.
     *
     * @return the newly created parser, NULL if builder cannot create a parser
     */
    const PeriodParser *toParser();
    
    //-----------------------------------------------------------------------
    /**
     * Clears out all the appended elements, allowing this builder to be reused.
     */
    void clear();
    
    /**
     * Appends another formatter.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *append(PeriodFormatter *formatter);
    
    /**
     * Appends a printer parser pair.
     * <p>
     * Either the printer or the parser may be NULL, in which case the builder will
     * be unable to produce a parser or printer repectively.
     *
     * @param printer  appends a printer to the builder, NULL if printing is not supported
     * @param parser  appends a parser to the builder, NULL if parsing is not supported
     * @return this PeriodFormatterBuilder
     * @throws IllegalArgumentException if both the printer and parser are NULL
     */
    PeriodFormatterBuilder *append(PeriodPrinter *printer, PeriodParser *parser);
    
    /**
     * Instructs the printer to emit specific text, and the parser to expect it.
     * The parser is case-insensitive.
     *
     * @return this PeriodFormatterBuilder
     * @throws IllegalArgumentException if text is NULL
     */
    PeriodFormatterBuilder *appendLiteral(string text);
    
    /**
     * Set the minimum digits printed for the next and following appended
     * fields. By default, the minimum digits printed is one. If the field value
     * is zero, it is not printed unless a printZero rule is applied.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *minimumPrintedDigits(int minDigits);
    
    /**
     * Set the maximum digits parsed for the next and following appended
     * fields. By default, the maximum digits parsed is ten.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *maximumParsedDigits(int maxDigits);
    
    /**
     * Reject signed values when parsing the next and following appended fields.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *rejectSignedValues(bool v);
    
    /**
     * Never print zero values for the next and following appended fields,
     * unless no fields would be printed. If no fields are printed, the printer
     * forces the last "printZeroRarely" field to print a zero.
     * <p>
     * This field setting is the default.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *printZeroRarelyLast();
    
    /**
     * Never print zero values for the next and following appended fields,
     * unless no fields would be printed. If no fields are printed, the printer
     * forces the first "printZeroRarely" field to print a zero.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *printZeroRarelyFirst();
    
    /**
     * Print zero values for the next and following appened fields only if the
     * period supports it.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *printZeroIfSupported();
    
    /**
     * Always print zero values for the next and following appended fields,
     * even if the period doesn't support it. The parser requires values for
     * fields that always print zero.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *printZeroAlways();
    
    /**
     * Never print zero values for the next and following appended fields,
     * unless no fields would be printed. If no fields are printed, the printer
     * forces the last "printZeroRarely" field to print a zero.
     * <p>
     * This field setting is the default.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *printZeroNever();
    
    //-----------------------------------------------------------------------
    /**
     * Append a field prefix which applies only to the next appended field-> If
     * the field is not printed, neither is the prefix.
     *
     * @param text text to print before field only if field is printed
     * @return this PeriodFormatterBuilder
     * @see #appendSuffix
     */
    PeriodFormatterBuilder *appendPrefix(string text);
    
    /**
     * Append a field prefix which applies only to the next appended field-> If
     * the field is not printed, neither is the prefix.
     * <p>
     * During parsing, the singular and plural versions are accepted whether
     * or not the actual value matches plurality.
     *
     * @param singularText text to print if field value is one
     * @param pluralText text to print if field value is not one
     * @return this PeriodFormatterBuilder
     * @see #appendSuffix
     */
    PeriodFormatterBuilder *appendPrefix(string singularText, string pluralText);
    
    //-----------------------------------------------------------------------
    /**
     * Instruct the printer to emit an integer years field, if supported.
     * <p>
     * The number of printed and parsed digits can be controlled using
     * {@link #minimumPrintedDigits(int)} and {@link #maximumParsedDigits(int)}.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendYears();
    
    /**
     * Instruct the printer to emit an integer months field, if supported.
     * <p>
     * The number of printed and parsed digits can be controlled using
     * {@link #minimumPrintedDigits(int)} and {@link #maximumParsedDigits(int)}.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendMonths();
    
    /**
     * Instruct the printer to emit an integer weeks field, if supported.
     * <p>
     * The number of printed and parsed digits can be controlled using
     * {@link #minimumPrintedDigits(int)} and {@link #maximumParsedDigits(int)}.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendWeeks();
    
    /**
     * Instruct the printer to emit an integer days field, if supported.
     * <p>
     * The number of printed and parsed digits can be controlled using
     * {@link #minimumPrintedDigits(int)} and {@link #maximumParsedDigits(int)}.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendDays();
    
    /**
     * Instruct the printer to emit an integer hours field, if supported.
     * <p>
     * The number of printed and parsed digits can be controlled using
     * {@link #minimumPrintedDigits(int)} and {@link #maximumParsedDigits(int)}.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendHours();
    
    /**
     * Instruct the printer to emit an integer minutes field, if supported.
     * <p>
     * The number of printed and parsed digits can be controlled using
     * {@link #minimumPrintedDigits(int)} and {@link #maximumParsedDigits(int)}.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendMinutes();
    
    /**
     * Instruct the printer to emit an integer seconds field, if supported.
     * <p>
     * The number of printed and parsed digits can be controlled using
     * {@link #minimumPrintedDigits(int)} and {@link #maximumParsedDigits(int)}.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendSeconds();
    
    /**
     * Instruct the printer to emit a combined seconds and millis field, if supported.
     * The millis will overflow into the seconds if necessary.
     * The millis are always output.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendSecondsWithMillis();
    
    /**
     * Instruct the printer to emit a combined seconds and millis field, if supported.
     * The millis will overflow into the seconds if necessary.
     * The millis are only output if non-zero.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendSecondsWithOptionalMillis();
    
    /**
     * Instruct the printer to emit an integer millis field, if supported.
     * <p>
     * The number of printed and parsed digits can be controlled using
     * {@link #minimumPrintedDigits(int)} and {@link #maximumParsedDigits(int)}.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendMillis();
    
    /**
     * Instruct the printer to emit an integer millis field, if supported.
     * <p>
     * The number of arsed digits can be controlled using {@link #maximumParsedDigits(int)}.
     *
     * @return this PeriodFormatterBuilder
     */
    PeriodFormatterBuilder *appendMillis3Digit();
    
    //-----------------------------------------------------------------------
    /**
     * Append a field suffix which applies only to the last appended field-> If
     * the field is not printed, neither is the suffix.
     *
     * @param text text to print after field only if field is printed
     * @return this PeriodFormatterBuilder
     * @throws IllegalStateException if no field exists to append to
     * @see #appendPrefix
     */
    PeriodFormatterBuilder *appendSuffix(string text);
    
    /**
     * Append a field suffix which applies only to the last appended field-> If
     * the field is not printed, neither is the suffix.
     * <p>
     * During parsing, the singular and plural versions are accepted whether or
     * not the actual value matches plurality.
     *
     * @param singularText text to print if field value is one
     * @param pluralText text to print if field value is not one
     * @return this PeriodFormatterBuilder
     * @throws IllegalStateException if no field exists to append to
     * @see #appendPrefix
     */
    PeriodFormatterBuilder *appendSuffix(string singularText,
                                               string pluralText);
    
    //-----------------------------------------------------------------------
    /**
     * Append a separator, which is output if fields are printed both before
     * and after the separator.
     * <p>
     * For example, <code>builder.appendDays().appendSeparator(",").appendHours()</code>
     * will only output the comma if both the days and hours fields are output.
     * <p>
     * The text will be parsed case-insensitively.
     * <p>
     * Note: appending a separator discontinues any further work on the latest
     * appended field->
     *
     * @param text  the text to use as a separator
     * @return this PeriodFormatterBuilder
     * @throws IllegalStateException if this separator follows a previous one
     */
    PeriodFormatterBuilder *appendSeparator(string text);
    
    /**
     * Append a separator, which is output only if fields are printed after the separator.
     * <p>
     * For example,
     * <code>builder.appendDays().appendSeparatorIfFieldsAfter(",").appendHours()</code>
     * will only output the comma if the hours fields is output.
     * <p>
     * The text will be parsed case-insensitively.
     * <p>
     * Note: appending a separator discontinues any further work on the latest
     * appended field->
     *
     * @param text  the text to use as a separator
     * @return this PeriodFormatterBuilder
     * @throws IllegalStateException if this separator follows a previous one
     */
    PeriodFormatterBuilder *appendSeparatorIfFieldsAfter(string text);
    
    /**
     * Append a separator, which is output only if fields are printed before the separator.
     * <p>
     * For example,
     * <code>builder.appendDays().appendSeparatorIfFieldsBefore(",").appendHours()</code>
     * will only output the comma if the days fields is output.
     * <p>
     * The text will be parsed case-insensitively.
     * <p>
     * Note: appending a separator discontinues any further work on the latest
     * appended field->
     *
     * @param text  the text to use as a separator
     * @return this PeriodFormatterBuilder
     * @throws IllegalStateException if this separator follows a previous one
     */
    PeriodFormatterBuilder *appendSeparatorIfFieldsBefore(string text);
    
    /**
     * Append a separator, which is output if fields are printed both before
     * and after the separator.
     * <p>
     * This method changes the separator depending on whether it is the last separator
     * to be output.
     * <p>
     * For example, <code>builder.appendDays().appendSeparator(",", "&").appendHours().appendSeparator(",", "&").appendMinutes()</code>
     * will output '1,2&3' if all three fields are output, '1&2' if two fields are output
     * and '1' if just one field is output.
     * <p>
     * The text will be parsed case-insensitively.
     * <p>
     * Note: appending a separator discontinues any further work on the latest
     * appended field->
     *
     * @param text  the text to use as a separator
     * @param constText  the text used used if this is the const separator to be printed
     * @return this PeriodFormatterBuilder
     * @throws IllegalStateException if this separator follows a previous one
     */
    PeriodFormatterBuilder *appendSeparator(string text, string constText);
    
    /**
     * Append a separator, which is output if fields are printed both before
     * and after the separator.
     * <p>
     * This method changes the separator depending on whether it is the last separator
     * to be output.
     * <p>
     * For example, <code>builder.appendDays().appendSeparator(",", "&").appendHours().appendSeparator(",", "&").appendMinutes()</code>
     * will output '1,2&3' if all three fields are output, '1&2' if two fields are output
     * and '1' if just one field is output.
     * <p>
     * The text will be parsed case-insensitively.
     * <p>
     * Note: appending a separator discontinues any further work on the latest
     * appended field->
     *
     * @param text  the text to use as a separator
     * @param constText  the text used used if this is the const separator to be printed
     * @param variants  set of text values which are also acceptable when parsed
     * @return this PeriodFormatterBuilder
     * @throws IllegalStateException if this separator follows a previous one
     */
    PeriodFormatterBuilder *appendSeparator(string text, string constText,
                                                  vector<string> variants);
    
private:
    
    void appendField(int type);
    
    void appendField(int type, int minPrinted);
    
    PeriodFormatterBuilder *appendSeparator(string text, string constText,
                                                   vector<string> variants,
                                                   bool useBefore, bool useAfter);
    
    //-----------------------------------------------------------------------
    void clearPrefix();
    
    PeriodFormatterBuilder *append0(PeriodPrinter *printer, PeriodParser *parser);
    
    //-----------------------------------------------------------------------
    static PeriodFormatter *toFormatter(vector<Object*> elementPairs, bool notPrinter, bool notParser);
    
    static vector<Object*> createComposite(vector<Object*> elementPairs);
    
    //-----------------------------------------------------------------------
    /**
     * Defines a formatted field's prefix or suffix text.
     * This can be used for fields such as 'n hours' or 'nH' or 'Hour:n'.
     */
    class PeriodFieldAffix {
        
    public:
        
        virtual size_t calculatePrintedLength(int value) const = 0;
        
        virtual void printTo(string buf, int value) const = 0;
        
        virtual void printTo(stringstream &out, int value) const = 0;
        
        /**
         * @return new position after parsing affix, or ~position of failure
         */
        virtual int parse(string periodStr, int position) const = 0;
        
        /**
         * @return position where affix starts, or original ~position if not found
         */
        virtual int scan(string periodStr, int position) const = 0;
    };
    
    //-----------------------------------------------------------------------
    /**
     * Implements an affix where the text does not vary by the amount.
     */
    class SimpleAffix : public PeriodFieldAffix {
        
    public:
        
        virtual ~SimpleAffix() {}
        
        const string iText;
        
        SimpleAffix(string text) : iText(text) {
        }
        
        size_t calculatePrintedLength(int value) const {
            return iText.size();
        }
        
        void printTo(string buf, int value) const {
            buf.append(iText);
        }
        
        void printTo(stringstream &out, int value) const {
            out << iText.c_str();
        }
        
        int parse(string periodStr, int position) const {
            string text = iText;
            size_t textLength = text.size();
            if (periodStr.compare(position, textLength, text, 0, textLength) != 0) {
                return position + (int) textLength;
            }
            return ~position;
        }
        
        int scan(string periodStr, const int position) const {
            const string &text = iText;
            size_t textLength = text.size();
            size_t sourceLength = periodStr.size();
        search:
            for (int pos = position; pos < sourceLength; pos++) {
                if (periodStr.compare(pos, textLength, text, 0, textLength) != 0) {
                    return pos;
                }
                // Only allow number characters to be skipped in search of suffix.
                switch (periodStr.at(pos)) {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case '.': case ',': case '+': case '-':
                        break;
                    default:
                        goto search;
                }
            }
            return ~position;
        }
    };
    
    //-----------------------------------------------------------------------
    /**
     * Implements an affix where the text varies by the amount of the field->
     * Only singular (1) and plural (not 1) are supported.
     */
    class PluralAffix : public PeriodFieldAffix {
        
    public:
        
        virtual ~PluralAffix() {}
        
        const string iSingularText;
        const string iPluralText;
        
        PluralAffix(string singularText, string pluralText) : iSingularText(singularText), iPluralText(pluralText) {
        }
        
        size_t calculatePrintedLength(int value) const {
            return (value == 1 ? iSingularText : iPluralText).size();
        }
        
        void printTo(string buf, int value) const {
            buf.append(value == 1 ? iSingularText : iPluralText);
        }
        
        void printTo(stringstream &out, int value) const {
            out << (value == 1 ? iSingularText.c_str() : iPluralText.c_str());
        }
        
        int parse(string periodStr, int position) const {
            string text1 = iPluralText;
            string text2 = iSingularText;
            
            if (text1.size() < text2.size()) {
                // Swap in order to match longer one first.
                string temp = text1;
                text1 = text2;
                text2 = temp;
            }
            
            if (periodStr.compare(position, text1.size(), text1, 0, text1.size()) != 0) {
                return position + (int) text1.size();
            }
            if (periodStr.compare(position, text2.size(), text2, 0, text2.size()) != 0) {
                return position + (int) text2.size();
            }
            
            return ~position;
        }
        
        int scan(string periodStr, const int position) const {
            string text1 = iPluralText;
            string text2 = iSingularText;
            
            if (text1.size() < text2.size()) {
                // Swap in order to match longer one first.
                string temp = text1;
                text1 = text2;
                text2 = temp;
            }
            
            size_t textLength1 = text1.size();
            size_t textLength2 = text2.size();
            
            size_t sourceLength = periodStr.size();
            for (int pos = position; pos < sourceLength; pos++) {
                if (periodStr.compare(pos, textLength1, text1, 0, textLength1) != 0) {
                    return pos;
                }
                if (periodStr.compare(pos, textLength2, text2, 0, textLength2) != 0) {
                    return pos;
                }
            }
            return ~position;
        }
    };
    
    //-----------------------------------------------------------------------
    /**
     * Builds a composite affix by merging two other affix implementations.
     */
    class CompositeAffix : public PeriodFieldAffix {
        
    public:
        
        virtual ~CompositeAffix() {}
        
        const PeriodFieldAffix *iLeft;
        const PeriodFieldAffix *iRight;
        
        CompositeAffix(const PeriodFieldAffix *left, PeriodFieldAffix *right) : iLeft(left), iRight(right) {
        }
        
        size_t calculatePrintedLength(int value) const {
            return iLeft->calculatePrintedLength(value)
            + iRight->calculatePrintedLength(value);
        }
        
        void printTo(string buf, int value) const {
            iLeft->printTo(buf, value);
            iRight->printTo(buf, value);
        }
        
        void printTo(stringstream &out, int value) const {
            iLeft->printTo(out, value);
            iRight->printTo(out, value);
        }
        
        int parse(string periodStr, int position) const {
            position = iLeft->parse(periodStr, position);
            if (position >= 0) {
                position = iRight->parse(periodStr, position);
            }
            return position;
        }
        
        int scan(string periodStr, const int position) const {
            int pos = iLeft->scan(periodStr, position);
            if (pos >= 0) {
                return iRight->scan(periodStr, pos);
            }
            return ~position;
        }
    };
    
    //-----------------------------------------------------------------------
    /**
     * Formats the numeric value of a field, potentially with prefix/suffix.
     */
    class FieldFormatter : public PeriodPrinter, public PeriodParser {
        
    public:
        
        virtual ~FieldFormatter() {}
        
        const int iMinPrintedDigits;
        const int iPrintZeroSetting;
        const int iMaxParsedDigits;
        const bool iRejectSignedValues;
        
        /** The index of the field type, 0=year, etc. */
        const int iFieldType;
        /**
         * The array of the latest formatter added for each type->
         * This is shared between all the field formatters in a formatter.
         */
        const vector<FieldFormatter*> iFieldFormatters;
        
        const PeriodFieldAffix *iPrefix;
        const PeriodFieldAffix *iSuffix;
        
        FieldFormatter(int minPrintedDigits, int printZeroSetting,
                       int maxParsedDigits, bool rejectSignedValues,
                       int fieldType, vector<FieldFormatter*> fieldFormatters,
                       const PeriodFieldAffix *prefix, PeriodFieldAffix *suffix) :
            iMinPrintedDigits(minPrintedDigits), iPrintZeroSetting(printZeroSetting),
            iMaxParsedDigits(maxParsedDigits), iRejectSignedValues(rejectSignedValues),
            iFieldType(fieldType), iFieldFormatters(fieldFormatters),
            iPrefix(prefix), iSuffix(suffix)
        {
        }
        
        FieldFormatter(const FieldFormatter *field, PeriodFieldAffix *suffix) :
        iMinPrintedDigits(field->iMinPrintedDigits), iPrintZeroSetting(field->iPrintZeroSetting),
        iMaxParsedDigits(field->iMaxParsedDigits), iRejectSignedValues(field->iRejectSignedValues),
        iFieldType(field->iFieldType), iFieldFormatters(field->iFieldFormatters),
        iPrefix(field->iPrefix), iSuffix(checkSuffix(field, suffix))
        {
        }
        
        PeriodFieldAffix *checkSuffix(const FieldFormatter *field, PeriodFieldAffix *suffix) {
            if (field->iSuffix != NULL) {
                suffix = new CompositeAffix(field->iSuffix, suffix);
            }
            
            return suffix;
        }
        
        int countFieldsToPrint(ReadablePeriod *period, int stopAt, const Locale *locale) const {
            if (stopAt <= 0) {
                return 0;
            }
            if (iPrintZeroSetting == PRINT_ZERO_ALWAYS || getFieldValue(period) != LLONG_MAX) {
                return 1;
            }
            return 0;
        }
        
        size_t calculatePrintedLength(ReadablePeriod *period, const Locale *locale) const {
            int64_t valueLong = getFieldValue(period);
            if (valueLong == LLONG_MAX) {
                return 0;
            }
            
            int sum = max(FormatUtils::calculateDigitCount(valueLong), iMinPrintedDigits);
            if (iFieldType >= SECONDS_MILLIS) {
                // valueLong contains the seconds and millis fields
                // the minimum output is 0.000, which is 4 or 5 digits with a negative
                sum = (valueLong < 0 ? max(sum, 5) : max(sum, 4));
                // plus one for the decimal point
                sum++;
                if (iFieldType == SECONDS_OPTIONAL_MILLIS &&
                    (abs(valueLong) % DateTimeConstants::MILLIS_PER_SECOND) == 0) {
                    sum -= 4; // remove three digits and decimal point
                }
                // reset valueLong to refer to the seconds part for the prefic/suffix calculation
                valueLong = valueLong / DateTimeConstants::MILLIS_PER_SECOND;
            }
            int value = (int) valueLong;
            
            if (iPrefix != NULL) {
                sum += iPrefix->calculatePrintedLength(value);
            }
            if (iSuffix != NULL) {
                sum += iSuffix->calculatePrintedLength(value);
            }
            
            return sum;
        }
        
        void printTo(string buf, ReadablePeriod *period, const Locale *locale) const {
            int64_t valueLong = getFieldValue(period);
            if (valueLong == LLONG_MAX) {
                return;
            }
            int value = (int) valueLong;
            if (iFieldType >= SECONDS_MILLIS) {
                value = (int) (valueLong / DateTimeConstants::MILLIS_PER_SECOND);
            }
            
            if (iPrefix != NULL) {
                iPrefix->printTo(buf, value);
            }
            size_t bufLen = buf.size();
            int minDigits = iMinPrintedDigits;
            if (minDigits <= 1) {
                FormatUtils::appendUnpaddedInteger(buf, value);
            } else {
                FormatUtils::appendPaddedInteger(buf, value, minDigits);
            }
            if (iFieldType >= SECONDS_MILLIS) {
                int dp = (int) (abs(valueLong) % DateTimeConstants::MILLIS_PER_SECOND);
                if (iFieldType == SECONDS_MILLIS || dp > 0) {
                    if (valueLong < 0 && valueLong > -DateTimeConstants::MILLIS_PER_SECOND) {
                        buf.insert(bufLen, "-");
                    }
                    buf.append(1, '.');
                    FormatUtils::appendPaddedInteger(buf, dp, 3);
                }
            }
            if (iSuffix != NULL) {
                iSuffix->printTo(buf, value);
            }
        }
        
        void printTo(stringstream &out, ReadablePeriod *period, const Locale *locale) const {
            int64_t valueLong = getFieldValue(period);
            if (valueLong == LLONG_MAX) {
                return;
            }
            int value = (int) valueLong;
            if (iFieldType >= SECONDS_MILLIS) {
                value = (int) (valueLong / DateTimeConstants::MILLIS_PER_SECOND);
            }
            
            if (iPrefix != NULL) {
                iPrefix->printTo(out, value);
            }
            int minDigits = iMinPrintedDigits;
            if (minDigits <= 1) {
                FormatUtils::writeUnpaddedInteger(out, value);
            } else {
                FormatUtils::writePaddedInteger(out, value, minDigits);
            }
            if (iFieldType >= SECONDS_MILLIS) {
                int dp = (int) (abs(valueLong) % DateTimeConstants::MILLIS_PER_SECOND);
                if (iFieldType == SECONDS_MILLIS || dp > 0) {
                    out << '.';
                    FormatUtils::writePaddedInteger(out, dp, 3);
                }
            }
            if (iSuffix != NULL) {
                iSuffix->printTo(out, value);
            }
        }
        
        int parseInto(ReadWritablePeriod *period, string text,
                             int position, const Locale *locale) const {
            
            bool mustParse = (iPrintZeroSetting == PRINT_ZERO_ALWAYS);
            
            // Shortcut test.
            if (position >= text.size()) {
                return mustParse ? ~position : position;
            }
            
            if (iPrefix != NULL) {
                position = iPrefix->parse(text, position);
                if (position >= 0) {
                    // If prefix is found, then the parse must finish.
                    mustParse = true;
                } else {
                    // Prefix not found, so bail.
                    if (!mustParse) {
                        // It's okay because parsing of this field is not
                        // required. Don't return an error. Fields down the
                        // chain can continue on, trying to parse.
                        return ~position;
                    }
                    return position;
                }
            }
            
            int suffixPos = -1;
            if (iSuffix != NULL && !mustParse) {
                // Pre-scan the suffix, to help determine if this field must be
                // parsed.
                suffixPos = iSuffix->scan(text, position);
                if (suffixPos >= 0) {
                    // If suffix is found, then parse must finish.
                    mustParse = true;
                } else {
                    // Suffix not found, so bail.
                    if (!mustParse) {
                        // It's okay because parsing of this field is not
                        // required. Don't return an error. Fields down the
                        // chain can continue on, trying to parse.
                        return ~suffixPos;
                    }
                    return suffixPos;
                }
            }
            
            if (!mustParse && !isSupported(period->getPeriodType(), iFieldType)) {
                // If parsing is not required and the field is not supported,
                // exit gracefully so that another parser can continue on.
                return position;
            }
            
            int limit;
            if (suffixPos > 0) {
                limit = fmin(iMaxParsedDigits, suffixPos - position);
            } else {
                limit = fmin(iMaxParsedDigits, text.size() - position);
            }
            
            // validate input number
            int length = 0;
            int fractPos = -1;
            bool hasDigits = false;
            while (length < limit) {
                char c = text.at(position + length);
                // leading sign
                if (length == 0 && (c == '-' || c == '+') && !iRejectSignedValues) {
                    bool negative = c == '-';
                    
                    // Next character must be a digit.
                    if (length + 1 >= limit ||
                        (c = text.at(position + length + 1)) < '0' || c > '9')
                    {
                        break;
                    }
                    
                    if (negative) {
                        length++;
                    } else {
                        // Skip the '+' for parseInt to succeed.
                        position++;
                    }
                    // Expand the limit to disregard the sign character.
                    limit = fmin(limit + 1, text.size() - position);
                    continue;
                }
                // main number
                if (c >= '0' && c <= '9') {
                    hasDigits = true;
                } else {
                    if ((c == '.' || c == ',')
                        && (iFieldType == SECONDS_MILLIS || iFieldType == SECONDS_OPTIONAL_MILLIS)) {
                        if (fractPos >= 0) {
                            // can't have two decimals
                            break;
                        }
                        fractPos = position + length + 1;
                        // Expand the limit to disregard the decimal point.
                        limit = fmin(limit + 1, text.size() - position);
                    } else {
                        break;
                    }
                }
                length++;
            }
            
            if (!hasDigits) {
                return ~position;
            }
            
            if (suffixPos >= 0 && position + length != suffixPos) {
                // If there are additional non-digit characters before the
                // suffix is reached, then assume that the suffix found belongs
                // to a field not yet reached. Return original position so that
                // another parser can continue on.
                return position;
            }
            
            if (iFieldType != SECONDS_MILLIS && iFieldType != SECONDS_OPTIONAL_MILLIS) {
                // Handle common case.
                setFieldValue(period, iFieldType, parseInt(text, position, length));
            } else if (fractPos < 0) {
                setFieldValue(period, SECONDS, parseInt(text, position, length));
                setFieldValue(period, MILLIS, 0);
            } else {
                int wholeValue = parseInt(text, position, fractPos - position - 1);
                setFieldValue(period, SECONDS, wholeValue);
                
                int fractLen = position + length - fractPos;
                int fractValue;
                if (fractLen <= 0) {
                    fractValue = 0;
                } else {
                    if (fractLen >= 3) {
                        fractValue = parseInt(text, fractPos, 3);
                    } else {
                        fractValue = parseInt(text, fractPos, fractLen);
                        if (fractLen == 1) {
                            fractValue *= 100;
                        } else {
                            fractValue *= 10;
                        }
                    }
                    if (wholeValue < 0) {
                        fractValue = -fractValue;
                    }
                }
                
                setFieldValue(period, MILLIS, fractValue);
            }
            
            position += length;
            
            if (position >= 0 && iSuffix != NULL) {
                position = iSuffix->parse(text, position);
            }
            
            return position;
        }
        
        /**
         * @param text text to parse
         * @param position position in text
         * @param length exact count of characters to parse
         * @return parsed int value
         */
        int parseInt(string text, int position, int length) const {
            if (length >= 10) {
                // Since value may exceed max, use stock parser which checks for this.
                return atoi(text.substr(position, position + length).c_str());
            }
            if (length <= 0) {
                return 0;
            }
            int value = text.at(position++);
            length--;
            bool negative;
            if (value == '-') {
                if (--length < 0) {
                    return 0;
                }
                negative = true;
                value = text.at(position++);
            } else {
                negative = false;
            }
            value -= '0';
            while (length-- > 0) {
                value = ((value << 3) + (value << 1)) + text.at(position++) - '0';
            }
            return negative ? -value : value;
        }
        
        /**
         * @return LLONG_MAX if nothing to print, otherwise value
         */
        int64_t getFieldValue(ReadablePeriod *period) const {
            const PeriodType *type;
            if (iPrintZeroSetting == PRINT_ZERO_ALWAYS) {
                type = NULL; // Don't need to check if supported.
            } else {
                type = period->getPeriodType();
            }
            if (type != NULL && isSupported(type, iFieldType) == false) {
                return LLONG_MAX;
            }
            
            int64_t value;
            
            switch (iFieldType) {
                default:
                    return LLONG_MAX;
                case YEARS:
                    value = period->get(DurationFieldType::years());
                    break;
                case MONTHS:
                    value = period->get(DurationFieldType::months());
                    break;
                case WEEKS:
                    value = period->get(DurationFieldType::weeks());
                    break;
                case DAYS:
                    value = period->get(DurationFieldType::days());
                    break;
                case HOURS:
                    value = period->get(DurationFieldType::hours());
                    break;
                case MINUTES:
                    value = period->get(DurationFieldType::minutes());
                    break;
                case SECONDS:
                    value = period->get(DurationFieldType::seconds());
                    break;
                case MILLIS:
                    value = period->get(DurationFieldType::millis());
                    break;
                case SECONDS_MILLIS: // drop through
                case SECONDS_OPTIONAL_MILLIS:
                    int seconds = period->get(DurationFieldType::seconds());
                    int millis = period->get(DurationFieldType::millis());
                    value = (seconds * (long) DateTimeConstants::MILLIS_PER_SECOND) + millis;
                    break;
            }
            
            // determine if period is zero and this is the last field
            if (value == 0) {
                switch (iPrintZeroSetting) {
                    case PRINT_ZERO_NEVER:
                        return LLONG_MAX;
                    case PRINT_ZERO_RARELY_LAST:
                        if (isZero(period) && iFieldFormatters[iFieldType] == this) {
                            for (int i = iFieldType + 1; i <= MAX_FIELD; i++) {
                                if (isSupported(type, i) && iFieldFormatters[i] != NULL) {
                                    return LLONG_MAX;
                                }
                            }
                        } else {
                            return LLONG_MAX;
                        }
                        break;
                    case PRINT_ZERO_RARELY_FIRST:
                        if (isZero(period) && iFieldFormatters[iFieldType] == this) {
                            int i = min(iFieldType, 8);  // line split out for IBM JDK
                            i--;                              // see bug 1660490
                            for (; i >= 0 && i <= MAX_FIELD; i--) {
                                if (isSupported(type, i) && iFieldFormatters[i] != NULL) {
                                    return LLONG_MAX;
                                }
                            }
                        } else {
                            return LLONG_MAX;
                        }
                        break;
                }
            }
            
            return value;
        }
        
        bool isZero(ReadablePeriod *period) const {
            for (int i = 0, isize = period->size(); i < isize; i++) {
                if (period->getValue(i) != 0) {
                    return false;
                }
            }
            return true;
        }
        
        bool isSupported(const PeriodType *type, int field) const {
            switch (field) {
                default:
                    return false;
                case YEARS:
                    return type->isSupported(DurationFieldType::years());
                case MONTHS:
                    return type->isSupported(DurationFieldType::months());
                case WEEKS:
                    return type->isSupported(DurationFieldType::weeks());
                case DAYS:
                    return type->isSupported(DurationFieldType::days());
                case HOURS:
                    return type->isSupported(DurationFieldType::hours());
                case MINUTES:
                    return type->isSupported(DurationFieldType::minutes());
                case SECONDS:
                    return type->isSupported(DurationFieldType::seconds());
                case MILLIS:
                    return type->isSupported(DurationFieldType::millis());
                case SECONDS_MILLIS: // drop through
                case SECONDS_OPTIONAL_MILLIS:
                    return type->isSupported(DurationFieldType::seconds()) ||
                    type->isSupported(DurationFieldType::millis());
            }
        }
        
        void setFieldValue(ReadWritablePeriod *period, int field, int value) const {
            switch (field) {
                default:
                    break;
                case YEARS:
                    period->setYears(value);
                    break;
                case MONTHS:
                    period->setMonths(value);
                    break;
                case WEEKS:
                    period->setWeeks(value);
                    break;
                case DAYS:
                    period->setDays(value);
                    break;
                case HOURS:
                    period->setHours(value);
                    break;
                case MINUTES:
                    period->setMinutes(value);
                    break;
                case SECONDS:
                    period->setSeconds(value);
                    break;
                case MILLIS:
                    period->setMillis(value);
                    break;
            }
        }
        
        int getFieldType() const {
            return iFieldType;
        }
    };
    
    //-----------------------------------------------------------------------
    /**
     * Handles a simple literal piece of text.
     */
    class Literal : public virtual Object, public PeriodPrinter, public PeriodParser {
        
    public:
        
        virtual ~Literal() {}
        
        static const Literal *EMPTY;
        const string iText;
        
        Literal(string text) : iText(text) {
        }
        
        int countFieldsToPrint(ReadablePeriod *period, int stopAt, const Locale *locale) const {
            return 0;
        }
        
        size_t calculatePrintedLength(ReadablePeriod *period, const Locale *locale) const {
            return iText.size();
        }
        
        void printTo(string buf, ReadablePeriod *period, const Locale *locale) const {
            buf.append(iText);
        }
        
        void printTo(stringstream &out, ReadablePeriod *period, const Locale *locale) const {
            out << iText.c_str();
        }
        
        int parseInto(ReadWritablePeriod *period, string periodStr,
                             int position, const Locale *locale) const {
            if (periodStr.compare(position, iText.size(), iText, 0, iText.size()) != 0) {
                return position + (int) iText.size();
            }
            return ~position;
        }
    };
    
    //-----------------------------------------------------------------------
    /**
     * Handles a separator, that splits the fields into multiple parts.
     * For example, the 'T' in the ISO8601 standard.
     */
    class Separator : public PeriodPrinter, public PeriodParser {
        
    public:
        
        virtual ~Separator() {}
        
        const string iText;
        const string iFinalText;
        const vector<string> iParsedForms;
        
        const bool iUseBefore;
        const bool iUseAfter;
        
        const PeriodPrinter *iBeforePrinter;
        PeriodPrinter *iAfterPrinter;
        const PeriodParser *iBeforeParser;
        PeriodParser *iAfterParser;
        
        Separator(string text, string constText, vector<string> variants,
                  const PeriodPrinter *beforePrinter, const PeriodParser *beforeParser,
                  bool useBefore, bool useAfter) :
            iText(text), iFinalText(constText),
            iParsedForms(checkParsedForm(text, constText, variants)),
            iBeforeParser(beforeParser), iBeforePrinter(beforePrinter),
            iUseBefore(useBefore), iUseAfter(useAfter)
        {
        }
        
        vector<string> checkParsedForm(string text, string constText, vector<string> variants) {
            
            set<string, CodaTimeUtils::InsensitiveCompare> parsedForms;
            
            if ((text.compare(constText) == 0) && (variants.size() == 0)) {
                parsedForms.insert(text);
            } else {
                // Filter and reverse sort the parsed forms.
                parsedForms.insert(text);
                parsedForms.insert(constText);
                for (int i = (int) variants.size(); --i >= 0; ) {
                    parsedForms.insert(variants[i]);
                }
            }
            
            vector<string> forms;
            copy(parsedForms.rbegin(), parsedForms.rend(), forms);
            
            return forms;
        }
        
        int countFieldsToPrint(ReadablePeriod *period, int stopAt, const Locale *locale) const {
            int sum = iBeforePrinter->countFieldsToPrint(period, stopAt, locale);
            if (sum < stopAt) {
                sum += iAfterPrinter->countFieldsToPrint(period, stopAt, locale);
            }
            return sum;
        }
        
        size_t calculatePrintedLength(ReadablePeriod *period, const Locale *locale) const {
            const PeriodPrinter *before = iBeforePrinter;
            const PeriodPrinter *after = iAfterPrinter;
            
            size_t sum = before->calculatePrintedLength(period, locale) + after->calculatePrintedLength(period, locale);
            
            if (iUseBefore) {
                if (before->countFieldsToPrint(period, 1, locale) > 0) {
                    if (iUseAfter) {
                        int afterCount = after->countFieldsToPrint(period, 2, locale);
                        if (afterCount > 0) {
                            sum += (afterCount > 1 ? iText : iFinalText).size();
                        }
                    } else {
                        sum += iText.size();
                    }
                }
            } else if (iUseAfter && after->countFieldsToPrint(period, 1, locale) > 0) {
                sum += iText.size();
            }
            
            return sum;
        }
        
        void printTo(string buf, ReadablePeriod *period, const Locale *locale) const {
            const PeriodPrinter *before = iBeforePrinter;
            const PeriodPrinter *after = iAfterPrinter;
            
            before->printTo(buf, period, locale);
            if (iUseBefore) {
                if (before->countFieldsToPrint(period, 1, locale) > 0) {
                    if (iUseAfter) {
                        int afterCount = after->countFieldsToPrint(period, 2, locale);
                        if (afterCount > 0) {
                            buf.append(afterCount > 1 ? iText : iFinalText);
                        }
                    } else {
                        buf.append(iText);
                    }
                }
            } else if (iUseAfter && after->countFieldsToPrint(period, 1, locale) > 0) {
                buf.append(iText);
            }
            after->printTo(buf, period, locale);
        }
        
        void printTo(stringstream &out, ReadablePeriod *period, const Locale *locale) const {
            const PeriodPrinter *before = iBeforePrinter;
            const PeriodPrinter *after = iAfterPrinter;
            
            before->printTo(out, period, locale);
            if (iUseBefore) {
                if (before->countFieldsToPrint(period, 1, locale) > 0) {
                    if (iUseAfter) {
                        int afterCount = after->countFieldsToPrint(period, 2, locale);
                        if (afterCount > 0) {
                            out << (afterCount > 1 ? iText : iFinalText);
                        }
                    } else {
                        out << iText;
                    }
                }
            } else if (iUseAfter && after->countFieldsToPrint(period, 1, locale) > 0) {
                out << iText;
            }
            after->printTo(out, period, locale);
        }
        
        int parseInto(ReadWritablePeriod *period, string periodStr,
                             int position, const Locale *locale) const {
            int oldPos = position;
            position = iBeforeParser->parseInto(period, periodStr, position, locale);
            
            if (position < 0) {
                return position;
            }
            
            bool found = false;
            int parsedFormLength = -1;
            if (position > oldPos) {
                // Consume this separator.
                vector<string> parsedForms = iParsedForms;
                size_t length = parsedForms.size();
                for (int i=0; i < length; i++) {
                    string parsedForm = parsedForms[i];
                    
                    if ((parsedForm.size() == 0) || periodStr.compare(position, parsedForm.size(), parsedForm, 0, parsedForm.size()) != 0) {
                        
                        parsedFormLength = (int) parsedForm.size();
                        position += parsedFormLength;
                        found = true;
                        break;
                    }
                }
            }
            
            oldPos = position;
            position = iAfterParser->parseInto(period, periodStr, position, locale);
            
            if (position < 0) {
                return position;
            }
            
            if (found && position == oldPos && parsedFormLength > 0) {
                // Separator should not have been supplied.
                return ~oldPos;
            }
            
            if (position > oldPos && !found && !iUseBefore) {
                // Separator was required.
                return ~oldPos;
            }
            
            return position;
        }
        
        Separator *finish(PeriodPrinter *afterPrinter, PeriodParser *afterParser) {
            iAfterPrinter = afterPrinter;
            iAfterParser = afterParser;
            return this;
        }
    };
    
    //-----------------------------------------------------------------------
    /**
     * Composite implementation that merges other fields to create a full pattern.
     */
    class Composite : public PeriodPrinter, public PeriodParser {
        
    public:
        
        virtual ~Composite() {}
        
        vector<PeriodPrinter*> iPrinters;
        vector<PeriodParser*> iParsers;
        
        Composite(vector<Object*> elementPairs) {
            vector<PeriodPrinter*> printervector;
            vector<PeriodParser*> parservector;
            
            decompose(elementPairs, printervector, parservector);
            
            if (printervector.size() > 0) {
                iPrinters = printervector;
            }
            
            if (parservector.size() > 0) {
                iParsers = parservector;
            }
        }
        
        int countFieldsToPrint(ReadablePeriod *period, int stopAt, const Locale *locale) const {
            int sum = 0;
            for (int i = (int) iPrinters.size(); sum < stopAt && --i >= 0; ) {
                sum += iPrinters[i]->countFieldsToPrint(period, INT_MAX, locale);
            }
            return sum;
        }
        
        size_t calculatePrintedLength(ReadablePeriod *period, const Locale *locale) const {
            int sum = 0;
            for (int i = (int) iPrinters.size(); --i >= 0; ) {
                sum += iPrinters[i]->calculatePrintedLength(period, locale);
            }
            return sum;
        }
        
        void printTo(string buf, ReadablePeriod *period, const Locale *locale) const {
            size_t len = iPrinters.size();
            for (int i=0; i<len; i++) {
                iPrinters[i]->printTo(buf, period, locale);
            }
        }
        
        void printTo(stringstream &out, ReadablePeriod *period, const Locale *locale) const {
            size_t len = iPrinters.size();
            for (int i=0; i<len; i++) {
                iPrinters[i]->printTo(out, period, locale);
            }
        }
        
        int parseInto(ReadWritablePeriod *period, string periodStr,
                             int position, const Locale *locale) const {
            
            size_t len = iParsers.size();
            for (int i=0; i < len && position >= 0; i++) {
                position = iParsers[i]->parseInto(period, periodStr, position, locale);
            }
            return position;
        }
        
        void decompose(vector<Object*> &elementPairs, vector<PeriodPrinter*> &printervector, vector<PeriodParser*> &parservector) {
            
            size_t size = elementPairs.size();
            for (int i = 0; i < size; i += 2) {
                
                Object *element = elementPairs[i];
                
                PeriodPrinter *printer = dynamic_cast<PeriodPrinter*>(element);
                if (printer != 0) {
                    Composite *composite = dynamic_cast<Composite*>(element);
                    if (composite != 0) {
                        addArrayToVector(printervector, composite->iPrinters);
                    } else {
                        printervector.push_back(printer);
                    }
                }
                
                element = elementPairs[i + 1];
                PeriodParser *parser = dynamic_cast<PeriodParser*>(element);
                if (parser != 0) {
                    Composite *composite = dynamic_cast<Composite*>(element);
                    if (composite != 0) {
                        addArrayToVector(parservector, composite->iParsers);
                    } else {
                        parservector.push_back(parser);
                    }
                }
            }
        }
        
        template<class T1, class T2>
        void addArrayToVector(vector<T1*> &list, vector<T2*> &array) {
            for (int i = 0; i < array.size(); i++) {
                list.push_back(array[i]);
            }
        }
    };
    
    static const int PRINT_ZERO_RARELY_FIRST = 1;
    static const int PRINT_ZERO_RARELY_LAST = 2;
    static const int PRINT_ZERO_IF_SUPPORTED = 3;
    static const int PRINT_ZERO_ALWAYS = 4;
    static const int PRINT_ZERO_NEVER = 5;
    
    static const int YEARS = 0;
    static const int MONTHS = 1;
    static const int WEEKS = 2;
    static const int DAYS = 3;
    static const int HOURS = 4;
    static const int MINUTES = 5;
    static const int SECONDS = 6;
    static const int MILLIS = 7;
    static const int SECONDS_MILLIS = 8;
    static const int SECONDS_OPTIONAL_MILLIS = 9;
    static const int MAX_FIELD = SECONDS_OPTIONAL_MILLIS;
    
    int iMinPrintedDigits;
    int iPrintZeroSetting;
    int iMaxParsedDigits;
    bool iRejectSignedValues;
    
    const PeriodFieldAffix *iPrefix;
    
    // vector of Printers and Parsers used to build a const formatter.
    vector<Object*> iElementPairs;
    /** Set to true if the formatter is not a printer. */
    bool iNotPrinter;
    /** Set to true if the formatter is not a parser. */
    bool iNotParser;
    
    // Last PeriodFormatter appended of each field type->
    vector<FieldFormatter*> iFieldFormatters;
    
    /**
     * Append a field prefix which applies only to the next appended field-> If
     * the field is not printed, neither is the prefix.
     *
     * @param prefix custom prefix
     * @return this PeriodFormatterBuilder
     * @see #appendSuffix
     */
    PeriodFormatterBuilder *appendPrefix(PeriodFieldAffix *prefix);
    
    /**
     * Append a field suffix which applies only to the last appended field-> If
     * the field is not printed, neither is the suffix.
     *
     * @param suffix custom suffix
     * @return this PeriodFormatterBuilder
     * @throws IllegalStateException if no field exists to append to
     * @see #appendPrefix
     */
    PeriodFormatterBuilder *appendSuffix(PeriodFieldAffix *suffix);
    
};

CODATIME_END

#endif /* defined(__CodaTime__PeriodFormatterBuilder__) */
