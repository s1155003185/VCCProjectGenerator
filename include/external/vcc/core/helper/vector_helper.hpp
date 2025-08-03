#pragma once

#include <algorithm>
#include <assert.h>
#include <memory>
#include <string>
#include <vector>

#include "i_object.hpp"

// Cannot use exception_macro.hpp, will throw compile error
namespace vcc
{
    // Validate
    template<typename T>
    bool isEmpty(const std::vector<T> &v);
    template<typename T>
    bool isContain(const std::vector<T> &v, const T &value);
    template<typename T>
    bool isContain(const std::vector<T> &v, const std::vector<T> &value);

    // Concat
    std::wstring concat(const std::vector<std::wstring> &v, const std::wstring &delimitor);

    // Search
    template <typename T>
    int64_t find(const std::vector<T> &sourceVector, const T &obj);
    template <typename T>
    int64_t findIObject(std::vector<std::shared_ptr<T>> &sourceVector, const IObject *obj);

    // Set
    template <typename T>
    void set(std::vector<T> &sourceVector, T obj, const int64_t &index = -1);
    template <typename T>
    void setIObject(std::vector<std::shared_ptr<T>> &sourceVector, std::shared_ptr<IObject> &obj, const int64_t &index = -1);

    // Insert
    template <typename T>
    void insert(std::vector<T> &sourceVector, T obj, const int64_t &index = -1);
    template <typename T>
    void insert(std::vector<T> &sourceVector, const std::vector<T> &objs, const int64_t &index = -1);
    template <typename T>
    void insertIObject(std::vector<std::shared_ptr<T>> &sourceVector, std::shared_ptr<IObject> obj, const int64_t &index = -1);
    template <typename T>
    void insertIObjects(std::vector<std::shared_ptr<T>> &sourceVector, const std::vector<std::shared_ptr<T>> &objs, const int64_t &index = -1);

    // Remove
    template <typename T>
    void remove(std::vector<T> &sourceVector, const T &obj);
    template <typename T>
    void remove(std::vector<T> &sourceVector, const std::vector<T> &filters);
    template <typename T>
    void removeAtIndex(std::vector<T> &sourceVector, const int64_t &index);
    template <typename T>
    void RemoveAll(std::vector<T> &sourceVector, const T &obj);

    template <typename T>
    void removeIObject(std::vector<std::shared_ptr<T>> &sourceVector, const IObject *obj);
    template <typename T>
    void removeIObjecttAtIndex(std::vector<T> &sourceVector, const int64_t &index);
    template <typename T>
    void removeIObjectAll(std::vector<std::shared_ptr<T>> &sourceVector, const IObject *obj);

    // ----------------------------------------------------------------------------------------------------
    // ---------------------------------------- Implement -------------------------------------------------
    // ----------------------------------------------------------------------------------------------------

    template<typename T>
    bool isEmpty(const std::vector<T> &v)
    {
        return v.empty();
    }
    
    template<typename T>
    bool isContain(const std::vector<T> &v, const T &value) 
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
    bool isContain(const std::vector<T> &v, const std::vector<T> &value)
    {
        if (isEmpty(v) || v.size() < value.size())
            return false;
        for (auto const &element : value) {
            if (!isContain(v, element))
                return false;
        }
        return true;
    }
    
    template <typename T>
    int64_t find(const std::vector<T> &sourceVector, const T &value)
    {
        if (isEmpty(sourceVector))
            return -1;
        auto it = find(sourceVector.begin(), sourceVector.end(), value);
        if (it != sourceVector.end())
            return it - sourceVector.begin();
        return -1;
    }

    template <typename T>
    int64_t findIObject(std::vector<std::shared_ptr<T>> &sourceVector, const IObject *obj)
    {
        if (isEmpty(sourceVector))
            return -1;
        for (size_t i = 0; i < sourceVector.size(); i++) {
            if (sourceVector.at(i)->getObjectId() == obj->getObjectId())
                return i;
        }
        return -1;
    }

    template <typename T>
    void set(std::vector<T> &sourceVector, T obj, const int64_t &index)
    {
        if (index >= 0)
            sourceVector[index] = obj;
        else
            insert(sourceVector, obj);
    }

    template <typename T>
    void setIObject(std::vector<std::shared_ptr<T>> &sourceVector, std::shared_ptr<IObject> &obj, const int64_t &index)
    {
        auto derivedObj = std::dynamic_pointer_cast<T>(obj);
        assert(derivedObj != nullptr);
        if (index >= 0)
            sourceVector[index] = derivedObj;
        else
            insertIObject(sourceVector, derivedObj);
    }

    template <typename T>
    void insert(std::vector<T> &sourceVector, T obj, const int64_t &index)
    {
        if (index >= 0)
            sourceVector.insert(sourceVector.begin() + index, obj);
        else
            sourceVector.push_back(obj);
    }
    
    template <typename T>
    void insert(std::vector<T> &sourceVector, const std::vector<T> &objs, const int64_t &index)
    {
        if (index >= 0)
            sourceVector.insert(sourceVector.begin() + index, objs.begin(), objs.end());
        else
            sourceVector.insert(sourceVector.end(), objs.begin(), objs.end());
    }
    
    template <typename T>
    void insertIObject(std::vector<std::shared_ptr<T>> &sourceVector, std::shared_ptr<IObject> obj, const int64_t &index)
    {
        auto derivedObj = std::dynamic_pointer_cast<T>(obj);
        assert(derivedObj != nullptr);
        if (index >= 0)
            sourceVector.insert(sourceVector.begin() + index, derivedObj);
        else
            sourceVector.push_back(derivedObj);
    }
    
    template <typename T>
    void insertIObjects(std::vector<std::shared_ptr<T>> &sourceVector, const std::vector<std::shared_ptr<T>> &objs, const int64_t &index)
    {
        insert(sourceVector, objs, index);
    }

    template <typename T>
    void remove(std::vector<T> &sourceVector, const T &obj)
    {
        auto it = std::find(sourceVector.begin(), sourceVector.end(), obj);
        if (it != sourceVector.end())
            sourceVector.erase(it);
    }
    
    template <typename T>
    void remove(std::vector<T> &sourceVector, const std::vector<T> &filters)
    {
        sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(), [&](const T &element) {
            return isContain(filters, element);
        }), sourceVector.end());
    }

    template <typename T>
    void removeAtIndex(std::vector<T> &sourceVector, const int64_t &index)
    {
        if (index >= 0)
            sourceVector.erase(sourceVector.begin() + (size_t)index);
    }
    
    template <typename T>
    void RemoveAll(std::vector<T> &sourceVector, const T &obj)
    {
        sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(),
            [&](const T &element) {
                return element == obj;
            }), sourceVector.end());
    }

    template <typename T>
    void removeIObject(std::vector<std::shared_ptr<T>> &sourceVector, const IObject *obj)
    {
        removeIObjecttAtIndex(sourceVector, findIObject(sourceVector, obj));
    }

    template <typename T>
    void removeIObjecttAtIndex(std::vector<T> &sourceVector, const int64_t &index)
    {
        removeAtIndex(sourceVector, index);
    }
    
    template <typename T>
    void removeIObjectAll(std::vector<std::shared_ptr<T>> &sourceVector, const IObject *obj)
    {
        sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(),
            [&](const T &element) {
                return element->getObjectId() == obj->getObjectId();
            }), sourceVector.end());
    }
};