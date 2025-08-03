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

class VPGConfigTemplate : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(std::wstring, Url, L"")
    GETSET(std::wstring, Workspace, L"")
    GETSET(bool, IsExcludeUnittest, false)
    GETSET(bool, IsExcludeVCCUnitTest, false)

    public:
        VPGConfigTemplate() : vcc::BaseObject(ObjectType::ConfigTemplate) {}
        virtual ~VPGConfigTemplate() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<VPGConfigTemplate>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void deserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfigBehavior : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(VPGConfigActionHistoryType, ActionHistoryType, VPGConfigActionHistoryType::NoHistory)
    GETSET(bool, IsActionResultThrowException, false)

    public:
        VPGConfigBehavior() : vcc::BaseObject(ObjectType::ConfigBehavior) {}
        virtual ~VPGConfigBehavior() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<VPGConfigBehavior>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void deserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfigInput : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(std::wstring, TypeWorkspace, L"include/type")

    public:
        VPGConfigInput() : vcc::BaseObject(ObjectType::ConfigInput) {}
        virtual ~VPGConfigInput() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<VPGConfigInput>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void deserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfigOutput : public vcc::BaseObject, public vcc::BaseJsonObject
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
        VPGConfigOutput() : vcc::BaseObject(ObjectType::ConfigOutput) {}
        virtual ~VPGConfigOutput() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<VPGConfigOutput>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void deserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfigExport : public vcc::BaseObject, public vcc::BaseJsonObject
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
        VPGConfigExport() : vcc::BaseObject(ObjectType::ConfigExport) {}
        virtual ~VPGConfigExport() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<VPGConfigExport>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void deserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfig : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(std::wstring, Version, L"v0.0.1")
    GETSET(VPGProjectType, ProjectType, VPGProjectType::VccModule)
    GETSET(std::wstring, ProjectPrefix, L"")
    GETSET(std::wstring, ProjectName, L"VCCModule")
    GETSET(std::wstring, ProjectNameDll, L"libVCCModule")
    GETSET(std::wstring, ProjectNameExe, L"VCCModule")
    GETSET(bool, IsGit, false)
    GETSET_SPTR(VPGConfigTemplate, Template)
    GETCUSTOM(std::wstring, TemplateUrl, return this->getTemplate() != nullptr ? this->getTemplate()->getUrl() : std::make_shared<VPGConfigTemplate>()->getUrl();)
    GETCUSTOM(std::wstring, TemplateWorkspace, return this->getTemplate() != nullptr ? this->getTemplate()->getWorkspace() : std::make_shared<VPGConfigTemplate>()->getWorkspace();)
    GETCUSTOM(bool, TemplateIsExcludeUnittest, return this->getTemplate() != nullptr ? this->getTemplate()->getIsExcludeUnittest() : std::make_shared<VPGConfigTemplate>()->getIsExcludeUnittest();)
    GETCUSTOM(bool, TemplateIsExcludeVCCUnitTest, return this->getTemplate() != nullptr ? this->getTemplate()->getIsExcludeVCCUnitTest() : std::make_shared<VPGConfigTemplate>()->getIsExcludeVCCUnitTest();)
    GETSET_SPTR(VPGConfigBehavior, Behavior)
    GETCUSTOM(VPGConfigActionHistoryType, BehaviorActionHistoryType, return this->getBehavior() != nullptr ? this->getBehavior()->getActionHistoryType() : std::make_shared<VPGConfigBehavior>()->getActionHistoryType();)
    GETCUSTOM(bool, BehaviorIsActionResultThrowException, return this->getBehavior() != nullptr ? this->getBehavior()->getIsActionResultThrowException() : std::make_shared<VPGConfigBehavior>()->getIsActionResultThrowException();)
    GETSET_SPTR(VPGConfigInput, Input)
    GETCUSTOM(std::wstring, InputTypeWorkspace, return this->getInput() != nullptr ? this->getInput()->getTypeWorkspace() : std::make_shared<VPGConfigInput>()->getTypeWorkspace();)
    GETSET_SPTR(VPGConfigOutput, Output)
    GETCUSTOM(std::wstring, OutputExceptionTypeDirectory, return this->getOutput() != nullptr ? this->getOutput()->getExceptionTypeDirectory() : std::make_shared<VPGConfigOutput>()->getExceptionTypeDirectory();)
    GETCUSTOM(std::wstring, OutputObjectTypeDirectory, return this->getOutput() != nullptr ? this->getOutput()->getObjectTypeDirectory() : std::make_shared<VPGConfigOutput>()->getObjectTypeDirectory();)
    GETCUSTOM(std::wstring, OutputApplicationDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getApplicationDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getApplicationDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputApplicationDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getApplicationDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getApplicationDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputActionDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getActionDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getActionDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputActionDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getActionDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getActionDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputFormDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getFormDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getFormDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputFormDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getFormDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getFormDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputObjectDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getObjectDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getObjectDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputObjectDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getObjectDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getObjectDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputPropertyAccessorDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getPropertyAccessorDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getPropertyAccessorDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputPropertyAccessorDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getPropertyAccessorDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getPropertyAccessorDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputObjectFactoryDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getObjectFactoryDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getObjectFactoryDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputObjectFactoryDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getObjectFactoryDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getObjectFactoryDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputPropertyAccessorFactoryDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getPropertyAccessorFactoryDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getPropertyAccessorFactoryDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputPropertyAccessorFactoryDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getPropertyAccessorFactoryDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getPropertyAccessorFactoryDirectoryCpp();)
    VECTOR(std::wstring, Plugins)
    VECTOR_SPTR(VPGConfigExport, Exports)

    public:
        VPGConfig() : vcc::BaseObject(ObjectType::Config)
        {
            _Template = std::make_shared<VPGConfigTemplate>();
            _Behavior = std::make_shared<VPGConfigBehavior>();
            _Input = std::make_shared<VPGConfigInput>();
            _Output = std::make_shared<VPGConfigOutput>();
        }

        virtual ~VPGConfig() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            auto obj = std::make_shared<VPGConfig>(*this);
            obj->cloneBehavior(this->_Behavior.get());
            obj->cloneExports(this->_Exports);
            obj->cloneInput(this->_Input.get());
            obj->cloneOutput(this->_Output.get());
            obj->cloneTemplate(this->_Template.get());
            return obj;
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void deserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};
