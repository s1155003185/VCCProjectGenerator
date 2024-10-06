#pragma once

#include <functional>
#include <memory>
#include <string>
#include <thread>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"
#include "process_state.hpp"

namespace vcc
{
    class ThreadManager;
    class Thread
    {
        GETSET_SPTR_NULL(LogProperty, LogProperty)
        GETSET(ProcessState, State, ProcessState::Idle)

        GETSET(int64_t, SeqNo, -1)
        GETSET(std::wstring, Id, L"")
        GETSET(std::wstring, MessageStart, L"")
        GETSET(std::wstring, MessageComplete, L"")
        GETSET(std::wstring, DebugMessage, L"")

        protected:
            mutable std::thread::id _Pid;
            mutable const ThreadManager *_Manager = nullptr;

            mutable std::function<void(const Thread *)> _Action = nullptr;
            mutable std::function<void(const Thread *)> _Callback = nullptr;

            Thread() = default;

        public:
            Thread(std::shared_ptr<LogProperty> logProperty, std::function<void(const Thread *)> action)
                : _LogProperty(logProperty), _Action(action) {}
            Thread(std::shared_ptr<LogProperty> logProperty, std::function<void(const Thread *)> action, std::function<void(const Thread *)> callback)
                : _LogProperty(logProperty), _Action(action), _Callback(callback) {}
            Thread(std::shared_ptr<LogProperty> logProperty, const std::wstring &id, const std::wstring &messageStart, const std::wstring &messageComplete, std::function<void(const Thread *)> action)
                : _LogProperty(logProperty), _Id(id), _MessageStart(messageStart), _MessageComplete(messageComplete), _Action(action) {}
            Thread(std::shared_ptr<LogProperty> logProperty, const std::wstring &id, const std::wstring &messageStart, const std::wstring &messageComplete, std::function<void(const Thread *)> action, std::function<void(const Thread *)> callback)
                : _LogProperty(logProperty), _Id(id), _MessageStart(messageStart), _MessageComplete(messageComplete), _Action(action), _Callback(callback) {}
            Thread(std::shared_ptr<LogProperty> logProperty, const std::wstring &id, const std::wstring &messageStart, const std::wstring &messageComplete, const std::wstring &debugMessage, std::function<void(const Thread *)> action)
                : _LogProperty(logProperty), _Id(id), _MessageStart(messageStart), _MessageComplete(messageComplete), _DebugMessage(debugMessage), _Action(action) {}
            Thread(std::shared_ptr<LogProperty> logProperty, const std::wstring &id, const std::wstring &messageStart, const std::wstring &messageComplete, const std::wstring &debugMessage, std::function<void(const Thread *)> action, std::function<void(const Thread *)> callback)
                : _LogProperty(logProperty), _Id(id), _MessageStart(messageStart), _MessageComplete(messageComplete), _DebugMessage(debugMessage), _Action(action), _Callback(callback) {}
            virtual ~Thread() {}

            const ThreadManager *GetManager() const;
            void SetManager(const ThreadManager *manager) const;

            std::wstring GetPid() const;

            void Execute() const;
    };
}