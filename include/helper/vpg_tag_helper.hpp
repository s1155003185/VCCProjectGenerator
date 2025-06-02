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

inline std::wstring GetVccTagCommandDelimiter(const VPGCodeType &codeType)
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

inline std::wstring GetVccTagModeString(const VPGTagMode &tagMode)
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

inline VPGTagMode GetVccTagModeType(const std::wstring &tagMode)
{
    TRY
        if (vcc::IsEqual(tagMode, L"RESERVE", true))
            return VPGTagMode::Reserve;
        else if (vcc::IsEqual(tagMode, L"REPLACE", true))
            return VPGTagMode::Replace;
        assert(false);
    CATCH
    return VPGTagMode::Replace;
}

inline std::wstring GetVccTagHeader(const VPGCodeType &codeType, const std::wstring &tagName, const VPGTagMode &syncMode, const VPGTagMode &genMode)
{
    TRY
        return GetVccTagCommandDelimiter(codeType) + L" <" + VCC_TAG + L":" + tagName + L" sync=\"" + GetVccTagModeString(syncMode) + L"\" gen=\"" + GetVccTagModeString(genMode) + L"\">";
    CATCH
    return L"";
}

inline std::wstring GetVccTagTailer(const VPGCodeType &codeType, const std::wstring &tagName)
{
    TRY
        return GetVccTagCommandDelimiter(codeType) + L" </" + VCC_TAG + L":" + tagName + L">";
    CATCH
    return L"";
}

inline std::wstring GetVccTagHeaderCustomClassCustomFunctions(const VPGCodeType &codeType, const std::wstring &prefix, const std::wstring &className, const std::wstring &functionName)
{
    TRY
        return GetVccTagHeader(codeType, prefix + className + functionName, VPGTagMode::Reserve, VPGTagMode::Reserve);
    CATCH
    return L"";
}

inline std::wstring GetVccTagTailerCustomClassCustomFunctions(const VPGCodeType &codeType, const std::wstring &prefix, const std::wstring &className, const std::wstring &functionName)
{
    TRY
        return GetVccTagTailer(codeType, prefix + className + functionName);
    CATCH
    return L"";
}

inline std::wstring GetVccTagHeaderCustomHeader(const VPGCodeType &codeType)
{
    TRY
        return GetVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", L"", L"Header");
    CATCH
    return L"";
}

inline std::wstring GetVccTagTailerCustomHeader(const VPGCodeType &codeType)
{
    TRY
        return GetVccTagTailerCustomClassCustomFunctions(codeType, L"custom", L"", L"Header");
    CATCH
    return L"";
}

inline std::wstring GetVccTagHeaderCustomClassProperties(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", className, L"Properties");
    CATCH
    return L"";
}

inline std::wstring GetVccTagTailerCustomClassProperties(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagTailerCustomClassCustomFunctions(codeType, L"custom", className, L"Properties");
    CATCH
    return L"";
}

inline std::wstring GetVccTagHeaderCustomClassPrivateFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", className, L"PrivateFunctions");
    CATCH
    return L"";
}

inline std::wstring GetVccTagTailerCustomClassPrivateFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagTailerCustomClassCustomFunctions(codeType, L"custom", className, L"PrivateFunctions");
    CATCH
    return L"";
}

inline std::wstring GetVccTagHeaderCustomClassProtectedFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", className, L"ProtectedFunctions");
    CATCH
    return L"";
}

inline std::wstring GetVccTagTailerCustomClassProtectedFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagTailerCustomClassCustomFunctions(codeType, L"custom", className, L"ProtectedFunctions");
    CATCH
    return L"";
}

inline std::wstring GetVccTagHeaderCustomClassPublicFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagHeaderCustomClassCustomFunctions(codeType, L"custom", className, L"PublicFunctions");
    CATCH
    return L"";
}

inline std::wstring GetVccTagTailerCustomClassPublicFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagTailerCustomClassCustomFunctions(codeType, L"custom", className, L"PublicFunctions");
    CATCH
    return L"";
}

inline std::wstring GetVccTagHeaderCustomClassFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagHeaderCustomClassCustomFunctions(codeType,  L"custom", className, L"Functions");
    CATCH
    return L"";
}

inline std::wstring GetVccTagTailerCustomClassFunctions(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagTailerCustomClassCustomFunctions(codeType,  L"custom", className,  L"Functions");
    CATCH
    return L"";
}

inline std::wstring GetVccTagHeaderCustomTypes(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagHeader(codeType, L"custom" + className + L"s", VPGTagMode::Reserve, VPGTagMode::Reserve);
    CATCH
    return L"";
}

inline std::wstring GetVccTagTailerCustomTypes(const VPGCodeType &codeType, const std::wstring &className)
{
    TRY
        return GetVccTagTailer(codeType, L"custom" + className + L"s");
    CATCH
    return L"";
}
