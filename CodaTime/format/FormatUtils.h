//
//  FormatUtils.h
//  CodaTime
//
//  Created by Saul Howard on 12/16/13.
//  Copyright (c) 2013 Saul Howard. All rights reserved.
//

#ifndef CodaTime_FormatUtils_h
#define CodaTime_FormatUtils_h

#include "CodaTimeMacros.h"

#include <string>
#include <math.h>
#include <sstream>

using namespace std;

CODATIME_BEGIN

/**
 * Utility methods used by formatters.
 * <p>
 * FormatUtils is thread-safe and immutable.
 *
 * @author Brian S O'Neill
 * @since 1.0
 */
class FormatUtils {
    
    friend class PeriodFormatter;
    friend class DateTimeFormatter;
    
private:
    
    static const double LOG_10;
    
    /**
     * Restricted constructor.
     */
    FormatUtils() {
    }
    
    static int parseTwoDigits(string text, int position) {
        int value = text.at(position) - '0';
        return ((value << 3) + (value << 1)) + text.at(position + 1) - '0';
    }
    
    static string createErrorMessage(const string text, const int errorPos) {
        int sampleLen = errorPos + 32;
        string sampleText;
        if (text.size() <= sampleLen + 3) {
            sampleText = text;
        } else {
            sampleText = text.substr(0, sampleLen);
            sampleText.append("...");
        }
        
        if (errorPos <= 0) {
            return "Invalid format: \"" + sampleText + '"';
        }
        
        if (errorPos >= text.length()) {
            return "Invalid format: \"" + sampleText + "\" is too short";
        }
        
        return "Invalid format: \"" + sampleText + "\" is malformed at \"" +
        sampleText.substr(errorPos) + "\"";
    }
    
public:
    
    /**
     * Converts an integer to a string, prepended with a variable amount of '0'
     * pad characters, and appends it to the given buffer.
     *
     * <p>This method is optimized for converting small values to strings.
     *
     * @param buf receives integer converted to a string
     * @param value value to convert to a string
     * @param size minumum amount of digits to append
     */
    static void appendPaddedInteger(string &buf, int value, int size) {
        if (value < 0) {
            buf.append("-");
            if (value != INT_MIN) {
                value = -value;
            } else {
                for (; size > 10; size--) {
                    buf.append("0");
                }
                buf.append(to_string(-(int64_t) INT_MIN));
                return;
            }
        }
        if (value < 10) {
            for (; size > 1; size--) {
                buf.append("0");
            }
            buf.append(1, (char)(value + '0'));
        } else if (value < 100) {
            for (; size > 2; size--) {
                buf.append("0");
            }
            // Calculate value div/mod by 10 without using two expensive
            // division operations. (2 ^ 27) / 10 = 13421772. Add one to
            // value to correct rounding error.
            int d = ((value + 1) * 13421772) >> 27;
            buf.append(1, (char) (d + '0'));
            // Append remainder by calculating (value - d * 10).
            buf.append(1, (char) (value - (d << 3) - (d << 1) + '0'));
        } else {
            int digits;
            if (value < 1000) {
                digits = 3;
            } else if (value < 10000) {
                digits = 4;
            } else {
                digits = (int)(log(value) / LOG_10) + 1;
            }
            for (; size > digits; size--) {
                buf.append("0");
            }
            buf.append(to_string(value));
        }
    }
    
    /**
     * Converts an integer to a string, prepended with a variable amount of '0'
     * pad characters, and appends it to the given buffer.
     *
     * <p>This method is optimized for converting small values to strings.
     *
     * @param buf receives integer converted to a string
     * @param value value to convert to a string
     * @param size minumum amount of digits to append
     */
    static void appendPaddedInteger(string &buf, int64_t value, int size) {
        int intValue = (int) value;
        if (intValue == value) {
            appendPaddedInteger(buf, intValue, size);
        } else if (size <= 19) {
            buf.append(to_string(value));
        } else {
            if (value < 0) {
                buf.append(1, '-');
                if (value != LONG_MIN) {
                    value = -value;
                } else {
                    for (; size > 19; size--) {
                        buf.append(1, '0');
                    }
                    buf.append("9223372036854775808");
                    return;
                }
            }
            int digits = (int)(log(value) / LOG_10) + 1;
            for (; size > digits; size--) {
                buf.append(1, '0');
            }
            buf.append(to_string(value));
        }
    }
    
    /**
     * Converts an integer to a string, prepended with a variable amount of '0'
     * pad characters, and writes it to the given writer.
     *
     * <p>This method is optimized for converting small values to strings.
     *
     * @param out receives integer converted to a string
     * @param value value to convert to a string
     * @param size minumum amount of digits to append
     */
    static void writePaddedInteger(stringstream &ss, int value, int size) {
        if (value < 0) {
            ss << '-';
            if (value != INT_MIN) {
                value = -value;
            } else {
                for (; size > 10; size--) {
                    ss << '0';
                }
                ss << to_string(-(int64_t)INT_MIN);
                return;
            }
        }
        if (value < 10) {
            for (; size > 1; size--) {
                ss << '0';
            }
            ss << (value + '0');
        } else if (value < 100) {
            for (; size > 2; size--) {
               ss << '0';
            }
            // Calculate value div/mod by 10 without using two expensive
            // division operations. (2 ^ 27) / 10 = 13421772. Add one to
            // value to correct rounding error.
            int d = ((value + 1) * 13421772) >> 27;
            ss << (d + '0');
            // Append remainder by calculating (value - d * 10).
            ss << (value - (d << 3) - (d << 1) + '0');
        } else {
            int digits;
            if (value < 1000) {
                digits = 3;
            } else if (value < 10000) {
                digits = 4;
            } else {
                digits = (int)(log(value) / LOG_10) + 1;
            }
            for (; size > digits; size--) {
                ss << '0';
            }
            ss << (to_string(value));
        }
    }
    
