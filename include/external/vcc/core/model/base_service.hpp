#pragma once

#include "i_service.hpp"

namespace vcc
{
    class BaseService : public IService
    {
        protected:
            BaseService() {}
            virtual ~BaseService() {}
    };
}
