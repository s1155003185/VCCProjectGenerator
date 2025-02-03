#pragma once

#include <string>

namespace vcc
{
    int GetDecimalPlaces(const double &value);

	std::wstring ToString(const double &value, const size_t &decimalPlaces);
};