//
//  PeriodFormatterBuilder.cpp
//  CodaTime
//
//  Created by Saul Howard on 12/18/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#include "PeriodFormatterBuilder.h"

#include "PeriodFormatter.h"

#include "Exceptions.h"

CODATIME_BEGIN


const PeriodFormatterBuilder::Literal *PeriodFormatterBuilder::Literal::EMPTY = new PeriodFormatterBuilder::Literal("");

PeriodFormatter *PeriodFormatterBuilder::toFormatter() {
    PeriodFormatter *formatter = toFormatter(iElementPairs, iNotPrinter, iNotParser);
//    iFieldFormatters = (vector<FieldFormatter>) iFieldFormatters.clone();
    return formatter;
}

const PeriodPrinter *PeriodFormatterBuilder::toPrinter() {
    if (iNotPrinter) {
        return NULL;
    }
    return toFormatter()->getPrinter();
}

const PeriodParser *PeriodFormatterBuilder::toParser() {
    if (iNotParser) {
        return NULL;
    }
    return toFormatter()->getParser();
}

//-----------------------------------------------------------------------
void PeriodFormatterBuilder::clear() {
    iMinPrintedDigits = 1;
    iPrintZeroSetting = PRINT_ZERO_RARELY_LAST;
    iMaxParsedDigits = 10;
    iRejectSignedValues = false;
    iPrefix = NULL;
    iElementPairs.clear();
    iNotPrinter = false;
    iNotParser = false;
    iFieldFormatters.clear();
}

