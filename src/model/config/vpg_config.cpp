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
        json->AddString(vcc::ConvertNamingStyle(L"Url", vcc::NamingStyle::PascalCase, namestyle), GetUrl());
        // Workspace
        json->AddString(vcc::ConvertNamingStyle(L"Workspace", vcc::NamingStyle::PascalCase, namestyle), GetWorkspace());
        // IsExcludeUnittest
        json->AddBool(vcc::ConvertNamingStyle(L"IsExcludeUnittest", vcc::NamingStyle::PascalCase, namestyle), GetIsExcludeUnittest());
        // IsExcludeVCCUnitTest
        json->AddBool(vcc::ConvertNamingStyle(L"IsExcludeVCCUnitTest", vcc::NamingStyle::PascalCase, namestyle), GetIsExcludeVCCUnitTest());
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
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Url", namestyle, vcc::NamingStyle::PascalCase)))
            SetUrl(json->GetString(vcc::ConvertNamingStyle(L"Url", namestyle, vcc::NamingStyle::PascalCase)));
        // Workspace
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)))
            SetWorkspace(json->GetString(vcc::ConvertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)));
        // IsExcludeUnittest
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"IsExcludeUnittest", namestyle, vcc::NamingStyle::PascalCase)))
            SetIsExcludeUnittest(json->GetBool(vcc::ConvertNamingStyle(L"IsExcludeUnittest", namestyle, vcc::NamingStyle::PascalCase)));
        // IsExcludeVCCUnitTest
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, vcc::NamingStyle::PascalCase)))
            SetIsExcludeVCCUnitTest(json->GetBool(vcc::ConvertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, vcc::NamingStyle::PascalCase)));
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
        json->AddBool(vcc::ConvertNamingStyle(L"IsActionResultThrowException", vcc::NamingStyle::PascalCase, namestyle), GetIsActionResultThrowException());
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
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ActionHistoryType", namestyle, vcc::NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->GetString(vcc::ConvertNamingStyle(L"ActionHistoryType", namestyle, vcc::NamingStyle::PascalCase));
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
                SetActionHistoryType(static_cast<VPGConfigActionHistoryType>(valueEnum));
        }
        // IsActionResultThrowException
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"IsActionResultThrowException", namestyle, vcc::NamingStyle::PascalCase)))
            SetIsActionResultThrowException(json->GetBool(vcc::ConvertNamingStyle(L"IsActionResultThrowException", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigInput::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // TypeWorkspace
        json->AddString(vcc::ConvertNamingStyle(L"TypeWorkspace", vcc::NamingStyle::PascalCase, namestyle), GetTypeWorkspace());
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
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"TypeWorkspace", namestyle, vcc::NamingStyle::PascalCase)))
            SetTypeWorkspace(json->GetString(vcc::ConvertNamingStyle(L"TypeWorkspace", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigOutput::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // ExceptionTypeDirectory
        json->AddString(vcc::ConvertNamingStyle(L"ExceptionTypeDirectory", vcc::NamingStyle::PascalCase, namestyle), GetExceptionTypeDirectory());
        // ObjectTypeDirectory
        json->AddString(vcc::ConvertNamingStyle(L"ObjectTypeDirectory", vcc::NamingStyle::PascalCase, namestyle), GetObjectTypeDirectory());
        // ApplicationDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"ApplicationDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), GetApplicationDirectoryHpp());
        // ApplicationDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"ApplicationDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), GetApplicationDirectoryCpp());
        // ActionDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"ActionDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), GetActionDirectoryHpp());
        // ActionDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"ActionDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), GetActionDirectoryCpp());
        // FormDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"FormDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), GetFormDirectoryHpp());
        // FormDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"FormDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), GetFormDirectoryCpp());
        // ObjectDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"ObjectDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), GetObjectDirectoryHpp());
        // ObjectDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"ObjectDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), GetObjectDirectoryCpp());
        // PropertyAccessorDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), GetPropertyAccessorDirectoryHpp());
        // PropertyAccessorDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), GetPropertyAccessorDirectoryCpp());
        // ObjectFactoryDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), GetObjectFactoryDirectoryHpp());
        // ObjectFactoryDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), GetObjectFactoryDirectoryCpp());
        // PropertyAccessorFactoryDirectoryHpp
        json->AddString(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", vcc::NamingStyle::PascalCase, namestyle), GetPropertyAccessorFactoryDirectoryHpp());
        // PropertyAccessorFactoryDirectoryCpp
        json->AddString(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", vcc::NamingStyle::PascalCase, namestyle), GetPropertyAccessorFactoryDirectoryCpp());
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
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            SetExceptionTypeDirectory(json->GetString(vcc::ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectTypeDirectory
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            SetObjectTypeDirectory(json->GetString(vcc::ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // ApplicationDirectoryHpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ApplicationDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetApplicationDirectoryHpp(json->GetString(vcc::ConvertNamingStyle(L"ApplicationDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ApplicationDirectoryCpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ApplicationDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetApplicationDirectoryCpp(json->GetString(vcc::ConvertNamingStyle(L"ApplicationDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ActionDirectoryHpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ActionDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetActionDirectoryHpp(json->GetString(vcc::ConvertNamingStyle(L"ActionDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ActionDirectoryCpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ActionDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetActionDirectoryCpp(json->GetString(vcc::ConvertNamingStyle(L"ActionDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // FormDirectoryHpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"FormDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetFormDirectoryHpp(json->GetString(vcc::ConvertNamingStyle(L"FormDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // FormDirectoryCpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"FormDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetFormDirectoryCpp(json->GetString(vcc::ConvertNamingStyle(L"FormDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectDirectoryHpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ObjectDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetObjectDirectoryHpp(json->GetString(vcc::ConvertNamingStyle(L"ObjectDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectDirectoryCpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ObjectDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetObjectDirectoryCpp(json->GetString(vcc::ConvertNamingStyle(L"ObjectDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorDirectoryHpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetPropertyAccessorDirectoryHpp(json->GetString(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorDirectoryCpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetPropertyAccessorDirectoryCpp(json->GetString(vcc::ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectFactoryDirectoryHpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetObjectFactoryDirectoryHpp(json->GetString(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectFactoryDirectoryCpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetObjectFactoryDirectoryCpp(json->GetString(vcc::ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorFactoryDirectoryHpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetPropertyAccessorFactoryDirectoryHpp(json->GetString(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, vcc::NamingStyle::PascalCase)));
        // PropertyAccessorFactoryDirectoryCpp
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)))
            SetPropertyAccessorFactoryDirectoryCpp(json->GetString(vcc::ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfigExport::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // Interface
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
        json->AddString(vcc::ConvertNamingStyle(L"Workspace", vcc::NamingStyle::PascalCase, namestyle), GetWorkspace());
        // IsExportExternalLib
        json->AddBool(vcc::ConvertNamingStyle(L"IsExportExternalLib", vcc::NamingStyle::PascalCase, namestyle), GetIsExportExternalLib());
        // ExportDirectoryDll
        json->AddString(vcc::ConvertNamingStyle(L"ExportDirectoryDll", vcc::NamingStyle::PascalCase, namestyle), GetExportDirectoryDll());
        // ExportDirectoryExe
        json->AddString(vcc::ConvertNamingStyle(L"ExportDirectoryExe", vcc::NamingStyle::PascalCase, namestyle), GetExportDirectoryExe());
        // DllBridgeDirectory
        json->AddString(vcc::ConvertNamingStyle(L"DllBridgeDirectory", vcc::NamingStyle::PascalCase, namestyle), GetDllBridgeDirectory());
        // FormDirectory
        json->AddString(vcc::ConvertNamingStyle(L"FormDirectory", vcc::NamingStyle::PascalCase, namestyle), GetFormDirectory());
        // ObjectDirectory
        json->AddString(vcc::ConvertNamingStyle(L"ObjectDirectory", vcc::NamingStyle::PascalCase, namestyle), GetObjectDirectory());
        // TypeDirectory
        json->AddString(vcc::ConvertNamingStyle(L"TypeDirectory", vcc::NamingStyle::PascalCase, namestyle), GetTypeDirectory());
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
        // Interface
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Interface", namestyle, vcc::NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->GetString(vcc::ConvertNamingStyle(L"Interface", namestyle, vcc::NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            vcc::ToUpper(valueEnumStrUpper);
            int64_t valueEnum = -1;
            if (valueEnumStrUpper == L"JAVA")
                valueEnum = static_cast<int64_t>(VPGConfigInterfaceType::Java);
            if (valueEnum > -1)
                SetInterface(static_cast<VPGConfigInterfaceType>(valueEnum));
        }
        // Workspace
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)))
            SetWorkspace(json->GetString(vcc::ConvertNamingStyle(L"Workspace", namestyle, vcc::NamingStyle::PascalCase)));
        // IsExportExternalLib
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"IsExportExternalLib", namestyle, vcc::NamingStyle::PascalCase)))
            SetIsExportExternalLib(json->GetBool(vcc::ConvertNamingStyle(L"IsExportExternalLib", namestyle, vcc::NamingStyle::PascalCase)));
        // ExportDirectoryDll
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ExportDirectoryDll", namestyle, vcc::NamingStyle::PascalCase)))
            SetExportDirectoryDll(json->GetString(vcc::ConvertNamingStyle(L"ExportDirectoryDll", namestyle, vcc::NamingStyle::PascalCase)));
        // ExportDirectoryExe
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ExportDirectoryExe", namestyle, vcc::NamingStyle::PascalCase)))
            SetExportDirectoryExe(json->GetString(vcc::ConvertNamingStyle(L"ExportDirectoryExe", namestyle, vcc::NamingStyle::PascalCase)));
        // DllBridgeDirectory
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"DllBridgeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            SetDllBridgeDirectory(json->GetString(vcc::ConvertNamingStyle(L"DllBridgeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // FormDirectory
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"FormDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            SetFormDirectory(json->GetString(vcc::ConvertNamingStyle(L"FormDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // ObjectDirectory
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ObjectDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            SetObjectDirectory(json->GetString(vcc::ConvertNamingStyle(L"ObjectDirectory", namestyle, vcc::NamingStyle::PascalCase)));
        // TypeDirectory
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"TypeDirectory", namestyle, vcc::NamingStyle::PascalCase)))
            SetTypeDirectory(json->GetString(vcc::ConvertNamingStyle(L"TypeDirectory", namestyle, vcc::NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<vcc::Json> VPGConfig::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // Version
        json->AddString(vcc::ConvertNamingStyle(L"Version", vcc::NamingStyle::PascalCase, namestyle), GetVersion());
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
        json->AddString(vcc::ConvertNamingStyle(L"ProjectPrefix", vcc::NamingStyle::PascalCase, namestyle), GetProjectPrefix());
        // ProjectName
        json->AddString(vcc::ConvertNamingStyle(L"ProjectName", vcc::NamingStyle::PascalCase, namestyle), GetProjectName());
        // ProjectNameDll
        json->AddString(vcc::ConvertNamingStyle(L"ProjectNameDll", vcc::NamingStyle::PascalCase, namestyle), GetProjectNameDll());
        // ProjectNameExe
        json->AddString(vcc::ConvertNamingStyle(L"ProjectNameExe", vcc::NamingStyle::PascalCase, namestyle), GetProjectNameExe());
        // IsGit
        json->AddBool(vcc::ConvertNamingStyle(L"IsGit", vcc::NamingStyle::PascalCase, namestyle), GetIsGit());
        // Template
        if (GetTemplate() != nullptr)
            json->AddObject(vcc::ConvertNamingStyle(L"Template", vcc::NamingStyle::PascalCase, namestyle), GetTemplate()->ToJson());
        else
            json->AddNull(vcc::ConvertNamingStyle(L"Template", vcc::NamingStyle::PascalCase, namestyle));
        // Behavior
        if (GetBehavior() != nullptr)
            json->AddObject(vcc::ConvertNamingStyle(L"Behavior", vcc::NamingStyle::PascalCase, namestyle), GetBehavior()->ToJson());
        else
            json->AddNull(vcc::ConvertNamingStyle(L"Behavior", vcc::NamingStyle::PascalCase, namestyle));
        // Input
        if (GetInput() != nullptr)
            json->AddObject(vcc::ConvertNamingStyle(L"Input", vcc::NamingStyle::PascalCase, namestyle), GetInput()->ToJson());
        else
            json->AddNull(vcc::ConvertNamingStyle(L"Input", vcc::NamingStyle::PascalCase, namestyle));
        // Output
        if (GetOutput() != nullptr)
            json->AddObject(vcc::ConvertNamingStyle(L"Output", vcc::NamingStyle::PascalCase, namestyle), GetOutput()->ToJson());
        else
            json->AddNull(vcc::ConvertNamingStyle(L"Output", vcc::NamingStyle::PascalCase, namestyle));
        // Plugins
        auto tmpPlugins = std::make_shared<vcc::Json>();
        json->AddArray(vcc::ConvertNamingStyle(L"Plugins", vcc::NamingStyle::PascalCase, namestyle), tmpPlugins);
        for (auto const &element : GetPlugins()) {
            tmpPlugins->AddArrayString(element);
        }
        // Exports
        auto tmpExports = std::make_shared<vcc::Json>();
        json->AddArray(vcc::ConvertNamingStyle(L"Exports", vcc::NamingStyle::PascalCase, namestyle), tmpExports);
        for (auto const &element : GetExports()) {
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
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Version", namestyle, vcc::NamingStyle::PascalCase)))
            SetVersion(json->GetString(vcc::ConvertNamingStyle(L"Version", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectType
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ProjectType", namestyle, vcc::NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->GetString(vcc::ConvertNamingStyle(L"ProjectType", namestyle, vcc::NamingStyle::PascalCase));
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
                SetProjectType(static_cast<VPGProjectType>(valueEnum));
        }
        // ProjectPrefix
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ProjectPrefix", namestyle, vcc::NamingStyle::PascalCase)))
            SetProjectPrefix(json->GetString(vcc::ConvertNamingStyle(L"ProjectPrefix", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectName
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ProjectName", namestyle, vcc::NamingStyle::PascalCase)))
            SetProjectName(json->GetString(vcc::ConvertNamingStyle(L"ProjectName", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectNameDll
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ProjectNameDll", namestyle, vcc::NamingStyle::PascalCase)))
            SetProjectNameDll(json->GetString(vcc::ConvertNamingStyle(L"ProjectNameDll", namestyle, vcc::NamingStyle::PascalCase)));
        // ProjectNameExe
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"ProjectNameExe", namestyle, vcc::NamingStyle::PascalCase)))
            SetProjectNameExe(json->GetString(vcc::ConvertNamingStyle(L"ProjectNameExe", namestyle, vcc::NamingStyle::PascalCase)));
        // IsGit
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"IsGit", namestyle, vcc::NamingStyle::PascalCase)))
            SetIsGit(json->GetBool(vcc::ConvertNamingStyle(L"IsGit", namestyle, vcc::NamingStyle::PascalCase)));
        // Template
        SetTemplate(std::make_shared<VPGConfigTemplate>());
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Template", namestyle, vcc::NamingStyle::PascalCase)) && json->GetObject(vcc::ConvertNamingStyle(L"Template", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigTemplate>();
            tmpObject->DeserializeJson(json->GetObject(vcc::ConvertNamingStyle(L"Template", namestyle, vcc::NamingStyle::PascalCase)));
            SetTemplate(tmpObject);
        }
        // Behavior
        SetBehavior(std::make_shared<VPGConfigBehavior>());
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Behavior", namestyle, vcc::NamingStyle::PascalCase)) && json->GetObject(vcc::ConvertNamingStyle(L"Behavior", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigBehavior>();
            tmpObject->DeserializeJson(json->GetObject(vcc::ConvertNamingStyle(L"Behavior", namestyle, vcc::NamingStyle::PascalCase)));
            SetBehavior(tmpObject);
        }
        // Input
        SetInput(std::make_shared<VPGConfigInput>());
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Input", namestyle, vcc::NamingStyle::PascalCase)) && json->GetObject(vcc::ConvertNamingStyle(L"Input", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigInput>();
            tmpObject->DeserializeJson(json->GetObject(vcc::ConvertNamingStyle(L"Input", namestyle, vcc::NamingStyle::PascalCase)));
            SetInput(tmpObject);
        }
        // Output
        SetOutput(std::make_shared<VPGConfigOutput>());
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Output", namestyle, vcc::NamingStyle::PascalCase)) && json->GetObject(vcc::ConvertNamingStyle(L"Output", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigOutput>();
            tmpObject->DeserializeJson(json->GetObject(vcc::ConvertNamingStyle(L"Output", namestyle, vcc::NamingStyle::PascalCase)));
            SetOutput(tmpObject);
        }
        // Plugins
        ClearPlugins();
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Plugins", namestyle, vcc::NamingStyle::PascalCase))) {
            for (auto const &element : json->GetArray(vcc::ConvertNamingStyle(L"Plugins", namestyle, vcc::NamingStyle::PascalCase))) {
                InsertPlugins(element->GetArrayElementString());
            }
        }
        // Exports
        ClearExports();
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Exports", namestyle, vcc::NamingStyle::PascalCase))) {
            for (auto const &element : json->GetArray(vcc::ConvertNamingStyle(L"Exports", namestyle, vcc::NamingStyle::PascalCase))) {
                auto tmpExports = std::make_shared<VPGConfigExport>();
                tmpExports->DeserializeJson(element->GetArrayElementObject());
                InsertExports(tmpExports);
            }
        }
    CATCH
}
