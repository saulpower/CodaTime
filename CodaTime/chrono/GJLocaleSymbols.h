//
//  GJLocaleSymbols.h
//  CodaTime
//
//  Created by Saul Howard on 12/17/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_GJLocaleSymbols_h
#define CodaTime_GJLocaleSymbols_h

#include "CodaTimeMacros.h"

#include "CodaTimeUtils.h"
#include "DateTimeFieldType.h"
#include "Exceptions.h"
#include "Locale.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

CODATIME_BEGIN

/**
 * Utility class used by a few of the GJDateTimeFields.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class GJLocaleSymbols {
    
private:
    
    static const int FAST_CACHE_SIZE = 64;
    
    static vector<GJLocaleSymbols*> cFastCache;
    
    static map<Locale*, GJLocaleSymbols*> cCache;
    
    static void addSymbols(map<string, int, CodaTimeUtils::InsensitiveCompare> &map, vector<string> symbols, vector<int> integers) {
        for (int i = (int) symbols.size(); --i >= 0; ) {
            string symbol = symbols[i];
            if (!symbol.empty()) {
                map.insert(pair<string, int>(symbol, integers[i]));
            }
        }
    }
    
    static void addNumerals(map<string, int, CodaTimeUtils::InsensitiveCompare> &map, int start, int end, vector<int> integers) {
        for (int i=start; i<=end; i++) {
            map.insert(pair<string, int>(to_string(i), integers[i]));
        }
    }
    
    static int maxLength(vector<string> a) {
        size_t max = 0;
        for (int i = (int) a.size(); --i >= 0; ) {
            string s = a[i];
            if (!s.empty()) {
                size_t len = s.size();
                if (len > max) {
                    max = len;
                }
            }
        }
        return (int) max;
    }
    
    Locale *iLocale;
    
    vector<string> iEras;
    vector<string> iDaysOfWeek;
    vector<string> iShortDaysOfWeek;
    vector<string> iMonths;
    vector<string> iShortMonths;
    vector<string> iHalfday;
    
    map<string, int, CodaTimeUtils::InsensitiveCompare> iParseEras;
    map<string, int, CodaTimeUtils::InsensitiveCompare> iParseDaysOfWeek;
    map<string, int, CodaTimeUtils::InsensitiveCompare> iParseMonths;
    
    int iMaxEraLength;
    int iMaxDayOfWeekLength;
    int iMaxShortDayOfWeekLength;
    int iMaxMonthLength;
    int iMaxShortMonthLength;
    int iMaxHalfdayLength;
    
    /**
     * @param locale must not be NULL
     */
    GJLocaleSymbols(Locale *locale) {
        
        iLocale = locale;
        
        iEras = {"BC", "AD"};
        iDaysOfWeek = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
        iShortDaysOfWeek = {"", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
        iMonths = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
        iShortMonths = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        iHalfday = {"AM", "PM"};
        
        vector<int> integers = vector<int>(13);
        for (int i=0; i<13; i++) {
            integers[i] = i;
        }
        
        addSymbols(iParseEras, iEras, integers);
        if (string("en").compare(locale->getLanguage()) == 0) {
            // Include support for parsing "BCE" and "CE" if the language is
            // English. At some point Joda-Time will need an independent set of
            // localized symbols and not depend on java.text.DateFormatSymbols.
            iParseEras.insert(pair<string, int>("BCE", integers[0]));
            iParseEras.insert(pair<string, int>("CE", integers[1]));
        }
        
        addSymbols(iParseDaysOfWeek, iDaysOfWeek, integers);
        addSymbols(iParseDaysOfWeek, iShortDaysOfWeek, integers);
        addNumerals(iParseDaysOfWeek, 1, 7, integers);
        
        addSymbols(iParseMonths, iMonths, integers);
        addSymbols(iParseMonths, iShortMonths, integers);
        addNumerals(iParseMonths, 1, 12, integers);
        
        iMaxEraLength = maxLength(iEras);
        iMaxDayOfWeekLength = maxLength(iDaysOfWeek);
        iMaxShortDayOfWeekLength = maxLength(iShortDaysOfWeek);
        iMaxMonthLength = maxLength(iMonths);
        iMaxShortMonthLength = maxLength(iShortMonths);
        iMaxHalfdayLength = maxLength(iHalfday);
    }
    
public:
    
    static GJLocaleSymbols *forLocale(Locale *locale) {
        if (locale == NULL) {
            locale = Locale::getDefault();
        }
        int index = 0; // TODO: Fix this -> System.identityHashCode(locale) & (FAST_CACHE_SIZE - 1);
        GJLocaleSymbols *symbols = cFastCache[index];
        if (symbols != NULL && symbols->iLocale == locale) {
            return symbols;
        }
        // TODO: synchronize
//        synchronized (cCache) {
            symbols = cCache[locale];
            if (symbols == NULL) {
                symbols = new GJLocaleSymbols(locale);
                cCache[locale] = symbols;
            }
//        }
        cFastCache[index] = symbols;
        return symbols;
    }
    
    string eraValueToText(int value) {
        return iEras[value];
    }
    
    int eraTextToValue(string text) {
        return iParseEras[text];
    }
    
    int getEraMaxTextLength() {
        return iMaxEraLength;
    }
    
    string monthOfYearValueToText(int value) {
        return iMonths[value];
    }
    
    string monthOfYearValueToShortText(int value) {
        return iShortMonths[value];
    }
    
    int monthOfYearTextToValue(string text) {
        return iParseMonths[text];
    }
    
    int getMonthMaxTextLength() {
        return iMaxMonthLength;
    }
    
    int getMonthMaxShortTextLength() {
        return iMaxShortMonthLength;
    }
    
    string dayOfWeekValueToText(int value) {
        return iDaysOfWeek[value];
    }
    
    string dayOfWeekValueToShortText(int value) {
        return iShortDaysOfWeek[value];
    }
    
    int dayOfWeekTextToValue(string text) {
        return iParseDaysOfWeek[text];
    }
    
    int getDayOfWeekMaxTextLength() {
        return iMaxDayOfWeekLength;
    }
    
    int getDayOfWeekMaxShortTextLength() {
        return iMaxShortDayOfWeekLength;
    }
    
    string halfdayValueToText(int value) {
        return iHalfday[value];
    }
    
    int halfdayTextToValue(string text) {
        vector<string> &halfday = iHalfday;
        for (int i = (int) halfday.size(); --i >= 0; ) {
            if (halfday[i].compare(text) == 0) {
                return i;
            }
        }
        throw IllegalFieldValueException(DateTimeFieldType::halfdayOfDay(), text);
    }
    
    int getHalfdayMaxTextLength() {
        return iMaxHalfdayLength;
    }
};

CODATIME_END

#endif
