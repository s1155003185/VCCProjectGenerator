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

class VPGGenerationOptionExport : public BaseObject, public BaseJsonObject
{
    GETSET(VPGGenerationOptionInterfaceType, Interface, VPGGenerationOptionInterfaceType::Java)
    GETSET(std::wstring, Workspace, L"../JavaProject")
    GETSET(bool, IsExportExternalLib, true)
    GETSET(std::wstring, ExportDirectoryDll, L"src/main/resources")
    GETSET(std::wstring, ExportDirectoryExe, L"")
    GETSET(std::wstring, DllBridgeDirectory, L"src/main/java/com/package")
    GETSET(std::wstring, FormDirectory, L"src/main/java/com/package/form")
    GETSET(std::wstring, ObjectDirectory, L"src/main/java/com/package/model")
    GETSET(std::wstring, TypeDirectory, L"src/main/java/com/package/type")

    public:
        VPGGenerationOptionExport() : BaseObject(ObjectType::GenerationOptionExport) {}
        virtual ~VPGGenerationOptionExport() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGGenerationOptionExport>(*this);
        }

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;
};

class VPGGenerationOption : public BaseObject, public BaseJsonObject
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
    GETSET(std::wstring, TypeWorkspace, L"include/type")
    GETSET(std::wstring, ExceptionTypeDirectory, L"include/type")
    GETSET(std::wstring, ObjectTypeDirectory, L"include/type")
    GETSET(std::wstring, ApplicationDirectoryHpp, L"include")
    GETSET(std::wstring, ApplicationDirectoryCpp, L"src")
    GETSET(std::wstring, ActionDirectoryHpp, L"")
    GETSET(std::wstring, ActionDirectoryCpp, L"")
    GETSET(std::wstring, FormDirectoryHpp, L"include/form")
    GETSET(std::wstring, FormDirectoryCpp, L"src/form")
    GETSET(std::wstring, ObjectDirectoryHpp, L"include/model")
    GETSET(std::wstring, ObjectDirectoryCpp, L"src/model")
    GETSET(std::wstring, PropertyAccessorDirectoryHpp, L"include/propertyAccessor")
    GETSET(std::wstring, PropertyAccessorDirectoryCpp, L"src/propertyAccessor")
    GETSET(std::wstring, ObjectFactoryDirectoryHpp, L"include/factory")
    GETSET(std::wstring, ObjectFactoryDirectoryCpp, L"src/factory")
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryHpp, L"include/factory")
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryCpp, L"src/factory")
    VECTOR(std::wstring, Plugins)
    VECTOR_SPTR(VPGGenerationOptionExport, Exports)

    public:
        VPGGenerationOption() : BaseObject(ObjectType::GenerationOption) {}
        virtual ~VPGGenerationOption() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            auto obj = std::make_shared<VPGGenerationOption>(*this);
            obj->CloneExports(this->_Exports);
            return obj;
        }

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;
};
