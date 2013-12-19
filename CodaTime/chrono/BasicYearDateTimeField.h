//
//  BasicYearDateTimeField.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BasicYearDateTimeField_h
#define CodaTime_BasicYearDateTimeField_h

#include "CodaTimeMacros.h"

#include "DateTimeFieldType.h"
#include "DurationField.h"
#include "field/FieldUtils.h"
#include "field/ImpreciseDateTimeField.h"

CODATIME_BEGIN

class BasicChronology;

/**
 * A year field suitable for many calendars.
 *
 * @author Guy Allard
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.1, refactored from GJYearDateTimeField
 */
class BasicYearDateTimeField : public ImpreciseDateTimeField {
    
protected:
    
    /** The underlying basic chronology. */
    BasicChronology *iChronology;
    
public:
    
    /**
     * Restricted constructor.
     *
     * @param chronology  the chronology this field belogs to
     */
    BasicYearDateTimeField(BasicChronology *chronology);
    
    bool isLenient() const { return false; }
    
    int get(int64_t instant) const;
    
    int64_t add(int64_t instant, int years) const {
        if (years == 0) {
            return instant;
        }
        int thisYear = get(instant);
        int newYear = FieldUtils::safeAdd(thisYear, years);
        return set(instant, newYear);
    }
    
    int64_t add(int64_t instant, int64_t years) const {
        return add(instant, FieldUtils::safeToInt(years));
    }
    
    int64_t addWrapField(int64_t instant, int years) const;
    
    int64_t set(int64_t instant, int year) const;
    
    int64_t getDifferenceAsLong(int64_t minuendInstant, int64_t subtrahendInstant) const;
    
    const DurationField *getRangeDurationField() const;
    
    bool isLeap(int64_t instant) const;
    
    int getLeapAmount(int64_t instant) const;
    const DurationField *getLeapDurationField() const;
    int getMinimumValue() const;
    int getMaximumValue() const;
    int64_t roundFloor(int64_t instant) const;
    int64_t roundCeiling(int64_t instant) const;
    int64_t remainder(int64_t instant) const;
    
private:
    
    /**
     * Serialization singleton
     */
    const Object *readResolve();
};

CODATIME_END

#endif
