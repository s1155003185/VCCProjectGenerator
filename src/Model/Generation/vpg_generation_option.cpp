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
        DECLARE_UPTR(Json, json);
        // Interface
        switch (_Interface)
        {
        case VPGGenerationOptionInterfaceType::Java:
            json->AddString(L"Interface", L"Java");
            break;
        default:
            assert(false);
            break;
        }
        return json;
    CATCH
    return nullptr;
}

void VPGGenerationOptionExport::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        std::shared_ptr<Json> json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // Interface
        if (json->IsContainKey(L"Interface")) {
            std::wstring tmpEnum = json->GetString(L"Interface");
            ToUpper(tmpEnum);
            if (tmpEnum == L"JAVA")
                _Interface = VPGGenerationOptionInterfaceType::Java;
            else
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknow Interface: " + tmpEnum);
        }
    CATCH
}

std::shared_ptr<Json> VPGGenerationOption::ToJson() const
{
    TRY
        DECLARE_UPTR(Json, json);
        // Version
        json->AddString(L"Version", _Version);
        // ProjectType
        switch (_ProjectType)
        {
        case VPGProjectType::VccModule:
            json->AddString(L"ProjectType", L"VccModule");
            break;
        case VPGProjectType::VccComplex:
            json->AddString(L"ProjectType", L"VccComplex");
            break;
        case VPGProjectType::VccDll:
            json->AddString(L"ProjectType", L"VccDll");
            break;
        case VPGProjectType::VccExe:
            json->AddString(L"ProjectType", L"VccExe");
            break;
        case VPGProjectType::CppComplex:
            json->AddString(L"ProjectType", L"CppComplex");
            break;
        case VPGProjectType::CppDll:
            json->AddString(L"ProjectType", L"CppDll");
            break;
        case VPGProjectType::CppExe:
            json->AddString(L"ProjectType", L"CppExe");
            break;
        default:
            assert(false);
            break;
        }
        // WorkspaceSourceGitUrl
        json->AddString(L"WorkspaceSourceGitUrl", _WorkspaceSourceGitUrl);
        // WorkspaceSource
        json->AddString(L"WorkspaceSource", _WorkspaceSource);
        // WorkspaceDestination
        json->AddString(L"WorkspaceDestination", _WorkspaceDestination);
        // ProjectPrefix
        json->AddString(L"ProjectPrefix", _ProjectPrefix);
        // ProjectName
        json->AddString(L"ProjectName", _ProjectName);
        // ProjectNameDll
        json->AddString(L"ProjectNameDll", _ProjectNameDll);
        // ProjectNameExe
        json->AddString(L"ProjectNameExe", _ProjectNameExe);
        // IsGit
        json->AddBool(L"IsGit", _IsGit);
        // IsExcludeUnittest
        json->AddBool(L"IsExcludeUnittest", _IsExcludeUnittest);
        // IsExcludeVCCUnitTest
        json->AddBool(L"IsExcludeVCCUnitTest", _IsExcludeVCCUnitTest);
        // TypeWorkspace
        json->AddString(L"TypeWorkspace", _TypeWorkspace);
        // ActionTypeDirectory
        json->AddString(L"ActionTypeDirectory", _ActionTypeDirectory);
        // ExceptionTypeDirectory
        json->AddString(L"ExceptionTypeDirectory", _ExceptionTypeDirectory);
        // ManagerTypeDirectory
        json->AddString(L"ManagerTypeDirectory", _ManagerTypeDirectory);
        // ObjectTypeDirectory
        json->AddString(L"ObjectTypeDirectory", _ObjectTypeDirectory);
        // ObjectDirectoryHpp
        json->AddString(L"ObjectDirectoryHpp", _ObjectDirectoryHpp);
        // ObjectDirectoryCpp
        json->AddString(L"ObjectDirectoryCpp", _ObjectDirectoryCpp);
        // PropertyAccessorDirectoryHpp
        json->AddString(L"PropertyAccessorDirectoryHpp", _PropertyAccessorDirectoryHpp);
        // PropertyAccessorDirectoryCpp
        json->AddString(L"PropertyAccessorDirectoryCpp", _PropertyAccessorDirectoryCpp);
        // ObjectFactoryDirectoryHpp
        json->AddString(L"ObjectFactoryDirectoryHpp", _ObjectFactoryDirectoryHpp);
        // ObjectFactoryDirectoryCpp
        json->AddString(L"ObjectFactoryDirectoryCpp", _ObjectFactoryDirectoryCpp);
        // PropertyAccessorFactoryDirectoryHpp
        json->AddString(L"PropertyAccessorFactoryDirectoryHpp", _PropertyAccessorFactoryDirectoryHpp);
        // PropertyAccessorFactoryDirectoryCpp
        json->AddString(L"PropertyAccessorFactoryDirectoryCpp", _PropertyAccessorFactoryDirectoryCpp);
        // Plugins
        DECLARE_SPTR(Json, tmpPlugins);
        json->AddArray(L"Plugins", tmpPlugins);
        for (auto const &element : _Plugins) {
            tmpPlugins->AddArrayString(element);
        }
        // Exports
        DECLARE_SPTR(Json, tmpExports);
        json->AddArray(L"Exports", tmpExports);
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
        std::shared_ptr<Json> json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // Version
        if (json->IsContainKey(L"Version"))
            _Version = json->GetString(L"Version");
        // ProjectType
        if (json->IsContainKey(L"ProjectType")) {
            std::wstring tmpEnum = json->GetString(L"ProjectType");
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
        // WorkspaceSourceGitUrl
        if (json->IsContainKey(L"WorkspaceSourceGitUrl"))
            _WorkspaceSourceGitUrl = json->GetString(L"WorkspaceSourceGitUrl");
        // WorkspaceSource
        if (json->IsContainKey(L"WorkspaceSource"))
            _WorkspaceSource = json->GetString(L"WorkspaceSource");
        // WorkspaceDestination
        if (json->IsContainKey(L"WorkspaceDestination"))
            _WorkspaceDestination = json->GetString(L"WorkspaceDestination");
        // ProjectPrefix
        if (json->IsContainKey(L"ProjectPrefix"))
            _ProjectPrefix = json->GetString(L"ProjectPrefix");
        // ProjectName
        if (json->IsContainKey(L"ProjectName"))
            _ProjectName = json->GetString(L"ProjectName");
        // ProjectNameDll
        if (json->IsContainKey(L"ProjectNameDll"))
            _ProjectNameDll = json->GetString(L"ProjectNameDll");
        // ProjectNameExe
        if (json->IsContainKey(L"ProjectNameExe"))
            _ProjectNameExe = json->GetString(L"ProjectNameExe");
        // IsGit
        if (json->IsContainKey(L"IsGit"))
            _IsGit = json->GetBool(L"IsGit");
        // IsExcludeUnittest
        if (json->IsContainKey(L"IsExcludeUnittest"))
            _IsExcludeUnittest = json->GetBool(L"IsExcludeUnittest");
        // IsExcludeVCCUnitTest
        if (json->IsContainKey(L"IsExcludeVCCUnitTest"))
            _IsExcludeVCCUnitTest = json->GetBool(L"IsExcludeVCCUnitTest");
        // TypeWorkspace
        if (json->IsContainKey(L"TypeWorkspace"))
            _TypeWorkspace = json->GetString(L"TypeWorkspace");
        // ActionTypeDirectory
        if (json->IsContainKey(L"ActionTypeDirectory"))
            _ActionTypeDirectory = json->GetString(L"ActionTypeDirectory");
        // ExceptionTypeDirectory
        if (json->IsContainKey(L"ExceptionTypeDirectory"))
            _ExceptionTypeDirectory = json->GetString(L"ExceptionTypeDirectory");
        // ManagerTypeDirectory
        if (json->IsContainKey(L"ManagerTypeDirectory"))
            _ManagerTypeDirectory = json->GetString(L"ManagerTypeDirectory");
        // ObjectTypeDirectory
        if (json->IsContainKey(L"ObjectTypeDirectory"))
            _ObjectTypeDirectory = json->GetString(L"ObjectTypeDirectory");
        // ObjectDirectoryHpp
        if (json->IsContainKey(L"ObjectDirectoryHpp"))
            _ObjectDirectoryHpp = json->GetString(L"ObjectDirectoryHpp");
        // ObjectDirectoryCpp
        if (json->IsContainKey(L"ObjectDirectoryCpp"))
            _ObjectDirectoryCpp = json->GetString(L"ObjectDirectoryCpp");
        // PropertyAccessorDirectoryHpp
        if (json->IsContainKey(L"PropertyAccessorDirectoryHpp"))
            _PropertyAccessorDirectoryHpp = json->GetString(L"PropertyAccessorDirectoryHpp");
        // PropertyAccessorDirectoryCpp
        if (json->IsContainKey(L"PropertyAccessorDirectoryCpp"))
            _PropertyAccessorDirectoryCpp = json->GetString(L"PropertyAccessorDirectoryCpp");
        // ObjectFactoryDirectoryHpp
        if (json->IsContainKey(L"ObjectFactoryDirectoryHpp"))
            _ObjectFactoryDirectoryHpp = json->GetString(L"ObjectFactoryDirectoryHpp");
        // ObjectFactoryDirectoryCpp
        if (json->IsContainKey(L"ObjectFactoryDirectoryCpp"))
            _ObjectFactoryDirectoryCpp = json->GetString(L"ObjectFactoryDirectoryCpp");
        // PropertyAccessorFactoryDirectoryHpp
        if (json->IsContainKey(L"PropertyAccessorFactoryDirectoryHpp"))
            _PropertyAccessorFactoryDirectoryHpp = json->GetString(L"PropertyAccessorFactoryDirectoryHpp");
        // PropertyAccessorFactoryDirectoryCpp
        if (json->IsContainKey(L"PropertyAccessorFactoryDirectoryCpp"))
            _PropertyAccessorFactoryDirectoryCpp = json->GetString(L"PropertyAccessorFactoryDirectoryCpp");
        // Plugins
        ClearPlugins();
        if (json->IsContainKey(L"Plugins")) {
            for (auto const &element : json->GetArray(L"Plugins")) {
                InsertPlugins(element->GetArrayElementString());
            }
        }
        // Exports
        ClearExports();
        if (json->IsContainKey(L"Exports")) {
            for (auto const &element : json->GetArray(L"Exports")) {
                DECLARE_SPTR(VPGGenerationOptionExport, tmpExports);
                tmpExports->DeserializeJson(element);
                InsertExports(tmpExports);
            }
        }
    CATCH
}
