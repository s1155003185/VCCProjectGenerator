#pragma once

#include <set>
#include <string>

#include "exception_macro.hpp"

namespace vcc
{
    template<typename T>
    inline bool IsEmpty(const std::set<T> &v)
    {
        return v.empty();
    }
    
    template<typename T>
    inline bool IsContain(const std::set<T> &v, const T &value) 
    {
        if (IsEmpty(v))
            return false;
        for (auto const &element : v) {
            if (element == value)
                return true;
        }
        return false;
    }

    // Concat
    inline std::wstring Concat(const std::set<std::wstring> &v, std::wstring delimitor)
    {
        std::wstring result = L"";
        if (IsEmpty(v))
            return result;
        for (std::wstring str : v)
            result += str + delimitor;
        return result.substr(0, result.size() - delimitor.size());
    }
};