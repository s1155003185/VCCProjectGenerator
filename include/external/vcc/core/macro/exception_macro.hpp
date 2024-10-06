#pragma once

#include <queue>
#include <string>

#include "exception.hpp"
#include "i_exception.hpp"
#include "log_service.hpp"

namespace vcc
{
    inline void ____HandleException(const std::exception &e, std::wstring file, std::wstring line)
    {
        std::wstring prefix = file + L":" + line + L":\r\n"; 
        const IException *ie = dynamic_cast<const IException *>(&e); 
        if (ie != nullptr)
            throw Exception(ie->GetErrorType(), prefix + ie->GetErrorMessage());
        else
            throw Exception(ExceptionType::CustomError, prefix + str2wstr(std::string(e.what())));
    }

    #ifdef __DEBUGLOG__
    #define THROW_EXCEPTION(e) THROW_EXCEPTION_DEBUG(e)
    #define THROW_EXCEPTION_MSG(exceptionType, message) THROW_EXCEPTION_STACK_TRACE(exceptionType, message)
    #else
    #define THROW_EXCEPTION(e) throw e
    #define THROW_EXCEPTION_MSG(exceptionType, message) throw Exception(exceptionType, message)
    #endif

    #define THROW_EXCEPTION_DEBUG(e) ____HandleException(e, str2wstr(__FILE__), std::to_wstring(__LINE__))

    #define THROW_EXCEPTION_STACK_TRACE(exceptionType, message) \
        throw Exception(exceptionType, str2wstr(__FILE__) + L":" + std::to_wstring(__LINE__) + L":\r\n" + message)

    #define TRY try {
    #define CATCH  } catch (const std::exception &e) { THROW_EXCEPTION(e); }
    #define CATCH_SLIENT } catch (...) { }
    #define CATCH_MSG(exceptionType, message) } catch (const std::exception &e) { THROW_EXCEPTION_MSG(exceptionType, message); }
}