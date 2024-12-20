#include "vpg_action_file_generation_service.hpp"

#include <assert.h>
#include <set>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

#include "vpg_enum_class.hpp"
#include "vpg_file_generation_service.hpp"

using namespace vcc;

#define LOG_ID L"Action File Generation"

std::wstring VPGActionFileGenerationService::GenerateHpp(const LogConfig *logConfig, const std::wstring &folderPathHpp, const VPGEnumClass *enumClass)
{
    TRY
        assert(enumClass != nullptr);
        // if (!IsFileExists(folderPathHpp))
        //     return L"";
        
        // LogService::LogInfo(logConfig, LOG_ID, L"Modify DllFunctions.hpp file: " + folderPathCpp);

        // header
        std::wstring content = L"";
        // std::set<std::wstring> customIncludeFiles;
        // std::wstring applicationStr = GenerateApplicationHpp(option);
        // std::wstring propertyAccessorStr = GeneratePropertyAccessorHpp(option, customIncludeFiles);
        // if (!customIncludeFiles.empty()) {
        //     for (auto const &file : customIncludeFiles)
        //         content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, file) + L"\r\n";
        //     content += L"\r\n"
        //         "using namespace vcc;\r\n";
        // }
        // WriteFile(filePathHpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathHpp), L"vcc:dllInterfaceHeader", content, L"//"), true);

        // // content
        // content = applicationStr;
        // content += propertyAccessorStr;

        // WriteFile(folderPathHpp, VPGFileGenerationService::GenerateFileContent(ReadFile(folderPathHpp), L"vcc:dllInterface", content, L"//"), true);
        // LogService::LogInfo(logConfig, LOG_ID, L"Modify DllFunctions.hpp file completed.");
    CATCH
    return L"";
}

std::wstring VPGActionFileGenerationService::GenerateCpp(const LogConfig *logConfig, const std::wstring &folderPathCpp, const VPGEnumClass *enumClass)
{
    TRY
        assert(enumClass != nullptr);
        // if (!IsFileExists(folderPathCpp))
        //     return L"";

        // LogService::LogInfo(logConfig, LOG_ID, L"Modify DllFunctions.cpp file: " + folderPathCpp);
        // // header
        // std::wstring content = L"";
        // std::set<std::wstring> customIncludeFiles;
        // std::wstring applicationStr = GenerateApplicationCpp(option, customIncludeFiles);
        // std::wstring propertyAccessorStr = GeneratePropertyAccessorCpp(option, customIncludeFiles);

        // if (!customIncludeFiles.empty()) {
        //     for (auto const &file : customIncludeFiles)
        //         content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, file) + L"\r\n";
        //     content += L"\r\n"
        //         "using namespace vcc;\r\n";
        // }
        //WriteFile(folderPathCpp, VPGFileGenerationService::GenerateFileContent(ReadFile(folderPathCpp), L"vcc:dllInterfaceHeader", content, L"//"), true);

        // content
        // content = applicationStr;
        // content += propertyAccessorStr;
        //WriteFile(folderPathCpp, VPGFileGenerationService::GenerateFileContent(ReadFile(folderPathCpp), L"vcc:dllInterface", content, L"//"), true);

        LogService::LogInfo(logConfig, LOG_ID, L"Modify DllFunctions.cpp completed.");
    CATCH
    return L"";
}