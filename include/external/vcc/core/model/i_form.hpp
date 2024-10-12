#pragma once

namespace vcc
{
    class IForm
    {
        protected:
            IForm() = default;
            virtual ~IForm() = default;
        
        public:
            // Is Closable
            virtual bool IsClosable() const = 0;

            // Clean and Reset
            virtual void OnInitialize() const = 0;
            
            // Close Form, return true if form is closed
            virtual bool OnClose(bool isForce = false) const = 0;
            
    };
}
