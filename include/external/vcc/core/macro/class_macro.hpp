#pragma once
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "exception.hpp"
#include "map_helper.hpp"
#include "vector_helper.hpp"

namespace vcc
{
    //------------------------------------------------------------------------------------------------------//
    //----------------------------------------------- Module -----------------------------------------------//
    //------------------------------------------------------------------------------------------------------//
    // general
    
    #define GETSET(type, var, def) \
    protected: \
        mutable type _##var = def; \
    public: \
        const type& Get##var() const { return _##var; }\
        void Set##var(type val) const { _##var = val; }

    // object

    // Cannot set type as current class, will throw Segmentation fault
    #define GETSET_SPTR(type, var, ...) \
    protected: \
        mutable std::shared_ptr<type> _##var = std::make_shared<type>(__VA_ARGS__); \
    public: \
        std::shared_ptr<type> Get##var() const { return _##var; } \
        void Set##var(std::shared_ptr<type> value) const { _##var = value; } \
        void Clone##var(const IObject *value) const { _##var = value != nullptr ? std::dynamic_pointer_cast<type>(value->Clone()) : nullptr; }

    #define GETSET_SPTR_NULL(type, var) \
    protected: \
        mutable std::shared_ptr<type> _##var = nullptr; \
    public: \
        std::shared_ptr<type> Get##var() const { return _##var; } \
        void Set##var(std::shared_ptr<type> value) const { _##var = value; } \
        void Clone##var(const IObject *value) const { _##var = value != nullptr ? std::dynamic_pointer_cast<type>(value->Clone()) : nullptr; }
        
    // std::vector
    
    #define VECTOR(type, var) \
    protected: \
        mutable std::vector<type> _##var; \
    public: \
        std::vector<type> &Get##var() const { return _##var; } \
        type Get##var(int64_t index) const { return _##var[index]; } \
        void Set##var(int64_t index, type value) const { Set(_##var, value, index); } \
        int64_t Find##var(type value) const { return Find(_##var, value); } \
        void Insert##var(type value) const { Insert(_##var, value); } \
        void Insert##var(int64_t index, type value) const { Insert(_##var, value, index); } \
        void Insert##var(const std::vector<type> &value) const { Insert(_##var, value); } \
        void Insert##var(int64_t index, std::vector<type> &value) const { Insert(_##var, value, index); } \
        void Remove##var(type value) const { Remove(_##var, value); } \
        void Remove##var##AtIndex(int64_t index) const { RemoveAtIndex(_##var, index); } \
        void Clone##var(const std::vector<type> &value) const { _##var.clear(); Insert##var(value); }\
        void Clear##var() const { _##var.clear(); }

    #define VECTOR_SPTR(type, var) \
    protected: \
        mutable std::vector<std::shared_ptr<type>> _##var; \
    public: \
        std::vector<std::shared_ptr<type>> &Get##var() const { return _##var; } \
        std::shared_ptr<type> Get##var(int64_t index) const { return _##var[index]; } \
        void Set##var(int64_t index, std::shared_ptr<IObject> value) const { SetIObject(_##var, value, index); } \
        int64_t Find##var(const std::shared_ptr<IObject> value) const { return FindIObject(_##var, value); } \
        void Insert##var(std::shared_ptr<IObject> value) const { InsertIObject(_##var, value); } \
        void Insert##var(int64_t index, std::shared_ptr<IObject> value) const { InsertIObject(_##var, value, index); } \
        void Insert##var(std::vector<std::shared_ptr<type>> &value) { InsertIObjects(_##var, value); } \
        void Insert##var(int64_t index, std::vector<std::shared_ptr<type>> &value) { InsertIObjects(_##var, value, index); } \
        void Remove##var(std::shared_ptr<IObject> value) const { RemoveIObject(_##var, value); } \
        void Remove##var##AtIndex(int64_t index) { RemoveIObjecttAtIndex(_##var, index); } \
        std::shared_ptr<type> Clone##var(int64_t index) const { return std::static_pointer_cast<type>(Get##var(index)->Clone()); } \
        void Clone##var(const std::vector<std::shared_ptr<type>> &value) { _##var.clear(); for (std::shared_ptr<type> element : value) { Insert##var(element != nullptr ? std::dynamic_pointer_cast<type>(element->Clone()) : nullptr); } }\
        void Clear##var() const { _##var.clear(); }

    // set

    #define SET(type, var) \
    protected: \
        mutable std::set<type> _##var; \
    public: \
        std::set<type> &Get##var() const { return _##var; } \
        void Insert##var(type value) {  _##var.insert(value); } \
        void Insert##var(std::set<type> value) { _##var.insert(value.begin(), value.end()); } \
        void Remove##var(type value) { _##var.erase(value); } \
        void Clone##var(const std::set<type> &value) { _##var.clear(); _##var.insert(value.begin(), value.end()); }\
        void Clear##var() const { _##var.clear(); }
    
