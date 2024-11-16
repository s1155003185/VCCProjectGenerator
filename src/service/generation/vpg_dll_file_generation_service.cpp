#include "vpg_dll_file_generation_service.hpp"

#include <set>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

#include "vpg_file_generation_service.hpp"

using namespace vcc;

#define LOG_ID L"Dll File Generation"

std::wstring VPGDllFileGenerationService::GenerateApplicationHpp(const VPGDllFileGenerationServiceOption *option)
{
    std::wstring result = L"";
    TRY
        if (!option->GetIsGenerateApplication())
            return result;

        result += 
            L"DLLEXPORT void ApplicationStart();\r\n"
            "DLLEXPORT void *ApplicationCreateForm(int64_t formType);\r\n"
            "DLLEXPORT bool ApplicationIsFormPresent(void *form);\r\n"
            "DLLEXPORT bool ApplicationIsFormClosable(void *form);\r\n"
            "DLLEXPORT bool ApplicationCloseForm(void *form, bool isForce);\r\n";
    CATCH
    return result;
}

std::wstring VPGDllFileGenerationService::GenerateApplicationCpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles)
{
    std::wstring result = L"";
    TRY
        if (!option->GetIsGenerateApplication())
            return result;

        customIncludeFiles.insert(L"application.hpp");
        customIncludeFiles.insert(L"i_object.hpp");
        customIncludeFiles.insert(L"object_type.hpp");

        result += 
            L"void ApplicationStart()\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        Application::Run();\r\n"
            "    CATCH\r\n"
            "}\r\n"
            "\r\n"
            "void *ApplicationCreateForm(int64_t objectType)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::CreateForm(static_cast<ObjectType>(objectType)).get();\r\n"
            "    CATCH\r\n"
            "    return nullptr;\r\n"
            "}\r\n"
            "\r\n"
            "bool ApplicationIsFormPresent(void *form)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::IsFormPresent(static_cast<IObject *>(form));\r\n"
            "    CATCH\r\n"
            "    return false;\r\n"
            "}\r\n"
            "\r\n"
            "bool ApplicationIsFormClosable(void *form)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::IsFormClosable(static_cast<IObject *>(form));\r\n"
            "    CATCH\r\n"
            "    return false;\r\n"
            "}\r\n"
            "\r\n"
            "bool ApplicationCloseForm(void *form, bool isForce)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::CloseForm(static_cast<IObject *>(form), isForce);\r\n"
            "    CATCH\r\n"
            "    return false;\r\n"
            "}\r\n";
    CATCH
    return result;
}

std::wstring VPGDllFileGenerationService::GeneratePropertyAccessorHpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles)
{
    std::wstring result = L"";
    TRY
        if (!option->GetIsGeneratePropertyAccessor())
            return result;

        customIncludeFiles.insert(L"object_factory.hpp");
        customIncludeFiles.insert(L"property_accessor_factory.hpp");
        customIncludeFiles.insert(L"property_accessor_macro.hpp");

        result +=
            L"PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(bool, Bool)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(char, Char)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(wchar_t, Wchar)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Int8)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Uint8)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Short)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, UnsignedShort)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Int)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedInt)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Long)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedLong)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(float, Float)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(double, Double)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER\r\n";
    CATCH
    return result;
}

std::wstring VPGDllFileGenerationService::GeneratePropertyAccessorCpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles)
{
    std::wstring result = L"";
    TRY
        if (!option->GetIsGeneratePropertyAccessor())
            return result;

        customIncludeFiles.insert(L"exception_macro.hpp");
        customIncludeFiles.insert(L"i_object.hpp");
        customIncludeFiles.insert(L"lock_type.hpp");
        customIncludeFiles.insert(L"property_accessor_factory.hpp");
        customIncludeFiles.insert(L"property_accessor_macro.hpp");

        result +=
            L"PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(bool, Bool, false)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(char, Char, L'\\0')\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(wchar_t, Wchar, L'\\0')\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Int8, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Uint8, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Short, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, UnsignedShort, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Int, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedInt, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Long, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedLong, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(float, Float, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(double, Double, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER\r\n";
    CATCH
    return result;
}

void VPGDllFileGenerationService::GenerateHpp(const LogConfig *logConfig, const std::wstring &filePathHpp, const VPGDllFileGenerationServiceOption *option)
{
    TRY
        assert(option != nullptr);
        if (!IsFileExists(filePathHpp))
            return;
        
        LogService::LogInfo(logConfig, LOG_ID, L"Modify DllFunctions.hpp file: " + filePathHpp);

        // header
        std::wstring content = L"";
        std::set<std::wstring> customIncludeFiles;
        std::wstring applicationStr = GenerateApplicationHpp(option);
        std::wstring propertyAccessorStr = GeneratePropertyAccessorHpp(option, customIncludeFiles);
        if (!customIncludeFiles.empty()) {
            for (auto const &file : customIncludeFiles)
                content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, file) + L"\r\n";
            content += L"\r\n"
                "using namespace vcc;\r\n";
        }
        WriteFile(filePathHpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathHpp), L"vcc:dllInterfaceHeader", content, L"//"), true);

        // content
        content = applicationStr;
        content += propertyAccessorStr;

        WriteFile(filePathHpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathHpp), L"vcc:dllInterface", content, L"//"), true);
        LogService::LogInfo(logConfig, LOG_ID, L"Modify DllFunctions.hpp file completed.");
    CATCH
}

void VPGDllFileGenerationService::GenerateCpp(const LogConfig *logConfig, const std::wstring &filePathCpp, const VPGDllFileGenerationServiceOption *option)
{
    TRY
        assert(option != nullptr);
        if (!IsFileExists(filePathCpp))
            return;

        LogService::LogInfo(logConfig, LOG_ID, L"Modify DllFunctions.cpp file: " + filePathCpp);
        // header
        std::wstring content = L"";
        std::set<std::wstring> customIncludeFiles;
        std::wstring applicationStr = GenerateApplicationCpp(option, customIncludeFiles);
        std::wstring propertyAccessorStr = GeneratePropertyAccessorCpp(option, customIncludeFiles);

        if (!customIncludeFiles.empty()) {
            for (auto const &file : customIncludeFiles)
                content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, file) + L"\r\n";
            content += L"\r\n"
                "using namespace vcc;\r\n";
        }
        WriteFile(filePathCpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathCpp), L"vcc:dllInterfaceHeader", content, L"//"), true);

        // content
        content = applicationStr;
        content += propertyAccessorStr;
        WriteFile(filePathCpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathCpp), L"vcc:dllInterface", content, L"//"), true);

        LogService::LogInfo(logConfig, LOG_ID, L"Modify DllFunctions.cpp completed.");
    CATCH
}