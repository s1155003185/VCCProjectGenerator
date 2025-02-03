#pragma once

#include <thread>

namespace vcc
{
    std::wstring ToString(const std::thread::id &threadId);
}
