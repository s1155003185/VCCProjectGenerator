#pragma once
#include "base_manager.hpp"

#include <memory>
#include <shared_mutex>
#include <stdint.h>

#include "base_action.hpp"
#include "class_macro.hpp"
#include "i_action.hpp"
#include "i_result.hpp"

namespace vcc
{
    class ActionManager : public BaseManager
    {
        GETSET(int64_t, MaxActionListSize, 100)
        MAP_SPTR_R(int64_t, BaseAction, Actions)
        GETSET(int64_t, CurrentSeqNo, -1)
        GETSET(int64_t, MaxSeqNo, -1)

    private:
        int64_t _getFirstSeqNo(const bool &fromBeginning) const;
        std::shared_ptr<IResult> _Redo(const int64_t &noOfStep);
        std::shared_ptr<IResult> _Undo(const int64_t &noOfStep);
        int64_t _removeAction(const int64_t &noOfAction, const bool &fromBeginning);
        int64_t _chopActionListToSize(const int64_t &size, const bool &fromBeginning);
        int64_t _clear();
        int64_t _truncate();
    public:
        ActionManager(std::shared_ptr<LogConfig> logConfig) : BaseManager(logConfig) {}
        virtual ~ActionManager() {}

        int64_t getFirstSeqNo() const;
        int64_t getLastSeqNo() const;

        std::shared_ptr<IResult> doAction(std::shared_ptr<IAction> action);
        
        std::shared_ptr<IResult> Redo(const int64_t &noOfStep = 1);
        std::shared_ptr<IResult> redoToSeqNo(const int64_t &seqNo);

        std::shared_ptr<IResult> Undo(const int64_t &noOfStep = 1);
        std::shared_ptr<IResult> UndoToSeqNo(const int64_t &seqNo);
        
        int64_t chopActionListToSize(const int64_t &size);

        int64_t clear();
        int64_t truncate();
    };
}
