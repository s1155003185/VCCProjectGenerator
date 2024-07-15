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

void VPGDllFileGenerationService::GenerateHpp(const LogProperty *logProperty, const std::wstring &filePathHpp, const VPGDllFileGenerationServiceOption *option)
{
    TRY
        assert(option != nullptr);
        if (!IsFileExists(filePathHpp))
            return;
        
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.hpp file: " + filePathHpp);

        // header
        std::wstring content = L"";
        std::set<std::wstring> customIncludeFiles;
        if (option->GetIsGeneratePropertyAccessor()) {
            customIncludeFiles.insert(L"property_accessor_macro.hpp");
        }

        if (!customIncludeFiles.empty()) {
            content += L"\r\n";
            for (auto const &file : customIncludeFiles)
                content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, file) + L"\r\n";
            content += L"\r\n"
                "using namespace vcc;\r\n";
        }
        WriteFile(filePathHpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathHpp), L"dllInterfaceHeader", content, L"//"), true);

        // content
        content = L"";
        if (option->GetIsGeneratePropertyAccessor()) {
            content += L"\r\n"
                "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(bool, Bool)\r\n"
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
        }

        WriteFile(filePathHpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathHpp), L"dllInterface", content, L"//"), true);
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.hpp file completed.");
    CATCH
}

void VPGDllFileGenerationService::GenerateCpp(const LogProperty *logProperty, const std::wstring &filePathCpp, const VPGDllFileGenerationServiceOption *option)
{
    TRY
        assert(option != nullptr);
        if (!IsFileExists(filePathCpp))
            return;

        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.cpp file: " + filePathCpp);
        // header
        std::wstring content = L"";
        std::set<std::wstring> customIncludeFiles;
        if (option->GetIsGeneratePropertyAccessor()) {
            customIncludeFiles.insert(L"exception_macro.hpp");
            customIncludeFiles.insert(L"i_object.hpp");
            customIncludeFiles.insert(L"i_type_union.hpp");
            customIncludeFiles.insert(L"lock_type.hpp");
            customIncludeFiles.insert(L"property_accessor_factory.hpp");
            customIncludeFiles.insert(L"property_accessor_macro.hpp");
        }

        if (!customIncludeFiles.empty()) {
            content += L"\r\n";
            for (auto const &file : customIncludeFiles)
                content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, file) + L"\r\n";
            content += L"\r\n"
                "using namespace vcc;\r\n";
        }
        WriteFile(filePathCpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathCpp), L"dllInterfaceHeader", content, L"//"), true);

        // content
        content = L"";
        if (option->GetIsGeneratePropertyAccessor()) {
            content += L"\r\n"
                "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(bool, Bool, false)\r\n"
                "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(char, Char, L'\0')\r\n"
                "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(wchar_t, Wchar, L'\0')\r\n"
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
        }
        WriteFile(filePathCpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathCpp), L"dllInterface", content, L"//"), true);

        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.cpp completed.");
    CATCH
}