#pragma once

#include <set>
#include <string>

#include "exception_macro.hpp"
#include "i_object.hpp"

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
    inline std::wstring Concat(const std::set<std::wstring> &v, const std::wstring &delimitor)
    {
        std::wstring result = L"";
        if (IsEmpty(v))
            return result;
        for (std::wstring str : v)
            result += str + delimitor;
        return result.substr(0, result.size() - delimitor.size());
    }
    
    // Remove
    template <typename T>
    inline void RemoveElement(std::set<T> &sourceSet, const T &obj)
    {
        sourceSet.erase(std::remove_if(sourceSet.begin(), sourceSet.end(),
            [&](const T &element) {
                return element == obj;
            }), sourceSet.end());
    }

    // template <typename T>
    // inline void RemoveIObject(std::set<std::shared_ptr<T>> &sourceSet, const std::shared_ptr<IObject> &obj)
    // {
    //     auto derivedObj = std::dynamic_pointer_cast<T>(obj);        
    //     if (derivedObj) {
    //         sourceSet.erase(std::remove_if(sourceSet.begin(), sourceSet.end(),
    //             [&](const std::shared_ptr<T> &element) {
    //                 return element == derivedObj;
    //             }), sourceSet.end());
    //     }
    // }
};