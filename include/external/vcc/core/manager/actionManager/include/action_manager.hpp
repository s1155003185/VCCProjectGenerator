#pragma once
#include "base_manager.hpp"

#include "class_macro.hpp"
#include "i_action.hpp"

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <stdint.h>

namespace vcc
{
    class ActionManager : public BaseManager
    {
        GETSET(int64_t, MaxActionListSize, 100)
        MAP_SPTR_R(int64_t, IAction, Actions)
        GETSET(int64_t, CurrentSeqNo, -1)
        GETSET(int64_t, MaxSeqNo, -1)

    private:
        //mutable std::shared_mutex _mutex;

        int64_t _GetFirstSeqNo(const bool &fromBeginning) const;
        int64_t _Redo(const int64_t &noOfStep) const;
        int64_t _Undo(const int64_t &noOfStep) const;
        int64_t _RemoveAction(const int64_t &noOfAction, const bool &fromBeginning) const;
        int64_t _ChopActionListToSize(const int64_t &size, const bool &fromBeginning) const;
        int64_t _Clear() const;
        int64_t _Truncate() const;
    public:
        ActionManager(std::shared_ptr<LogConfig> logConfig) : BaseManager(logConfig) {}
        virtual ~ActionManager() {}

        int64_t GetFirstSeqNo() const;
        int64_t GetLastSeqNo() const;

        int64_t DoAction(std::shared_ptr<IAction> action) const;
        
        int64_t Redo(const int64_t &noOfStep = 1) const;
        int64_t RedoToSeqNo(const int64_t &seqNo) const;

        int64_t Undo(const int64_t &noOfStep = 1) const;
        int64_t UndoToSeqNo(const int64_t &seqNo) const;
        
        int64_t ChopActionListToSize(const int64_t &size) const;

        int64_t Clear() const;
        int64_t Truncate() const;
    };
}
