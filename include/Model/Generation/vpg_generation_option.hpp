#pragma once

#include <string>

#include "base_json_object.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "i_document.hpp"
#include "json.hpp"
#include "object_type.hpp"
#include "vpg_generation_option_interface_type.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGGenerationOptionExport : public BaseObject<VPGGenerationOptionExport>, public BaseJsonObject
{
    GETSET(VPGGenerationOptionInterfaceType, Interface, VPGGenerationOptionInterfaceType::Java)
    GETSET(std::wstring, Workspace, L"")
    GETSET(bool, IsExportExternalLib, true)
    GETSET(std::wstring, ExportDirectoryDll, L"")
    GETSET(std::wstring, ExportDirectoryExe, L"")
    GETSET(std::wstring, DllBridgeDirectory, L"")
    GETSET(std::wstring, ObjectDirectory, L"")
    GETSET(std::wstring, TypeDirectory, L"")

    public:
        VPGGenerationOptionExport() : BaseObject(ObjectType::GenerationOptionExport) {}
        virtual ~VPGGenerationOptionExport() {}

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;
};

class VPGGenerationOption : public BaseObject<VPGGenerationOption>, public BaseJsonObject
{
    GETSET(std::wstring, Version, L"v0.0.1")
    GETSET(VPGProjectType, ProjectType, VPGProjectType::VccModule)
    GETSET(std::wstring, TemplateGitUrl, L"")
    GETSET(std::wstring, TemplateWorkspace, L"")
    GETSET(std::wstring, ProjectPrefix, L"")
    GETSET(std::wstring, ProjectName, L"VCCModule")
    GETSET(std::wstring, ProjectNameDll, L"libVCCModule")
    GETSET(std::wstring, ProjectNameExe, L"VCCModule")
    GETSET(bool, IsGit, false)
    GETSET(bool, IsExcludeUnittest, false)
    GETSET(bool, IsExcludeVCCUnitTest, false)
    GETSET(std::wstring, TypeWorkspace, L"include/Type")
    GETSET(std::wstring, ActionTypeDirectory, L"include/Type")
    GETSET(std::wstring, ExceptionTypeDirectory, L"include/Type")
    GETSET(std::wstring, ManagerTypeDirectory, L"include/Type")
    GETSET(std::wstring, ObjectTypeDirectory, L"include/Type")
    GETSET(std::wstring, ObjectDirectoryHpp, L"include/Model")
    GETSET(std::wstring, ObjectDirectoryCpp, L"src/Model")
    GETSET(std::wstring, PropertyAccessorDirectoryHpp, L"include/PropertyAccessor")
    GETSET(std::wstring, PropertyAccessorDirectoryCpp, L"src/PropertyAccessor")
    GETSET(std::wstring, ObjectFactoryDirectoryHpp, L"include/Factory")
    GETSET(std::wstring, ObjectFactoryDirectoryCpp, L"src/Factory")
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryHpp, L"include/Factory")
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryCpp, L"src/Factory")
    VECTOR(std::wstring, Plugins)
    VECTOR_SPTR(VPGGenerationOptionExport, Exports)

    public:
        VPGGenerationOption() : BaseObject(ObjectType::GenerationOption) {}
        virtual ~VPGGenerationOption() {}

        virtual std::shared_ptr<IObject> Clone() const override {
            std::shared_ptr<VPGGenerationOption> obj = std::make_shared<VPGGenerationOption>(*this);
            obj->CloneExports(this->_Exports);
            return obj;
        }

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;
};
