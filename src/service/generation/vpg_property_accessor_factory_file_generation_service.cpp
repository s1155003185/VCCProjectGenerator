#include "vpg_property_accessor_factory_file_generation_service.hpp"

#include <set>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

using namespace vcc;

#define LOG_ID L"Prorperty Accessor Factory File Generation"

void VPGPropertyAccessorFactoryFileGenerationService::GenerateHpp(const LogConfig *logConfig, const std::wstring &filePathHpp)
{
    TRY
        LogService::LogInfo(logConfig, LOG_ID, L"Generate property accessor factory file: " + filePathHpp);
        std::wstring content = L""
            "#pragma once\r\n"
            "\r\n"
            "#include <memory>\r\n"
            "\r\n"
            "#include \"base_factory.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"i_property_accessor.hpp\"\r\n"
            "\r\n"
            "using namespace vcc;\r\n"
            "\r\n"
            "class PropertyAccessorFactory : public BaseFactory\r\n"
            "{\r\n"
            "    private:\r\n"
            "        PropertyAccessorFactory() = default;\r\n"
            "        virtual ~PropertyAccessorFactory() {}\r\n"
            "\r\n"
            "    public:\r\n"
            "        static std::shared_ptr<IPropertyAccessor> Create(std::shared_ptr<IObject> object);\r\n"
            "};\r\n";
        WriteFile(filePathHpp, content, true);
        LogService::LogInfo(logConfig, LOG_ID, L"Generate property accessor factory file completed.");
    CATCH
}

void VPGPropertyAccessorFactoryFileGenerationService::GenerateCpp(const LogConfig *logConfig, const std::wstring &projectPrefix, const std::set<std::wstring> &includeFiles,
    const std::wstring &filePathCpp, const std::set<std::wstring> &propertyTypes)
{
    TRY
        LogService::LogInfo(logConfig, LOG_ID, L"Generate property accessor factory file: " + filePathCpp);

        std::set<std::wstring> tmpIncludePaths = includeFiles;
        tmpIncludePaths.insert(L"base_property_accessor.hpp");
        tmpIncludePaths.insert(L"i_object.hpp");
        tmpIncludePaths.insert(L"i_property_accessor.hpp");

        std::wstring content = L""
            "#include \"property_accessor_factory.hpp\"\r\n"
            "\r\n"
            "#include <assert.h>\r\n"
            "#include <memory>\r\n"
            "\r\n";
        
        for (auto const &str : tmpIncludePaths)
            content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, str) + L"\r\n";

        content += L"\r\n"
            "using namespace vcc;\r\n"
            "\r\n"
            "std::shared_ptr<IPropertyAccessor> PropertyAccessorFactory::Create(std::shared_ptr<IObject> object)\r\n"
            "{\r\n"
            + INDENT + L"assert(object != nullptr);\r\n"
            "\r\n"
            + INDENT + L"switch (object->GetObjectType())\r\n"
            + INDENT + L"{\r\n";
        for (auto const &propertyType : propertyTypes) {
            content += INDENT + L"case ObjectType::" + propertyType + L":\r\n"
                + INDENT + INDENT + L"return std::make_shared<" + projectPrefix + propertyType + L"PropertyAccessor>(object);\r\n";
        }
        content += L""
            + INDENT + L"default:\r\n"
            + INDENT + INDENT + L"assert(false);\r\n"
            + INDENT + INDENT + L"break;\r\n"
            + INDENT + L"}\r\n"
            + INDENT + L"return nullptr;\r\n"
            "}\r\n";
        WriteFile(filePathCpp, content, true);
        LogService::LogInfo(logConfig, LOG_ID, L"Generate property accessor factory completed.");
    CATCH
}