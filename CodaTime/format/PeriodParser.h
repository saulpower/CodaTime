//
//  PeriodParser.h
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_PeriodParser_h
#define CodaTime_PeriodParser_h

#include "CodaTimeMacros.h"

#include "Object.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class ReadWritablePeriod;
class Locale;

/**
 * Internal interface for parsing textual representations of time periods.
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
class PeriodParser : public virtual Object {
    
public:
    
    /**
     * Parses a period from the given text, at the given position, saving the
     * result into the fields of the given ReadWritablePeriod. If the parse
     * succeeds, the return value is the new text position. Note that the parse
     * may succeed without fully reading the text.
     * <p>
     * If it fails, the return value is negative, but the period may still be
     * modified. To determine the position where the parse failed, apply the
     * one's complement operator (~) on the return value.
     *
     * @param period  a period that will be modified
     * @param periodStr  text to parse
     * @param position position to start parsing from
     * @param locale  the locale to use for parsing
     * @return new position, if negative, parse failed. Apply complement
     * operator (~) to get position of failure
     * @throws IllegalArgumentException if any field is out of range
     */
    virtual int parseInto(ReadWritablePeriod *period, string periodStr, int position, const Locale *locale) const = 0;
    
};

CODATIME_END

#endif
