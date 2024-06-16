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

void VPGDllFileGenerationService::GenerateHpp(const LogProperty *logProperty, const std::wstring &filePathHpp)
{
    TRY
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.hpp file: " + filePathHpp);
        std::wstring content = L"";
        WriteFile(filePathHpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathHpp), L"propertyAccessor", content, L"//"), true);
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.hpp file completed.");
    CATCH
}

void VPGDllFileGenerationService::GenerateCpp(const LogProperty *logProperty, const std::wstring &filePathCpp)
{
    TRY
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.cpp file: " + filePathCpp);
        std::wstring content = L"";
        WriteFile(filePathCpp, VPGFileGenerationService::GenerateFileContent(ReadFile(filePathCpp), L"propertyAccessor", content, L"//"), true);
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.cpp completed.");
    CATCH
}