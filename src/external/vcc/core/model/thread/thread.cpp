#include "thread.hpp"

#include <thread>

#include "exception_macro.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"
#include "thread_helper.hpp"
#include "thread_manager.hpp"

namespace vcc
{
    const ThreadManager *Thread::GetManager() const
    {
        return _Manager;
    }

    void Thread::SetManager(const ThreadManager *manager) const
    {
        _Manager = manager;
    }

    std::wstring Thread::GetPid() const
    {
        return ToString(_Pid);
    }

    void Thread::Execute() const
    {
        TRY
            _Pid = std::this_thread::get_id();
            if (_Action) {
                _State = ProcessState::Busy;
                std::wstring id = IsBlank(_Id) ? (L"Thread." + GetPid()) : _Id;
                LogService::LogThread(_LogConfig.get(), id, IsBlank(_MessageStart) ? L"Thread Start" : _MessageStart);
                if (!IsBlank(_DebugMessage))
                    LogService::LogDebug(_LogConfig.get(), id, _DebugMessage);

                _Action(this);
                LogService::LogThread(_LogConfig.get(), id, IsBlank(_MessageComplete) ? L"Thread Terminated" : _MessageComplete);
                if (_State == ProcessState::Busy || _State == ProcessState::Idle)
                    _State = ProcessState::Complete;
            }
            if (_Callback)
                _Callback(this);

            if (_Manager)
                _Manager->Trigger();
        CATCH
    }
};