    #define SET_SPTR(type, var) \
    protected: \
        mutable std::set<std::shared_ptr<type>> _##var; \
    public: \
        std::set<std::shared_ptr<type>> &Get##var() const { return _##var; } \
        void Insert##var(std::shared_ptr<type> value) const {  _##var.insert(value); } \
        void Clone##var(const std::set<std::shared_ptr<type>> &value) const { _##var.clear(); for (std::shared_ptr<type> element : value) { Insert##var(element != nullptr ? std::dynamic_pointer_cast<type>(element->Clone()) : nullptr); } }\
        void Clear##var() const { _##var.clear(); }

    // map

    #define MAP(keyType, valueType, var) \
    protected: \
        mutable std::map<keyType, valueType> _##var; \
    public: \
        std::map<keyType, valueType> &Get##var() const { return _##var; } \
        std::set<keyType> Get##var##Keys() const { return GetKeys(_##var); } \
        std::set<void *> Get##var##VoidKeys() const { return GetVoidKeys(_##var); } \
        bool Is##var##ContainKey(keyType key) const { return IsContain(_##var, key); } \
        valueType Get##var(keyType key) const { return _##var[key]; } \
        void Set##var(keyType key, valueType value) const { Set(_##var, key, value); } \
        void Insert##var(keyType key, valueType value) const { Set(_##var, key, value); } \
        void Insert##var(const std::map<keyType, valueType> &value) const { Set(_##var, value); } \
        void Clone##var(const std::map<keyType, valueType> &value) const { _##var.clear(); Insert##var(value); } \
        void Remove##var##AtKey(keyType key) { RemoveAtKey(_##var, key); } \
        void Clear##var() const { _##var.clear(); }

    #define MAP_SPTR_R(keyType, valueType, var) \
    protected: \
        mutable std::map<keyType, std::shared_ptr<valueType>> _##var; \
    public: \
        std::map<keyType, std::shared_ptr<valueType>> &Get##var() const { return _##var; } \
        std::set<keyType> Get##var##Keys() const { std::set<keyType> result; for (auto const &pair : _##var) result.insert(pair.first); return result; } \
        std::set<void *> Get##var##VoidKeys() const { std::set<void *> result; for (auto const &pair : _##var) result.insert((void *)&pair.first); return result; } \
        bool Is##var##ContainKey(keyType key) const { return IsContain(_##var, key); } \
        std::shared_ptr<valueType> Get##var(keyType key) const { return _##var[key]; } \
        void Set##var(keyType key, std::shared_ptr<valueType> value) const { SetIObject(_##var, key, value); } \
        void Insert##var(keyType key, std::shared_ptr<valueType> value) const { SetIObject(_##var, key, value); } \
        void Insert##var(const std::map<keyType, std::shared_ptr<valueType>> &value) const { SetIObjects(_##var, value); } \
        std::shared_ptr<valueType> Clone##var(keyType key) const { return std::static_pointer_cast<valueType>(Get##var(key)->Clone()); } \
        void Clone##var(const std::map<keyType, std::shared_ptr<valueType>> &value) const { _##var.clear(); for (auto const& element : value) { Insert##var(element.first, element.second != nullptr ? std::dynamic_pointer_cast<valueType>(element.second->Clone()) : nullptr); } }\
        void Remove##var##AtKey(keyType key) { RemoveIObjectAtKey(_##var, key); } \
        void Clear##var() const { _##var.clear(); }

    #define ORDERED_MAP(keyType, valueType, var) \
    protected: \
        mutable std::vector<std::pair<keyType, valueType>> _##var; \
    public: \
        std::vector<std::pair<keyType, valueType>> &Get##var() const { return _##var; } \
        std::set<keyType> Get##var##Keys() const { std::set<keyType> result; for (auto const &pair : _##var) result.insert(pair.first); return result; } \
        std::set<void *> Get##var##VoidKeys() const { std::set<void *> result; for (auto const &pair : _##var) result.insert((void *)&pair.first); return result; } \
        std::pair<keyType, valueType> Get##var##AtIndex(int64_t index) const { return _##var[index]; } \
        valueType Get##var##AtKey(keyType key) const { for (auto const &pair : _##var) {  if (pair.first == key) return pair.second; } throw Exception(ExceptionType::CustomError, L"key not found");  } \
        void Set##var##AtIndex(int64_t index, std::pair<keyType, valueType> value) const { _##var[index] = value; } \
        void Set##var##AtKey(keyType key, valueType value) const { for (auto &pair : _##var) {  if (pair.first == key) { pair.second = value; return; } } Insert##var(key, value); } \
        int64_t Find##var(keyType key) const { for (size_t i = 0; i < _##var.size(); i++) { if (_##var[i].first == key) return i; } return -1; } \
        bool Is##var##ContainKey(keyType key) const { return Find##var(key) > -1; } \
        void Insert##var(keyType key, valueType value) const { _##var.push_back(std::make_pair(key, value)); } \
        void Insert##var(const std::vector<std::pair<keyType, valueType>> &value) const { _##var.insert(_##var.end(), value.begin(), value.end()); } \
        void Clone##var(const std::vector<std::pair<keyType, valueType>> &value) const { _##var.clear(); Insert##var(value); } \
        void Remove##var##AtIndex(int64_t index) { if (index >= 0) { _##var.erase(_##var.begin() + (size_t)index); } } \
        void Remove##var##AtKey(keyType key) { int64_t index = Find##var(key); if (index >= 0) (_##var.erase(_##var.begin() + (size_t)(Find##var(key)))); } \
        void Clear##var() const { _##var.clear(); }

