#pragma once

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "string_helper.hpp"

const std::wstring VCC_TAG = L"vcc";

enum class VPGCodeType
{
    Cpp,
    MakeFile,
    Java
};

enum class VPGTagMode
{
    Replace,
    Reserve
};

inline std::wstring getVccTagCommandDelimiter(const VPGCodeType &codeType)
{
    TRY
        switch (codeType)
        {
        case VPGCodeType::Cpp:
        case VPGCodeType::Java:
            return L"//";
        case VPGCodeType::MakeFile:
            return L"#";
        default:
            assert(false);
            break;
        }
    CATCH
    return L"";
}

inline std::wstring getVccTagModeString(const VPGTagMode &tagMode)
{
    TRY
        switch (tagMode)
        {
        case VPGTagMode::Reserve:
            return L"RESERVE";
        case VPGTagMode::Replace:
            return L"REPLACE";
        default:
            assert(false);
            break;
        }
    CATCH
    return L"";
}

inline VPGTagMode getVccTagModeType(const std::wstring &tagMode)
{
    TRY
        if (vcc::isEqual(tagMode, L"RESERVE", true))
            return VPGTagMode::Reserve;
        else if (vcc::isEqual(tagMode, L"REPLACE", true))
            return VPGTagMode::Replace;
        assert(false);
    CATCH
    return VPGTagMode::Replace;
}

inline std::wstring getVccTagHeader(const VPGCodeType &codeType, const std::wstring &tagName, const VPGTagMode &syncMode, const VPGTagMode &genMode)
{
    TRY
        return getVccTagCommandDelimiter(codeType) + L" <" + VCC_TAG + L":" + tagName + L" sync=\"" + getVccTagModeString(syncMode) + L"\" gen=\"" + getVccTagModeString(genMode) + L"\">";
    CATCH
    return L"";
}

inline std::wstring getVccTagTailer(const VPGCodeType &codeType, const std::wstring &tagName)
{
    TRY
        return getVccTagCommandDelimiter(codeType) + L" </" + VCC_TAG + L":" + tagName + L">";
    CATCH
    return L"";
}

inline std::wstring getVccTagHeaderCustomClassCustomFunctions(const VPGCodeType &codeType, const std::wstring &prefix, const std::wstring &className, const std::wstring &functionName)
{
    TRY
        return getVccTagHeader(codeType, prefix + className + functionName, VPGTagMode::Reserve, VPGTagMode::Reserve);
    CATCH
    return L"";
}

inline std::wstring getVccTagTailerCustomClassCustomFunctions(const VPGCodeType &codeType, const std::wstring &prefix, const std::wstring &className, const std::wstring &functionName)
{
    TRY
        return getVccTagTailer(codeType, prefix + className + functionName);
    CATCH
    return L"";
}

inline std::wstring getVccTagHeaderCustomHeader(const VPGCodeType &codeType)
{
    TRY
        return getVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", L"", L"Header");
    CATCH
    return L"";
}

inline std::wstring getVccTagTailerCustomHeader(const VPGCodeType &codeType)
{
    TRY
        return getVccTagTailerCustomClassCustomFunctions(codeType, L"custom", L"", L"Header");
    CATCH
    return L"";
}

inline std::wstring getVccTagHeaderCustomClassProperties(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", className, L"Properties");
    CATCH
    return L"";
}

inline std::wstring getVccTagTailerCustomClassProperties(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagTailerCustomClassCustomFunctions(codeType, L"custom", className, L"Properties");
    CATCH
    return L"";
}

inline std::wstring getVccTagHeaderCustomClassPrivateFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", className, L"PrivateFunctions");
    CATCH
    return L"";
}

inline std::wstring getVccTagTailerCustomClassPrivateFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagTailerCustomClassCustomFunctions(codeType, L"custom", className, L"PrivateFunctions");
    CATCH
    return L"";
}

inline std::wstring getVccTagHeaderCustomClassProtectedFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", className, L"ProtectedFunctions");
    CATCH
    return L"";
}

inline std::wstring getVccTagTailerCustomClassProtectedFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagTailerCustomClassCustomFunctions(codeType, L"custom", className, L"ProtectedFunctions");
    CATCH
    return L"";
}

inline std::wstring getVccTagHeaderCustomClassPublicFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", className, L"PublicFunctions");
    CATCH
    return L"";
}

inline std::wstring getVccTagTailerCustomClassPublicFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagTailerCustomClassCustomFunctions(codeType, L"custom", className, L"PublicFunctions");
    CATCH
    return L"";
}

inline std::wstring getVccTagHeaderCustomClassFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagHeaderCustomClassCustomFunctions(codeType,  L"custom", className, L"Functions");
    CATCH
    return L"";
}

inline std::wstring getVccTagTailerCustomClassFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagTailerCustomClassCustomFunctions(codeType,  L"custom", className,  L"Functions");
    CATCH
    return L"";
}

inline std::wstring getVccTagHeaderCustomTypes(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagHeader(codeType, L"custom" + className + L"s", VPGTagMode::Reserve, VPGTagMode::Reserve);
    CATCH
    return L"";
}

inline std::wstring getVccTagTailerCustomTypes(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return getVccTagTailer(codeType, L"custom" + className + L"s");
    CATCH
    return L"";
}
