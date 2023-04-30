#pragma once
#include <chrono>
#include <mutex>

#include "string_helper.hpp"

namespace vcc
{
    inline std::wstring GetDateString(time_t timer)
    {
        std::tm bt{};
    #if defined(__unix__)
        localtime_r(&timer, &bt);
    #elif defined(_MSC_VER)
        localtime_s(&bt, &timer);
    #else
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        bt = *std::localtime(&timer);
    #endif
        char dateString[100];
        strftime(dateString, 50, "%Y-%m-%d", &bt);
        return str2wstr(std::string(dateString));
    }

    inline std::wstring GetCurrentDateString()
    {
        auto ts = std::chrono::system_clock::now();
        return GetDateString(std::chrono::system_clock::to_time_t(ts));
    }

    inline std::wstring GetDatetimeString(time_t timer)
    {
        std::tm bt{};
    #if defined(__unix__)
        localtime_r(&timer, &bt);
    #elif defined(_MSC_VER)
        localtime_s(&bt, &timer);
    #else
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        bt = *std::localtime(&timer);
    #endif
        char dateString[100];
        strftime(dateString, 50, "%Y-%m-%d %X", &bt);
        return str2wstr(std::string(dateString));
    }

    inline std::wstring GetCurrentDatetimeString()
    {
        auto ts = std::chrono::system_clock::now();
        std::wstring dateTimeStr = GetDatetimeString(std::chrono::system_clock::to_time_t(ts));
        auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(ts.time_since_epoch()) % 1000;
        auto nowMsStr = std::to_string(nowMs.count());
        return dateTimeStr + str2wstr("." + PadLeft(nowMsStr, 3, '0'));
    }
}
