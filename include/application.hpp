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

using namespace vcc;

class Application : public BaseForm
{
    private:
        std::set<std::shared_ptr<IObject>> _Forms;
        std::set<std::shared_ptr<IObject>> _ActionArguments;
        std::set<std::shared_ptr<IObject>> _Results;

        static std::shared_ptr<IObject> GetFormSharedPtr(IObject *IObject);
        static IForm *GetIFormPtrFromIObject(IObject *obj);
        static IObject *GetIObjectPtrFromIResult(IResult *obj);
        static IResult *GetIResultPtrFromIObject(IObject *obj);

        // <vcc:customApplicationProperties sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationProperties>
        
    private:
        // <vcc:customApplicationPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationPrivateFunctions>

    protected:
        // <vcc:customApplicationProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationProtectedFunctions>

    public:
        Application() : BaseForm() {}
        ~Application() {}

        virtual void InitializeComponents() const override;
 
        static void Run();

        // Create Form
        static std::shared_ptr<IObject> CreateForm(const ObjectType &objectType);
        
        // Result
        static int64_t GetResultErrorCode(IObject *result);
        static std::wstring GetResultMessage(IObject *result);
        static bool IsErrorResult(IObject *result);
        static bool IsWarningResult(IObject *result);
        static void EraseResult(IObject *result);

        // Form Action
        static std::shared_ptr<IObject> CreateActionArgument(const ObjectType &objectType);
        static std::shared_ptr<IObject> DoFormAction(IObject *form, const int64_t &formProperty, IObject *argument);
        static int64_t GetFormActionCurrentSeqNo(IObject *form);
        static int64_t GetFormActionFirstSeqNo(IObject *form);
        static int64_t GetFormActionLastSeqNo(IObject *form);
        
        static void RedoFormAction(IObject *form, const int64_t &noOfStep = 1);
        static void RedoFormActionToSeqNo(IObject *form, const int64_t &seqNo);

        static void UndoFormAction(IObject *form, const int64_t &noOfStep = 1);
        static void UndoFormActionToSeqNo(IObject *form, const int64_t &seqNo);

        static int64_t ClearFormAction(IObject *form);
        static int64_t TruncateFormAction(IObject *form);

        // Close Form
        static bool IsFormClosable(IObject *form);
        static bool IsFormClosed(IObject *form);
        static bool CloseForm(IObject *form, const bool &isForce = false);

        // Useless
        virtual std::shared_ptr<IObject> Clone() const override { return nullptr; }
        virtual std::shared_ptr<IResult> DoAction(const int64_t &/*formProperty*/, std::shared_ptr<IObject> /*argument*/) override { return nullptr; }

        // <vcc:customApplicationPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationPublicFunctions>
};

static std::shared_ptr<Application> application = nullptr;