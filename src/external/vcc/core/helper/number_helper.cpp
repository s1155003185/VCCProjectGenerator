#include "number_helper.hpp"

#include <iomanip>
#include <sstream>
#include <string>

#include "exception_macro.hpp"

namespace vcc
{
    int getDecimalPlaces(const double &value) {
        TRY
            std::string numStr = std::to_string(value);
            size_t decimalPos = numStr.find_first_of(".");
            if (decimalPos == std::string::npos)
                return 0;
            return numStr.length() - decimalPos - 1;
        CATCH
        return 0;
    }

	std::wstring ToString(const double &value, const size_t &decimalPlaces)
	{
		TRY
			std::wstringstream ss;
			ss << std::fixed << std::setprecision(decimalPlaces) << value;
			return ss.str();
		CATCH
		return L"";
	}
};