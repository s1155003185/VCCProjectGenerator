#include "vpg_object_type_file_generation_service.hpp"

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

using namespace vcc;

#define LOG_ID L"Object Type File Generation"

void VPGObjectTypeFileGenerationService::Generate(const LogProperty *logProperty, const std::wstring &filePathHpp, const std::set<std::wstring> &propertyTypes)
{
    TRY
        LogService::LogInfo(logProperty, LOG_ID, L"Generate object type file: " + filePathHpp);
        std::wstring content = L"#pragma once\r\n";
        content += L"\r\n";
        content += L"enum class ObjectType {\r\n";
        content += INDENT + L"NA" + (!propertyTypes.empty() ? L"," : L"");
        if (!propertyTypes.empty()) {
            content += L"\r\n" + INDENT + Concat(propertyTypes, L",\r\n" + INDENT);
        }
        content += L"\r\n};\r\n";
        WriteFile(filePathHpp, content, true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate object type file completed.");
    CATCH
}