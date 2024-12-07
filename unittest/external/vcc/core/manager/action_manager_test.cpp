#include <gtest/gtest.h>

#include <map>
#include <string>

#include "i_action.hpp"
#include "base_action.hpp"
#include "action_manager.hpp"
#include "memory_macro.hpp"

using namespace vcc;

class ActionManagerTestClass final : public BaseAction
{
    private:
        int uuid = 0;
    
    protected:
        virtual void _DoRedo() override {}
        virtual void _DoUndo() override {}

        virtual std::wstring _GetRedoMessage() override { return L""; }
        virtual std::wstring _GetUndoMessage() override { return L""; }

        virtual void _LogRedo() override {}
        virtual void _LogUndo() override {}
    public:
        virtual std::wstring GetMessage() override { return std::to_wstring(uuid); }
 
        ActionManagerTestClass(int uuid) : BaseAction() { this->uuid = uuid; }
        ~ActionManagerTestClass() {}
};

class ActionManagerTest : public testing::Test 
{
    private:
        MANAGER_SPTR(ActionManager, Manager, nullptr);

    public:
        std::shared_ptr<BaseAction> CreateAction(int index)
        {
            return std::make_shared<ActionManagerTestClass>(index);
        }

        void ResetWithFiveActions()
        {
            this->_Manager->Truncate();
            this->_Manager->DoAction(this->CreateAction(0));
            this->_Manager->DoAction(this->CreateAction(1));
            this->_Manager->DoAction(this->CreateAction(2));
            this->_Manager->DoAction(this->CreateAction(3));
            this->_Manager->DoAction(this->CreateAction(4));
        }

        void SetUp() override
        {
            this->_Manager->SetMaxActionListSize(10);
        }

        void TearDown() override
        {

        }
};

TEST_F(ActionManagerTest, EmptyTest) 
{
    auto manager = this->GetManager().get();
    // Get
    EXPECT_EQ(manager->GetFirstSeqNo(), -1);
    EXPECT_EQ(manager->GetLastSeqNo(), -1);
    // Redo
    EXPECT_EQ(manager->Redo(), -1);
    EXPECT_EQ(manager->Redo(2), -1);
    EXPECT_EQ(manager->RedoToSeqNo(2), -1);
    // Undo
    EXPECT_EQ(manager->Undo(), -1);
    EXPECT_EQ(manager->Undo(2), -1);
    EXPECT_EQ(manager->UndoToSeqNo(2), -1);
    // Chop
    EXPECT_EQ(manager->ChopActionListToSize(1), -1);
    // Clear
    EXPECT_EQ(manager->Clear(), -1);
    // Truncate
    EXPECT_EQ(manager->Truncate(), -1);
}

TEST_F(ActionManagerTest, NegativeTest) 
{
    auto manager = this->GetManager().get();
    this->ResetWithFiveActions();
    // Redo
    EXPECT_EQ(manager->Redo(-1), 4);
    EXPECT_EQ(manager->RedoToSeqNo(-1), 4);
    // Undo
    EXPECT_EQ(manager->Undo(-1), 4);
    EXPECT_EQ(manager->UndoToSeqNo(-1), -1);
    // Chop
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(-1), -1);
}

TEST_F(ActionManagerTest, ExceedTest) 
{
    auto manager = this->GetManager().get();
    // Redo
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->Redo(100), 4);
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->RedoToSeqNo(100), 4);
    // Undo
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->Undo(100), -1);
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->UndoToSeqNo(100), 4);
    // Chop
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(100), 4);
 }

TEST_F(ActionManagerTest, RedoUndoTest) 
{
    auto manager = this->GetManager().get();
    // Add
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->DoAction(this->CreateAction(10)), 5);
    EXPECT_TRUE(manager->GetActions().size() == 6);
    EXPECT_EQ(manager->GetActions().at(0)->GetMessage(), L"0");
    EXPECT_EQ(manager->GetActions().at(1)->GetMessage(), L"1");
    EXPECT_EQ(manager->GetActions().at(2)->GetMessage(), L"2");
    EXPECT_EQ(manager->GetActions().at(3)->GetMessage(), L"3");
    EXPECT_EQ(manager->GetActions().at(4)->GetMessage(), L"4");
    EXPECT_EQ(manager->GetActions().at(5)->GetMessage(), L"10");
    // Redo Undo
    EXPECT_EQ(manager->Undo(), 4);
    EXPECT_EQ(manager->Undo(2), 2);
    EXPECT_EQ(manager->Undo(), 1);
    EXPECT_EQ(manager->Redo(), 2);
    EXPECT_EQ(manager->Redo(3), 5);
    EXPECT_TRUE(manager->GetActions().size() == 6);

    // Redo Chop
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->Undo(2), 2);
    EXPECT_EQ(manager->DoAction(this->CreateAction(20)), 3);
    EXPECT_TRUE(manager->GetActions().size() == 4);
    EXPECT_EQ(manager->GetActions().at(0)->GetMessage(), L"0");
    EXPECT_EQ(manager->GetActions().at(1)->GetMessage(), L"1");
    EXPECT_EQ(manager->GetActions().at(2)->GetMessage(), L"2");
    EXPECT_EQ(manager->GetActions().at(3)->GetMessage(), L"20");
}

TEST_F(ActionManagerTest, ChopTest) 
{
    auto manager = this->GetManager().get();
    // ChopWithSize
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(2), 4);
    EXPECT_TRUE(manager->GetActions().size() == 2);
    int64_t firstSeqNo = manager->GetFirstSeqNo();
    EXPECT_EQ(manager->GetActions().at(firstSeqNo)->GetMessage(), L"3");
    EXPECT_EQ(manager->GetActions().at(firstSeqNo + 1)->GetMessage(), L"4");

    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(4), 4);
    EXPECT_TRUE(manager->GetActions().size() == 4);
    firstSeqNo = manager->GetFirstSeqNo();
    EXPECT_EQ(manager->GetActions().at(firstSeqNo)->GetMessage(), L"1");
    EXPECT_EQ(manager->GetActions().at(firstSeqNo + 1)->GetMessage(), L"2");
    EXPECT_EQ(manager->GetActions().at(firstSeqNo + 2)->GetMessage(), L"3");
    EXPECT_EQ(manager->GetActions().at(firstSeqNo + 3)->GetMessage(), L"4");

}

TEST_F(ActionManagerTest, SeqNoContunusTest) 
{
    auto manager = this->GetManager().get();
    // Chop All and seq no can continus
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(0), -1);
    EXPECT_TRUE(manager->GetActions().size() == 0);
    EXPECT_EQ(manager->DoAction(this->CreateAction(10)), 5);

    // Clear
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->Clear(), -1);
    EXPECT_TRUE(manager->GetActions().size() == 0);
    EXPECT_EQ(manager->DoAction(this->CreateAction(10)), 5);

    // Truncate
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->Truncate(), -1);
    EXPECT_TRUE(manager->GetActions().size() == 0);
    EXPECT_EQ(manager->DoAction(this->CreateAction(10)), 0);
}