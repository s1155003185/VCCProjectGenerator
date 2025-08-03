#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <string>

#include "i_action.hpp"
#include "base_action.hpp"
#include "action_manager.hpp"
#include "operation_result.hpp"

class ActionManagerTestClass final : public vcc::BaseAction
{
    private:
        int uuid = 0;
    
    protected:
        virtual std::shared_ptr<vcc::IResult> onRedo() override { return std::make_shared<vcc::OperationResult>(); }
        virtual std::shared_ptr<vcc::IResult> onUndo() override { return std::make_shared<vcc::OperationResult>(); }
        
        virtual std::wstring getRedoMessageStart() const override { return L""; }
        virtual std::wstring getRedoMessageComplete() const override { return L""; }
        virtual std::wstring getUndoMessageStart() const override { return L""; }
        virtual std::wstring getUndoMessageComplete() const override { return L""; }
    public:
        ActionManagerTestClass(int uuid) : vcc::BaseAction() { this->uuid = uuid; }
        ~ActionManagerTestClass() {}

        std::wstring getMessage() { return std::to_wstring(uuid); }
};

class ActionManagerTest : public testing::Test 
{
    private:
        MANAGER_SPTR_NULL(vcc::ActionManager, Manager);

    public:
        std::shared_ptr<vcc::BaseAction> CreateAction(int index)
        {
            return std::make_shared<ActionManagerTestClass>(index);
        }

        void ResetWithFiveActions()
        {
            this->_Manager->truncate();
            this->_Manager->doAction(this->CreateAction(0));
            this->_Manager->doAction(this->CreateAction(1));
            this->_Manager->doAction(this->CreateAction(2));
            this->_Manager->doAction(this->CreateAction(3));
            this->_Manager->doAction(this->CreateAction(4));
        }

        void SetUp() override
        {
            _Manager = std::make_shared<vcc::ActionManager>(nullptr);
            this->_Manager->setMaxActionListSize(10);
        }

        void TearDown() override
        {

        }
};

TEST_F(ActionManagerTest, EmptyTest) 
{
    auto manager = this->getManager().get();
    // get
    EXPECT_EQ(manager->getFirstSeqNo(), -1);
    EXPECT_EQ(manager->getLastSeqNo(), -1);
    // Redo
    manager->Redo();
    EXPECT_EQ(manager->getCurrentSeqNo(), -1);
    manager->Redo(2);
    EXPECT_EQ(manager->getCurrentSeqNo(), -1);
    manager->redoToSeqNo(2);
    EXPECT_EQ(manager->getCurrentSeqNo(), -1);
    // Undo
    manager->Undo();
    EXPECT_EQ(manager->getCurrentSeqNo(), -1);
    manager->Undo(2);
    EXPECT_EQ(manager->getCurrentSeqNo(), -1);
    manager->UndoToSeqNo(2);
    EXPECT_EQ(manager->getCurrentSeqNo(), -1);
    // Chop
    EXPECT_EQ(manager->chopActionListToSize(1), -1);
    // Clear
    EXPECT_EQ(manager->clear(), -1);
    // truncate
    EXPECT_EQ(manager->truncate(), -1);
}

TEST_F(ActionManagerTest, NegativeTest) 
{
    auto manager = this->getManager().get();
    this->ResetWithFiveActions();
    // Redo
    manager->Redo(-1);
    EXPECT_EQ(manager->getCurrentSeqNo(), 4);
    manager->redoToSeqNo(-1);
    EXPECT_EQ(manager->getCurrentSeqNo(), 4);
    // Undo
    manager->Undo(-1);
    EXPECT_EQ(manager->getCurrentSeqNo(), 4);
    manager->UndoToSeqNo(-1);
    EXPECT_EQ(manager->getCurrentSeqNo(), -1);
    // Chop
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->chopActionListToSize(-1), -1);
}

