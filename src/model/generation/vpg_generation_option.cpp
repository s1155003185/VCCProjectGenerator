#include "vpg_generation_option.hpp"

#include <assert.h>
#include <memory>
#include <string>

#include "exception_macro.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "json.hpp"
#include "memory_macro.hpp"
#include "number_helper.hpp"
#include "string_helper.hpp"
#include "vpg_generation_option_interface_type.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

std::shared_ptr<Json> VPGGenerationOptionExport::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        DECLARE_UPTR(Json, json);
        // Interface
        std::wstring interfaceValueStr = L"";
        switch (_Interface)
        {
        case VPGGenerationOptionInterfaceType::Java:
            interfaceValueStr = L"Java";
            break;
        default:
            assert(false);
            break;
        }
        json->AddString(ConvertNamingStyle(L"Interface", NamingStyle::PascalCase, namestyle), interfaceValueStr);
        // Workspace
        json->AddString(ConvertNamingStyle(L"Workspace", NamingStyle::PascalCase, namestyle), _Workspace);
        // IsExportExternalLib
        json->AddBool(ConvertNamingStyle(L"IsExportExternalLib", NamingStyle::PascalCase, namestyle), _IsExportExternalLib);
        // ExportDirectoryDll
        json->AddString(ConvertNamingStyle(L"ExportDirectoryDll", NamingStyle::PascalCase, namestyle), _ExportDirectoryDll);
        // ExportDirectoryExe
        json->AddString(ConvertNamingStyle(L"ExportDirectoryExe", NamingStyle::PascalCase, namestyle), _ExportDirectoryExe);
        // DllBridgeDirectory
        json->AddString(ConvertNamingStyle(L"DllBridgeDirectory", NamingStyle::PascalCase, namestyle), _DllBridgeDirectory);
        // FormDirectory
        json->AddString(ConvertNamingStyle(L"FormDirectory", NamingStyle::PascalCase, namestyle), _FormDirectory);
        // ObjectDirectory
        json->AddString(ConvertNamingStyle(L"ObjectDirectory", NamingStyle::PascalCase, namestyle), _ObjectDirectory);
        // TypeDirectory
        json->AddString(ConvertNamingStyle(L"TypeDirectory", NamingStyle::PascalCase, namestyle), _TypeDirectory);
        return json;
    CATCH
    return nullptr;
}

void VPGGenerationOptionExport::DeserializeJson(std::shared_ptr<IDocument> document) const
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
                valueEnum = static_cast<int64_t>(VPGGenerationOptionInterfaceType::Java);
            if (valueEnum > -1)
                _Interface = static_cast<VPGGenerationOptionInterfaceType>(valueEnum);
        }
        // Workspace
        if (json->IsContainKey(ConvertNamingStyle(L"Workspace", namestyle, NamingStyle::PascalCase)))
            _Workspace = json->GetString(ConvertNamingStyle(L"Workspace", namestyle, NamingStyle::PascalCase));
        // IsExportExternalLib
        if (json->IsContainKey(ConvertNamingStyle(L"IsExportExternalLib", namestyle, NamingStyle::PascalCase)))
            _IsExportExternalLib = json->GetBool(ConvertNamingStyle(L"IsExportExternalLib", namestyle, NamingStyle::PascalCase));
        // ExportDirectoryDll
        if (json->IsContainKey(ConvertNamingStyle(L"ExportDirectoryDll", namestyle, NamingStyle::PascalCase)))
            _ExportDirectoryDll = json->GetString(ConvertNamingStyle(L"ExportDirectoryDll", namestyle, NamingStyle::PascalCase));
        // ExportDirectoryExe
        if (json->IsContainKey(ConvertNamingStyle(L"ExportDirectoryExe", namestyle, NamingStyle::PascalCase)))
            _ExportDirectoryExe = json->GetString(ConvertNamingStyle(L"ExportDirectoryExe", namestyle, NamingStyle::PascalCase));
        // DllBridgeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"DllBridgeDirectory", namestyle, NamingStyle::PascalCase)))
            _DllBridgeDirectory = json->GetString(ConvertNamingStyle(L"DllBridgeDirectory", namestyle, NamingStyle::PascalCase));
        // FormDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"FormDirectory", namestyle, NamingStyle::PascalCase)))
            _FormDirectory = json->GetString(ConvertNamingStyle(L"FormDirectory", namestyle, NamingStyle::PascalCase));
        // ObjectDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectDirectory", namestyle, NamingStyle::PascalCase)))
            _ObjectDirectory = json->GetString(ConvertNamingStyle(L"ObjectDirectory", namestyle, NamingStyle::PascalCase));
        // TypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"TypeDirectory", namestyle, NamingStyle::PascalCase)))
            _TypeDirectory = json->GetString(ConvertNamingStyle(L"TypeDirectory", namestyle, NamingStyle::PascalCase));
    CATCH
}

