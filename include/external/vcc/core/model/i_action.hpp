#pragma once
#include <string>

namespace vcc
{
    class IAction
    {
    protected:
        IAction() {}
        virtual ~IAction() {}

        virtual void OnRedo() const = 0;
        virtual void OnUndo() const = 0;

        virtual std::wstring GetRedoMessage() const = 0;
        virtual std::wstring GetUndoMessage() const = 0;

    public:
        virtual void SetSeqNo(const size_t &seqNo) const = 0;
        virtual size_t GetSeqNo() const = 0;
        virtual std::wstring GetMessage() const = 0;
        
        virtual void Redo() const = 0;
        virtual void Undo() const = 0;
    };
}
