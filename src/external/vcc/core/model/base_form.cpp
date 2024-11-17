#include "base_form.hpp"

#include "exception_macro.hpp"

namespace vcc
{
    State BaseForm::GetState() const
    {
        TRY
            return _State;
        CATCH
        return State::Active;
    }

    bool BaseForm::IsClosable() const
    {
        TRY
            return OnIsClosable();
        CATCH
        return false;
    }

    bool BaseForm::IsClosed() const
    {
        TRY
            return _State == State::Closed;
        CATCH
        return false;
    }

    void BaseForm::Initialize() const
    {
        TRY
            OnInitialize();
        CATCH
    }

    void BaseForm::Reload() const
    {
        TRY
            OnReload();
        CATCH
    }

    bool BaseForm::Close(bool isForce) const
    {
        TRY
            if (!isForce && !IsClosable())
                return false;
            if (!OnClose())
                return false;
            _State = State::Closed;
            return true;
        CATCH
        return false;
    }
}