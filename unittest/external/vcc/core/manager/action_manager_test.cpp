#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <string>

#include "i_action.hpp"
#include "base_action.hpp"
#include "action_manager.hpp"
#include "operation_result.hpp"

using namespace vcc;

class ActionManagerTestClass final : public BaseAction
{
    private:
        int uuid = 0;
    
    protected:
        virtual std::shared_ptr<IResult> OnRedo() override { return std::make_shared<OperationResult>(); }
        virtual std::shared_ptr<IResult> OnUndo() override { return std::make_shared<OperationResult>(); }
        
        virtual std::wstring GetRedoMessageStart() const override { return L""; }
        virtual std::wstring GetRedoMessageComplete() const override { return L""; }
        virtual std::wstring GetUndoMessageStart() const override { return L""; }
        virtual std::wstring GetUndoMessageComplete() const override { return L""; }
    public:
        ActionManagerTestClass(int uuid) : BaseAction() { this->uuid = uuid; }
        ~ActionManagerTestClass() {}

        std::wstring GetMessage() { return std::to_wstring(uuid); }
};

class ActionManagerTest : public testing::Test 
{
    private:
        MANAGER_SPTR_NULL(ActionManager, Manager);

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
            _Manager = std::make_shared<ActionManager>(nullptr);
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
    manager->Redo();
    EXPECT_EQ(manager->GetCurrentSeqNo(), -1);
    manager->Redo(2);
    EXPECT_EQ(manager->GetCurrentSeqNo(), -1);
    manager->RedoToSeqNo(2);
    EXPECT_EQ(manager->GetCurrentSeqNo(), -1);
    // Undo
    manager->Undo();
    EXPECT_EQ(manager->GetCurrentSeqNo(), -1);
    manager->Undo(2);
    EXPECT_EQ(manager->GetCurrentSeqNo(), -1);
    manager->UndoToSeqNo(2);
    EXPECT_EQ(manager->GetCurrentSeqNo(), -1);
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
    manager->Redo(-1);
    EXPECT_EQ(manager->GetCurrentSeqNo(), 4);
    manager->RedoToSeqNo(-1);
    EXPECT_EQ(manager->GetCurrentSeqNo(), 4);
    // Undo
    manager->Undo(-1);
    EXPECT_EQ(manager->GetCurrentSeqNo(), 4);
    manager->UndoToSeqNo(-1);
    EXPECT_EQ(manager->GetCurrentSeqNo(), -1);
    // Chop
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(-1), -1);
}

TEST_F(ActionManagerTest, ExceedTest) 
{
    auto manager = this->GetManager().get();
    // Redo
    this->ResetWithFiveActions();
    manager->Redo(100);
    EXPECT_EQ(manager->GetCurrentSeqNo(), 4);
    this->ResetWithFiveActions();
    manager->RedoToSeqNo(100);
    EXPECT_EQ(manager->GetCurrentSeqNo(), 4);
    // Undo
    this->ResetWithFiveActions();
    manager->Undo(100);
    EXPECT_EQ(manager->GetCurrentSeqNo(), -1);
    this->ResetWithFiveActions();
    manager->UndoToSeqNo(100);
    EXPECT_EQ(manager->GetCurrentSeqNo(), 4);
    // Chop
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(100), 4);
 }

TEST_F(ActionManagerTest, RedoUndoTest) 
{
    auto manager = this->GetManager().get();
    // Add
    this->ResetWithFiveActions();
    manager->DoAction(this->CreateAction(10));
    EXPECT_EQ(manager->GetCurrentSeqNo(), 5);
    EXPECT_TRUE(manager->GetActions().size() == 6);
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(0))->GetMessage(), L"0");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(1))->GetMessage(), L"1");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(2))->GetMessage(), L"2");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(3))->GetMessage(), L"3");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(4))->GetMessage(), L"4");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(5))->GetMessage(), L"10");
    // Redo Undo
    manager->Undo();
    EXPECT_EQ(manager->GetCurrentSeqNo(), 4);
    manager->Undo(2);
    EXPECT_EQ(manager->GetCurrentSeqNo(), 2);
    manager->Undo();
    EXPECT_EQ(manager->GetCurrentSeqNo(), 1);
    manager->Redo();
    EXPECT_EQ(manager->GetCurrentSeqNo(), 2);
    manager->Redo(3);
    EXPECT_EQ(manager->GetCurrentSeqNo(), 5);
    EXPECT_TRUE(manager->GetActions().size() == 6);

    // Redo Chop
    this->ResetWithFiveActions();
    manager->Undo(2);
    EXPECT_EQ(manager->GetCurrentSeqNo(), 2);
    manager->DoAction(this->CreateAction(20));
    EXPECT_EQ(manager->GetCurrentSeqNo(), 3);
    EXPECT_TRUE(manager->GetActions().size() == 4);
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(0))->GetMessage(), L"0");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(1))->GetMessage(), L"1");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(2))->GetMessage(), L"2");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(3))->GetMessage(), L"20");
}

TEST_F(ActionManagerTest, ChopTest) 
{
    auto manager = this->GetManager().get();
    // ChopWithSize
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(2), 4);
    EXPECT_TRUE(manager->GetActions().size() == 2);
    int64_t firstSeqNo = manager->GetFirstSeqNo();
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(firstSeqNo))->GetMessage(), L"3");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(firstSeqNo + 1))->GetMessage(), L"4");

    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(4), 4);
    EXPECT_TRUE(manager->GetActions().size() == 4);
    firstSeqNo = manager->GetFirstSeqNo();
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(firstSeqNo))->GetMessage(), L"1");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(firstSeqNo + 1))->GetMessage(), L"2");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(firstSeqNo + 2))->GetMessage(), L"3");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->GetActions().at(firstSeqNo + 3))->GetMessage(), L"4");
}

TEST_F(ActionManagerTest, SeqNoContunusTest) 
{
    auto manager = this->GetManager().get();
    // Chop All and seq no can continus
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->ChopActionListToSize(0), -1);
    EXPECT_TRUE(manager->GetActions().size() == 0);
    manager->DoAction(this->CreateAction(10));
    EXPECT_EQ(manager->GetCurrentSeqNo(), 5);

    // Clear
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->Clear(), -1);
    EXPECT_TRUE(manager->GetActions().size() == 0);
    manager->DoAction(this->CreateAction(10));
    EXPECT_EQ(manager->GetCurrentSeqNo(), 5);

    // Truncate
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->Truncate(), -1);
    EXPECT_TRUE(manager->GetActions().size() == 0);
    manager->DoAction(this->CreateAction(10));
    EXPECT_EQ(manager->GetCurrentSeqNo(), 0);
}