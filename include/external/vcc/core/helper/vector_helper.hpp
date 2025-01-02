#pragma once

#include <memory>
#include <string>
#include <vector>

namespace vcc
{
    template<typename T>
    inline bool IsEmpty(const std::vector<T> &v)
    {
        return v.empty();
    }

    template<typename T>
    inline bool IsContain(const std::vector<T> &v, const T &value) 
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
    inline std::wstring Concat(const std::vector<std::wstring> &v, std::wstring delimitor)
    {
        std::wstring result = L"";
        if (IsEmpty(v))
            return result;
        for (std::wstring str : v) {
            result += str + delimitor;
        }
        return result.substr(0, result.size() - delimitor.size());
    }

    // Remove
    template <typename T>
    inline void RemoveElement(std::vector<T> &sourceVector, const T &obj)
    {
        sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(),
            [&](const T &element) {
                return element == obj;
            }), sourceVector.end());
    }

    template <typename T>
    inline void RemoveIObject(std::vector<std::shared_ptr<T>> &sourceVector, const std::shared_ptr<IObject> &obj)
    {
        auto derivedObj = std::dynamic_pointer_cast<T>(obj);        
        if (derivedObj) {
            sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(),
                [&](const std::shared_ptr<T> &element) {
                    return element == derivedObj;
                }), sourceVector.end());
        }
    }

    template <typename T>
    inline void RemoveElements(std::vector<T> &sourceVector, const std::vector<T> &obj)
    {
        sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(), [&](const T &element) {
            return IsContain(obj, element);
        }), sourceVector.end());
    }
};