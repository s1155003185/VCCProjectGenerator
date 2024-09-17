#pragma once

namespace vcc
{
    enum class ProcessState
    {
        Idle,
        Busy,
        Suspend,
        Stop,
        Complete
    };
}