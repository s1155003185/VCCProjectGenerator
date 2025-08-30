#include "vpg_object_factory_file_generation_service.hpp"

#include <set>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

#define LOG_ID L"Object Factory File Generation"

void VPGObjectFactoryFileGenerationService::generateHpp(const vcc::LogConfig *logConfig, const std::wstring &filePathHpp)
{
    TRY
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate object factory file: " + filePathHpp);
        std::wstring content = L""
            "#pragma once\r\n"
            "\r\n"
            "#include <memory>\r\n"
            "\r\n"
            "#include \"base_factory.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"object_type.hpp\"\r\n"
            "\r\n"
            "class ObjectFactory : public vcc::BaseFactory\r\n"
            "{\r\n"
            "    private:\r\n"
            "        ObjectFactory() = default;\r\n"
            "        virtual ~ObjectFactory() {}\r\n"
            "\r\n"
            "    public:\r\n"
            "        static std::shared_ptr<vcc::IObject> create(const ObjectType &objectType, std::shared_ptr<vcc::IObject> parentObject = nullptr);\r\n"
            "};\r\n";
        vcc::writeFile(filePathHpp, content, true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate object factory file completed.");
    CATCH
}

void VPGObjectFactoryFileGenerationService::generateCpp(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix, const std::set<std::wstring> &includeFiles,
    const std::wstring &filePathCpp, const std::set<std::wstring> &propertyTypes)
{
    TRY
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate object factory file: " + filePathCpp);

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
            content += L"#include " + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, str) + L"\r\n";

        content += L"\r\n"
            "std::shared_ptr<vcc::IObject> ObjectFactory::create(const ObjectType &objectType, std::shared_ptr<vcc::IObject> parentObject)\r\n"
            "{\r\n"
            + INDENT + L"std::shared_ptr<vcc::IObject> result = nullptr;\r\n"
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
            + INDENT + INDENT + INDENT + L"result->setParentObject(parentObject);\r\n"
            + INDENT + L"CATCH\r\n"
            + INDENT + L"return result;\r\n"
            "}\r\n";
        vcc::writeFile(filePathCpp, content, true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate object factory completed.");
    CATCH
}