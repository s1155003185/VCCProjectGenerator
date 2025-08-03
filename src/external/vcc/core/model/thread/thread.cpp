#include "thread.hpp"

#include <thread>

#include "exception_macro.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"
#include "thread_helper.hpp"
#include "thread_manager.hpp"

namespace vcc
{
    const ThreadManager *Thread::getManager() const
    {
        return _Manager;
    }

    void Thread::setManager(const ThreadManager *manager) const
    {
        _Manager = manager;
    }

    std::wstring Thread::getPid() const
    {
        return ToString(_Pid);
    }

    void Thread::execute() const
    {
        TRY
            _Pid = std::this_thread::get_id();
            if (_Action) {
                _State = ProcessState::Busy;
                std::wstring id = isBlank(_Id) ? (L"Thread." + getPid()) : _Id;
                LogService::LogThread(_LogConfig.get(), id, isBlank(_MessageStart) ? L"Thread Start" : _MessageStart);
                if (!isBlank(_DebugMessage))
                    LogService::LogDebug(_LogConfig.get(), id, _DebugMessage);

                _Action(this);
                LogService::LogThread(_LogConfig.get(), id, isBlank(_MessageComplete) ? L"Thread Terminated" : _MessageComplete);
                if (_State == ProcessState::Busy || _State == ProcessState::Idle)
                    _State = ProcessState::Complete;
            }
            if (_Callback)
                _Callback(this);

            if (_Manager)
                _Manager->trigger();
        CATCH
    }
};