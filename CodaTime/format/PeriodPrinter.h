//
//  PeriodPrinter.h
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_PeriodPrinter_h
#define CodaTime_PeriodPrinter_h

#include "CodaTimeMacros.h"

#include "Object.h"

#include <string>
#include <sstream>

using namespace std;

CODATIME_BEGIN

class ReadablePeriod;
class Locale;

/**
 * Internal interface for printing textual representations of time periods.
 * <p>
 * Application users will rarely use this class directly. Instead, you
 * will use one of the factory classes to create a {@link PeriodFormatter}.
 * <p>
 * The factory classes are:<br />
 * - {@link PeriodFormatterBuilder}<br />
 * - {@link PeriodFormat}<br />
 * - {@link ISOPeriodFormat}<br />
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 * @see PeriodFormatter
 * @see PeriodFormatterBuilder
 * @see PeriodFormat
 */
class PeriodPrinter : public virtual Object {
    
public:
    
    /**
     * Returns the exact number of characters produced for the given period.
     *
     * @param period  the period to use
     * @param locale  the locale to use
     * @return the estimated length
     */
    virtual size_t calculatePrintedLength(ReadablePeriod *period, const Locale *locale) const = 0;
    
    /**
     * Returns the amount of fields from the given period that this printer
     * will print.
     *
     * @param period  the period to use
     * @param stopAt stop counting at this value, enter a number &ge; 256 to count all
     * @param locale  the locale to use
     * @return amount of fields printed
     */
    virtual int countFieldsToPrint(ReadablePeriod *period, int stopAt, const Locale *locale) const = 0;
    
    //-----------------------------------------------------------------------
    /**
     * Prints a ReadablePeriod to a StringBuffer.
     *
     * @param buf  the formatted period is appended to this buffer
     * @param period  the period to format
     * @param locale  the locale to use
     */
    virtual void printTo(string buf, ReadablePeriod *period, const Locale *locale) const = 0;
    
    /**
     * Prints a ReadablePeriod to a Writer.
     *
     * @param out  the formatted period is written out
     * @param period  the period to format
     * @param locale  the locale to use
     */
    virtual void printTo(stringstream &out, ReadablePeriod *period, const Locale *locale) const = 0;
    
};

CODATIME_END

#endif
