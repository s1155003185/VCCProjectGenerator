#pragma once

#include <chrono>
#include <string>

namespace vcc
{
    void Sleep(const long long &milliseconds);

    std::wstring getDateString(const time_t &timer);
    std::wstring getCurrentDateString();
    std::wstring getDatetimeString(const time_t &timer);
    std::wstring getCurrentDatetimeString();

    time_t ParseDatetime(const std::wstring &timeStr, const std::wstring &format);
}
