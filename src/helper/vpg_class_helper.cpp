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
        return vcc::isStartWith(str, projectPrefix + L"_", 0, true);
    CATCH
    return false;
}

bool IsClassStartWithProjectPrefix(const std::wstring &str, const std::wstring &projectPrefix)
{
    TRY
        if (projectPrefix.empty())
            return true;
        return vcc::isStartWith(getTypeOrClassWithoutNamespace(str), projectPrefix);
    CATCH
    return false;
}

bool IsPropertyClassNameValidToGenerateClass(const std::wstring &str, const std::wstring &projectPrefix)
{
    TRY
        return IsClassStartWithProjectPrefix(str, projectPrefix) && vcc::isEndWith(str, propertyClassSuffix);
    CATCH
    return false;
}

bool IsPropertyFile(const std::wstring &filePath, const std::wstring &projectPrefix)
{
    TRY
        return IsFileStartWithProjectPrefix(vcc::getFileName(filePath), projectPrefix) && vcc::isEndWith(filePath, propertyClassFileSuffix);
    CATCH
    return false;
}

bool IsPropertyClass(const std::wstring &className, const std::wstring &projectPrefix)
{
    TRY
        return IsClassStartWithProjectPrefix(className, projectPrefix) && vcc::isEndWith(className, propertyClassSuffix);
    CATCH
    return false;
}

bool IsCustomType(const std::wstring &value)
{
    TRY
        if (value.empty())
            return false;
        return !value.empty() && vcc::isCapital(vcc::splitString(value, {L"::"}).back());
    CATCH
    return false;
}

std::wstring getTypeOrClassWithoutNamespace(const std::wstring &value)
{
    TRY
    if (value.empty())
        return L"";
    return vcc::splitString(value, {L"::"}).back();
    CATCH
    return L"";
}

std::wstring getNamespaceFromClassName(const std::wstring &className)
{   
    TRY
        auto namespaceList = vcc::splitString(className, {L"::"});
        if (namespaceList.size() < 2)
            return L"";
        namespaceList.pop_back();
        return vcc::concat(namespaceList, L"::");
    CATCH
    return L"";
}

bool IsClassInNamespace(const std::wstring &currentNamespace, const std::wstring &fullClassName)
{
    TRY
        return currentNamespace == getNamespaceFromClassName(fullClassName);
    CATCH
    return false;
}

std::wstring getActionClassName(const VPGEnumClass* enumClass, const VPGEnumClassAttribute * property)
{
    TRY
        return getClassNameFromPropertyClassName(enumClass->getName())  + property->getPropertyName();
    CATCH
    return L"";
}

std::wstring getActionFileNameWithoutExtension(const std::wstring &actionClassName, const std::wstring &projectPrefix)
{
    std::wstring result = L"";
    TRY
        std::wstring currentActionClassName = getTypeOrClassWithoutNamespace(actionClassName);
        std::wstring projectPrefixLower = projectPrefix;
        vcc::toLower(projectPrefixLower);
        if (!vcc::isBlank(projectPrefix) && vcc::isStartWith(getTypeOrClassWithoutNamespace(actionClassName), projectPrefix)) {
            result += projectPrefixLower;
            currentActionClassName = currentActionClassName.substr(projectPrefix.length());
        }
        auto tokens = vcc::splitStringByUpperCase(currentActionClassName, false, true);
        for (auto token : tokens) {
            vcc::toLower(token);
            if (!result.empty())
                result += L"_";
            result += token;
        }
    CATCH
    return result;
}

std::wstring getClassNameFromPropertyClassName(const std::wstring &className)
{
    TRY
        if (vcc::isEndWith(className, propertyClassSuffix) && className.length() > propertyClassSuffix.length())
            return className.substr(0, className.length() - propertyClassSuffix.length());
    CATCH
    return className;
}

std::wstring getActionMessage(const std::wstring &id, const std::wstring &msg)
{
    return L"[" + id + L"] " + msg;
}