    /**
     * Converts an integer to a string, prepended with a variable amount of '0'
     * pad characters, and writes it to the given writer.
     *
     * <p>This method is optimized for converting small values to strings.
     *
     * @param out receives integer converted to a string
     * @param value value to convert to a string
     * @param size minumum amount of digits to append
     */
    static void writePaddedInteger(stringstream &out, int64_t value, int size) {
        int intValue = (int)value;
        if (intValue == value) {
            writePaddedInteger(out, intValue, size);
        } else if (size <= 19) {
            out << (to_string(value));
        } else {
            if (value < 0) {
                out << ('-');
                if (value != LONG_MIN) {
                    value = -value;
                } else {
                    for (; size > 19; size--) {
                        out << ('0');
                    }
                    out << ("9223372036854775808");
                    return;
                }
            }
            int digits = (int)(log(value) / LOG_10) + 1;
            for (; size > digits; size--) {
                out << ('0');
            }
            out << (to_string(value));
        }
    }
    
    /**
     * Converts an integer to a string, and appends it to the given buffer.
     *
     * <p>This method is optimized for converting small values to strings.
     *
     * @param buf receives integer converted to a string
     * @param value value to convert to a string
     */
    static void appendUnpaddedInteger(string &buf, int value) {
        if (value < 0) {
            buf.append(1, '-');
            if (value != INT_MIN) {
                value = -value;
            } else {
                buf.append(to_string(-(int64_t)LONG_MIN));
                return;
            }
        }
        if (value < 10) {
            buf.append(1, (char)(value + '0'));
        } else if (value < 100) {
            // Calculate value div/mod by 10 without using two expensive
            // division operations. (2 ^ 27) / 10 = 13421772. Add one to
            // value to correct rounding error.
            int d = ((value + 1) * 13421772) >> 27;
            buf.append(1, (char) (d + '0'));
            // Append remainder by calculating (value - d * 10).
            buf.append(1, (char) (value - (d << 3) - (d << 1) + '0'));
        } else {
            buf.append(to_string(value));
        }
    }
    
    /**
     * Converts an integer to a string, and appends it to the given buffer.
     *
     * <p>This method is optimized for converting small values to strings.
     *
     * @param buf receives integer converted to a string
     * @param value value to convert to a string
     */
    static void appendUnpaddedInteger(string &buf, int64_t value) {
        int intValue = (int)value;
        if (intValue == value) {
            appendUnpaddedInteger(buf, intValue);
        } else {
            buf.append(to_string(value));
        }
    }
    
    /**
     * Converts an integer to a string, and writes it to the given writer.
     *
     * <p>This method is optimized for converting small values to strings.
     *
     * @param out receives integer converted to a string
     * @param value value to convert to a string
     */
    static void writeUnpaddedInteger(stringstream &out, int value) {
        if (value < 0) {
            out << ('-');
            if (value != INT_MIN) {
                value = -value;
            } else {
                out << (to_string(-(int64_t)INT_MIN));
                return;
            }
        }
        if (value < 10) {
            out << (value + '0');
        } else if (value < 100) {
            // Calculate value div/mod by 10 without using two expensive
            // division operations. (2 ^ 27) / 10 = 13421772. Add one to
            // value to correct rounding error.
            int d = ((value + 1) * 13421772) >> 27;
            out << (d + '0');
            // Append remainder by calculating (value - d * 10).
            out << (value - (d << 3) - (d << 1) + '0');
        } else {
            out << (to_string(value));
        }
    }
    
    /**
     * Converts an integer to a string, and writes it to the given writer.
     *
     * <p>This method is optimized for converting small values to strings.
     *
     * @param out receives integer converted to a string
     * @param value value to convert to a string
     */
    static void writeUnpaddedInteger(stringstream &out, int64_t value) {
        int intValue = (int)value;
        if (intValue == value) {
            writeUnpaddedInteger(out, intValue);
        } else {
            out << (to_string(value));
        }
    }
    
    /**
     * Calculates the number of decimal digits for the given value,
     * including the sign.
     */
    static int calculateDigitCount(int64_t value) {
        if (value < 0) {
            if (value != LONG_MIN) {
                return calculateDigitCount(-value) + 1;
            } else {
                return 20;
            }
        }
        return
        (value < 10 ? 1 :
         (value < 100 ? 2 :
          (value < 1000 ? 3 :
           (value < 10000 ? 4 :
            ((int)(log(value) / LOG_10) + 1)))));
    }
    
};

const double FormatUtils::LOG_10 = log(10);

CODATIME_END

#endif