std::shared_ptr<Json> VPGGenerationOption::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        DECLARE_UPTR(Json, json);
        // Version
        json->AddString(ConvertNamingStyle(L"Version", NamingStyle::PascalCase, namestyle), _Version);
        // ProjectType
        std::wstring projectTypeValueStr = L"";
        switch (_ProjectType)
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
        // TemplateGitUrl
        json->AddString(ConvertNamingStyle(L"TemplateGitUrl", NamingStyle::PascalCase, namestyle), _TemplateGitUrl);
        // TemplateWorkspace
        json->AddString(ConvertNamingStyle(L"TemplateWorkspace", NamingStyle::PascalCase, namestyle), _TemplateWorkspace);
        // ProjectPrefix
        json->AddString(ConvertNamingStyle(L"ProjectPrefix", NamingStyle::PascalCase, namestyle), _ProjectPrefix);
        // ProjectName
        json->AddString(ConvertNamingStyle(L"ProjectName", NamingStyle::PascalCase, namestyle), _ProjectName);
        // ProjectNameDll
        json->AddString(ConvertNamingStyle(L"ProjectNameDll", NamingStyle::PascalCase, namestyle), _ProjectNameDll);
        // ProjectNameExe
        json->AddString(ConvertNamingStyle(L"ProjectNameExe", NamingStyle::PascalCase, namestyle), _ProjectNameExe);
        // IsGit
        json->AddBool(ConvertNamingStyle(L"IsGit", NamingStyle::PascalCase, namestyle), _IsGit);
        // IsExcludeUnittest
        json->AddBool(ConvertNamingStyle(L"IsExcludeUnittest", NamingStyle::PascalCase, namestyle), _IsExcludeUnittest);
        // IsExcludeVCCUnitTest
        json->AddBool(ConvertNamingStyle(L"IsExcludeVCCUnitTest", NamingStyle::PascalCase, namestyle), _IsExcludeVCCUnitTest);
        // TypeWorkspace
        json->AddString(ConvertNamingStyle(L"TypeWorkspace", NamingStyle::PascalCase, namestyle), _TypeWorkspace);
        // ExceptionTypeDirectory
        json->AddString(ConvertNamingStyle(L"ExceptionTypeDirectory", NamingStyle::PascalCase, namestyle), _ExceptionTypeDirectory);
        // ObjectTypeDirectory
        json->AddString(ConvertNamingStyle(L"ObjectTypeDirectory", NamingStyle::PascalCase, namestyle), _ObjectTypeDirectory);
        // ApplicationDirectoryHpp
        json->AddString(ConvertNamingStyle(L"ApplicationDirectoryHpp", NamingStyle::PascalCase, namestyle), _ApplicationDirectoryHpp);
        // ApplicationDirectoryCpp
        json->AddString(ConvertNamingStyle(L"ApplicationDirectoryCpp", NamingStyle::PascalCase, namestyle), _ApplicationDirectoryCpp);
        // ActionDirectoryHpp
        json->AddString(ConvertNamingStyle(L"ActionDirectoryHpp", NamingStyle::PascalCase, namestyle), _ActionDirectoryHpp);
        // ActionDirectoryCpp
        json->AddString(ConvertNamingStyle(L"ActionDirectoryCpp", NamingStyle::PascalCase, namestyle), _ActionDirectoryCpp);
        // FormDirectoryHpp
        json->AddString(ConvertNamingStyle(L"FormDirectoryHpp", NamingStyle::PascalCase, namestyle), _FormDirectoryHpp);
        // FormDirectoryCpp
        json->AddString(ConvertNamingStyle(L"FormDirectoryCpp", NamingStyle::PascalCase, namestyle), _FormDirectoryCpp);
        // ObjectDirectoryHpp
        json->AddString(ConvertNamingStyle(L"ObjectDirectoryHpp", NamingStyle::PascalCase, namestyle), _ObjectDirectoryHpp);
        // ObjectDirectoryCpp
        json->AddString(ConvertNamingStyle(L"ObjectDirectoryCpp", NamingStyle::PascalCase, namestyle), _ObjectDirectoryCpp);
        // PropertyAccessorDirectoryHpp
        json->AddString(ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", NamingStyle::PascalCase, namestyle), _PropertyAccessorDirectoryHpp);
        // PropertyAccessorDirectoryCpp
        json->AddString(ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", NamingStyle::PascalCase, namestyle), _PropertyAccessorDirectoryCpp);
        // ObjectFactoryDirectoryHpp
        json->AddString(ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", NamingStyle::PascalCase, namestyle), _ObjectFactoryDirectoryHpp);
        // ObjectFactoryDirectoryCpp
        json->AddString(ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", NamingStyle::PascalCase, namestyle), _ObjectFactoryDirectoryCpp);
        // PropertyAccessorFactoryDirectoryHpp
        json->AddString(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", NamingStyle::PascalCase, namestyle), _PropertyAccessorFactoryDirectoryHpp);
        // PropertyAccessorFactoryDirectoryCpp
        json->AddString(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", NamingStyle::PascalCase, namestyle), _PropertyAccessorFactoryDirectoryCpp);
        // Plugins
        DECLARE_SPTR(Json, tmpPlugins);
        json->AddArray(ConvertNamingStyle(L"Plugins", NamingStyle::PascalCase, namestyle), tmpPlugins);
        for (auto const &element : _Plugins) {
            tmpPlugins->AddArrayString(element);
        }
        // Exports
        DECLARE_SPTR(Json, tmpExports);
        json->AddArray(ConvertNamingStyle(L"Exports", NamingStyle::PascalCase, namestyle), tmpExports);
        for (auto const &element : _Exports) {
            tmpExports->AddArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGGenerationOption::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // Version
        if (json->IsContainKey(ConvertNamingStyle(L"Version", namestyle, NamingStyle::PascalCase)))
            _Version = json->GetString(ConvertNamingStyle(L"Version", namestyle, NamingStyle::PascalCase));
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
                _ProjectType = static_cast<VPGProjectType>(valueEnum);
        }
        // TemplateGitUrl
        if (json->IsContainKey(ConvertNamingStyle(L"TemplateGitUrl", namestyle, NamingStyle::PascalCase)))
            _TemplateGitUrl = json->GetString(ConvertNamingStyle(L"TemplateGitUrl", namestyle, NamingStyle::PascalCase));
        // TemplateWorkspace
        if (json->IsContainKey(ConvertNamingStyle(L"TemplateWorkspace", namestyle, NamingStyle::PascalCase)))
            _TemplateWorkspace = json->GetString(ConvertNamingStyle(L"TemplateWorkspace", namestyle, NamingStyle::PascalCase));
        // ProjectPrefix
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectPrefix", namestyle, NamingStyle::PascalCase)))
            _ProjectPrefix = json->GetString(ConvertNamingStyle(L"ProjectPrefix", namestyle, NamingStyle::PascalCase));
        // ProjectName
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectName", namestyle, NamingStyle::PascalCase)))
            _ProjectName = json->GetString(ConvertNamingStyle(L"ProjectName", namestyle, NamingStyle::PascalCase));
        // ProjectNameDll
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectNameDll", namestyle, NamingStyle::PascalCase)))
            _ProjectNameDll = json->GetString(ConvertNamingStyle(L"ProjectNameDll", namestyle, NamingStyle::PascalCase));
        // ProjectNameExe
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectNameExe", namestyle, NamingStyle::PascalCase)))
            _ProjectNameExe = json->GetString(ConvertNamingStyle(L"ProjectNameExe", namestyle, NamingStyle::PascalCase));
        // IsGit
        if (json->IsContainKey(ConvertNamingStyle(L"IsGit", namestyle, NamingStyle::PascalCase)))
            _IsGit = json->GetBool(ConvertNamingStyle(L"IsGit", namestyle, NamingStyle::PascalCase));
        // IsExcludeUnittest
        if (json->IsContainKey(ConvertNamingStyle(L"IsExcludeUnittest", namestyle, NamingStyle::PascalCase)))
            _IsExcludeUnittest = json->GetBool(ConvertNamingStyle(L"IsExcludeUnittest", namestyle, NamingStyle::PascalCase));
        // IsExcludeVCCUnitTest
        if (json->IsContainKey(ConvertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, NamingStyle::PascalCase)))
            _IsExcludeVCCUnitTest = json->GetBool(ConvertNamingStyle(L"IsExcludeVCCUnitTest", namestyle, NamingStyle::PascalCase));
        // TypeWorkspace
        if (json->IsContainKey(ConvertNamingStyle(L"TypeWorkspace", namestyle, NamingStyle::PascalCase)))
            _TypeWorkspace = json->GetString(ConvertNamingStyle(L"TypeWorkspace", namestyle, NamingStyle::PascalCase));
        // ExceptionTypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, NamingStyle::PascalCase)))
            _ExceptionTypeDirectory = json->GetString(ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, NamingStyle::PascalCase));
        // ObjectTypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, NamingStyle::PascalCase)))
            _ObjectTypeDirectory = json->GetString(ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, NamingStyle::PascalCase));
        // ApplicationDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"ApplicationDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            _ApplicationDirectoryHpp = json->GetString(ConvertNamingStyle(L"ApplicationDirectoryHpp", namestyle, NamingStyle::PascalCase));
        // ApplicationDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"ApplicationDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            _ApplicationDirectoryCpp = json->GetString(ConvertNamingStyle(L"ApplicationDirectoryCpp", namestyle, NamingStyle::PascalCase));
        // ActionDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"ActionDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            _ActionDirectoryHpp = json->GetString(ConvertNamingStyle(L"ActionDirectoryHpp", namestyle, NamingStyle::PascalCase));
        // ActionDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"ActionDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            _ActionDirectoryCpp = json->GetString(ConvertNamingStyle(L"ActionDirectoryCpp", namestyle, NamingStyle::PascalCase));
        // FormDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"FormDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            _FormDirectoryHpp = json->GetString(ConvertNamingStyle(L"FormDirectoryHpp", namestyle, NamingStyle::PascalCase));
        // FormDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"FormDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            _FormDirectoryCpp = json->GetString(ConvertNamingStyle(L"FormDirectoryCpp", namestyle, NamingStyle::PascalCase));
        // ObjectDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            _ObjectDirectoryHpp = json->GetString(ConvertNamingStyle(L"ObjectDirectoryHpp", namestyle, NamingStyle::PascalCase));
        // ObjectDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            _ObjectDirectoryCpp = json->GetString(ConvertNamingStyle(L"ObjectDirectoryCpp", namestyle, NamingStyle::PascalCase));
        // PropertyAccessorDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            _PropertyAccessorDirectoryHpp = json->GetString(ConvertNamingStyle(L"PropertyAccessorDirectoryHpp", namestyle, NamingStyle::PascalCase));
        // PropertyAccessorDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            _PropertyAccessorDirectoryCpp = json->GetString(ConvertNamingStyle(L"PropertyAccessorDirectoryCpp", namestyle, NamingStyle::PascalCase));
        // ObjectFactoryDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            _ObjectFactoryDirectoryHpp = json->GetString(ConvertNamingStyle(L"ObjectFactoryDirectoryHpp", namestyle, NamingStyle::PascalCase));
        // ObjectFactoryDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            _ObjectFactoryDirectoryCpp = json->GetString(ConvertNamingStyle(L"ObjectFactoryDirectoryCpp", namestyle, NamingStyle::PascalCase));
        // PropertyAccessorFactoryDirectoryHpp
        if (json->IsContainKey(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, NamingStyle::PascalCase)))
            _PropertyAccessorFactoryDirectoryHpp = json->GetString(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryHpp", namestyle, NamingStyle::PascalCase));
        // PropertyAccessorFactoryDirectoryCpp
        if (json->IsContainKey(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, NamingStyle::PascalCase)))
            _PropertyAccessorFactoryDirectoryCpp = json->GetString(ConvertNamingStyle(L"PropertyAccessorFactoryDirectoryCpp", namestyle, NamingStyle::PascalCase));
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
                DECLARE_SPTR(VPGGenerationOptionExport, tmpExports);
                tmpExports->DeserializeJson(element->GetArrayElementObject());
                InsertExports(tmpExports);
            }
        }
    CATCH
}
