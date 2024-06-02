#include "vpg_generation_option.hpp"

#include <assert.h>
#include <filesystem>
#include <iostream>
#include <memory>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "json.hpp"
#include "memory_macro.hpp"
#include "platform_type.hpp"
#include "string_helper.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

// std::shared_ptr<Json> VPGGenerationOptionPlatform::ToJson() const
// {
//     TRY
//         DECLARE_UPTR(Json, json);

//         std::wstring platform = L"";
//         switch (_Platform)
//         {
//         case PlatformType::All:
//             platform += L"All";
//             break;
//         case PlatformType::Window:
//             platform += L"Window";
//             break;
//         case PlatformType::Linux:
//             platform += L"Linux";
//             break;
//         case PlatformType::MacOs:
//             platform += L"MacOS";
//             break;
//         case PlatformType::Android:
//             platform += L"Android";
//             break;
//         case PlatformType::IOs:
//             platform += L"IOS";
//             break;
//         default:
//             // unknown
//             assert(false);
//             break;
//         }
//         json->AddString(L"Platform", platform);

//         // Plugins
//         DECLARE_SPTR(Json, includePaths);
//         json->AddArray(L"IncludePaths", includePaths);
//         for (auto const &includePath : _IncludePaths) {
//             includePaths->AddArrayString(includePath);
//         }
//         return json;
//     }
//     return nullptr;
// }

// void VPGGenerationOptionPlatform::DeserializeJson(std::shared_ptr<IDocument> document) const
// {
//     TRY
//         std::shared_ptr<Json> json = dynamic_pointer_cast<Json>(document);
//         assert(json != nullptr);

//         if (json->IsContainKey(L"Platform")) {
//             std::wstring platform = json->GetString(L"Platform");
//             ToUpper(platform);
//             if (platform == L"ALL")
//                 this->SetPlatform(PlatformType::All);
//             else if (platform == L"WINDOW")
//                 this->SetPlatform(PlatformType::Window);
//             else if (platform == L"LINUX")
//                 this->SetPlatform(PlatformType::Linux);
//             else if (platform == L"MACOS")
//                 this->SetPlatform(PlatformType::MacOs);
//             else if (platform == L"ANDRIOD")
//                 this->SetPlatform(PlatformType::Android);
//             else if (platform == L"IOS")
//                 this->SetPlatform(PlatformType::IOs);
//             else
//                 THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknow Platform: " + platform);
//         }

//         // Plugins
//         if (json->IsContainKey(L"IncludePaths")) {
//             for (auto const &plugin : json->GetArray(L"IncludePaths")) {
//                 this->InsertIncludePaths(plugin->GetJsonInternalValue());
//             }
//         }
//     }
// }

std::shared_ptr<Json> VPGGenerationOptionExport::ToJson() const
{
    TRY
        DECLARE_UPTR(Json, json);
        std::wstring interface = L"";
        switch (_Interface)
        {
        case VPGGenerationOptionInterfaceType::Java:
            interface = L"Java";
            break;        
        default:
            assert(false);
            break;
        }
        json->AddString(L"Interface", interface);
        return json;
    CATCH
    return nullptr;
}

void VPGGenerationOptionExport::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        std::shared_ptr<Json> json = std::dynamic_pointer_cast<Json>(document);
        if (json->IsContainKey(L"Interface")) {
            std::wstring interface = json->GetString(L"Interface");
            ToUpper(interface);
            if (interface == L"JAVA")
                this->SetInterface(VPGGenerationOptionInterfaceType::Java);
            else
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknow Interface: " + interface);
        }

        assert(json != nullptr);
    CATCH
}

