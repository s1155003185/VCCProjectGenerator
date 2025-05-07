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

using namespace vcc;

std::shared_ptr<Json> VPGConfigTemplate::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::make_unique<Json>();
        // Url
        json->AddString(ConvertNamingStyle(L"Url", NamingStyle::PascalCase, namestyle), GetUrl());
        // Workspace
        json->AddString(ConvertNamingStyle(L"Workspace", NamingStyle::PascalCase, namestyle), GetWorkspace());
        // IsExcludeUnittest
        json->AddBool(ConvertNamingStyle(L"IsExcludeUnittest", NamingStyle::PascalCase, namestyle), GetIsExcludeUnittest());
        // IsExcludeVCCUnitTest
        json->AddBool(ConvertNamingStyle(L"IsExcludeVCCUnitTest", NamingStyle::PascalCase, namestyle), GetIsExcludeVCCUnitTest());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigTemplate::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // Url
        if (json->IsContainKey(ConvertNamingStyle(L"Url", namestyle, NamingStyle::PascalCase)))
            SetUrl(json->GetString(ConvertNamingStyle(L"Url", namestyle, NamingStyle::PascalCase)));
        // Workspace
        if (json->IsContainKey(ConvertNamingStyle(L"Workspace", namestyle, NamingStyle::PascalCase)))
            SetWorkspace(json->GetString(ConvertNamingStyle(L"Workspace", namestyle, NamingStyle::PascalCase)));
        // IsExcludeUnittest
        if (json->IsContainKey(ConvertNamingStyle(L"IsExcludeUnittest", namestyle, NamingStyle::PascalCase)))
            SetIsExcludeUnittest(json->GetBool(ConvertNamingStyle(L"IsExcludeUnittest", namestyle, NamingStyle::PascalCase)));
        // IsExcludeVCCUnitTest
        if (json->IsContainKey(ConvertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, NamingStyle::PascalCase)))
            SetIsExcludeVCCUnitTest(json->GetBool(ConvertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<Json> VPGConfigBehavior::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::make_unique<Json>();
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
        json->AddString(ConvertNamingStyle(L"ActionHistoryType", NamingStyle::PascalCase, namestyle), actionHistoryTypeValueStr);
        // IsActionResultThrowException
        json->AddBool(ConvertNamingStyle(L"IsActionResultThrowException", NamingStyle::PascalCase, namestyle), GetIsActionResultThrowException());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigBehavior::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // ActionHistoryType
        if (json->IsContainKey(ConvertNamingStyle(L"ActionHistoryType", namestyle, NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->GetString(ConvertNamingStyle(L"ActionHistoryType", namestyle, NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            ToUpper(valueEnumStrUpper);
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
        if (json->IsContainKey(ConvertNamingStyle(L"IsActionResultThrowException", namestyle, NamingStyle::PascalCase)))
            SetIsActionResultThrowException(json->GetBool(ConvertNamingStyle(L"IsActionResultThrowException", namestyle, NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<Json> VPGConfigInput::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::make_unique<Json>();
        // TypeWorkspace
        json->AddString(ConvertNamingStyle(L"TypeWorkspace", NamingStyle::PascalCase, namestyle), GetTypeWorkspace());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigInput::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // TypeWorkspace
        if (json->IsContainKey(ConvertNamingStyle(L"TypeWorkspace", namestyle, NamingStyle::PascalCase)))
            SetTypeWorkspace(json->GetString(ConvertNamingStyle(L"TypeWorkspace", namestyle, NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<Json> VPGConfigOutput::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::make_unique<Json>();
        // ExceptionTypeDirectory
        json->AddString(ConvertNamingStyle(L"ExceptionTypeDirectory", NamingStyle::PascalCase, namestyle), GetExceptionTypeDirectory());
        // ObjectTypeDirectory
        json->AddString(ConvertNamingStyle(L"ObjectTypeDirectory", NamingStyle::PascalCase, namestyle), GetObjectTypeDirectory());
        // ApplicationDirectoryHpp
        json->AddString(ConvertNamingStyle(L"ApplicationDirectoryHpp", NamingStyle::PascalCase, namestyle), GetApplicationDirectoryHpp());
        // ApplicationDirectoryCpp
        json->AddString(ConvertNamingStyle(L"ApplicationDirectoryCpp", NamingStyle::PascalCase, namestyle), GetApplicationDirectoryCpp());
        // ActionDirectoryHpp
        json->AddString(ConvertNamingStyle(L"ActionDirectoryHpp", NamingStyle::PascalCase, namestyle), GetActionDirectoryHpp());
        // ActionDirectoryCpp
        json->AddString(ConvertNamingStyle(L"ActionDirectoryCpp", NamingStyle::PascalCase, namestyle), GetActionDirectoryCpp());
        // FormDirectoryHpp
        json->AddString(ConvertNamingStyle(L"FormDirectoryHpp", NamingStyle::PascalCase, namestyle), GetFormDirectoryHpp());
        // FormDirectoryCpp
        json->AddString(ConvertNamingStyle(L"FormDirectoryCpp", NamingStyle::PascalCase, namestyle), GetFormDirectoryCpp());
        // ObjectDirectoryHpp
        json->AddString(ConvertNamingStyle(L"ObjectDirectoryHpp", NamingStyle::PascalCase, namestyle), GetObjectDirectoryHpp());
        // ObjectDirectoryCpp
        json->AddString(ConvertNamingStyle(L"ObjectDirectoryCpp", NamingStyle::PascalCase, namestyle), GetObjectDirectoryCpp());
        // PropertyAccessorDirectoryHpp
        json->AddString(ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", NamingStyle::PascalCase, namestyle), GetPropertyAccessorDirectoryHpp());
        // PropertyAccessorDirectoryCpp
        json->AddString(ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", NamingStyle::PascalCase, namestyle), GetPropertyAccessorDirectoryCpp());
        // ObjectFactoryDirectoryHpp
        json->AddString(ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", NamingStyle::PascalCase, namestyle), GetObjectFactoryDirectoryHpp());
        // ObjectFactoryDirectoryCpp
        json->AddString(ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", NamingStyle::PascalCase, namestyle), GetObjectFactoryDirectoryCpp());
        // PropertyAccessorFactoryDirectoryHpp
        json->AddString(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", NamingStyle::PascalCase, namestyle), GetPropertyAccessorFactoryDirectoryHpp());
        // PropertyAccessorFactoryDirectoryCpp
        json->AddString(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", NamingStyle::PascalCase, namestyle), GetPropertyAccessorFactoryDirectoryCpp());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigOutput::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // ExceptionTypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, NamingStyle::PascalCase)))
            SetExceptionTypeDirectory(json->GetString(ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, NamingStyle::PascalCase)));
        // ObjectTypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, NamingStyle::PascalCase)))
            SetObjectTypeDirectory(json->GetString(ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, NamingStyle::PascalCase)));
        // ApplicationDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"ApplicationDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            SetApplicationDirectoryHpp(json->GetString(ConvertNamingStyle(L"ApplicationDirectoryHpp", namestyle, NamingStyle::PascalCase)));
        // ApplicationDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"ApplicationDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            SetApplicationDirectoryCpp(json->GetString(ConvertNamingStyle(L"ApplicationDirectoryCpp", namestyle, NamingStyle::PascalCase)));
        // ActionDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"ActionDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            SetActionDirectoryHpp(json->GetString(ConvertNamingStyle(L"ActionDirectoryHpp", namestyle, NamingStyle::PascalCase)));
        // ActionDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"ActionDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            SetActionDirectoryCpp(json->GetString(ConvertNamingStyle(L"ActionDirectoryCpp", namestyle, NamingStyle::PascalCase)));
        // FormDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"FormDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            SetFormDirectoryHpp(json->GetString(ConvertNamingStyle(L"FormDirectoryHpp", namestyle, NamingStyle::PascalCase)));
        // FormDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"FormDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            SetFormDirectoryCpp(json->GetString(ConvertNamingStyle(L"FormDirectoryCpp", namestyle, NamingStyle::PascalCase)));
        // ObjectDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            SetObjectDirectoryHpp(json->GetString(ConvertNamingStyle(L"ObjectDirectoryHpp", namestyle, NamingStyle::PascalCase)));
        // ObjectDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            SetObjectDirectoryCpp(json->GetString(ConvertNamingStyle(L"ObjectDirectoryCpp", namestyle, NamingStyle::PascalCase)));
        // PropertyAccessorDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            SetPropertyAccessorDirectoryHpp(json->GetString(ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, NamingStyle::PascalCase)));
        // PropertyAccessorDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            SetPropertyAccessorDirectoryCpp(json->GetString(ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, NamingStyle::PascalCase)));
        // ObjectFactoryDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            SetObjectFactoryDirectoryHpp(json->GetString(ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, NamingStyle::PascalCase)));
        // ObjectFactoryDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            SetObjectFactoryDirectoryCpp(json->GetString(ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, NamingStyle::PascalCase)));
        // PropertyAccessorFactoryDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            SetPropertyAccessorFactoryDirectoryHpp(json->GetString(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, NamingStyle::PascalCase)));
        // PropertyAccessorFactoryDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            SetPropertyAccessorFactoryDirectoryCpp(json->GetString(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<Json> VPGConfigExport::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::make_unique<Json>();
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
        json->AddString(ConvertNamingStyle(L"Interface", NamingStyle::PascalCase, namestyle), interfaceValueStr);
        // Workspace
        json->AddString(ConvertNamingStyle(L"Workspace", NamingStyle::PascalCase, namestyle), GetWorkspace());
        // IsExportExternalLib
        json->AddBool(ConvertNamingStyle(L"IsExportExternalLib", NamingStyle::PascalCase, namestyle), GetIsExportExternalLib());
        // ExportDirectoryDll
        json->AddString(ConvertNamingStyle(L"ExportDirectoryDll", NamingStyle::PascalCase, namestyle), GetExportDirectoryDll());
        // ExportDirectoryExe
        json->AddString(ConvertNamingStyle(L"ExportDirectoryExe", NamingStyle::PascalCase, namestyle), GetExportDirectoryExe());
        // DllBridgeDirectory
        json->AddString(ConvertNamingStyle(L"DllBridgeDirectory", NamingStyle::PascalCase, namestyle), GetDllBridgeDirectory());
        // FormDirectory
        json->AddString(ConvertNamingStyle(L"FormDirectory", NamingStyle::PascalCase, namestyle), GetFormDirectory());
        // ObjectDirectory
        json->AddString(ConvertNamingStyle(L"ObjectDirectory", NamingStyle::PascalCase, namestyle), GetObjectDirectory());
        // TypeDirectory
        json->AddString(ConvertNamingStyle(L"TypeDirectory", NamingStyle::PascalCase, namestyle), GetTypeDirectory());
        return json;
    CATCH
    return nullptr;
}

void VPGConfigExport::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // Interface
        if (json->IsContainKey(ConvertNamingStyle(L"Interface", namestyle, NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->GetString(ConvertNamingStyle(L"Interface", namestyle, NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            ToUpper(valueEnumStrUpper);
            int64_t valueEnum = -1;
            if (valueEnumStrUpper == L"JAVA")
                valueEnum = static_cast<int64_t>(VPGConfigInterfaceType::Java);
            if (valueEnum > -1)
                SetInterface(static_cast<VPGConfigInterfaceType>(valueEnum));
        }
        // Workspace
        if (json->IsContainKey(ConvertNamingStyle(L"Workspace", namestyle, NamingStyle::PascalCase)))
            SetWorkspace(json->GetString(ConvertNamingStyle(L"Workspace", namestyle, NamingStyle::PascalCase)));
        // IsExportExternalLib
        if (json->IsContainKey(ConvertNamingStyle(L"IsExportExternalLib", namestyle, NamingStyle::PascalCase)))
            SetIsExportExternalLib(json->GetBool(ConvertNamingStyle(L"IsExportExternalLib", namestyle, NamingStyle::PascalCase)));
        // ExportDirectoryDll
        if (json->IsContainKey(ConvertNamingStyle(L"ExportDirectoryDll", namestyle, NamingStyle::PascalCase)))
            SetExportDirectoryDll(json->GetString(ConvertNamingStyle(L"ExportDirectoryDll", namestyle, NamingStyle::PascalCase)));
        // ExportDirectoryExe
        if (json->IsContainKey(ConvertNamingStyle(L"ExportDirectoryExe", namestyle, NamingStyle::PascalCase)))
            SetExportDirectoryExe(json->GetString(ConvertNamingStyle(L"ExportDirectoryExe", namestyle, NamingStyle::PascalCase)));
        // DllBridgeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"DllBridgeDirectory", namestyle, NamingStyle::PascalCase)))
            SetDllBridgeDirectory(json->GetString(ConvertNamingStyle(L"DllBridgeDirectory", namestyle, NamingStyle::PascalCase)));
        // FormDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"FormDirectory", namestyle, NamingStyle::PascalCase)))
            SetFormDirectory(json->GetString(ConvertNamingStyle(L"FormDirectory", namestyle, NamingStyle::PascalCase)));
        // ObjectDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectDirectory", namestyle, NamingStyle::PascalCase)))
            SetObjectDirectory(json->GetString(ConvertNamingStyle(L"ObjectDirectory", namestyle, NamingStyle::PascalCase)));
        // TypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"TypeDirectory", namestyle, NamingStyle::PascalCase)))
            SetTypeDirectory(json->GetString(ConvertNamingStyle(L"TypeDirectory", namestyle, NamingStyle::PascalCase)));
    CATCH
}

std::shared_ptr<Json> VPGConfig::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::make_unique<Json>();
        // Version
        json->AddString(ConvertNamingStyle(L"Version", NamingStyle::PascalCase, namestyle), GetVersion());
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
        json->AddString(ConvertNamingStyle(L"ProjectType", NamingStyle::PascalCase, namestyle), projectTypeValueStr);
        // ProjectPrefix
        json->AddString(ConvertNamingStyle(L"ProjectPrefix", NamingStyle::PascalCase, namestyle), GetProjectPrefix());
        // ProjectName
        json->AddString(ConvertNamingStyle(L"ProjectName", NamingStyle::PascalCase, namestyle), GetProjectName());
        // ProjectNameDll
        json->AddString(ConvertNamingStyle(L"ProjectNameDll", NamingStyle::PascalCase, namestyle), GetProjectNameDll());
        // ProjectNameExe
        json->AddString(ConvertNamingStyle(L"ProjectNameExe", NamingStyle::PascalCase, namestyle), GetProjectNameExe());
        // IsGit
        json->AddBool(ConvertNamingStyle(L"IsGit", NamingStyle::PascalCase, namestyle), GetIsGit());
        // Template
        if (GetTemplate() != nullptr)
            json->AddObject(ConvertNamingStyle(L"Template", NamingStyle::PascalCase, namestyle), GetTemplate()->ToJson());
        else
            json->AddNull(ConvertNamingStyle(L"Template", NamingStyle::PascalCase, namestyle));
        // Behavior
        if (GetBehavior() != nullptr)
            json->AddObject(ConvertNamingStyle(L"Behavior", NamingStyle::PascalCase, namestyle), GetBehavior()->ToJson());
        else
            json->AddNull(ConvertNamingStyle(L"Behavior", NamingStyle::PascalCase, namestyle));
        // Input
        if (GetInput() != nullptr)
            json->AddObject(ConvertNamingStyle(L"Input", NamingStyle::PascalCase, namestyle), GetInput()->ToJson());
        else
            json->AddNull(ConvertNamingStyle(L"Input", NamingStyle::PascalCase, namestyle));
        // Output
        if (GetOutput() != nullptr)
            json->AddObject(ConvertNamingStyle(L"Output", NamingStyle::PascalCase, namestyle), GetOutput()->ToJson());
        else
            json->AddNull(ConvertNamingStyle(L"Output", NamingStyle::PascalCase, namestyle));
        // Plugins
        auto tmpPlugins = std::make_shared<Json>();
        json->AddArray(ConvertNamingStyle(L"Plugins", NamingStyle::PascalCase, namestyle), tmpPlugins);
        for (auto const &element : GetPlugins()) {
            tmpPlugins->AddArrayString(element);
        }
        // Exports
        auto tmpExports = std::make_shared<Json>();
        json->AddArray(ConvertNamingStyle(L"Exports", NamingStyle::PascalCase, namestyle), tmpExports);
        for (auto const &element : GetExports()) {
            tmpExports->AddArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGConfig::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // Version
        if (json->IsContainKey(ConvertNamingStyle(L"Version", namestyle, NamingStyle::PascalCase)))
            SetVersion(json->GetString(ConvertNamingStyle(L"Version", namestyle, NamingStyle::PascalCase)));
        // ProjectType
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectType", namestyle, NamingStyle::PascalCase))) {
            std::wstring valueEnumStr = json->GetString(ConvertNamingStyle(L"ProjectType", namestyle, NamingStyle::PascalCase));
            std::wstring valueEnumStrUpper = valueEnumStr;
            ToUpper(valueEnumStrUpper);
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
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectPrefix", namestyle, NamingStyle::PascalCase)))
            SetProjectPrefix(json->GetString(ConvertNamingStyle(L"ProjectPrefix", namestyle, NamingStyle::PascalCase)));
        // ProjectName
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectName", namestyle, NamingStyle::PascalCase)))
            SetProjectName(json->GetString(ConvertNamingStyle(L"ProjectName", namestyle, NamingStyle::PascalCase)));
        // ProjectNameDll
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectNameDll", namestyle, NamingStyle::PascalCase)))
            SetProjectNameDll(json->GetString(ConvertNamingStyle(L"ProjectNameDll", namestyle, NamingStyle::PascalCase)));
        // ProjectNameExe
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectNameExe", namestyle, NamingStyle::PascalCase)))
            SetProjectNameExe(json->GetString(ConvertNamingStyle(L"ProjectNameExe", namestyle, NamingStyle::PascalCase)));
        // IsGit
        if (json->IsContainKey(ConvertNamingStyle(L"IsGit", namestyle, NamingStyle::PascalCase)))
            SetIsGit(json->GetBool(ConvertNamingStyle(L"IsGit", namestyle, NamingStyle::PascalCase)));
        // Template
        SetTemplate(std::make_shared<VPGConfigTemplate>());
        if (json->IsContainKey(ConvertNamingStyle(L"Template", namestyle, NamingStyle::PascalCase)) && json->GetObject(ConvertNamingStyle(L"Template", namestyle, NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigTemplate>();
            tmpObject->DeserializeJson(json->GetObject(ConvertNamingStyle(L"Template", namestyle, NamingStyle::PascalCase)));
            SetTemplate(tmpObject);
        }
        // Behavior
        SetBehavior(std::make_shared<VPGConfigBehavior>());
        if (json->IsContainKey(ConvertNamingStyle(L"Behavior", namestyle, NamingStyle::PascalCase)) && json->GetObject(ConvertNamingStyle(L"Behavior", namestyle, NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigBehavior>();
            tmpObject->DeserializeJson(json->GetObject(ConvertNamingStyle(L"Behavior", namestyle, NamingStyle::PascalCase)));
            SetBehavior(tmpObject);
        }
        // Input
        SetInput(std::make_shared<VPGConfigInput>());
        if (json->IsContainKey(ConvertNamingStyle(L"Input", namestyle, NamingStyle::PascalCase)) && json->GetObject(ConvertNamingStyle(L"Input", namestyle, NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigInput>();
            tmpObject->DeserializeJson(json->GetObject(ConvertNamingStyle(L"Input", namestyle, NamingStyle::PascalCase)));
            SetInput(tmpObject);
        }
        // Output
        SetOutput(std::make_shared<VPGConfigOutput>());
        if (json->IsContainKey(ConvertNamingStyle(L"Output", namestyle, NamingStyle::PascalCase)) && json->GetObject(ConvertNamingStyle(L"Output", namestyle, NamingStyle::PascalCase)) != nullptr) {
            auto tmpObject = std::make_shared<VPGConfigOutput>();
            tmpObject->DeserializeJson(json->GetObject(ConvertNamingStyle(L"Output", namestyle, NamingStyle::PascalCase)));
            SetOutput(tmpObject);
        }
        // Plugins
        ClearPlugins();
        if (json->IsContainKey(ConvertNamingStyle(L"Plugins", namestyle, NamingStyle::PascalCase))) {
            for (auto const &element : json->GetArray(ConvertNamingStyle(L"Plugins", namestyle, NamingStyle::PascalCase))) {
                InsertPlugins(element->GetArrayElementString());
            }
        }
        // Exports
        ClearExports();
        if (json->IsContainKey(ConvertNamingStyle(L"Exports", namestyle, NamingStyle::PascalCase))) {
            for (auto const &element : json->GetArray(ConvertNamingStyle(L"Exports", namestyle, NamingStyle::PascalCase))) {
                auto tmpExports = std::make_shared<VPGConfigExport>();
                tmpExports->DeserializeJson(element->GetArrayElementObject());
                InsertExports(tmpExports);
            }
        }
    CATCH
}
