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

        static std::shared_ptr<IObject> GetFormSharedPtr(IObject *IObject);
        static const IForm *GetIFormPtrFromIObject(IObject *obj);

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

        // Form Action
        static void DoFormAction(IObject *form, const int64_t &formProperty);
        static int64_t GetFormActionFirstSeqNo(IObject *form);
        static int64_t GetFormActionLastSeqNo(IObject *form);
        
        static int64_t RedoFormAction(IObject *form, const int64_t &noOfStep = 1);
        static int64_t RedoFormActionToSeqNo(IObject *form, const int64_t &seqNo);

        static int64_t UndoFormAction(IObject *form, const int64_t &noOfStep = 1);
        static int64_t UndoFormActionToSeqNo(IObject *form, const int64_t &seqNo);

        static int64_t ClearFormAction(IObject *form);
        static int64_t TruncateFormAction(IObject *form);

        // Close Form
        static bool IsFormClosable(IObject *form);
        static bool IsFormClosed(IObject *form);
        static bool CloseForm(IObject *form, const bool &isForce = false);

        // Useless
        virtual std::shared_ptr<IObject> Clone() const override { return nullptr; }
        virtual void DoAction(const int64_t &/*formProperty*/) const override {}

        // <vcc:customApplicationPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationPublicFunctions>
};

static std::shared_ptr<Application> application = nullptr;