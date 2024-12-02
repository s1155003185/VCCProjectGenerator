#include "action_manager.hpp"

#include <math.h>

#include "exception_macro.hpp"
#include "i_action.hpp"

namespace vcc
{
    int64_t ActionManager::_GetFirstSeqNo(const bool &fromBeginning) const
    {
        TRY
            return _Actions.empty() ? -1 : (fromBeginning ? _Actions.begin()->first : _Actions.rbegin()->first);
        CATCH
        return -1;
    }

    int64_t ActionManager::_Redo(const int64_t &noOfStep) const
    { 
        TRY
            for (int64_t i = 0; i < noOfStep; i++) {
                if (_CurrentSeqNo < _GetFirstSeqNo(false)) {
                    _CurrentSeqNo++;
                    _Actions.find(_CurrentSeqNo)->second->Redo();
                }
            }
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::_Undo(const int64_t &noOfStep) const
    {
        TRY
            for (int64_t i = 0; i < noOfStep; i++) {
                if (_CurrentSeqNo > -1 && _CurrentSeqNo >= _GetFirstSeqNo(true)) {
                    _Actions.find(_CurrentSeqNo)->second->Undo();
                    _CurrentSeqNo--;
                }
            }
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::_RemoveAction(const int64_t &noOfAction, const bool &fromBeginning) const
    {
        TRY
            for (auto i = 0; i < noOfAction; i++) {
                if (_Actions.size() > 0)
                    _Actions.erase(fromBeginning ? _Actions.begin()->first : _Actions.rbegin()->first);
            }

            _CurrentSeqNo = _Actions.size() > 0 ? std::min(_CurrentSeqNo, _GetFirstSeqNo(false)) : -1;
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::_ChopActionListToSize(const int64_t &size, const bool &fromBeginning) const
    {
        TRY
            int64_t needToRemove = std::max(static_cast<int64_t>(_Actions.size()) - size, static_cast<int64_t>(0));
            return _RemoveAction(needToRemove, fromBeginning);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::_Clear() const
    {
        TRY
            _Actions.clear();
            _CurrentSeqNo = -1;
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::_Truncate() const
    {
        TRY
            _MaxSeqNo = -1;
            return _Clear();
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::GetFirstSeqNo() const
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _GetFirstSeqNo(true);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::GetLastSeqNo() const
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _GetFirstSeqNo(false);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::DoAction(std::shared_ptr<IAction> action) const
    {
        TRY
            //std::unique_lock lock(_mutex);
            // 1. Remove the action after index
            // 2. Refresh maxSeqNo to last no
            // 2. Append action
            // 3. Do Action
            // 4. Current Index + 1
            _RemoveAction(_GetFirstSeqNo(false) - _CurrentSeqNo, false);
            if (_Actions.size() > 0)
                _MaxSeqNo = _GetFirstSeqNo(false);
            int64_t nextSeqNo = _MaxSeqNo + 1;
            action->SetSeqNo(nextSeqNo);
            _Actions.emplace(nextSeqNo, action);
            action->Redo();
            _MaxSeqNo = _CurrentSeqNo = _GetFirstSeqNo(false);

            return _ChopActionListToSize(_MaxActionListSize, true);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::Redo(const int64_t &noOfStep) const
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _Redo(noOfStep);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::RedoToSeqNo(const int64_t &seqNo) const
    {
        TRY
            return Redo(seqNo - _CurrentSeqNo);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::Undo(const int64_t &noOfStep) const
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _Undo(noOfStep);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::UndoToSeqNo(const int64_t &seqNo) const
    {
        TRY
            return Undo(_CurrentSeqNo - seqNo);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::ChopActionListToSize(const int64_t &size) const
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _ChopActionListToSize(size, true);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::Clear() const
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _Clear();
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::Truncate()const
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _Truncate();
        CATCH
        return _CurrentSeqNo;
    }
}