std::shared_ptr<Json> VPGGenerationOption::ToJson() const
{
    TRY
        DECLARE_UPTR(Json, json);
        json->AddString(L"Version", _Version);
        
        json->AddString(L"ProjectType", L"VCCModule");
        json->AddString(L"WorkspaceSourceGitUrl", _WorkspaceSourceGitUrl);
        json->AddString(L"WorkspaceSource", _WorkspaceSource);
        json->AddString(L"WorkspaceDestination", _WorkspaceDestination);

        // --------------------------------------------------
        // Config
        // --------------------------------------------------
        json->AddString(L"ProjectPrefix", _ProjectPrefix);

        json->AddString(L"ProjectName", _ProjectName);
        json->AddString(L"ProjectNameDll", _ProjectNameDll);
        json->AddString(L"ProjectNameExe", _ProjectNameExe);
        json->AddBool(L"IsGit", _IsGit);

        json->AddBool(L"IsExcludeUnittest", _IsExcludeUnittest);
        json->AddBool(L"IsExcludeVCCUnitTest", _IsExcludeVCCUnitTest);

        json->AddString(L"TypeWorkspace", _TypeWorkspace);

        json->AddString(L"ActionTypeDirectory", _ActionTypeDirectory);
        json->AddString(L"ExceptionTypeDirectory", _ExceptionTypeDirectory);
        json->AddString(L"ManagerTypeDirectory", _ManagerTypeDirectory);
        json->AddString(L"ObjectTypeDirectory", _ObjectTypeDirectory);

        json->AddString(L"ModelDirectory", _ModelDirectory);
        json->AddString(L"PropertyAccessorDirectoryHpp", _PropertyAccessorDirectoryHpp);
        json->AddString(L"PropertyAccessorDirectoryCpp", _PropertyAccessorDirectoryCpp);

        json->AddString(L"PropertyAccessorFactoryDirectoryHpp", _PropertyAccessorFactoryDirectoryHpp);
        json->AddString(L"PropertyAccessorFactoryDirectoryCpp", _PropertyAccessorFactoryDirectoryCpp);

        // Platform
        // DECLARE_SPTR(Json, platforms);
        // json->AddArray(L"Platforms", platforms);
        // for (auto const &element : _Platforms) {
        //     platforms->AddArrayObject(element->ToJson());
        // }

        // Plugins
        DECLARE_SPTR(Json, plugins);
        json->AddArray(L"Plugins", plugins);
        for (auto const &plugin : _Plugins) {
            plugins->AddArrayString(plugin);
        }
        // Export
        DECLARE_SPTR(Json, exports);
        json->AddArray(L"Exports", exports);
        for (auto const &element : _Exports) {
            exports->AddArrayObject(element->ToJson());
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

        if (json->IsContainKey(L"Version"))
            this->SetVersion(json->GetString(L"Version"));
        if (json->IsContainKey(L"IsGit"))
            this->SetIsGit(json->GetBool(L"IsGit"));
        if (json->IsContainKey(L"ProjectPrefix"))
            this->SetProjectPrefix(json->GetString(L"ProjectPrefix"));


        if (json->IsContainKey(L"ProjectName"))
            this->SetProjectName(json->GetString(L"ProjectName"));
        if (json->IsContainKey(L"ProjectNameDll"))
            this->SetProjectNameDll(json->GetString(L"ProjectNameDll"));
        if (json->IsContainKey(L"ProjectNameExe"))
            this->SetProjectNameExe(json->GetString(L"ProjectNameExe"));
            
        if (json->IsContainKey(L"IsExcludeUnittest"))
            this->SetIsExcludeUnittest(json->GetBool(L"IsExcludeUnittest"));
        if (json->IsContainKey(L"IsExcludeVCCUnitTest"))
            this->SetIsExcludeVCCUnitTest(json->GetBool(L"IsExcludeVCCUnitTest"));

        if (json->IsContainKey(L"TypeWorkspace"))
            this->SetTypeWorkspace(json->GetString(L"TypeWorkspace"));

        if (json->IsContainKey(L"ActionTypeDirectory"))
            this->SetActionTypeDirectory(json->GetString(L"ActionTypeDirectory"));
        if (json->IsContainKey(L"ExceptionTypeDirectory"))
            this->SetExceptionTypeDirectory(json->GetString(L"ExceptionTypeDirectory"));
        if (json->IsContainKey(L"ManagerTypeDirectory"))
            this->SetManagerTypeDirectory(json->GetString(L"ManagerTypeDirectory"));
        if (json->IsContainKey(L"ObjectTypeDirectory"))
            this->SetObjectTypeDirectory(json->GetString(L"ObjectTypeDirectory"));

        if (json->IsContainKey(L"ModelDirectory"))
            this->SetModelDirectory(json->GetString(L"ModelDirectory"));
        if (json->IsContainKey(L"PropertyAccessorDirectoryHpp"))
            this->SetPropertyAccessorDirectoryHpp(json->GetString(L"PropertyAccessorDirectoryHpp"));
        if (json->IsContainKey(L"PropertyAccessorDirectoryCpp"))
            this->SetPropertyAccessorDirectoryCpp(json->GetString(L"PropertyAccessorDirectoryCpp"));

        if (json->IsContainKey(L"PropertyAccessorFactoryDirectoryHpp"))
            this->SetPropertyAccessorFactoryDirectoryHpp(json->GetString(L"PropertyAccessorFactoryDirectoryHpp"));
        if (json->IsContainKey(L"PropertyAccessorFactoryDirectoryCpp"))
            this->SetPropertyAccessorFactoryDirectoryCpp(json->GetString(L"PropertyAccessorFactoryDirectoryCpp"));

        // Platform
        // ClearPlatforms();
        // if (json->IsContainKey(L"Platforms")) {
        //     for (auto const &element : json->GetArray(L"Platforms")) {
        //         DECLARE_SPTR(VPGGenerationOptionPlatform, platform);
        //         platform->DeserializeJson(element);
        //         this->InsertPlatforms(platform);
        //     }
        // }

        // Plugins
        ClearPlugins();
        if (json->IsContainKey(L"Plugins")) {
            for (auto const &plugin : json->GetArray(L"Plugins")) {
                this->InsertPlugins(plugin->GetJsonInternalValue());
            }
        }
        // Exports
        ClearExports();
        if (json->IsContainKey(L"Exports")) {
            for (auto const &element : json->GetArray(L"Exports")) {
                DECLARE_SPTR(VPGGenerationOptionExport, tmpExport);
                tmpExport->DeserializeJson(element);
                this->InsertExports(tmpExport);
            }
        }
    CATCH
}
