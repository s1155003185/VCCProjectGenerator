#pragma once
#include <memory>

namespace vcc
{
    #define UPTR(type) std::unique_ptr<type>
    #define SPTR(type) std::shared_ptr<type>

    #define DECLARE_UPTR(type, name, ...) std::unique_ptr<type> name = std::make_unique<type>(__VA_ARGS__)
    #define DECLARE_SPTR(type, name, ...) std::shared_ptr<type> name = std::make_shared<type>(__VA_ARGS__)
}