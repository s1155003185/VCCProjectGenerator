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
    class ActionManager : public BaseManager<ActionManager>
    {
        GETSET(int64_t, MaxActionListSize, 100)
        MAP_SPTR_R(int64_t, IAction, Actions)
        GET(int64_t, CurrentSeqNo, -1)
        GET(int64_t, MaxSeqNo, -1)

    private:
        //mutable std::shared_mutex _mutex;

        int64_t _GetFirstSeqNo(bool fromBeginning);
        int64_t _Redo(int64_t noOfStep);
        int64_t _Undo(int64_t noOfStep);
        int64_t _RemoveAction(int64_t noOfAction, bool fromBeginning);
        int64_t _ChopActionListToSize(int64_t size, bool fromBeginning);
        int64_t _Clear();
        int64_t _Truncate();
    public:
        ActionManager(std::shared_ptr<LogProperty> logProperty) : BaseManager(logProperty, ManagerType::Action) {}
        virtual ~ActionManager() {}

        int64_t GetFirstSeqNo();
        int64_t GetLastSeqNo();

        int64_t DoAction(std::shared_ptr<IAction> action);
        
        int64_t Redo(int64_t noOfStep = 1);
        int64_t RedoToSeqNo(int64_t seqNo);

        int64_t Undo(int64_t noOfStep = 1);
        int64_t UndoToSeqNo(int64_t seqNo);
        
        int64_t ChopActionListToSize(int64_t size);

        int64_t Clear();
        int64_t Truncate();
    };
}
