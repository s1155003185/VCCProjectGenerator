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
        json->AddString(vcc::ConvertNamingStyle(L"Url", vcc::NamingStyle::PascalCase, namestyle), getUrl());
        // Workspace
        json->AddString(vcc::ConvertNamingStyle(L"Workspace", vcc::NamingStyle::PascalCase, namestyle), getWorkspace());
        // IsExcludeUnittest
        json->AddBool(vcc::ConvertNamingStyle(L"IsExcludeUnittest", vcc::NamingStyle::PascalCase, namestyle), getIsExcludeUnittest());
        // IsExcludeVCCUnitTest
        json->AddBool(vcc::ConvertNamingStyle(L"IsExcludeVCCUnitTest", vcc::NamingStyle::PascalCase, namestyle), getIsExcludeVCCUnitTest());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigTemplate::DeserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // Url
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Url", namestyle, vcc::NamingStyle::PascalCase)))
            setUrl(json->getString(vcc::ConvertNamingStyle(L"Url", namestyle, vcc::NamingStyle::PascalCase)));
        // Workspace
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)))
            setWorkspace(json->getString(vcc::ConvertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)));
        // IsExcludeUnittest
        if (json->isContainKey(vcc::ConvertNamingStyle(L"IsExcludeUnittest", namestyle, vcc::NamingStyle::PascalCase)))
            setIsExcludeUnittest(json->getBool(vcc::ConvertNamingStyle(L"IsExcludeUnittest", namestyle, vcc::NamingStyle::PascalCase)));
        // IsExcludeVCCUnitTest
        if (json->isContainKey(vcc::ConvertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, vcc::NamingStyle::PascalCase)))
            setIsExcludeVCCUnitTest(json->getBool(vcc::ConvertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigBehavior::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // ActionHistoryType
        std::wstring actionHistoryTypeValueStr = L"";
        switch (GetActionHistoryType())
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
        json->AddString(vcc::ConvertNamingStyle(L"ActionHistoryType", vcc::NamingStyle::PascalCase, namestyle), actionHistoryTypeValueStr);
        // IsActionResultThrowException
        json->AddBool(vcc::ConvertNamingStyle(L"IsActionResultThrowException", vcc::NamingStyle::PascalCase, namestyle), getIsActionResultThrowException());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigBehavior::DeserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // ActionHistoryType
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ActionHistoryType", namestyle, vcc::NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->getString(vcc::ConvertNamingStyle(L"ActionHistoryType", namestyle, vcc::NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            vcc::ToUpper(valueEnumStrUpper);
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
        if (json->isContainKey(vcc::ConvertNamingStyle(L"IsActionResultThrowException", namestyle, vcc::NamingStyle::PascalCase)))
            setIsActionResultThrowException(json->getBool(vcc::ConvertNamingStyle(L"IsActionResultThrowException", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigInput::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // TypeWorkspace
        json->AddString(vcc::ConvertNamingStyle(L"TypeWorkspace", vcc::NamingStyle::PascalCase, namestyle), getTypeWorkspace());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigInput::DeserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // TypeWorkspace
        if (json->isContainKey(vcc::ConvertNamingStyle(L"TypeWorkspace", namestyle, vcc::NamingStyle::PascalCase)))
            setTypeWorkspace(json->getString(vcc::ConvertNamingStyle(L"TypeWorkspace", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigOutput::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // ExceptionTypeDirectory
        json->AddString(vcc::ConvertNamingStyle(L"ExceptionTypeDirectory", vcc::NamingStyle::PascalCase, namestyle), getExceptionTypeDirectory());
        // ObjectTypeDirectory
        json->AddString(vcc::ConvertNamingStyle(L"ObjectTypeDirectory", vcc::NamingStyle::PascalCase, namestyle), getObjectTypeDirectory());
        // ApplicationDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"ApplicationDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getApplicationDirectoryHpp());
        // ApplicationDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"ApplicationDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getApplicationDirectoryCpp());
        // ActionDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"ActionDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getActionDirectoryHpp());
        // ActionDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"ActionDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getActionDirectoryCpp());
        // FormDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"FormDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getFormDirectoryHpp());
        // FormDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"FormDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getFormDirectoryCpp());
        // ObjectDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"ObjectDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getObjectDirectoryHpp());
        // ObjectDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"ObjectDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getObjectDirectoryCpp());
        // PropertyAccessorDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getPropertyAccessorDirectoryHpp());
        // PropertyAccessorDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getPropertyAccessorDirectoryCpp());
        // ObjectFactoryDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getObjectFactoryDirectoryHpp());
        // ObjectFactoryDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getObjectFactoryDirectoryCpp());
        // PropertyAccessorFactoryDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), getPropertyAccessorFactoryDirectoryHpp());
        // PropertyAccessorFactoryDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), getPropertyAccessorFactoryDirectoryCpp());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigOutput::DeserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // ExceptionTypeDirectory
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setExceptionTypeDirectory(json->getString(vcc::ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectTypeDirectory
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectTypeDirectory(json->getString(vcc::ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // ApplicationDirectoryHpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ApplicationDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setApplicationDirectoryHpp(json->getString(vcc::ConvertNamingStyle(L"ApplicationDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ApplicationDirectoryCpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ApplicationDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setApplicationDirectoryCpp(json->getString(vcc::ConvertNamingStyle(L"ApplicationDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ActionDirectoryHpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ActionDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setActionDirectoryHpp(json->getString(vcc::ConvertNamingStyle(L"ActionDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ActionDirectoryCpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ActionDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setActionDirectoryCpp(json->getString(vcc::ConvertNamingStyle(L"ActionDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // FormDirectoryHpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"FormDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setFormDirectoryHpp(json->getString(vcc::ConvertNamingStyle(L"FormDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // FormDirectoryCpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"FormDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setFormDirectoryCpp(json->getString(vcc::ConvertNamingStyle(L"FormDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectDirectoryHpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ObjectDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectDirectoryHpp(json->getString(vcc::ConvertNamingStyle(L"ObjectDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectDirectoryCpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ObjectDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectDirectoryCpp(json->getString(vcc::ConvertNamingStyle(L"ObjectDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorDirectoryHpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setPropertyAccessorDirectoryHpp(json->getString(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorDirectoryCpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setPropertyAccessorDirectoryCpp(json->getString(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectFactoryDirectoryHpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectFactoryDirectoryHpp(json->getString(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectFactoryDirectoryCpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectFactoryDirectoryCpp(json->getString(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorFactoryDirectoryHpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            setPropertyAccessorFactoryDirectoryHpp(json->getString(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorFactoryDirectoryCpp
        if (json->isContainKey(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            setPropertyAccessorFactoryDirectoryCpp(json->getString(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigExport::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // interface
        std::wstring interfaceValueStr = L"";
        switch (GetInterface())
        {
        case VPGConfigInterfaceType::Java:
            interfaceValueStr = L"Java";
            break;
        default:
            assert(false);
            break;
        }
        json->AddString(vcc::ConvertNamingStyle(L"Interface", vcc::NamingStyle::PascalCase, namestyle), interfaceValueStr);
        // Workspace
        json->AddString(vcc::ConvertNamingStyle(L"Workspace", vcc::NamingStyle::PascalCase, namestyle), getWorkspace());
        // IsExportExternalLib
        json->AddBool(vcc::ConvertNamingStyle(L"IsExportExternalLib", vcc::NamingStyle::PascalCase, namestyle), getIsExportExternalLib());
        // ExportDirectoryDll
        json->AddString(vcc::ConvertNamingStyle(L"ExportDirectoryDll", vcc::NamingStyle::PascalCase, namestyle), getExportDirectoryDll());
        // ExportDirectoryExe
        json->AddString(vcc::ConvertNamingStyle(L"ExportDirectoryExe", vcc::NamingStyle::PascalCase, namestyle), getExportDirectoryExe());
        // DllBridgeDirectory
        json->AddString(vcc::ConvertNamingStyle(L"DllBridgeDirectory", vcc::NamingStyle::PascalCase, namestyle), getDllBridgeDirectory());
        // FormDirectory
        json->AddString(vcc::ConvertNamingStyle(L"FormDirectory", vcc::NamingStyle::PascalCase, namestyle), getFormDirectory());
        // ObjectDirectory
        json->AddString(vcc::ConvertNamingStyle(L"ObjectDirectory", vcc::NamingStyle::PascalCase, namestyle), getObjectDirectory());
        // TypeDirectory
        json->AddString(vcc::ConvertNamingStyle(L"TypeDirectory", vcc::NamingStyle::PascalCase, namestyle), getTypeDirectory());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigExport::DeserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // interface
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Interface", namestyle, vcc::NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->getString(vcc::ConvertNamingStyle(L"Interface", namestyle, vcc::NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            vcc::ToUpper(valueEnumStrUpper);
            int64_t valueEnum = -1;
            if (valueEnumStrUpper == L"JAVA")
                valueEnum = static_cast<int64_t>(VPGConfigInterfaceType::Java);
            if (valueEnum > -1)
                setInterface(static_cast<VPGConfigInterfaceType>(valueEnum));
        }
        // Workspace
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)))
            setWorkspace(json->getString(vcc::ConvertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)));
        // IsExportExternalLib
        if (json->isContainKey(vcc::ConvertNamingStyle(L"IsExportExternalLib", namestyle, vcc::NamingStyle::PascalCase)))
            setIsExportExternalLib(json->getBool(vcc::ConvertNamingStyle(L"IsExportExternalLib", namestyle, vcc::NamingStyle::PascalCase)));
        // ExportDirectoryDll
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ExportDirectoryDll", namestyle, vcc::NamingStyle::PascalCase)))
            setExportDirectoryDll(json->getString(vcc::ConvertNamingStyle(L"ExportDirectoryDll", namestyle, vcc::NamingStyle::PascalCase)));
        // ExportDirectoryExe
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ExportDirectoryExe", namestyle, vcc::NamingStyle::PascalCase)))
            setExportDirectoryExe(json->getString(vcc::ConvertNamingStyle(L"ExportDirectoryExe", namestyle, vcc::NamingStyle::PascalCase)));
        // DllBridgeDirectory
        if (json->isContainKey(vcc::ConvertNamingStyle(L"DllBridgeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setDllBridgeDirectory(json->getString(vcc::ConvertNamingStyle(L"DllBridgeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // FormDirectory
        if (json->isContainKey(vcc::ConvertNamingStyle(L"FormDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setFormDirectory(json->getString(vcc::ConvertNamingStyle(L"FormDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectDirectory
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ObjectDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setObjectDirectory(json->getString(vcc::ConvertNamingStyle(L"ObjectDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // TypeDirectory
        if (json->isContainKey(vcc::ConvertNamingStyle(L"TypeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            setTypeDirectory(json->getString(vcc::ConvertNamingStyle(L"TypeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfig::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // Version
        json->AddString(vcc::ConvertNamingStyle(L"Version", vcc::NamingStyle::PascalCase, namestyle), getVersion());
        // ProjectType
        std::wstring projectTypeValueStr = L"";
        switch (GetProjectType())
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
        json->AddString(vcc::ConvertNamingStyle(L"ProjectType", vcc::NamingStyle::PascalCase, namestyle), projectTypeValueStr);
        // ProjectPrefix
        json->AddString(vcc::ConvertNamingStyle(L"ProjectPrefix", vcc::NamingStyle::PascalCase, namestyle), getProjectPrefix());
        // ProjectName
        json->AddString(vcc::ConvertNamingStyle(L"ProjectName", vcc::NamingStyle::PascalCase, namestyle), getProjectName());
        // ProjectNameDll
        json->AddString(vcc::ConvertNamingStyle(L"ProjectNameDll", vcc::NamingStyle::PascalCase, namestyle), getProjectNameDll());
        // ProjectNameExe
        json->AddString(vcc::ConvertNamingStyle(L"ProjectNameExe", vcc::NamingStyle::PascalCase, namestyle), getProjectNameExe());
        // IsGit
        json->AddBool(vcc::ConvertNamingStyle(L"IsGit", vcc::NamingStyle::PascalCase, namestyle), getIsGit());
        // Template
        if (GetTemplate() != nullptr)
            json->AddObject(vcc::ConvertNamingStyle(L"Template", vcc::NamingStyle::PascalCase, namestyle), getTemplate()->ToJson());
        else
            json->AddNull(vcc::ConvertNamingStyle(L"Template", vcc::NamingStyle::PascalCase, namestyle));
        // Behavior
        if (GetBehavior() != nullptr)
            json->AddObject(vcc::ConvertNamingStyle(L"Behavior", vcc::NamingStyle::PascalCase, namestyle), getBehavior()->ToJson());
        else
            json->AddNull(vcc::ConvertNamingStyle(L"Behavior", vcc::NamingStyle::PascalCase, namestyle));
        // input
        if (GetInput() != nullptr)
            json->AddObject(vcc::ConvertNamingStyle(L"Input", vcc::NamingStyle::PascalCase, namestyle), getInput()->ToJson());
        else
            json->AddNull(vcc::ConvertNamingStyle(L"Input", vcc::NamingStyle::PascalCase, namestyle));
        // Output
        if (GetOutput() != nullptr)
            json->AddObject(vcc::ConvertNamingStyle(L"Output", vcc::NamingStyle::PascalCase, namestyle), getOutput()->ToJson());
        else
            json->AddNull(vcc::ConvertNamingStyle(L"Output", vcc::NamingStyle::PascalCase, namestyle));
        // Plugins
        auto tmpPlugins = std::make_shared<vcc::Json>();
        json->AddArray(vcc::ConvertNamingStyle(L"Plugins", vcc::NamingStyle::PascalCase, namestyle), tmpPlugins);
        for (auto const &element : getPlugins()) {
            tmpPlugins->AddArrayString(element);
        }
        // Exports
        auto tmpExports = std::make_shared<vcc::Json>();
        json->AddArray(vcc::ConvertNamingStyle(L"Exports", vcc::NamingStyle::PascalCase, namestyle), tmpExports);
        for (auto const &element : getExports()) {
            tmpExports->AddArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGConfig::DeserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // Version
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Version", namestyle, vcc::NamingStyle::PascalCase)))
            setVersion(json->getString(vcc::ConvertNamingStyle(L"Version", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectType
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ProjectType", namestyle, vcc::NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->getString(vcc::ConvertNamingStyle(L"ProjectType", namestyle, vcc::NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            vcc::ToUpper(valueEnumStrUpper);
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
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ProjectPrefix", namestyle, vcc::NamingStyle::PascalCase)))
            setProjectPrefix(json->getString(vcc::ConvertNamingStyle(L"ProjectPrefix", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectName
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ProjectName", namestyle, vcc::NamingStyle::PascalCase)))
            setProjectName(json->getString(vcc::ConvertNamingStyle(L"ProjectName", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectNameDll
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ProjectNameDll", namestyle, vcc::NamingStyle::PascalCase)))
            setProjectNameDll(json->getString(vcc::ConvertNamingStyle(L"ProjectNameDll", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectNameExe
        if (json->isContainKey(vcc::ConvertNamingStyle(L"ProjectNameExe", namestyle, vcc::NamingStyle::PascalCase)))
            setProjectNameExe(json->getString(vcc::ConvertNamingStyle(L"ProjectNameExe", namestyle, vcc::NamingStyle::PascalCase)));
        // IsGit
        if (json->isContainKey(vcc::ConvertNamingStyle(L"IsGit", namestyle, vcc::NamingStyle::PascalCase)))
            setIsGit(json->getBool(vcc::ConvertNamingStyle(L"IsGit", namestyle, vcc::NamingStyle::PascalCase)));
        // Template
        setTemplate(std::make_shared<VPGConfigTemplate>());
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Template", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::ConvertNamingStyle(L"Template", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigTemplate>();
            tmpObject->DeserializeJson(json->getObject(vcc::ConvertNamingStyle(L"Template", namestyle, vcc::NamingStyle::PascalCase)));
            setTemplate(tmpObject);
        }
        // Behavior
        setBehavior(std::make_shared<VPGConfigBehavior>());
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Behavior", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::ConvertNamingStyle(L"Behavior", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigBehavior>();
            tmpObject->DeserializeJson(json->getObject(vcc::ConvertNamingStyle(L"Behavior", namestyle, vcc::NamingStyle::PascalCase)));
            setBehavior(tmpObject);
        }
        // input
        setInput(std::make_shared<VPGConfigInput>());
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Input", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::ConvertNamingStyle(L"Input", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigInput>();
            tmpObject->DeserializeJson(json->getObject(vcc::ConvertNamingStyle(L"Input", namestyle, vcc::NamingStyle::PascalCase)));
            setInput(tmpObject);
        }
        // Output
        setOutput(std::make_shared<VPGConfigOutput>());
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Output", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::ConvertNamingStyle(L"Output", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigOutput>();
            tmpObject->DeserializeJson(json->getObject(vcc::ConvertNamingStyle(L"Output", namestyle, vcc::NamingStyle::PascalCase)));
            setOutput(tmpObject);
        }
        // Plugins
        ClearPlugins();
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Plugins", namestyle, vcc::NamingStyle::PascalCase))) {
            for (auto const &element : json->getArray(vcc::ConvertNamingStyle(L"Plugins", namestyle, vcc::NamingStyle::PascalCase))) {
                insertPlugins(element->getArrayElementString());
            }
        }
        // Exports
        ClearExports();
        if (json->isContainKey(vcc::ConvertNamingStyle(L"Exports", namestyle, vcc::NamingStyle::PascalCase))) {
            for (auto const &element : json->getArray(vcc::ConvertNamingStyle(L"Exports", namestyle, vcc::NamingStyle::PascalCase))) {
                auto tmpExports = std::make_shared<VPGConfigExport>();
                tmpExports->DeserializeJson(element->getArrayElementObject());
                insertExports(tmpExports);
            }
        }
    CATCH
}
