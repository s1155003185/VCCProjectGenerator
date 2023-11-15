#pragma once

#include <string>
#include <vector>

namespace vcc
{
    // Concat
    inline std::wstring ConcatVector(const std::vector<std::wstring> &v, std::wstring delimitor)
    {
        std::wstring result = L"";
        if (v.empty())
            return result;
        for (std::wstring str : v) {
            result += str + delimitor;
        }
        return result.substr(0, result.size() - delimitor.size());
    }

    // Remove
    template <typename T>
    inline void RemoveVectorIfContainElement(std::vector<T> &sourceVector, const T &obj)
    {
        sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(), [&](const T &element) {
            return element == obj;
        }), sourceVector.end());
    }

    template <typename T>
    inline void RemoveVectorIfContainElements(std::vector<T> &sourceVector, const std::vector<T> &obj)
    {
        sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(), [&](const T &element) {
            return std::find(obj.begin(), obj.end(), element) != obj.end();
        }), sourceVector.end());
    }
};