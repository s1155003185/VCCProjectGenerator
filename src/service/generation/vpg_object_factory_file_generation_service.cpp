#include "vpg_object_factory_file_generation_service.hpp"

#include <set>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

using namespace vcc;

#define LOG_ID L"Object Factory File Generation"

void VPGObjectFactoryFileGenerationService::GenerateHpp(const LogConfig *logConfig, const std::wstring &filePathHpp)
{
    TRY
        LogService::LogInfo(logConfig, LOG_ID, L"Generate object factory file: " + filePathHpp);
        std::wstring content = L""
            "#pragma once\r\n"
            "\r\n"
            "#include <memory>\r\n"
            "\r\n"
            "#include \"base_factory.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"object_type.hpp\"\r\n"
            "\r\n"
            "using namespace vcc;\r\n"
            "\r\n"
            "class ObjectFactory : public BaseFactory\r\n"
            "{\r\n"
            "    private:\r\n"
            "        ObjectFactory() = default;\r\n"
            "        virtual ~ObjectFactory() {}\r\n"
            "\r\n"
            "    public:\r\n"
            "        static std::shared_ptr<IObject> Create(const ObjectType &objectType, std::shared_ptr<IObject> parentObject = nullptr);\r\n"
            "};\r\n";
        WriteFile(filePathHpp, content, true);
        LogService::LogInfo(logConfig, LOG_ID, L"Generate object factory file completed.");
    CATCH
}

void VPGObjectFactoryFileGenerationService::GenerateCpp(const LogConfig *logConfig, const std::wstring &projectPrefix, const std::set<std::wstring> &includeFiles,
    const std::wstring &filePathCpp, const std::set<std::wstring> &propertyTypes)
{
    TRY
        LogService::LogInfo(logConfig, LOG_ID, L"Generate object factory file: " + filePathCpp);

        std::set<std::wstring> tmpIncludePaths = includeFiles;
        tmpIncludePaths.insert(L"exception_macro.hpp");
        tmpIncludePaths.insert(L"i_object.hpp");
        tmpIncludePaths.insert(L"object_type.hpp");

        std::wstring content = L""
            "#include \"object_factory.hpp\"\r\n"
            "\r\n"
            "#include <assert.h>\r\n"
            "#include <memory>\r\n"
            "\r\n";
        
        for (auto const &str : tmpIncludePaths)
            content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, str) + L"\r\n";

        content += L"\r\n"
            "using namespace vcc;\r\n"
            "\r\n"
            "std::shared_ptr<IObject> ObjectFactory::Create(const ObjectType &objectType, std::shared_ptr<IObject> parentObject)\r\n"
            "{\r\n"
            + INDENT + L"std::shared_ptr<IObject> result = nullptr;\r\n"
            + INDENT + L"TRY\r\n"
            + INDENT + INDENT + L"switch (objectType)\r\n"
            + INDENT + INDENT + L"{\r\n";
        for (auto const &propertyType : propertyTypes) {
            content += INDENT + INDENT + L"case ObjectType::" + propertyType + L":\r\n"
                + INDENT + INDENT + INDENT + L"result = std::make_shared<" + projectPrefix + propertyType + L">();\r\n"
                + INDENT + INDENT + INDENT + L"break;\r\n";
        }
        content += L""
            + INDENT + INDENT + L"default:\r\n"
            + INDENT + INDENT + INDENT + L"assert(false);\r\n"
            + INDENT + INDENT + INDENT + L"break;\r\n"
            + INDENT + INDENT + L"}\r\n"
            + INDENT + INDENT + L"if (result != nullptr)\r\n"
            + INDENT + INDENT + INDENT + L"result->SetParentObject(parentObject);\r\n"
            + INDENT + L"CATCH\r\n"
            + INDENT + L"return result;\r\n"
            "}\r\n";
        WriteFile(filePathCpp, content, true);
        LogService::LogInfo(logConfig, LOG_ID, L"Generate object factory completed.");
    CATCH
}