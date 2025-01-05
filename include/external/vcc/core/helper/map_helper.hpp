#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>

#include "i_object.hpp"

// Cannot use exception_macro.hpp, will throw compile error
namespace vcc
{
    // Validate
    template<typename A, typename B>
    bool IsEmpty(const std::map<A, B> &sourceMap);
    template<typename A, typename B>
    bool IsContain(const std::map<A, B> &sourceMap, const A &key);

    // Flip
    template<typename A, typename B>
    std::pair<B,A> Flip(const std::pair<A,B> &pair);
    template<typename A, typename B>
    std::map<B,A> Flip(const std::map<A,B> &map);

    // Search
    template <typename A, typename B>
    std::set<A> GetKeys(const std::map<A, B> &sourceMap);
    template <typename A, typename B>
    std::set<void *> GetVoidKeys(const std::map<A, B> &sourceMap);
    template <typename A, typename B>
    std::set<A> Find(const std::map<A, B> &sourceMap, const B &obj);
    template <typename A, typename B>
    std::set<A> FindIObject(std::map<A, std::shared_ptr<B>> &sourceMap, const std::shared_ptr<IObject> &obj);

    // Set
    template <typename A, typename B>
    void Set(std::map<A, B> &sourceMap, const A &key, const B &value);
    template <typename A, typename B>
    void Set(std::map<A, B> &sourceMap, const std::map<A, B> &appendMap);
    template <typename A, typename B>
    void SetIObject(std::map<A, std::shared_ptr<B>> &sourceMap, A key, std::shared_ptr<B> value);
    template <typename A, typename B>
    void SetIObjects(std::map<A, std::shared_ptr<B>> &sourceMap, const std::map<A, std::shared_ptr<B>> &appendMap);
    
    template <typename A, typename B>
    void RemoveAtKey(std::map<A, B> &sourceMap, const A& key);
    template <typename A, typename B>
    void RemoveIObjectAtKey(std::map<A, B> &sourceMap, const A& key);
    
    // ----------------------------------------------------------------------------------------------------
    // ---------------------------------------- Implement -------------------------------------------------
    // ----------------------------------------------------------------------------------------------------
    template<typename A, typename B>
    bool IsEmpty(const std::map<A, B> &sourceMap)
    {
        return sourceMap.empty();
    }

    template<typename A, typename B>
    bool IsContain(const std::map<A, B> &sourceMap, const A &key)
    {
        return sourceMap.find(key) != sourceMap.end();
    }
    
    template<typename A, typename B>
    std::pair<B,A> Flip(const std::pair<A,B> &pair)
    {
        return std::pair<B,A>(pair.second, pair.first);
    }

    template<typename A, typename B>
    std::map<B,A> Flip(const std::map<A,B> &map)
    {
        std::map<B,A> result;
        transform(map.begin(), map.end(), inserter(result, result.begin()),
            [](const std::pair<A, B>& p) { return Flip(p); });
        return result;
    }

    template <typename A, typename B>
    std::set<A> GetKeys(const std::map<A, B> &sourceMap)
    {
        std::set<A> result;
        for (auto const &pair : sourceMap)
            result.insert(pair.first);
        return result;
    }

    template <typename A, typename B>
    std::set<void *> GetVoidKeys(const std::map<A, B> &sourceMap)
    {
        std::set<void *> result;
        for (auto const &pair : sourceMap)
            result.insert((void *)&pair.first);
        return result;
    }

    template <typename A, typename B>
    std::set<A> Find(const std::map<A, B> &sourceMap, const B &obj)
    {
        std::set<A> result;
        for (auto const &pair : sourceMap) {
            if (pair.second == obj)
                result.insert(pair.first);
        }
        return result;
    }

    template <typename A, typename B>
    std::set<A> FindIObject(std::map<A, std::shared_ptr<B>> &sourceMap, const std::shared_ptr<IObject> &obj)
    {
        auto derivedObj = std::dynamic_pointer_cast<B>(obj);
        if (derivedObj != nullptr)
            return Find(sourceMap, derivedObj);
        std::set<A> result;
        return result;
    }

    template <typename A, typename B>
    void Set(std::map<A, B> &sourceMap, const A &key, const B &value)
    {
        if (sourceMap.find(key) != sourceMap.end())
            sourceMap[key] = value;
        else
            sourceMap.insert(std::make_pair(key, value));
    }

    template <typename A, typename B>
    void Set(std::map<A, B> &sourceMap, const std::map<A, B> &appendMap)
    {
        for (auto const &pair : appendMap)
            Set(sourceMap, pair.first, pair.second);
    }

    template <typename A, typename B>
    void SetIObject(std::map<A, std::shared_ptr<B>> &sourceMap, A key, std::shared_ptr<B> value)
    {
        if (sourceMap.find(key) != sourceMap.end())
            sourceMap[key] = value;
        else
            sourceMap.insert(std::make_pair(key, value));
    }

    template <typename A, typename B>
    void SetIObjects(std::map<A, std::shared_ptr<B>> &sourceMap, const std::map<A, std::shared_ptr<B>> &appendMap)
    {
        Set(sourceMap, appendMap);
    }

    template <typename A, typename B>
    void RemoveAtKey(std::map<A, B> &sourceMap, const A& key)
    {
        sourceMap.erase(sourceMap.find(key));
    }

    template <typename A, typename B>
    void RemoveIObjectAtKey(std::map<A, B> &sourceMap, const A& key)
    {
        RemoveAtKey(sourceMap, key);
    }
}