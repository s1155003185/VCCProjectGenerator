#pragma once
#include "exception.hpp"
#include "log_service.hpp"

namespace vcc
{
    #ifdef __DEBUGLOG__
    #define THROW_EXCEPTION(exceptionType, message) THROW_EXCEPTION_DEBUG(exceptionType, message)
    #else
    #define THROW_EXCEPTION(exceptionType, message) throw Exception(exceptionType, message)
    #endif

    #define THROW_EXCEPTION_DEBUG(exceptionType, message) \
        throw Exception(exceptionType, str2wstr(__FILE__) + L":" + std::to_wstring(__LINE__) + L": " + message)
}