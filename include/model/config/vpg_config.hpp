#pragma once

#include <memory>
#include <string>

#include "base_json_object.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "i_document.hpp"
#include "json.hpp"
#include "object_type.hpp"
#include "vpg_config_type.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGConfigTemplate : public BaseObject, public BaseJsonObject
{
    GETSET(std::wstring, Url, L"")
    GETSET(std::wstring, Workspace, L"")
    GETSET(bool, IsExcludeUnittest, false)
    GETSET(bool, IsExcludeVCCUnitTest, false)

    public:
        VPGConfigTemplate() : BaseObject(ObjectType::ConfigTemplate) {}
        virtual ~VPGConfigTemplate() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGConfigTemplate>(*this);
        }

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) override;
};

class VPGConfigBehavior : public BaseObject, public BaseJsonObject
{
    GETSET(VPGConfigActionHistoryType, ActionHistoryType, VPGConfigActionHistoryType::NoHistory)
    GETSET(bool, IsActionResultThrowException, false)

    public:
        VPGConfigBehavior() : BaseObject(ObjectType::ConfigBehavior) {}
        virtual ~VPGConfigBehavior() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGConfigBehavior>(*this);
        }

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) override;
};

class VPGConfigInput : public BaseObject, public BaseJsonObject
{
    GETSET(std::wstring, TypeWorkspace, L"include/type")

    public:
        VPGConfigInput() : BaseObject(ObjectType::ConfigInput) {}
        virtual ~VPGConfigInput() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGConfigInput>(*this);
        }

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) override;
};

class VPGConfigOutput : public BaseObject, public BaseJsonObject
{
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
    GETSET(std::wstring, PropertyAccessorDirectoryHpp, L"include/propertyaccessor")
    GETSET(std::wstring, PropertyAccessorDirectoryCpp, L"src/propertyaccessor")
    GETSET(std::wstring, ObjectFactoryDirectoryHpp, L"include/factory")
    GETSET(std::wstring, ObjectFactoryDirectoryCpp, L"src/factory")
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryHpp, L"include/factory")
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryCpp, L"src/factory")

    public:
        VPGConfigOutput() : BaseObject(ObjectType::ConfigOutput) {}
        virtual ~VPGConfigOutput() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGConfigOutput>(*this);
        }

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) override;
};

class VPGConfigExport : public BaseObject, public BaseJsonObject
{
    GETSET(VPGConfigInterfaceType, Interface, VPGConfigInterfaceType::Java)
    GETSET(std::wstring, Workspace, L"../JavaProject")
    GETSET(bool, IsExportExternalLib, true)
    GETSET(std::wstring, ExportDirectoryDll, L"src/main/resources")
    GETSET(std::wstring, ExportDirectoryExe, L"")
    GETSET(std::wstring, DllBridgeDirectory, L"src/main/java/com/package")
    GETSET(std::wstring, FormDirectory, L"src/main/java/com/package/form")
    GETSET(std::wstring, ObjectDirectory, L"src/main/java/com/package/model")
    GETSET(std::wstring, TypeDirectory, L"src/main/java/com/package/type")

    public:
        VPGConfigExport() : BaseObject(ObjectType::ConfigExport) {}
        virtual ~VPGConfigExport() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGConfigExport>(*this);
        }

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) override;
};

class VPGConfig : public BaseObject, public BaseJsonObject
{
    GETSET(std::wstring, Version, L"v0.0.1")
    GETSET(VPGProjectType, ProjectType, VPGProjectType::VccModule)
    GETSET(std::wstring, ProjectPrefix, L"")
    GETSET(std::wstring, ProjectName, L"VCCModule")
    GETSET(std::wstring, ProjectNameDll, L"libVCCModule")
    GETSET(std::wstring, ProjectNameExe, L"VCCModule")
    GETSET(bool, IsGit, false)
    GETSET_SPTR(VPGConfigTemplate, Template)
    GETSET_SPTR(VPGConfigBehavior, Behavior)
    GETSET_SPTR(VPGConfigInput, Input)
    GETSET_SPTR(VPGConfigOutput, Output)
    VECTOR(std::wstring, Plugins)
    VECTOR_SPTR(VPGConfigExport, Exports)

    public:
        VPGConfig() : BaseObject(ObjectType::Config)
        {
            _Template = std::make_shared<VPGConfigTemplate>();
            _Behavior = std::make_shared<VPGConfigBehavior>();
            _Input = std::make_shared<VPGConfigInput>();
            _Output = std::make_shared<VPGConfigOutput>();
        }

        virtual ~VPGConfig() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            auto obj = std::make_shared<VPGConfig>(*this);
            obj->CloneBehavior(this->_Behavior.get());
            obj->CloneExports(this->_Exports);
            obj->CloneInput(this->_Input.get());
            obj->CloneOutput(this->_Output.get());
            obj->CloneTemplate(this->_Template.get());
            return obj;
        }

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) override;
};
