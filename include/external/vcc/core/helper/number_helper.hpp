#pragma once

#include <string>

namespace vcc
{
    int getDecimalPlaces(const double &value);

	std::wstring ToString(const double &value, const size_t &decimalPlaces);
};