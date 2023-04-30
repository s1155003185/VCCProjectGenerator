#pragma once
#include <exception>
#include <string>

#include "i_exception.hpp"
#include "log_property.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

namespace vcc
{
    inline void HandleException(LogProperty &logProperty, exception *ex)
    {
        IException *iex = dynamic_cast<IException *>(ex); 
        if (iex != nullptr)
            LogService::LogError(logProperty, iex->GetErrorMessage());
        else
            LogService::LogError(logProperty, str2wstr(std::string(ex->what())));
    }

    inline void HandleException(exception *ex)
    {
        LogProperty defaultProperty;
        HandleException(defaultProperty, ex);
    }
}