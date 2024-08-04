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
        switch (_Interface)
        {
        case VPGGenerationOptionInterfaceType::Java:
            json->AddString(ConvertNamingStyle(L"Interface", NamingStyle::PascalCase, namestyle), L"Java");
            break;
        default:
            assert(false);
            break;
        }
        // Workspace
        json->AddString(ConvertNamingStyle(L"Workspace", NamingStyle::PascalCase, namestyle), _Workspace);
        // DllBridgeDirectory
        json->AddString(ConvertNamingStyle(L"DllBridgeDirectory", NamingStyle::PascalCase, namestyle), _DllBridgeDirectory);
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
        std::shared_ptr<Json> json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // Interface
        if (json->IsContainKey(ConvertNamingStyle(L"Interface", namestyle, NamingStyle::PascalCase))) {
            std::wstring tmpEnum = json->GetString(ConvertNamingStyle(L"Interface", namestyle, NamingStyle::PascalCase));
            ToUpper(tmpEnum);
            if (tmpEnum == L"JAVA")
                _Interface = VPGGenerationOptionInterfaceType::Java;
            else
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknow Interface: " + tmpEnum);
        }
        // Workspace
        if (json->IsContainKey(ConvertNamingStyle(L"Workspace", namestyle, NamingStyle::PascalCase)))
            _Workspace = json->GetString(ConvertNamingStyle(L"Workspace", namestyle, NamingStyle::PascalCase));
        // DllBridgeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"DllBridgeDirectory", namestyle, NamingStyle::PascalCase)))
            _DllBridgeDirectory = json->GetString(ConvertNamingStyle(L"DllBridgeDirectory", namestyle, NamingStyle::PascalCase));
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
        switch (_ProjectType)
        {
        case VPGProjectType::VccModule:
            json->AddString(ConvertNamingStyle(L"ProjectType", NamingStyle::PascalCase, namestyle), L"VccModule");
            break;
        case VPGProjectType::VccComplex:
            json->AddString(ConvertNamingStyle(L"ProjectType", NamingStyle::PascalCase, namestyle), L"VccComplex");
            break;
        case VPGProjectType::VccDll:
            json->AddString(ConvertNamingStyle(L"ProjectType", NamingStyle::PascalCase, namestyle), L"VccDll");
            break;
        case VPGProjectType::VccExe:
            json->AddString(ConvertNamingStyle(L"ProjectType", NamingStyle::PascalCase, namestyle), L"VccExe");
            break;
        case VPGProjectType::CppComplex:
            json->AddString(ConvertNamingStyle(L"ProjectType", NamingStyle::PascalCase, namestyle), L"CppComplex");
            break;
        case VPGProjectType::CppDll:
            json->AddString(ConvertNamingStyle(L"ProjectType", NamingStyle::PascalCase, namestyle), L"CppDll");
            break;
        case VPGProjectType::CppExe:
            json->AddString(ConvertNamingStyle(L"ProjectType", NamingStyle::PascalCase, namestyle), L"CppExe");
            break;
        default:
            assert(false);
            break;
        }
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
        // ActionTypeDirectory
        json->AddString(ConvertNamingStyle(L"ActionTypeDirectory", NamingStyle::PascalCase, namestyle), _ActionTypeDirectory);
        // ExceptionTypeDirectory
        json->AddString(ConvertNamingStyle(L"ExceptionTypeDirectory", NamingStyle::PascalCase, namestyle), _ExceptionTypeDirectory);
        // ManagerTypeDirectory
        json->AddString(ConvertNamingStyle(L"ManagerTypeDirectory", NamingStyle::PascalCase, namestyle), _ManagerTypeDirectory);
        // ObjectTypeDirectory
        json->AddString(ConvertNamingStyle(L"ObjectTypeDirectory", NamingStyle::PascalCase, namestyle), _ObjectTypeDirectory);
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
        std::shared_ptr<Json> json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // Version
        if (json->IsContainKey(ConvertNamingStyle(L"Version", namestyle, NamingStyle::PascalCase)))
            _Version = json->GetString(ConvertNamingStyle(L"Version", namestyle, NamingStyle::PascalCase));
        // ProjectType
        if (json->IsContainKey(ConvertNamingStyle(L"ProjectType", namestyle, NamingStyle::PascalCase))) {
            std::wstring tmpEnum = json->GetString(ConvertNamingStyle(L"ProjectType", namestyle, NamingStyle::PascalCase));
            ToUpper(tmpEnum);
            if (tmpEnum == L"VCCMODULE")
                _ProjectType = VPGProjectType::VccModule;
            else if (tmpEnum == L"VCCCOMPLEX")
                _ProjectType = VPGProjectType::VccComplex;
            else if (tmpEnum == L"VCCDLL")
                _ProjectType = VPGProjectType::VccDll;
            else if (tmpEnum == L"VCCEXE")
                _ProjectType = VPGProjectType::VccExe;
            else if (tmpEnum == L"CPPCOMPLEX")
                _ProjectType = VPGProjectType::CppComplex;
            else if (tmpEnum == L"CPPDLL")
                _ProjectType = VPGProjectType::CppDll;
            else if (tmpEnum == L"CPPEXE")
                _ProjectType = VPGProjectType::CppExe;
            else
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknow Interface: " + tmpEnum);
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
        // ActionTypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ActionTypeDirectory", namestyle, NamingStyle::PascalCase)))
            _ActionTypeDirectory = json->GetString(ConvertNamingStyle(L"ActionTypeDirectory", namestyle, NamingStyle::PascalCase));
        // ExceptionTypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, NamingStyle::PascalCase)))
            _ExceptionTypeDirectory = json->GetString(ConvertNamingStyle(L"ExceptionTypeDirectory", namestyle, NamingStyle::PascalCase));
        // ManagerTypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ManagerTypeDirectory", namestyle, NamingStyle::PascalCase)))
            _ManagerTypeDirectory = json->GetString(ConvertNamingStyle(L"ManagerTypeDirectory", namestyle, NamingStyle::PascalCase));
        // ObjectTypeDirectory
        if (json->IsContainKey(ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, NamingStyle::PascalCase)))
            _ObjectTypeDirectory = json->GetString(ConvertNamingStyle(L"ObjectTypeDirectory", namestyle, NamingStyle::PascalCase));
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
