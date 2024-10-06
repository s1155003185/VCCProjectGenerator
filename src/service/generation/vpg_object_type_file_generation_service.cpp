#include "vpg_object_type_file_generation_service.hpp"

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "memory_macro.hpp"
#include "set_helper.hpp"
#include "xml.hpp"

#include "vpg_code_reader.hpp"

using namespace vcc;

#define LOG_ID L"Object Type File Generation"

void VPGObjectTypeFileGenerationService::Generate(const LogProperty *logProperty, const std::wstring &filePathHpp, const std::set<std::wstring> &propertyTypes)
{
    TRY
        LogService::LogInfo(logProperty, LOG_ID, L"Generate object type file: " + filePathHpp);

        std::wstring customContent = L"";
        if (IsFileExists(filePathHpp)) {
            std::wstring orginalContent = ReadFile(filePathHpp);
            DECLARE_UPTR(VPGCodeReader, reader, L"//");
            DECLARE_SPTR(Xml, element);
            reader->Deserialize(orginalContent, element);
            for (std::shared_ptr<Xml> child : element->GetChildren()) {
                if (IsStartWith(child->GetName(), L"vcc:custom")) {
                    customContent = child->GetFullText();
                    break;
                }
            }
        }
        
        std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
            "#pragma once\r\n"
            "\r\n"
            "enum class ObjectType\r\n"
            "{\r\n"
            + INDENT + L"NA" + (!propertyTypes.empty() ? L"," : L"") + L"\r\n";
        if (!propertyTypes.empty())
            content += INDENT + Concat(propertyTypes, L",\r\n" + INDENT) + L"\r\n";
        
        if (!IsBlank(customContent))
            content += INDENT + customContent + L"\r\n";
        else
            content += INDENT + L"// <vcc:custom sync=\"SKIP\" gen=\"SKIP\">\r\n"
                + INDENT + L"// </vcc:custom>\r\n";
        content += L"};\r\n";
        WriteFile(filePathHpp, content, true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate object type file completed.");
    CATCH
}