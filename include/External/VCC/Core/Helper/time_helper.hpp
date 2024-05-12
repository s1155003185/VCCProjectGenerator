#pragma once
#include <chrono>
#include <mutex>

#ifdef _WIN32
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#endif

#include "exception_macro.hpp"
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

    inline std::time_t ParseDatetime(const std::wstring &timeStr, const std::wstring &format)
    {
        std::time_t time = -1;
        TRY_CATCH() {
            #ifdef _WIN32
                std::tm timeStruct = {};
                std::wistringstream dateStream(timeStr);
                dateStream.imbue(std::locale(setlocale(LC_ALL, nullptr)));
                dateStream >> std::get_time(&timeStruct, format.c_str());
                time = mktime(&timeStruct);
            #else
                struct tm tm;
                strptime(wstr2str(timeStr).c_str(), wstr2str(format).c_str(), &tm);
                time = mktime(&tm);
            #endif
        }
        return time;
    }
}
