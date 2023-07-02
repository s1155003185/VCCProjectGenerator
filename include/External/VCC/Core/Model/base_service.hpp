#pragma once
#include "i_service.hpp"

#include "class_macro.hpp"

namespace vcc
{
    class BaseService : public IService
    {
        protected:
            BaseService() {}
            ~BaseService() {}
    };
}
