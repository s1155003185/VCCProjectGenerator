#pragma once

#include <chrono>
#include <string>

namespace vcc
{
    void Sleep(const long long &milliseconds);

    std::wstring GetDateString(const time_t &timer);
    std::wstring GetCurrentDateString();
    std::wstring GetDatetimeString(const time_t &timer);
    std::wstring GetCurrentDatetimeString();

    std::time_t ParseDatetime(const std::wstring &timeStr, const std::wstring &format);
}
