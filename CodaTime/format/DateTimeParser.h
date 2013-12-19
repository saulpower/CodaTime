//
//  DateTimeParser.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_DateTimeParser_h
#define CodaTime_DateTimeParser_h

#include "CodaTimeMacros.h"

#include <string>

using namespace std;

CODATIME_BEGIN

class DateTimeParserBucket;

/**
 * Internal interface for parsing textual representations of datetimes.
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
 * @see DateTimeFormatter
 * @see DateTimeFormatterBuilder
 * @see DateTimeFormat
 * @since 1.0
 */
class DateTimeParser {
    
public:
    
    /**
     * Returns the expected maximum number of characters consumed.
     * The actual amount should rarely exceed this estimate.
     *
     * @return the estimated length
     */
    virtual int estimateParsedLength() = 0;
    
    /**
     * Parse an element from the given text, saving any fields into the given
     * DateTimeParserBucket. If the parse succeeds, the return value is the new
     * text position. Note that the parse may succeed without fully reading the
     * text.
     * <p>
     * If it fails, the return value is negative. To determine the position
     * where the parse failed, apply the one's complement operator (~) on the
     * return value.
     *
     * @param bucket  field are saved into this, not null
     * @param text  the text to parse, not null
     * @param position  position to start parsing from
     * @return new position, negative value means parse failed -
     *  apply complement operator (~) to get position of failure
     * @throws IllegalArgumentException if any field is out of range
     */
    virtual int parseInto(DateTimeParserBucket *bucket, string text, int position) = 0;
    
};

CODATIME_END

#endif
