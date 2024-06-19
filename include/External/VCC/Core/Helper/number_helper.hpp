#pragma once

#include <set>
#include <string>

#include "exception_macro.hpp"

namespace vcc
{
    int GetDecimalPlaces(double value) {
        TRY
            std::string numStr = std::to_string(value);
            size_t decimalPos = numStr.find_first_of(".");
            if (decimalPos == std::string::npos)
                return 0;
            return numStr.length() - decimalPos - 1;
        CATCH
        return 0;
    }

};