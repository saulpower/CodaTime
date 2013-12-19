//
//  ISOPeriodFormat.h
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_ISOPeriodFormat_h
#define CodaTime_ISOPeriodFormat_h

#include "CodaTimeMacros.h"

#include "format/PeriodFormatter.h"
#include "format/PeriodFormatterBuilder.h"

CODATIME_BEGIN

/**
 * Factory that creates instances of PeriodFormatter for the ISO8601 standard.
 * <p>
 * Period formatting is performed by the {@link PeriodFormatter} class.
 * Three classes provide factory methods to create formatters, and this is one.
 * The others are {@link PeriodFormat} and {@link PeriodFormatterBuilder}.
 * <p>
 * ISOPeriodFormat is thread-safe and immutable, and the formatters it
 * returns are as well.
 *
 * @author Brian S O'Neill
 * @since 1.0
 * @see PeriodFormat
 * @see PeriodFormatterBuilder
 */
class ISOPeriodFormat {
    
private:
    
    /** Cache of standard format. */
    static PeriodFormatter *cStandard;
    /** Cache of alternate months format. */
    static PeriodFormatter *cAlternate;
    /** Cache of alternate extended months format. */
    static PeriodFormatter *cAlternateExtended;
    /** Cache of alternate weeks format. */
    static PeriodFormatter *cAlternateWithWeeks;
    /** Cache of alternate extended weeks format. */
    static PeriodFormatter *cAlternateExtendedWihWeeks;
    
protected:
    
    /**
     * Constructor.
     *
     * @since 1.1 (previously private)
     */
    ISOPeriodFormat() {
    }
    
public:
    
    //-----------------------------------------------------------------------
    /**
     * The standard ISO format - PyYmMwWdDThHmMsS.
     * Milliseconds are not output.
     * Note that the ISO8601 standard actually indicates weeks should not
     * be shown if any other field is present and vice versa.
     *
     * @return the formatter
     */
    static PeriodFormatter *standard() {
        if (cStandard == NULL) {
            cStandard = (new PeriodFormatterBuilder())
            ->appendLiteral("P")
            ->appendYears()
            ->appendSuffix("Y")
            ->appendMonths()
            ->appendSuffix("M")
            ->appendWeeks()
            ->appendSuffix("W")
            ->appendDays()
            ->appendSuffix("D")
            ->appendSeparatorIfFieldsAfter("T")
            ->appendHours()
            ->appendSuffix("H")
            ->appendMinutes()
            ->appendSuffix("M")
            ->appendSecondsWithOptionalMillis()
            ->appendSuffix("S")
            ->toFormatter();
        }
        return cStandard;
    }
    
    /**
     * The alternate ISO format, PyyyymmddThhmmss, which excludes weeks.
     * <p>
     * Even if weeks are present in the period, they are not output.
     * Fractional seconds (milliseconds) will appear if required.
     *
     * @return the formatter
     */
    static PeriodFormatter *alternate() {
        if (cAlternate == NULL) {
            cAlternate = (new PeriodFormatterBuilder())
            ->appendLiteral("P")
            ->printZeroAlways()
            ->minimumPrintedDigits(4)
            ->appendYears()
            ->minimumPrintedDigits(2)
            ->appendMonths()
            ->appendDays()
            ->appendSeparatorIfFieldsAfter("T")
            ->appendHours()
            ->appendMinutes()
            ->appendSecondsWithOptionalMillis()
            ->toFormatter();
        }
        return cAlternate;
    }
    
    /**
     * The alternate ISO format, Pyyyy-mm-ddThh:mm:ss, which excludes weeks->
     * <p>
     * Even if weeks are present in the period, they are not output->
     * Fractional seconds (milliseconds) will appear if required.
     *
     * @return the formatter
     */
    static PeriodFormatter *alternateExtended() {
        if (cAlternateExtended == NULL) {
            cAlternateExtended = (new PeriodFormatterBuilder())
            ->appendLiteral("P")
            ->printZeroAlways()
            ->minimumPrintedDigits(4)
            ->appendYears()
            ->appendSeparator("-")
            ->minimumPrintedDigits(2)
            ->appendMonths()
            ->appendSeparator("-")
            ->appendDays()
            ->appendSeparatorIfFieldsAfter("T")
            ->appendHours()
            ->appendSeparator(":")
            ->appendMinutes()
            ->appendSeparator(":")
            ->appendSecondsWithOptionalMillis()
            ->toFormatter();
        }
        return cAlternateExtended;
    }
    
    /**
     * The alternate ISO format, PyyyyWwwddThhmmss, which excludes months.
     * <p>
     * Even if months are present in the period, they are not output.
     * Fractional seconds (milliseconds) will appear if required.
     *
     * @return the formatter
     */
    static PeriodFormatter *alternateWithWeeks() {
        if (cAlternateWithWeeks == NULL) {
            cAlternateWithWeeks = (new PeriodFormatterBuilder())
            ->appendLiteral("P")
            ->printZeroAlways()
            ->minimumPrintedDigits(4)
            ->appendYears()
            ->minimumPrintedDigits(2)
            ->appendPrefix("W")
            ->appendWeeks()
            ->appendDays()
            ->appendSeparatorIfFieldsAfter("T")
            ->appendHours()
            ->appendMinutes()
            ->appendSecondsWithOptionalMillis()
            ->toFormatter();
        }
        return cAlternateWithWeeks;
    }
    
    /**
     * The alternate ISO format, Pyyyy-Www-ddThh:mm:ss, which excludes months.
     * <p>
     * Even if months are present in the period, they are not output.
     * Fractional seconds (milliseconds) will appear if required.
     *
     * @return the formatter
     */
    static PeriodFormatter *alternateExtendedWithWeeks() {
        if (cAlternateExtendedWihWeeks == NULL) {
            cAlternateExtendedWihWeeks = (new PeriodFormatterBuilder())
            ->appendLiteral("P")
            ->printZeroAlways()
            ->minimumPrintedDigits(4)
            ->appendYears()
            ->appendSeparator("-")
            ->minimumPrintedDigits(2)
            ->appendPrefix("W")
            ->appendWeeks()
            ->appendSeparator("-")
            ->appendDays()
            ->appendSeparatorIfFieldsAfter("T")
            ->appendHours()
            ->appendSeparator(":")
            ->appendMinutes()
            ->appendSeparator(":")
            ->appendSecondsWithOptionalMillis()
            ->toFormatter();
        }
        return cAlternateExtendedWihWeeks;
    }
    
};

CODATIME_END

#endif
