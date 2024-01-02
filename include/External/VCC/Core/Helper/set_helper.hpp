#pragma once

#include <set>

namespace vcc
{
    template<typename T>
    inline bool IsEmpty(const std::set<T> &v)
    {
        return v.empty();
    }

};