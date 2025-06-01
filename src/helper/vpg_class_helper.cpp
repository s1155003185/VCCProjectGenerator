#include "vpg_class_helper.hpp"

#include "exception_macro.hpp"
#include "file_helper.hpp"

const std::wstring propertyClassFileSuffix = L"_property.hpp";
const std::wstring propertyClassSuffix = L"Property";

bool IsFileStartWithProjectPrefix(const std::wstring &str, const std::wstring &projectPrefix)
{
    TRY
        if (projectPrefix.empty())
            return true;
        return vcc::IsStartWith(str, projectPrefix + L"_", 0, true);
    CATCH
    return false;
}

bool IsClassStartWithProjectPrefix(const std::wstring &str, const std::wstring &projectPrefix)
{
    TRY
        if (projectPrefix.empty())
            return true;
        return vcc::IsStartWith(GetTypeOrClassWithoutNamespace(str), projectPrefix);
    CATCH
    return false;
}

bool IsPropertyClassNameValidToGenerateClass(const std::wstring &str, const std::wstring &projectPrefix)
{
    TRY
        return IsClassStartWithProjectPrefix(str, projectPrefix) && vcc::IsEndWith(str, propertyClassSuffix);
    CATCH
    return false;
}

bool IsPropertyFile(const std::wstring &filePath, const std::wstring &projectPrefix)
{
    TRY
        return IsFileStartWithProjectPrefix(vcc::GetFileName(filePath), projectPrefix) && vcc::IsEndWith(filePath, propertyClassFileSuffix);
    CATCH
    return false;
}

bool IsPropertyClass(const std::wstring &className, const std::wstring &projectPrefix)
{
    TRY
        return IsClassStartWithProjectPrefix(className, projectPrefix) && vcc::IsEndWith(className, propertyClassSuffix);
    CATCH
    return false;
}

bool IsCustomType(const std::wstring &value)
{
    TRY
        if (value.empty())
            return false;
        return !value.empty() && vcc::IsCapital(vcc::SplitString(value, {L"::"}).back());
    CATCH
    return false;
}

std::wstring GetTypeOrClassWithoutNamespace(const std::wstring &value)
{
    TRY
    if (value.empty())
        return L"";
    return vcc::SplitString(value, {L"::"}).back();
    CATCH
    return L"";
}

std::wstring GetNamespaceFromClassName(const std::wstring &className)
{   
    TRY
        auto namespaceList = vcc::SplitString(className, {L"::"});
        if (namespaceList.size() < 2)
            return L"";
        namespaceList.pop_back();
        return vcc::Concat(namespaceList, L"::");
    CATCH
    return L"";
}

bool IsClassInNamespace(const std::wstring &currentNamespace, const std::wstring &fullClassName)
{
    TRY
        return currentNamespace == GetNamespaceFromClassName(fullClassName);
    CATCH
    return false;
}

std::wstring GetActionClassName(const VPGEnumClass* enumClass, const VPGEnumClassAttribute * property)
{
    TRY
        return GetClassNameFromPropertyClassName(enumClass->GetName())  + property->GetPropertyName();
    CATCH
    return L"";
}

std::wstring GetActionFileNameWithoutExtension(const std::wstring &actionClassName, const std::wstring &projectPrefix)
{
    std::wstring result = L"";
    TRY
        std::wstring currentActionClassName = GetTypeOrClassWithoutNamespace(actionClassName);
        std::wstring projectPrefixLower = projectPrefix;
        vcc::ToLower(projectPrefixLower);
        if (!vcc::IsBlank(projectPrefix) && vcc::IsStartWith(GetTypeOrClassWithoutNamespace(actionClassName), projectPrefix)) {
            result += projectPrefixLower;
            currentActionClassName = currentActionClassName.substr(projectPrefix.length());
        }
        auto tokens = vcc::SplitStringByUpperCase(currentActionClassName, false, true);
        for (auto token : tokens) {
            vcc::ToLower(token);
            if (!result.empty())
                result += L"_";
            result += token;
        }
    CATCH
    return result;
}

std::wstring GetClassNameFromPropertyClassName(const std::wstring &className)
{
    TRY
        if (vcc::IsEndWith(className, propertyClassSuffix) && className.length() > propertyClassSuffix.length())
            return className.substr(0, className.length() - propertyClassSuffix.length());
    CATCH
    return className;
}

std::wstring GetActionMessage(const std::wstring &id, const std::wstring &msg)
{
    return L"[" + id + L"] " + msg;
}