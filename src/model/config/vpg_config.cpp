// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_config.hpp"

#include <assert.h>
#include <memory>
#include <string>

#include "exception_macro.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "json.hpp"
#include "number_helper.hpp"
#include "string_helper.hpp"
#include "vpg_config_type.hpp"
#include "vpg_project_type.hpp"

std::shared_ptr<vcc::Json> VPGConfigTemplate::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // Url
        json->addString(vcc::convertNamingStyle(L"Url", vcc::NamingStyle::PascalCase, namestyle), getUrl());
        // Workspace
        json->addString(vcc::convertNamingStyle(L"Workspace", vcc::NamingStyle::PascalCase, namestyle), getWorkspace());
        // IsExcludeUnittest
        json->addBool(vcc::convertNamingStyle(L"IsExcludeUnittest", vcc::NamingStyle::PascalCase, namestyle), getIsExcludeUnittest());
        // IsExcludeVCCUnitTest
        json->addBool(vcc::convertNamingStyle(L"IsExcludeVCCUnitTest", vcc::NamingStyle::PascalCase, namestyle), getIsExcludeVCCUnitTest());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigTemplate::deserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // Url
        if (json->isContainKey(vcc::convertNamingStyle(L"Url", namestyle, vcc::NamingStyle::PascalCase)))
            setUrl(json->getString(vcc::convertNamingStyle(L"Url", namestyle, vcc::NamingStyle::PascalCase)));
        // Workspace
        if (json->isContainKey(vcc::convertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)))
            setWorkspace(json->getString(vcc::convertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)));
        // IsExcludeUnittest
        if (json->isContainKey(vcc::convertNamingStyle(L"IsExcludeUnittest", namestyle, vcc::NamingStyle::PascalCase)))
            setIsExcludeUnittest(json->getBool(vcc::convertNamingStyle(L"IsExcludeUnittest", namestyle, vcc::NamingStyle::PascalCase)));
        // IsExcludeVCCUnitTest
        if (json->isContainKey(vcc::convertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, vcc::NamingStyle::PascalCase)))
            setIsExcludeVCCUnitTest(json->getBool(vcc::convertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigBehavior::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // ActionHistoryType
        std::wstring actionHistoryTypeValueStr = L"";
        switch (getActionHistoryType())
        {
        case VPGConfigActionHistoryType::NoHistory:
            actionHistoryTypeValueStr = L"NoHistory";
            break;
        case VPGConfigActionHistoryType::Local:
            actionHistoryTypeValueStr = L"Local";
            break;
        case VPGConfigActionHistoryType::Global:
            actionHistoryTypeValueStr = L"Global";
            break;
        default:
            assert(false);
            break;
        }
        json->addString(vcc::convertNamingStyle(L"ActionHistoryType", vcc::NamingStyle::PascalCase, namestyle), actionHistoryTypeValueStr);
        // IsActionResultThrowException
        json->addBool(vcc::convertNamingStyle(L"IsActionResultThrowException", vcc::NamingStyle::PascalCase, namestyle), getIsActionResultThrowException());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigBehavior::deserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // ActionHistoryType
        if (json->isContainKey(vcc::convertNamingStyle(L"ActionHistoryType", namestyle, vcc::NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->getString(vcc::convertNamingStyle(L"ActionHistoryType", namestyle, vcc::NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            vcc::toUpper(valueEnumStrUpper);
            int64_t valueEnum = -1;
            if (valueEnumStrUpper == L"NOHISTORY")
                valueEnum = static_cast<int64_t>(VPGConfigActionHistoryType::NoHistory);
            else if (valueEnumStrUpper == L"LOCAL")
                valueEnum = static_cast<int64_t>(VPGConfigActionHistoryType::Local);
            else if (valueEnumStrUpper == L"GLOBAL")
                valueEnum = static_cast<int64_t>(VPGConfigActionHistoryType::Global);
            if (valueEnum > -1)
                setActionHistoryType(static_cast<VPGConfigActionHistoryType>(valueEnum));
        }
        // IsActionResultThrowException
        if (json->isContainKey(vcc::convertNamingStyle(L"IsActionResultThrowException", namestyle, vcc::NamingStyle::PascalCase)))
            setIsActionResultThrowException(json->getBool(vcc::convertNamingStyle(L"IsActionResultThrowException", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigInput::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // TypeWorkspace
        json->addString(vcc::convertNamingStyle(L"TypeWorkspace", vcc::NamingStyle::PascalCase, namestyle), getTypeWorkspace());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigInput::deserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // TypeWorkspace
        if (json->isContainKey(vcc::convertNamingStyle(L"TypeWorkspace", namestyle, vcc::NamingStyle::PascalCase)))
            setTypeWorkspace(json->getString(vcc::convertNamingStyle(L"TypeWorkspace", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigOutput::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // ExceptionTypeDirectory
        json->addString(vcc::convertNamingStyle(L"ExceptionTypeDirectory", vcc::NamingStyle::PascalCase, namestyle), getExceptionTypeDirectory());
        // ObjectTypeDirectory
        json->addString(vcc::convertNamingStyle(L"ObjectTypeDirectory", vcc::NamingStyle::PascalCase, namestyle), getObjectTypeDirectory());
        // ApplicationDirectoryHpp
        json->addString(vcc::convertNamingStyle(L"ApplicationDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getApplicationDirectoryHpp());
        // ApplicationDirectoryCpp
        json->addString(vcc::convertNamingStyle(L"ApplicationDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getApplicationDirectoryCpp());
        // ActionDirectoryHpp
        json->addString(vcc::convertNamingStyle(L"ActionDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getActionDirectoryHpp());
        // ActionDirectoryCpp
        json->addString(vcc::convertNamingStyle(L"ActionDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getActionDirectoryCpp());
        // FormDirectoryHpp
        json->addString(vcc::convertNamingStyle(L"FormDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getFormDirectoryHpp());
        // FormDirectoryCpp
        json->addString(vcc::convertNamingStyle(L"FormDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getFormDirectoryCpp());
        // ObjectDirectoryHpp
        json->addString(vcc::convertNamingStyle(L"ObjectDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getObjectDirectoryHpp());
        // ObjectDirectoryCpp
        json->addString(vcc::convertNamingStyle(L"ObjectDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getObjectDirectoryCpp());
        // PropertyAccessorDirectoryHpp
        json->addString(vcc::convertNamingStyle(L"PropertyAccessorDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getPropertyAccessorDirectoryHpp());
        // PropertyAccessorDirectoryCpp
        json->addString(vcc::convertNamingStyle(L"PropertyAccessorDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getPropertyAccessorDirectoryCpp());
        // ObjectFactoryDirectoryHpp
        json->addString(vcc::convertNamingStyle(L"ObjectFactoryDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getObjectFactoryDirectoryHpp());
        // ObjectFactoryDirectoryCpp
        json->addString(vcc::convertNamingStyle(L"ObjectFactoryDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getObjectFactoryDirectoryCpp());
        // PropertyAccessorFactoryDirectoryHpp
        json->addString(vcc::convertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getPropertyAccessorFactoryDirectoryHpp());
        // PropertyAccessorFactoryDirectoryCpp
        json->addString(vcc::convertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getPropertyAccessorFactoryDirectoryCpp());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigOutput::deserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // ExceptionTypeDirectory
        if (json->isContainKey(vcc::convertNamingStyle(L"ExceptionTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setExceptionTypeDirectory(json->getString(vcc::convertNamingStyle(L"ExceptionTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectTypeDirectory
        if (json->isContainKey(vcc::convertNamingStyle(L"ObjectTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectTypeDirectory(json->getString(vcc::convertNamingStyle(L"ObjectTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // ApplicationDirectoryHpp
        if (json->isContainKey(vcc::convertNamingStyle(L"ApplicationDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setApplicationDirectoryHpp(json->getString(vcc::convertNamingStyle(L"ApplicationDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ApplicationDirectoryCpp
        if (json->isContainKey(vcc::convertNamingStyle(L"ApplicationDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setApplicationDirectoryCpp(json->getString(vcc::convertNamingStyle(L"ApplicationDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ActionDirectoryHpp
        if (json->isContainKey(vcc::convertNamingStyle(L"ActionDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setActionDirectoryHpp(json->getString(vcc::convertNamingStyle(L"ActionDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ActionDirectoryCpp
        if (json->isContainKey(vcc::convertNamingStyle(L"ActionDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setActionDirectoryCpp(json->getString(vcc::convertNamingStyle(L"ActionDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // FormDirectoryHpp
        if (json->isContainKey(vcc::convertNamingStyle(L"FormDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setFormDirectoryHpp(json->getString(vcc::convertNamingStyle(L"FormDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // FormDirectoryCpp
        if (json->isContainKey(vcc::convertNamingStyle(L"FormDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setFormDirectoryCpp(json->getString(vcc::convertNamingStyle(L"FormDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectDirectoryHpp
        if (json->isContainKey(vcc::convertNamingStyle(L"ObjectDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectDirectoryHpp(json->getString(vcc::convertNamingStyle(L"ObjectDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectDirectoryCpp
        if (json->isContainKey(vcc::convertNamingStyle(L"ObjectDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectDirectoryCpp(json->getString(vcc::convertNamingStyle(L"ObjectDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorDirectoryHpp
        if (json->isContainKey(vcc::convertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setPropertyAccessorDirectoryHpp(json->getString(vcc::convertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorDirectoryCpp
        if (json->isContainKey(vcc::convertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setPropertyAccessorDirectoryCpp(json->getString(vcc::convertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectFactoryDirectoryHpp
        if (json->isContainKey(vcc::convertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectFactoryDirectoryHpp(json->getString(vcc::convertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectFactoryDirectoryCpp
        if (json->isContainKey(vcc::convertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectFactoryDirectoryCpp(json->getString(vcc::convertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorFactoryDirectoryHpp
        if (json->isContainKey(vcc::convertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setPropertyAccessorFactoryDirectoryHpp(json->getString(vcc::convertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorFactoryDirectoryCpp
        if (json->isContainKey(vcc::convertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setPropertyAccessorFactoryDirectoryCpp(json->getString(vcc::convertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigExport::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // Interface
        std::wstring interfaceValueStr = L"";
        switch (getInterface())
        {
        case VPGConfigInterfaceType::Java:
            interfaceValueStr = L"Java";
            break;
        default:
            assert(false);
            break;
        }
        json->addString(vcc::convertNamingStyle(L"Interface", vcc::NamingStyle::PascalCase, namestyle), interfaceValueStr);
        // Workspace
        json->addString(vcc::convertNamingStyle(L"Workspace", vcc::NamingStyle::PascalCase, namestyle), getWorkspace());
        // IsExportExternalLib
        json->addBool(vcc::convertNamingStyle(L"IsExportExternalLib", vcc::NamingStyle::PascalCase, namestyle), getIsExportExternalLib());
        // ExportDirectoryDll
        json->addString(vcc::convertNamingStyle(L"ExportDirectoryDll", vcc::NamingStyle::PascalCase, namestyle), getExportDirectoryDll());
        // ExportDirectoryExe
        json->addString(vcc::convertNamingStyle(L"ExportDirectoryExe", vcc::NamingStyle::PascalCase, namestyle), getExportDirectoryExe());
        // DllBridgeDirectory
        json->addString(vcc::convertNamingStyle(L"DllBridgeDirectory", vcc::NamingStyle::PascalCase, namestyle), getDllBridgeDirectory());
        // FormDirectory
        json->addString(vcc::convertNamingStyle(L"FormDirectory", vcc::NamingStyle::PascalCase, namestyle), getFormDirectory());
        // ObjectDirectory
        json->addString(vcc::convertNamingStyle(L"ObjectDirectory", vcc::NamingStyle::PascalCase, namestyle), getObjectDirectory());
        // TypeDirectory
        json->addString(vcc::convertNamingStyle(L"TypeDirectory", vcc::NamingStyle::PascalCase, namestyle), getTypeDirectory());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigExport::deserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // Interface
        if (json->isContainKey(vcc::convertNamingStyle(L"Interface", namestyle, vcc::NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->getString(vcc::convertNamingStyle(L"Interface", namestyle, vcc::NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            vcc::toUpper(valueEnumStrUpper);
            int64_t valueEnum = -1;
            if (valueEnumStrUpper == L"JAVA")
                valueEnum = static_cast<int64_t>(VPGConfigInterfaceType::Java);
            if (valueEnum > -1)
                setInterface(static_cast<VPGConfigInterfaceType>(valueEnum));
        }
        // Workspace
        if (json->isContainKey(vcc::convertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)))
            setWorkspace(json->getString(vcc::convertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)));
        // IsExportExternalLib
        if (json->isContainKey(vcc::convertNamingStyle(L"IsExportExternalLib", namestyle, vcc::NamingStyle::PascalCase)))
            setIsExportExternalLib(json->getBool(vcc::convertNamingStyle(L"IsExportExternalLib", namestyle, vcc::NamingStyle::PascalCase)));
        // ExportDirectoryDll
        if (json->isContainKey(vcc::convertNamingStyle(L"ExportDirectoryDll", namestyle, vcc::NamingStyle::PascalCase)))
            setExportDirectoryDll(json->getString(vcc::convertNamingStyle(L"ExportDirectoryDll", namestyle, vcc::NamingStyle::PascalCase)));
        // ExportDirectoryExe
        if (json->isContainKey(vcc::convertNamingStyle(L"ExportDirectoryExe", namestyle, vcc::NamingStyle::PascalCase)))
            setExportDirectoryExe(json->getString(vcc::convertNamingStyle(L"ExportDirectoryExe", namestyle, vcc::NamingStyle::PascalCase)));
        // DllBridgeDirectory
        if (json->isContainKey(vcc::convertNamingStyle(L"DllBridgeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setDllBridgeDirectory(json->getString(vcc::convertNamingStyle(L"DllBridgeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // FormDirectory
        if (json->isContainKey(vcc::convertNamingStyle(L"FormDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setFormDirectory(json->getString(vcc::convertNamingStyle(L"FormDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectDirectory
        if (json->isContainKey(vcc::convertNamingStyle(L"ObjectDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectDirectory(json->getString(vcc::convertNamingStyle(L"ObjectDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // TypeDirectory
        if (json->isContainKey(vcc::convertNamingStyle(L"TypeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setTypeDirectory(json->getString(vcc::convertNamingStyle(L"TypeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfig::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // Version
        json->addString(vcc::convertNamingStyle(L"Version", vcc::NamingStyle::PascalCase, namestyle), getVersion());
        // ProjectType
        std::wstring projectTypeValueStr = L"";
        switch (getProjectType())
        {
        case VPGProjectType::VccModule:
            projectTypeValueStr = L"VccModule";
            break;
        case VPGProjectType::VccComplex:
            projectTypeValueStr = L"VccComplex";
            break;
        case VPGProjectType::VccDll:
            projectTypeValueStr = L"VccDll";
            break;
        case VPGProjectType::VccExe:
            projectTypeValueStr = L"VccExe";
            break;
        case VPGProjectType::CppComplex:
            projectTypeValueStr = L"CppComplex";
            break;
        case VPGProjectType::CppDll:
            projectTypeValueStr = L"CppDll";
            break;
        case VPGProjectType::CppExe:
            projectTypeValueStr = L"CppExe";
            break;
        default:
            assert(false);
            break;
        }
        json->addString(vcc::convertNamingStyle(L"ProjectType", vcc::NamingStyle::PascalCase, namestyle), projectTypeValueStr);
        // ProjectPrefix
        json->addString(vcc::convertNamingStyle(L"ProjectPrefix", vcc::NamingStyle::PascalCase, namestyle), getProjectPrefix());
        // ProjectName
        json->addString(vcc::convertNamingStyle(L"ProjectName", vcc::NamingStyle::PascalCase, namestyle), getProjectName());
        // ProjectNameDll
        json->addString(vcc::convertNamingStyle(L"ProjectNameDll", vcc::NamingStyle::PascalCase, namestyle), getProjectNameDll());
        // ProjectNameExe
        json->addString(vcc::convertNamingStyle(L"ProjectNameExe", vcc::NamingStyle::PascalCase, namestyle), getProjectNameExe());
        // IsGit
        json->addBool(vcc::convertNamingStyle(L"IsGit", vcc::NamingStyle::PascalCase, namestyle), getIsGit());
        // Template
        if (getTemplate() != nullptr)
            json->addObject(vcc::convertNamingStyle(L"Template", vcc::NamingStyle::PascalCase, namestyle), getTemplate()->ToJson());
        else
            json->addNull(vcc::convertNamingStyle(L"Template", vcc::NamingStyle::PascalCase, namestyle));
        // Behavior
        if (getBehavior() != nullptr)
            json->addObject(vcc::convertNamingStyle(L"Behavior", vcc::NamingStyle::PascalCase, namestyle), getBehavior()->ToJson());
        else
            json->addNull(vcc::convertNamingStyle(L"Behavior", vcc::NamingStyle::PascalCase, namestyle));
        // Input
        if (getInput() != nullptr)
            json->addObject(vcc::convertNamingStyle(L"Input", vcc::NamingStyle::PascalCase, namestyle), getInput()->ToJson());
        else
            json->addNull(vcc::convertNamingStyle(L"Input", vcc::NamingStyle::PascalCase, namestyle));
        // Output
        if (getOutput() != nullptr)
            json->addObject(vcc::convertNamingStyle(L"Output", vcc::NamingStyle::PascalCase, namestyle), getOutput()->ToJson());
        else
            json->addNull(vcc::convertNamingStyle(L"Output", vcc::NamingStyle::PascalCase, namestyle));
        // Plugins
        auto tmpPlugins = std::make_shared<vcc::Json>();
        json->addArray(vcc::convertNamingStyle(L"Plugins", vcc::NamingStyle::PascalCase, namestyle), tmpPlugins);
        for (auto const &element : getPlugins()) {
            tmpPlugins->addArrayString(element);
        }
        // Exports
        auto tmpExports = std::make_shared<vcc::Json>();
        json->addArray(vcc::convertNamingStyle(L"Exports", vcc::NamingStyle::PascalCase, namestyle), tmpExports);
        for (auto const &element : getExports()) {
            tmpExports->addArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGConfig::deserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // Version
        if (json->isContainKey(vcc::convertNamingStyle(L"Version", namestyle, vcc::NamingStyle::PascalCase)))
            setVersion(json->getString(vcc::convertNamingStyle(L"Version", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectType
        if (json->isContainKey(vcc::convertNamingStyle(L"ProjectType", namestyle, vcc::NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->getString(vcc::convertNamingStyle(L"ProjectType", namestyle, vcc::NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            vcc::toUpper(valueEnumStrUpper);
            int64_t valueEnum = -1;
            if (valueEnumStrUpper == L"VCCMODULE")
                valueEnum = static_cast<int64_t>(VPGProjectType::VccModule);
            else if (valueEnumStrUpper == L"VCCCOMPLEX")
                valueEnum = static_cast<int64_t>(VPGProjectType::VccComplex);
            else if (valueEnumStrUpper == L"VCCDLL")
                valueEnum = static_cast<int64_t>(VPGProjectType::VccDll);
            else if (valueEnumStrUpper == L"VCCEXE")
                valueEnum = static_cast<int64_t>(VPGProjectType::VccExe);
            else if (valueEnumStrUpper == L"CPPCOMPLEX")
                valueEnum = static_cast<int64_t>(VPGProjectType::CppComplex);
            else if (valueEnumStrUpper == L"CPPDLL")
                valueEnum = static_cast<int64_t>(VPGProjectType::CppDll);
            else if (valueEnumStrUpper == L"CPPEXE")
                valueEnum = static_cast<int64_t>(VPGProjectType::CppExe);
            if (valueEnum > -1)
                setProjectType(static_cast<VPGProjectType>(valueEnum));
        }
        // ProjectPrefix
        if (json->isContainKey(vcc::convertNamingStyle(L"ProjectPrefix", namestyle, vcc::NamingStyle::PascalCase)))
            setProjectPrefix(json->getString(vcc::convertNamingStyle(L"ProjectPrefix", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectName
        if (json->isContainKey(vcc::convertNamingStyle(L"ProjectName", namestyle, vcc::NamingStyle::PascalCase)))
            setProjectName(json->getString(vcc::convertNamingStyle(L"ProjectName", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectNameDll
        if (json->isContainKey(vcc::convertNamingStyle(L"ProjectNameDll", namestyle, vcc::NamingStyle::PascalCase)))
            setProjectNameDll(json->getString(vcc::convertNamingStyle(L"ProjectNameDll", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectNameExe
        if (json->isContainKey(vcc::convertNamingStyle(L"ProjectNameExe", namestyle, vcc::NamingStyle::PascalCase)))
            setProjectNameExe(json->getString(vcc::convertNamingStyle(L"ProjectNameExe", namestyle, vcc::NamingStyle::PascalCase)));
        // IsGit
        if (json->isContainKey(vcc::convertNamingStyle(L"IsGit", namestyle, vcc::NamingStyle::PascalCase)))
            setIsGit(json->getBool(vcc::convertNamingStyle(L"IsGit", namestyle, vcc::NamingStyle::PascalCase)));
        // Template
        setTemplate(std::make_shared<VPGConfigTemplate>());
        if (json->isContainKey(vcc::convertNamingStyle(L"Template", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::convertNamingStyle(L"Template", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigTemplate>();
            tmpObject->deserializeJson(json->getObject(vcc::convertNamingStyle(L"Template", namestyle, vcc::NamingStyle::PascalCase)));
            setTemplate(tmpObject);
        }
        // Behavior
        setBehavior(std::make_shared<VPGConfigBehavior>());
        if (json->isContainKey(vcc::convertNamingStyle(L"Behavior", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::convertNamingStyle(L"Behavior", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigBehavior>();
            tmpObject->deserializeJson(json->getObject(vcc::convertNamingStyle(L"Behavior", namestyle, vcc::NamingStyle::PascalCase)));
            setBehavior(tmpObject);
        }
        // Input
        setInput(std::make_shared<VPGConfigInput>());
        if (json->isContainKey(vcc::convertNamingStyle(L"Input", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::convertNamingStyle(L"Input", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigInput>();
            tmpObject->deserializeJson(json->getObject(vcc::convertNamingStyle(L"Input", namestyle, vcc::NamingStyle::PascalCase)));
            setInput(tmpObject);
        }
        // Output
        setOutput(std::make_shared<VPGConfigOutput>());
        if (json->isContainKey(vcc::convertNamingStyle(L"Output", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::convertNamingStyle(L"Output", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigOutput>();
            tmpObject->deserializeJson(json->getObject(vcc::convertNamingStyle(L"Output", namestyle, vcc::NamingStyle::PascalCase)));
            setOutput(tmpObject);
        }
        // Plugins
        clearPlugins();
        if (json->isContainKey(vcc::convertNamingStyle(L"Plugins", namestyle, vcc::NamingStyle::PascalCase))) {
            for (auto const &element : json->getArray(vcc::convertNamingStyle(L"Plugins", namestyle, vcc::NamingStyle::PascalCase))) {
                insertPlugins(element->getArrayElementString());
            }
        }
        // Exports
        clearExports();
        if (json->isContainKey(vcc::convertNamingStyle(L"Exports", namestyle, vcc::NamingStyle::PascalCase))) {
            for (auto const &element : json->getArray(vcc::convertNamingStyle(L"Exports", namestyle, vcc::NamingStyle::PascalCase))) {
                auto tmpExports = std::make_shared<VPGConfigExport>();
                tmpExports->deserializeJson(element->getArrayElementObject());
                insertExports(tmpExports);
            }
        }
    CATCH
}
