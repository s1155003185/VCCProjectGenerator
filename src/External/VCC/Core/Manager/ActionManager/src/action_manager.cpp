#include "action_manager.hpp"

#include <math.h>

#include "exception_macro.hpp"
#include "i_action.hpp"

namespace vcc
{
    int64_t ActionManager::_GetFirstSeqNo(bool fromBeginning)
    {
        try {
            return this->_Actions.empty() ? -1 : (fromBeginning ? this->_Actions.begin()->first : this->_Actions.rbegin()->first);
        } catch (exception &e) {
            THROW_EXCEPTION(ExceptionType::CUSTOM_ERROR, str2wstr(e.what()));
        }
    }

    int64_t ActionManager::_Redo(int64_t noOfStep) 
    {
        for (int64_t i = 0; i < noOfStep; i++) {
            if (this->_CurrentSeqNo < this->_GetFirstSeqNo(false)) {
                this->_CurrentSeqNo++;
                this->_Actions[this->_CurrentSeqNo]->Redo();
            }
        }
        return this->_CurrentSeqNo;
    }

    int64_t ActionManager::_Undo(int64_t noOfStep) 
    {
        for (int64_t i = 0; i < noOfStep; i++) {
            if (this->_CurrentSeqNo > -1 && this->_CurrentSeqNo >= this->_GetFirstSeqNo(true)) {
                this->_Actions[this->_CurrentSeqNo]->Undo();
                this->_CurrentSeqNo--;
            }
        }
        return this->_CurrentSeqNo;
    }

    int64_t ActionManager::_RemoveAction(int64_t noOfAction, bool fromBeginning)
    {
        for (auto i = 0; i < noOfAction; i++) {
            if (this->_Actions.size() > 0)
                this->_Actions.erase(fromBeginning ? this->_Actions.begin()->first : this->_Actions.rbegin()->first);
        }

        this->_CurrentSeqNo = this->_Actions.size() > 0 ? min(this->_CurrentSeqNo, this->_GetFirstSeqNo(false)) : -1;
        return this->_CurrentSeqNo;
    }

    int64_t ActionManager::_ChopActionListToSize(int64_t size, bool fromBeginning)
    {
        int64_t needToRemove = max(static_cast<int64_t>(this->_Actions.size()) - size, static_cast<int64_t>(0));
        return this->_RemoveAction(needToRemove, fromBeginning);
    }

    int64_t ActionManager::_Clear()
    {
        this->_Actions.clear();
        this->_CurrentSeqNo = -1;
        return this->_CurrentSeqNo;
    }

    int64_t ActionManager::_Truncate()
    {
        this->_MaxSeqNo = -1;
        return this->_Clear();
    }

    int64_t ActionManager::GetFirstSeqNo()
    {
        LOCK_GUAND;
        return this->_GetFirstSeqNo(true);
    }

    int64_t ActionManager::GetLastSeqNo()
    {
        LOCK_GUAND;
        return this->_GetFirstSeqNo(false);
    }

    int64_t ActionManager::DoAction(std::shared_ptr<IAction> action)
    {
        LOCK_GUAND;
        // 1. Remove the action after index
        // 2. Refresh maxSeqNo to last no
        // 2. Append action
        // 3. Do Action
        // 4. Current Index + 1
        this->_RemoveAction(this->_GetFirstSeqNo(false) - this->_CurrentSeqNo, false);
        if (this->_Actions.size() > 0)
            this->_MaxSeqNo = this->_GetFirstSeqNo(false);
        int64_t nextSeqNo = this->_MaxSeqNo + 1;
        action->SetSeqNo(nextSeqNo);
        this->_Actions[nextSeqNo] = action;
        action->Redo();
        this->_MaxSeqNo = this->_CurrentSeqNo = this->_GetFirstSeqNo(false);

        return this->_ChopActionListToSize(this->_MaxActionListSize, true);
    }

    int64_t ActionManager::Redo(int64_t noOfStep)
    {
        LOCK_GUAND;
        return this->_Redo(noOfStep);
    }

    int64_t ActionManager::RedoToSeqNo(int64_t seqNo)
    {
        return this->Redo(seqNo - this->_CurrentSeqNo);
    }

    int64_t ActionManager::Undo(int64_t noOfStep)
    {
        LOCK_GUAND;
        return this->_Undo(noOfStep);
    }

    int64_t ActionManager::UndoToSeqNo(int64_t seqNo)
    {
        return this->Undo(this->_CurrentSeqNo - seqNo);
    }

    int64_t ActionManager::ChopActionListToSize(int64_t size)
    {
        LOCK_GUAND;
        return this->_ChopActionListToSize(size, true);
    }

    int64_t ActionManager::Clear()
    {
        LOCK_GUAND;
        return this->_Clear();
    }

    int64_t ActionManager::Truncate()
    {
        LOCK_GUAND;
        return this->_Truncate();
    }
}
