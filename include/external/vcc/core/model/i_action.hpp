#pragma once
#include <string>
#include "i_object.hpp"

namespace vcc
{
    class IAction : public IObject
    {
    protected:
        IAction() {}
        virtual ~IAction() {}

        virtual void _DoRedo() = 0;
        virtual void _DoUndo() = 0;

        virtual std::wstring _GetRedoMessage() = 0;
        virtual std::wstring _GetUndoMessage() = 0;

    public:
        virtual void SetSeqNo(size_t seqNo) = 0;
        virtual size_t GetSeqNo() = 0;
        virtual std::wstring GetMessage() = 0;
        
        virtual void Redo() = 0;
        virtual void Undo() = 0;
    };
}
