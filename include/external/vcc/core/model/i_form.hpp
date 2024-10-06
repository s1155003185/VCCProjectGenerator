#pragma once

namespace vcc
{
    class IForm
    {
        protected:
            IForm() {}
            virtual ~IForm() {}
        
        public:
            // Clean and Reset
            virtual void OnInitialize() = 0;
            
    };
}
