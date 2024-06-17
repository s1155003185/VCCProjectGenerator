#include "vpg_dll_file_generation_service.hpp"

#include <set>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

using namespace vcc;

#define LOG_ID L"Dll File Generation"

void VPGDllFileGenerationService::GenerateHpp(const LogProperty *logProperty, const std::wstring &hppFilePath)
{
    TRY
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.hpp file: " + hppFilePath);
        std::wstring content = L"";
        WriteFile(hppFilePath, content, true);
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.hpp file completed.");
    CATCH
}

void VPGDllFileGenerationService::GenerateCpp(const LogProperty *logProperty, const std::wstring &cppFilePath)
{
    TRY
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.cpp file: " + cppFilePath);
        std::wstring content = L"";
        WriteFile(cppFilePath, content, true);
        LogService::LogInfo(logProperty, LOG_ID, L"Modify DllFunctions.cpp completed.");
    CATCH
}