    #define ORDERED_MAP_SPTR_R(keyType, valueType, var) \
    protected: \
        mutable std::vector<std::pair<keyType, std::shared_ptr<valueType>>> _##var; \
    public: \
        std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &Get##var() const { return _##var; } \
        std::set<keyType> Get##var##Keys() const { std::set<keyType> result; for (auto const &pair : _##var) result.insert(pair.first); return result; } \
        std::set<void *> Get##var##VoidKeys() const { std::set<void *> result; for (auto const &pair : _##var) result.insert((void *)&pair.first); return result; } \
        std::pair<keyType, std::shared_ptr<valueType>> Get##var##AtIndex(int64_t index) const { return _##var[index]; } \
        std::shared_ptr<valueType> Get##var##AtKey(keyType key) const { for (auto const &pair : _##var) { if (pair.first == key) return pair.second; } throw Exception(ExceptionType::CustomError, L"key not found"); return nullptr; } \
        int64_t Find##var(keyType key) const { for (size_t i = 0; i < _##var.size(); i++) { if (_##var[i].first == key) return i; } return -1; } \
        bool Is##var##ContainKey(keyType key) const { return Find##var(key) > -1; } \
        void Set##var##AtIndex(int64_t index, std::pair<keyType, std::shared_ptr<valueType>> value) const { _##var[index] = value; } \
        void Set##var##AtKey(keyType key, std::shared_ptr<valueType> value) const { for (auto &pair : _##var) {  if (pair.first == key) { pair.second = value; return; } } Insert##var(key, value); } \
        void Insert##var(keyType key, std::shared_ptr<valueType> value) const { _##var.push_back(std::make_pair(key, value)); } \
        void Insert##var(const std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &value) const { _##var.insert(_##var.end(),value.begin(), value.end()); } \
        std::shared_ptr<valueType> Clone##var##AtIndex(int64_t index) const { return std::static_pointer_cast<valueType>(Get##var##AtIndex(index).second->Clone()); } \
        std::shared_ptr<valueType> Clone##var##AtKey(keyType key) const { return std::static_pointer_cast<valueType>(Get##var##AtKey(key)->Clone()); } \
        void Clone##var(const std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &value) const { _##var.clear(); for (auto const& element : value) { Insert##var(element.first, element.second != nullptr ? std::dynamic_pointer_cast<valueType>(element.second->Clone()) : nullptr); } }\
        void Remove##var##AtIndex(int64_t index) { if (index >= 0) { _##var.erase(_##var.begin() + (size_t)index); } } \
        void Remove##var##AtKey(keyType key) { int64_t index = Find##var(key); if (index >= 0) (_##var.erase(_##var.begin() + (size_t)(Find##var(key)))); } \
        void Clear##var() const { _##var.clear(); }

    //------------------------------------------------------------------------------------------------------//
    //--------------------------------------------- MANAGER ------------------------------------------------//
    //------------------------------------------------------------------------------------------------------//
    
    #define MANAGER_SPTR(type, var, ...) \
    protected: \
        mutable std::shared_ptr<type> _##var = std::make_shared<type>(__VA_ARGS__); \
    public: \
        std::shared_ptr<type> Get##var() const { return _##var; } \
        void Set##var(std::shared_ptr<type> value) const { _##var = value; }
        
    #define MANAGER_SPTR_NULL(type, var, ...) \
    protected: \
        mutable std::shared_ptr<type> _##var = nullptr; \
    public: \
        std::shared_ptr<type> Get##var() const { return _##var; } \
        void Set##var(std::shared_ptr<type> value) const { _##var = value; }
        
    #define MANAGER_SPTR_PARENT(type, var, parentClass) \
    protected: \
        mutable std::shared_ptr<type> _##var = nullptr; \
    public: \
        std::shared_ptr<type> Get##var() const { auto baseForm = std::dynamic_pointer_cast<parentClass>(_ParentObject); return (_##var == nullptr && baseForm != nullptr) ? baseForm->Get##var() : _##var; } \
        void Set##var(std::shared_ptr<type> value) const { _##var = value; }

    //------------------------------------------------------------------------------------------------------//
    //--------------------------------------------- ACTION -------------------------------------------------//
    //------------------------------------------------------------------------------------------------------//
    
    // ActionName, DoActionReturn, UndoActionReturn, DoActionParameters
    #define ACTION(name) \
        public: \
            virtual void Do##name();
}
