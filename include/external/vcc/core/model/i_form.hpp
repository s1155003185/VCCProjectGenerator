#pragma once

#include "action_manager.hpp"
#include "log_config.hpp"
#include "state.hpp"

namespace vcc
{
    class IForm
    {
        protected:
            IForm() = default;
            virtual ~IForm() = default;
        
            // Initialize
            virtual void OnInitialize() const = 0;
            virtual void OnReload() const = 0;

            // Close
            virtual bool OnIsClosable() const = 0;
            virtual bool OnClose() const = 0;
        public:
            // Log
            virtual std::shared_ptr<LogConfig> GetLogConfig() const = 0;
            virtual void SetLogConfig(std::shared_ptr<LogConfig> logConfig) const = 0;
            
            // Action
            virtual std::shared_ptr<ActionManager> GetActionManager() const = 0;
            virtual void SetActionManager(std::shared_ptr<ActionManager> actionManager) const =0;

            // State
            virtual State GetState() const = 0;
            virtual bool IsClosable() const = 0;
            virtual bool IsClosed() const = 0;

            // Initialize
            virtual void Initialize() const = 0;
            virtual void Reload() const = 0;

            // Close
            virtual bool Close(bool isForce = false) const = 0;            
    };
}