TEST_F(ActionManagerTest, ExceedTest) 
{
    auto manager = this->getManager().get();
    // Redo
    this->ResetWithFiveActions();
    manager->Redo(100);
    EXPECT_EQ(manager->getCurrentSeqNo(), 4);
    this->ResetWithFiveActions();
    manager->redoToSeqNo(100);
    EXPECT_EQ(manager->getCurrentSeqNo(), 4);
    // Undo
    this->ResetWithFiveActions();
    manager->Undo(100);
    EXPECT_EQ(manager->getCurrentSeqNo(), -1);
    this->ResetWithFiveActions();
    manager->UndoToSeqNo(100);
    EXPECT_EQ(manager->getCurrentSeqNo(), 4);
    // Chop
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->chopActionListToSize(100), 4);
 }

TEST_F(ActionManagerTest, RedoUndoTest) 
{
    auto manager = this->getManager().get();
    // Add
    this->ResetWithFiveActions();
    manager->doAction(this->CreateAction(10));
    EXPECT_EQ(manager->getCurrentSeqNo(), 5);
    EXPECT_TRUE(manager->getActions().size() == 6);
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(0))->getMessage(), L"0");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(1))->getMessage(), L"1");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(2))->getMessage(), L"2");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(3))->getMessage(), L"3");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(4))->getMessage(), L"4");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(5))->getMessage(), L"10");
    // Redo Undo
    manager->Undo();
    EXPECT_EQ(manager->getCurrentSeqNo(), 4);
    manager->Undo(2);
    EXPECT_EQ(manager->getCurrentSeqNo(), 2);
    manager->Undo();
    EXPECT_EQ(manager->getCurrentSeqNo(), 1);
    manager->Redo();
    EXPECT_EQ(manager->getCurrentSeqNo(), 2);
    manager->Redo(3);
    EXPECT_EQ(manager->getCurrentSeqNo(), 5);
    EXPECT_TRUE(manager->getActions().size() == 6);

    // Redo Chop
    this->ResetWithFiveActions();
    manager->Undo(2);
    EXPECT_EQ(manager->getCurrentSeqNo(), 2);
    manager->doAction(this->CreateAction(20));
    EXPECT_EQ(manager->getCurrentSeqNo(), 3);
    EXPECT_TRUE(manager->getActions().size() == 4);
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(0))->getMessage(), L"0");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(1))->getMessage(), L"1");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(2))->getMessage(), L"2");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(3))->getMessage(), L"20");
}

TEST_F(ActionManagerTest, ChopTest) 
{
    auto manager = this->getManager().get();
    // ChopWithSize
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->chopActionListToSize(2), 4);
    EXPECT_TRUE(manager->getActions().size() == 2);
    int64_t firstSeqNo = manager->getFirstSeqNo();
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(firstSeqNo))->getMessage(), L"3");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(firstSeqNo + 1))->getMessage(), L"4");

    this->ResetWithFiveActions();
    EXPECT_EQ(manager->chopActionListToSize(4), 4);
    EXPECT_TRUE(manager->getActions().size() == 4);
    firstSeqNo = manager->getFirstSeqNo();
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(firstSeqNo))->getMessage(), L"1");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(firstSeqNo + 1))->getMessage(), L"2");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(firstSeqNo + 2))->getMessage(), L"3");
    EXPECT_EQ(std::dynamic_pointer_cast<ActionManagerTestClass>(manager->getActions().at(firstSeqNo + 3))->getMessage(), L"4");
}

TEST_F(ActionManagerTest, SeqNoContunusTest) 
{
    auto manager = this->getManager().get();
    // Chop All and seq no can continus
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->chopActionListToSize(0), -1);
    EXPECT_TRUE(manager->getActions().size() == 0);
    manager->doAction(this->CreateAction(10));
    EXPECT_EQ(manager->getCurrentSeqNo(), 5);

    // Clear
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->clear(), -1);
    EXPECT_TRUE(manager->getActions().size() == 0);
    manager->doAction(this->CreateAction(10));
    EXPECT_EQ(manager->getCurrentSeqNo(), 5);

    // truncate
    this->ResetWithFiveActions();
    EXPECT_EQ(manager->truncate(), -1);
    EXPECT_TRUE(manager->getActions().size() == 0);
    manager->doAction(this->CreateAction(10));
    EXPECT_EQ(manager->getCurrentSeqNo(), 0);
}