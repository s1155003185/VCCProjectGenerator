#pragma once

#include <queue>
#include <string>

#include "exception.hpp"
#include "i_exception.hpp"
#include "i_result.hpp"
#include "log_service.hpp"

namespace vcc
{
    inline void ____HandleException(const std::exception &e, const std::wstring &file, const std::wstring &line)
    {
        std::wstring prefix = !isBlank(file) && !isBlank(line) ? (file + L":" + line + L":\r\n") : L"";
        const IException *ie = dynamic_cast<const IException *>(&e); 
        if (ie != nullptr)
            throw Exception(ie->getErrorType(), prefix + ie->getErrorMessage());
        else
            throw Exception(ExceptionType::CustomError, prefix + str2wstr(std::string(e.what())));
    }
    
    template<typename ResultClass>
    inline std::shared_ptr<IResult> ____HandleResult(const std::exception &e, const std::wstring &file, const std::wstring &line)
    {
        std::wstring prefix = !isBlank(file) && !isBlank(line) ? (file + L":" + line + L":\r\n") : L"";
        const IException *ie = dynamic_cast<const IException *>(&e); 
        if (ie != nullptr)
            return std::make_shared<ResultClass>(ie->getErrorType(), prefix + ie->getErrorMessage());
        else
            return std::make_shared<ResultClass>(ExceptionType::CustomError, prefix + str2wstr(std::string(e.what())));
    }
}

#ifdef __DEBUGLOG__
#define THROW_EXCEPTION(e) THROW_EXCEPTION_DEBUG(e)
#define THROW_EXCEPTION_MSG(exceptionType, message) THROW_EXCEPTION_STACK_TRACE(exceptionType, message)
#define CATCH_RETURN_RESULT(resultClass)  CATCH_RETURN_RESULT_DEBUG(resultClass)
#else
#define THROW_EXCEPTION(e) vcc::____HandleException(e, L"", L"")
#define THROW_EXCEPTION_MSG(exceptionType, message) throw vcc::Exception(exceptionType, message)
#define CATCH_RETURN_RESULT(resultClass)  } catch (const std::exception &e) { return vcc::____HandleResult<resultClass>(e, L"", L""); }
#endif

#define THROW_EXCEPTION_DEBUG(e) vcc::____HandleException(e, vcc::str2wstr(__FILE__), std::to_wstring(__LINE__))
#define CATCH_RETURN_RESULT_DEBUG(resultClass)  } catch (const std::exception &e) { return vcc::____HandleResult<resultClass>(e, vcc::str2wstr(__FILE__), std::to_wstring(__LINE__)); }

#define THROW_EXCEPTION_STACK_TRACE(exceptionType, message) throw vcc::Exception(exceptionType, vcc::str2wstr(__FILE__) + L":" + std::to_wstring(__LINE__) + L":\r\n" + message)

#define TRY try {
#define CATCH  } catch (const std::exception &e) { THROW_EXCEPTION(e); }
#define CATCH_SLIENT } catch (...) { }
#define CATCH_MSG(exceptionType, message) } catch (const std::exception &e) { THROW_EXCEPTION_MSG(exceptionType, message); }

// Validation
#define VALIDATE(msg, ...) if (!(__VA_ARGS__)) THROW_EXCEPTION_MSG(ExceptionType::ArgumentNotValid, msg);
