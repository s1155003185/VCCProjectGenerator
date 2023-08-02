#pragma once
#include "exception.hpp"
#include "i_exception.hpp"
#include "log_service.hpp"

namespace vcc
{
    inline void ____HandleException(std::exception &ex, std::wstring file, std::wstring line)
    {
        std::wstring prefix = file + L":" + line + L":\r\n"; 
        IException *iex = dynamic_cast<IException *>(&ex); 
        if (iex != nullptr)
            throw Exception(iex->GetErrorType(), prefix + iex->GetErrorMessage());
        else
            throw Exception(ExceptionType::CUSTOM_ERROR, prefix + str2wstr(std::string(ex.what())));
    }

    #ifdef __DEBUGLOG__
    #define THROW_EXCEPTION(e) THROW_EXCEPTION_DEBUG(e)
    #define THROW_EXCEPTION_M(exceptionType, message) THROW_EXCEPTION_DEBUG_W(exceptionType, message)
    #else
    #define THROW_EXCEPTION(e) throw e
    #define THROW_EXCEPTION(exceptionType, message) throw Exception(exceptionType, message)
    #endif


    #define THROW_EXCEPTION_DEBUG(e) ____HandleException(e, str2wstr(__FILE__), std::to_wstring(__LINE__))

    #define THROW_EXCEPTION_DEBUG_W(exceptionType, message) \
        throw Exception(exceptionType, str2wstr(__FILE__) + L":" + std::to_wstring(__LINE__) + L":\r\n" + message)
}