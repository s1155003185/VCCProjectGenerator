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

        static std::shared_ptr<vcc::IObject> getFormSharedPtr(vcc::IObject *IObject);
        static vcc::IForm *getIFormPtrFromIObject(vcc::IObject *obj);
        static vcc::IObject *getIObjectPtrFromIResult(vcc::IResult *obj);
        static vcc::IResult *getIResultPtrFromIObject(vcc::IObject *obj);

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

        virtual void initializeComponents() override;
 
        static void Run();

        // Create Form
        static std::shared_ptr<vcc::IObject> createForm(const ObjectType &objectType);
        
        // Result
        static int64_t getResultErrorCode(vcc::IObject *result);
        static std::wstring getResultMessage(vcc::IObject *result);
        static bool isErrorResult(vcc::IObject *result);
        static bool isWarningResult(vcc::IObject *result);
        static void eraseResult(vcc::IObject *result);

        // Form Action
        static std::shared_ptr<vcc::IObject> createActionArgument(const ObjectType &objectType);
        static std::shared_ptr<vcc::IObject> doFormAction(vcc::IObject *form, const int64_t &formProperty, IObject *argument);
        static int64_t getFormActionCurrentSeqNo(vcc::IObject *form);
        static int64_t getFormActionFirstSeqNo(vcc::IObject *form);
        static int64_t getFormActionLastSeqNo(vcc::IObject *form);
        
        static void redoFormAction(vcc::IObject *form, const int64_t &noOfStep = 1);
        static void redoFormActionToSeqNo(vcc::IObject *form, const int64_t &seqNo);

        static void undoFormAction(vcc::IObject *form, const int64_t &noOfStep = 1);
        static void undoFormActionToSeqNo(vcc::IObject *form, const int64_t &seqNo);

        static int64_t clearFormAction(vcc::IObject *form);
        static int64_t truncateFormAction(vcc::IObject *form);

        // Close Form
        static bool isFormClosable(vcc::IObject *form);
        static bool isFormClosed(vcc::IObject *form);
        static bool closeForm(vcc::IObject *form, const bool &isForce = false);

        // Useless
        virtual std::shared_ptr<vcc::IObject> clone() const override { return nullptr; }
        virtual std::shared_ptr<vcc::IResult> doAction(const int64_t &/*formProperty*/, std::shared_ptr<vcc::IObject> /*argument*/) override { return nullptr; }

        // <vcc:customApplicationPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationPublicFunctions>
};

static std::shared_ptr<Application> application = nullptr;