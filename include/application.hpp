// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <set>
#include <memory>

#include "base_form.hpp"
#include "i_form.hpp"
#include "i_object.hpp"
#include "object_type.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

class Application : public vcc::BaseForm
{
    private:
        std::set<std::shared_ptr<vcc::IObject>> _Forms;
        std::set<std::shared_ptr<vcc::IObject>> _ActionArguments;
        std::set<std::shared_ptr<vcc::IObject>> _Results;

        static std::shared_ptr<vcc::IObject> GetFormSharedPtr(vcc::IObject *IObject);
        static vcc::IForm *GetIFormPtrFromIObject(vcc::IObject *obj);
        static vcc::IObject *GetIObjectPtrFromIResult(vcc::IResult *obj);
        static vcc::IResult *GetIResultPtrFromIObject(vcc::IObject *obj);

        // <vcc:customApplicationProperties sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationProperties>
        
    private:
        // <vcc:customApplicationPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationPrivateFunctions>

    protected:
        // <vcc:customApplicationProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationProtectedFunctions>

    public:
        Application() : vcc::BaseForm() {}
        ~Application() {}

        virtual void InitializeComponents() override;
 
        static void Run();

        // Create Form
        static std::shared_ptr<vcc::IObject> CreateForm(const ObjectType &objectType);
        
        // Result
        static int64_t GetResultErrorCode(vcc::IObject *result);
        static std::wstring GetResultMessage(vcc::IObject *result);
        static bool IsErrorResult(vcc::IObject *result);
        static bool IsWarningResult(vcc::IObject *result);
        static void EraseResult(vcc::IObject *result);

        // Form Action
        static std::shared_ptr<vcc::IObject> CreateActionArgument(const ObjectType &objectType);
        static std::shared_ptr<vcc::IObject> DoFormAction(vcc::IObject *form, const int64_t &formProperty, IObject *argument);
        static int64_t GetFormActionCurrentSeqNo(vcc::IObject *form);
        static int64_t GetFormActionFirstSeqNo(vcc::IObject *form);
        static int64_t GetFormActionLastSeqNo(vcc::IObject *form);
        
        static void RedoFormAction(vcc::IObject *form, const int64_t &noOfStep = 1);
        static void RedoFormActionToSeqNo(vcc::IObject *form, const int64_t &seqNo);

        static void UndoFormAction(vcc::IObject *form, const int64_t &noOfStep = 1);
        static void UndoFormActionToSeqNo(vcc::IObject *form, const int64_t &seqNo);

        static int64_t ClearFormAction(vcc::IObject *form);
        static int64_t TruncateFormAction(vcc::IObject *form);

        // Close Form
        static bool IsFormClosable(vcc::IObject *form);
        static bool IsFormClosed(vcc::IObject *form);
        static bool CloseForm(vcc::IObject *form, const bool &isForce = false);

        // Useless
        virtual std::shared_ptr<vcc::IObject> Clone() const override { return nullptr; }
        virtual std::shared_ptr<vcc::IResult> DoAction(const int64_t &/*formProperty*/, std::shared_ptr<vcc::IObject> /*argument*/) override { return nullptr; }

        // <vcc:customApplicationPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationPublicFunctions>
};

static std::shared_ptr<Application> application = nullptr;