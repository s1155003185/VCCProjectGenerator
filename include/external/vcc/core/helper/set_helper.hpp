#pragma once

#include <algorithm>
#include <memory>
#include <set>
#include <string>

#include "exception_macro.hpp"
#include "i_object.hpp"

namespace vcc
{
    template<typename T>
    inline bool isEmpty(const std::set<T> &v)
    {
        return v.empty();
    }
    
    template<typename T>
    inline bool isContain(const std::set<T> &v, const T &value) 
    {
        if (isEmpty(v))
            return false;
        for (auto const &element : v) {
            if (element == value)
                return true;
        }
        return false;
    }
    
    template<typename T>
    inline bool isContain(const std::set<T> &v, const std::set<T> &value) 
    {
        if (isEmpty(v) || v.size() < value.size())
            return false;
        for (auto const &element : value) {
            if (!isContain(v, element))
                return false;
        }
        return true;
    }

    // Concat
    inline std::wstring concat(const std::set<std::wstring> &v, const std::wstring &delimitor)
    {
        std::wstring result = L"";
        if (isEmpty(v))
            return result;
        for (std::wstring str : v)
            result += str + delimitor;
        return result.substr(0, result.size() - delimitor.size());
    }
    
    // Remove
    template <typename T>
    inline void removeElement(std::set<T> &sourceSet, const T &obj)
    {
        for (auto it = sourceSet.begin(); it != sourceSet.end(); ) {
            if (*it == obj) {
                it = sourceSet.erase(it);
                break;
            }
        }
    }
    
    template <typename T>
    inline void removeElementAll(std::set<T> &sourceSet, const T &obj)
    {
        for (auto it = sourceSet.begin(); it != sourceSet.end(); ) {
            if (*it == obj)
                it = sourceSet.erase(it);
            else
                it++;
        }
    }
    
    template <typename T>
    inline void removeIObject(std::set<std::shared_ptr<T>> &sourceSet, const IObject *obj)
    {
        for (auto it = sourceSet.begin(); it != sourceSet.end(); ) {
            if ((*it)->getObjectId() == obj->getObjectId()) {
                it = sourceSet.erase(it);
                break;
            }
        }
    }

    template <typename T>
    inline void removeIObjectAll(std::set<std::shared_ptr<T>> &sourceSet, const IObject *obj)
    {
        for (auto it = sourceSet.begin(); it != sourceSet.end(); ) {
            if ((*it)->getObjectId() == obj->getObjectId())
                it = sourceSet.erase(it);
            else
                it++;
        }
    }
};