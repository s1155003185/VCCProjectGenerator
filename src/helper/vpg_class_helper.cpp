#include "vpg_class_helper.hpp"

#include "file_helper.hpp"

const std::wstring propertyClassFileSuffix = L"_property.hpp";
const std::wstring propertyClassSuffix = L"Property";

bool IsFileStartWithProjectPrefix(const std::wstring &str, const std::wstring &projectPrefix)
{
    TRY
        if (projectPrefix.empty())
            return true;
        return IsStartWithCaseInsensitive(str, projectPrefix + L"_");
    CATCH
    return false;
}

bool IsClassStartWithProjectPrefix(const std::wstring &str, const std::wstring &projectPrefix)
{
    TRY
        if (projectPrefix.empty())
            return true;
        return IsStartWith(str, projectPrefix + L"_");
    CATCH
    return false;
}

bool IsPropertyFile(const std::wstring &filePath, const std::wstring &projectPrefix)
{
    TRY
        return IsFileStartWithProjectPrefix(GetFileName(filePath), projectPrefix) && IsEndWith(filePath, propertyClassFileSuffix);
    CATCH
    return false;
}

bool IsPropertyClass(const std::wstring &className, const std::wstring &projectPrefix)
{
    TRY
        return IsClassStartWithProjectPrefix(className, projectPrefix) && IsEndWith(className, propertyClassSuffix);
    CATCH
    return false;
}

std::wstring GetClassNameFromPropertyClassName(const std::wstring &className, const std::wstring &projectPrefix)
{
    TRY
        if (!IsPropertyClass(className, projectPrefix))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, className + L" is not start with " + projectPrefix + L" or end with Property");
        return className.substr(0, className.length() - propertyClassSuffix.length());
    CATCH
    return className;
}