#include "action_manager.hpp"

#include <assert.h>
#include <math.h>

#include "exception_macro.hpp"
#include "i_action.hpp"

namespace vcc
{
    int64_t ActionManager::_getFirstSeqNo(const bool &fromBeginning) const
    {
        TRY
            return _Actions.empty() ? -1 : (fromBeginning ? _Actions.begin()->first : _Actions.rbegin()->first);
        CATCH
        return -1;
    }

    std::shared_ptr<IResult> ActionManager::_Redo(const int64_t &noOfStep)
    { 
        TRY
            std::shared_ptr<IResult> result = nullptr;
            for (int64_t i = 0; i < noOfStep; i++) {
                if (_CurrentSeqNo < _getFirstSeqNo(false)) {
                    _CurrentSeqNo++;
                    result = _Actions.find(_CurrentSeqNo)->second->Redo();
                }
            }
            return result;
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> ActionManager::_Undo(const int64_t &noOfStep)
    {
        TRY
            std::shared_ptr<IResult> result = nullptr;
            for (int64_t i = 0; i < noOfStep; i++) {
                if (_CurrentSeqNo > -1 && _CurrentSeqNo >= _getFirstSeqNo(true)) {
                    result = _Actions.find(_CurrentSeqNo)->second->Undo();
                    _CurrentSeqNo--;
                }
            }
            return result;
        CATCH
        return nullptr;
    }

    int64_t ActionManager::_removeAction(const int64_t &noOfAction, const bool &fromBeginning)
    {
        TRY
            for (auto i = 0; i < noOfAction; i++) {
                if (_Actions.size() > 0)
                    _Actions.erase(fromBeginning ? _Actions.begin()->first : _Actions.rbegin()->first);
            }

            _CurrentSeqNo = _Actions.size() > 0 ? std::min(_CurrentSeqNo, _getFirstSeqNo(false)) : -1;
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::_chopActionListToSize(const int64_t &size, const bool &fromBeginning)
    {
        TRY
            int64_t needToRemove = std::max(static_cast<int64_t>(_Actions.size()) - size, static_cast<int64_t>(0));
            return _removeAction(needToRemove, fromBeginning);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::_clear()
    {
        TRY
            _Actions.clear();
            _CurrentSeqNo = -1;
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::_truncate()
    {
        TRY
            _MaxSeqNo = -1;
            return _clear();
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::getFirstSeqNo() const
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _getFirstSeqNo(true);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::getLastSeqNo() const
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _getFirstSeqNo(false);
        CATCH
        return _CurrentSeqNo;
    }

    std::shared_ptr<IResult> ActionManager::doAction(std::shared_ptr<IAction> action)
    {
        TRY
            //std::unique_lock lock(_mutex);
            // 1. Execute Action If error then throw exception
            // 2. Remove the action after index
            // 3. Refresh maxSeqNo to last no
            // 4. Append action
            // 5. Current Index + 1
            auto baseAction = std::dynamic_pointer_cast<BaseAction>(action);
            assert(baseAction != nullptr);

            auto result = action->Redo();
            if (result->isError()) {
                if (result->isThrowException())
                    THROW_EXCEPTION_MSG(result->getExceptionType(), result->getMessage());
                return result;
            }

            _removeAction(_getFirstSeqNo(false) - _CurrentSeqNo, false);
            if (_Actions.size() > 0)
                _MaxSeqNo = _getFirstSeqNo(false);
            int64_t nextSeqNo = _MaxSeqNo + 1;
            action->setSeqNo(nextSeqNo);
            _Actions.emplace(nextSeqNo, baseAction);
            _MaxSeqNo = _CurrentSeqNo = _getFirstSeqNo(false);
            _chopActionListToSize(_MaxActionListSize, true);
            return result;
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> ActionManager::Redo(const int64_t &noOfStep)
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _Redo(noOfStep);
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> ActionManager::redoToSeqNo(const int64_t &seqNo)
    {
        TRY
            return Redo(seqNo - _CurrentSeqNo);
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> ActionManager::Undo(const int64_t &noOfStep)
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _Undo(noOfStep);
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> ActionManager::UndoToSeqNo(const int64_t &seqNo)
    {
        TRY
            return Undo(_CurrentSeqNo - seqNo);
        CATCH
        return nullptr;
    }

    int64_t ActionManager::chopActionListToSize(const int64_t &size)
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _chopActionListToSize(size, true);
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::clear()
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _clear();
        CATCH
        return _CurrentSeqNo;
    }

    int64_t ActionManager::truncate()
    {
        TRY
            //std::unique_lock lock(_mutex);
            return _truncate();
        CATCH
        return _CurrentSeqNo;
    }
}
