//
//  BaseLocal.h
//  CodaTime
//
//  Created by Saul Howard on 12/13/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_BaseLocal_h
#define CodaTime_BaseLocal_h

#include "CodaTimeMacros.h"

#include "AbstractPartial.h"

CODATIME_BEGIN

/**
 * BaseLocal is an abstract implementation of ReadablePartial that
 * use a local milliseconds internal representation.
 * <p>
 * This class should generally not be used directly by API users.
 * The {@link org.joda.time.ReadablePartial} interface should be used when different
 * kinds of partial objects are to be referenced.
 * <p>
 * BasePartial subclasses may be mutable and not thread-safe.
 *
 * @author Stephen Colebourne
 * @since 1.5
 */
class BaseLocal : public AbstractPartial {
    
    friend class BasePeriod;
    
private:
    
    /** Serialization version */
    static const long long serialVersionUID = 276453175381783L;
    
protected:
    
    //-----------------------------------------------------------------------
    /**
     * Constructs a partial with the current time, using ISOChronology in
     * the default zone to extract the fields.
     * <p>
     * The constructor uses the default time zone, resulting in the local time
     * being initialised. Once the constructor is complete, all further calculations
     * are performed without reference to a timezone (by switching to UTC).
     */
    BaseLocal() : AbstractPartial() {
    }
    
    //-----------------------------------------------------------------------
    /**
     * Gets the local milliseconds from the Java epoch
     * of 1970-01-01T00:00:00 (not fixed to any specific time zone).
     * <p>
     * This method is useful in certain circustances for high performance
     * access to the datetime fields.
     *
     * @return the number of milliseconds since 1970-01-01T00:00:00
     */
    virtual int64_t getLocalMillis() const = 0;
};

CODATIME_END

#endif
