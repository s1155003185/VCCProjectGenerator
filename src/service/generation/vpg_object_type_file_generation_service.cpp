#include "vpg_object_type_file_generation_service.hpp"

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"
#include "xml.hpp"

#include "vpg_code_reader.hpp"
#include "vpg_tag_helper.hpp"

#define LOG_ID L"Object Type File Generation"

void VPGObjectTypeFileGenerationService::Generate(const vcc::LogConfig *logConfig, const std::wstring &filePathHpp, const std::set<std::wstring> &propertyTypes)
{
    TRY
        vcc::LogService::LogInfo(logConfig, LOG_ID, L"Generate object type file: " + filePathHpp);

        std::wstring customContent = L"";
        if (vcc::IsFilePresent(filePathHpp)) {
            std::wstring orginalContent = vcc::ReadFile(filePathHpp);
            auto reader = std::make_unique<VPGCodeReader>(L"//");
            auto element = std::make_shared<vcc::Xml>();
            reader->Deserialize(orginalContent, element);
            for (std::shared_ptr<vcc::Xml> child : element->GetChildren()) {
                if (vcc::IsStartWith(child->GetName(), L"vcc:custom")) {
                    customContent = child->GetFullText();
                    break;
                }
            }
        }

        std::set<std::wstring> overallPropertyTypes = propertyTypes;
        overallPropertyTypes.insert(L"OperationResult");
        
        std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
            "#pragma once\r\n"
            "\r\n"
            "enum class ObjectType\r\n"
            "{\r\n"
            + INDENT + L"NA\r\n";
        if (!overallPropertyTypes.empty())
            content += INDENT + L", " + vcc::Concat(overallPropertyTypes, L"\r\n" + INDENT + L", ") + L"\r\n";
        
        if (!vcc::IsBlank(customContent))
            content += INDENT + customContent + L"\r\n";
        else
            content += INDENT + GetVccTagHeaderCustomTypes(VPGCodeType::Cpp, L"ObjectType") + L"\r\n"
                + INDENT + GetVccTagTailerCustomTypes(VPGCodeType::Cpp, L"ObjectType") + L"\r\n";
        content += L"};\r\n";
        vcc::WriteFile(filePathHpp, content, true);
        vcc::LogService::LogInfo(logConfig, LOG_ID, L"Generate object type file completed.");
    CATCH
}