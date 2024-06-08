#pragma once

#include <string>

#include "base_object.hpp"
#include "base_json_object.hpp"
#include "platform_type.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

// class VPGGenerationOptionPlatform : public BaseObject<VPGGenerationOptionPlatform>, public BaseJsonObject
// {
//     GETSET(PlatformType, Platform, PlatformType::All);
//     VECTOR(std::wstring, IncludePaths);

//     public:;
//         VPGGenerationOptionPlatform() = default;
//         virtual ~VPGGenerationOptionPlatform() {}

//         virtual std::shared_ptr<Json> ToJson() const override;
//         virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;
// };

enum class VPGGenerationOptionInterfaceType
{
    Java
};

class VPGGenerationOptionExport : public BaseObject<VPGGenerationOptionExport>, public BaseJsonObject
{
    GETSET(VPGGenerationOptionInterfaceType, Interface, VPGGenerationOptionInterfaceType::Java);

    public:;
        VPGGenerationOptionExport() = default;
        virtual ~VPGGenerationOptionExport() {}

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;
};

class VPGGenerationOption : public BaseObject<VPGGenerationOption>, public BaseJsonObject
{
    GETSET(std::wstring, Version, L"v0.0.1");
    // Generation Use
    GETSET(VPGProjectType, ProjectType, VPGProjectType::NA);
    GETSET(std::wstring, WorkspaceSourceGitUrl, L"");
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceDestination, L"");

    // --------------------------------------------------
    // Config
    // --------------------------------------------------
    // Project
    GETSET(std::wstring, ProjectPrefix, L"");

    GETSET(std::wstring, ProjectName, L"VCCModule"); // Need to assign Default Name first to pass validation
    GETSET(std::wstring, ProjectNameDll, L"libVCCModule"); // Need to assign Default Name first to pass validation
    GETSET(std::wstring, ProjectNameExe, L"VCCModule"); // Need to assign Default Name first to pass validation
    GETSET(bool, IsGit, false);

    GETSET(bool, IsExcludeUnittest, false);
    GETSET(bool, IsExcludeVCCUnitTest, false);

    // Files
    GETSET(std::wstring, TypeWorkspace, L"include/Type");

    GETSET(std::wstring, ActionTypeDirectory, L"include/Type");
    GETSET(std::wstring, ExceptionTypeDirectory, L"include/Type");
    GETSET(std::wstring, ManagerTypeDirectory, L"include/Type");
    GETSET(std::wstring, ObjectTypeDirectory, L"include/Type");

    GETSET(std::wstring, ObjectDirectory, L"include/Model");
    GETSET(std::wstring, PropertyAccessorDirectoryHpp, L"include/PropertyAccessor");
    GETSET(std::wstring, PropertyAccessorDirectoryCpp, L"src/PropertyAccessor");

    GETSET(std::wstring, ObjectFactoryDirectoryHpp, L"include/Factory");
    GETSET(std::wstring, ObjectFactoryDirectoryCpp, L"src/Factory");
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryHpp, L"include/Factory");
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryCpp, L"src/Factory");
    
    // Platform
    //VECTOR_SPTR(VPGGenerationOptionPlatform, Platforms);

    // Plugins
    VECTOR(std::wstring, Plugins);

    // Export
    VECTOR_SPTR(VPGGenerationOptionExport, Exports);

    public:;
        VPGGenerationOption() = default;
        virtual ~VPGGenerationOption() {}

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;
};
