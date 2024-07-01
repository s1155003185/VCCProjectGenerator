#pragma once

#include <set>
#include <string>

#include "exception_macro.hpp"

namespace vcc
{
    template <typename T>
    int GetDecimalPlaces(T value) {
        TRY
            static_assert(std::is_floating_point<T>::value, "GetDecimalPlaces() only for floating-point types");
            std::string numStr = std::to_string(value);
            size_t decimalPos = numStr.find_first_of(".");
            if (decimalPos == std::string::npos)
                return 0;
            return numStr.length() - decimalPos - 1;
        CATCH
        return 0;
    }

};