PeriodFormatterBuilder *PeriodFormatterBuilder::append(PeriodFormatter *formatter) {
    if (formatter == NULL) {
        throw IllegalArgumentException("No formatter supplied");
    }
    clearPrefix();
    append0(formatter->getPrinter(), formatter->getParser());
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::append(PeriodPrinter *printer, PeriodParser *parser) {
    if (printer == NULL && parser == NULL) {
        throw IllegalArgumentException("No printer or parser supplied");
    }
    clearPrefix();
    append0(printer, parser);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendLiteral(string text) {
    clearPrefix();
    Literal *literal = new Literal(text);
    append0(literal, literal);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::minimumPrintedDigits(int minDigits) {
    iMinPrintedDigits = minDigits;
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::maximumParsedDigits(int maxDigits) {
    iMaxParsedDigits = maxDigits;
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::rejectSignedValues(bool v) {
    iRejectSignedValues = v;
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::printZeroRarelyLast() {
    iPrintZeroSetting = PRINT_ZERO_RARELY_LAST;
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::printZeroRarelyFirst() {
    iPrintZeroSetting = PRINT_ZERO_RARELY_FIRST;
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::printZeroIfSupported() {
    iPrintZeroSetting = PRINT_ZERO_IF_SUPPORTED;
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::printZeroAlways() {
    iPrintZeroSetting = PRINT_ZERO_ALWAYS;
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::printZeroNever() {
    iPrintZeroSetting = PRINT_ZERO_NEVER;
    return this;
}

//-----------------------------------------------------------------------
PeriodFormatterBuilder *PeriodFormatterBuilder::appendPrefix(string text) {
    return appendPrefix(new SimpleAffix(text));
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendPrefix(string singularText, string pluralText) {
    return appendPrefix(new PluralAffix(singularText, pluralText));
}

//-----------------------------------------------------------------------
PeriodFormatterBuilder *PeriodFormatterBuilder::appendYears() {
    appendField(YEARS);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendMonths() {
    appendField(MONTHS);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendWeeks() {
    appendField(WEEKS);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendDays() {
    appendField(DAYS);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendHours() {
    appendField(HOURS);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendMinutes() {
    appendField(MINUTES);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSeconds() {
    appendField(SECONDS);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSecondsWithMillis() {
    appendField(SECONDS_MILLIS);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSecondsWithOptionalMillis() {
    appendField(SECONDS_OPTIONAL_MILLIS);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendMillis() {
    appendField(MILLIS);
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendMillis3Digit() {
    appendField(7, 3);
    return this;
}

//-----------------------------------------------------------------------
PeriodFormatterBuilder *PeriodFormatterBuilder::appendSuffix(string text) {
    return appendSuffix(new SimpleAffix(text));
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSuffix(string singularText,
                                     string pluralText) {
    return appendSuffix(new PluralAffix(singularText, pluralText));
}

//-----------------------------------------------------------------------
PeriodFormatterBuilder *PeriodFormatterBuilder::appendSeparator(string text) {
    return appendSeparator(text, text, vector<string>(), true, true);
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSeparatorIfFieldsAfter(string text) {
    return appendSeparator(text, text, vector<string>(), false, true);
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSeparatorIfFieldsBefore(string text) {
    return appendSeparator(text, text, vector<string>(), true, false);
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSeparator(string text, string constText) {
    return appendSeparator(text, constText, vector<string>(), true, true);
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSeparator(string text, string constText,
                                        vector<string> variants) {
    return appendSeparator(text, constText, variants, true, true);
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendPrefix(PeriodFieldAffix *prefix) {
    if (prefix == NULL) {
        throw IllegalArgumentException();
    }
    if (iPrefix != NULL) {
        prefix = new CompositeAffix(iPrefix, prefix);
    }
    iPrefix = prefix;
    return this;
}

void PeriodFormatterBuilder::appendField(int type) {
    appendField(type, iMinPrintedDigits);
}

void PeriodFormatterBuilder::appendField(int type, int minPrinted) {
    FieldFormatter *field = new FieldFormatter(minPrinted, iPrintZeroSetting,
                                              iMaxParsedDigits, iRejectSignedValues, type, iFieldFormatters, iPrefix, NULL);
    append0(field, field);
    iFieldFormatters[type] = field;
    iPrefix = NULL;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSuffix(PeriodFieldAffix *suffix) {
    const Object *originalPrinter;
    const Object *originalParser;
    if (iElementPairs.size() > 0) {
        originalPrinter = iElementPairs[iElementPairs.size() - 2];
        originalParser = iElementPairs[iElementPairs.size() - 1];
    } else {
        originalPrinter = NULL;
        originalParser = NULL;
    }
    
    const FieldFormatter *fieldFormatter = dynamic_cast<const FieldFormatter*>(originalPrinter);
    if (originalPrinter == NULL || originalParser == NULL ||
        originalPrinter != originalParser ||
        fieldFormatter == 0) {
        throw IllegalStateException("No field to apply suffix to");
    }
    
    clearPrefix();
    FieldFormatter *newField = new FieldFormatter(fieldFormatter, suffix);
    iElementPairs[iElementPairs.size() - 2] = newField;
    iElementPairs[iElementPairs.size() - 1] = newField;
    iFieldFormatters[newField->getFieldType()] = newField;
    
    return this;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::appendSeparator(string text, string constText,
                                        vector<string> variants,
                                        bool useBefore, bool useAfter) {
    
    clearPrefix();
    
    // optimise zero formatter case
    vector<Object*> &pairs = iElementPairs;
    if (pairs.size() == 0) {
        if (useAfter && useBefore == false) {
            Separator *separator = new Separator(text, constText, variants,
                                                Literal::EMPTY, Literal::EMPTY, useBefore, useAfter);
            append0(separator, separator);
        }
        return this;
    }
    
    // find the last separator added
    int i;
    const Separator *lastSeparator = NULL;
    for (i = (int) pairs.size(); --i >= 0; ) {
        Separator *separator = dynamic_cast<Separator*>(pairs[i]);
        if (separator != 0) {
            lastSeparator = separator;
            vector<Object*>::const_iterator first = pairs.begin() + i + 1;
            vector<Object*>::const_iterator last = pairs.end();
            pairs = vector<Object*>(first, last);
            break;
        }
        i--;  // element pairs
    }
    
    // merge formatters
    if (lastSeparator != NULL && pairs.size() == 0) {
        throw IllegalStateException("Cannot have two adjacent separators");
    } else {
        vector<Object*> comp = createComposite(pairs);
        pairs.clear();
        Separator *separator = new Separator(text, constText, variants,
                                            dynamic_cast<PeriodPrinter*>(comp[0]),
                                            dynamic_cast<PeriodParser*>(comp[1]),
                                            useBefore, useAfter);
        pairs.push_back(separator);
        pairs.push_back(separator);
    }
    
    return this;
}

//-----------------------------------------------------------------------
void PeriodFormatterBuilder::clearPrefix() {
    if (iPrefix != NULL) {
        throw IllegalStateException("Prefix not followed by field");
    }
    iPrefix = NULL;
}

PeriodFormatterBuilder *PeriodFormatterBuilder::append0(PeriodPrinter *printer, PeriodParser *parser) {
    iElementPairs.push_back(printer);
    iElementPairs.push_back(parser);
    iNotPrinter |= (printer == NULL);
    iNotParser |= (parser == NULL);
    return this;
}

//-----------------------------------------------------------------------
PeriodFormatter *PeriodFormatterBuilder::toFormatter(vector<Object*> elementPairs, bool notPrinter, bool notParser) {
    
    if (notPrinter && notParser) {
        throw IllegalStateException("Builder has created neither a printer nor a parser");
    }
    
    size_t size = elementPairs.size();
    Separator *sep = dynamic_cast<Separator*>(elementPairs[0]);
    
    if (size >= 2 && sep != 0) {
        if (sep->iAfterParser == NULL && sep->iAfterPrinter == NULL) {
            
            vector<Object*>::const_iterator first = elementPairs.begin() + 2;
            vector<Object*>::const_iterator last = elementPairs.end();
            vector<Object*> subvector(first, last);
            
            PeriodFormatter *f = toFormatter(subvector, notPrinter, notParser);
            sep = sep->finish(f->getPrinter(), f->getParser());
            return new PeriodFormatter(sep, sep);
        }
    }
    vector<Object*> comp = createComposite(elementPairs);
    if (notPrinter) {
        return new PeriodFormatter(NULL, dynamic_cast<PeriodParser*>(comp[1]));
    } else if (notParser) {
        return new PeriodFormatter(dynamic_cast<PeriodPrinter*>(comp[0]), NULL);
    } else {
        return new PeriodFormatter(dynamic_cast<PeriodPrinter*>(comp[0]), dynamic_cast<PeriodParser*>(comp[1]));
    }
}

vector<Object*> PeriodFormatterBuilder::createComposite(vector<Object*> elementPairs) {
    switch (elementPairs.size()) {
        case 0:
            return {Literal::EMPTY, Literal::EMPTY};
        case 1:
            return {elementPairs[0], elementPairs[1]};
        default:
            Composite *comp = new Composite(elementPairs);
            return {comp, comp};
    }
}

CODATIME_END