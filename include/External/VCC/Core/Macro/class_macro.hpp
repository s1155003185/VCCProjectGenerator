#pragma once
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "exception.hpp"

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

    #define GET(type, var, def) \
    protected: \
        mutable type _##var = def; \
    public: \
        const type &Get##var() const { return _##var; }\

    #define STATICGET(type, var, def) \
    public: \
        static const type& Get##var() const { return def; }

    // object

    // Cannot set type as current class, will throw Segmentation fault
    #define GET_SPTR(type, var, ...) \
    protected: \
        mutable std::shared_ptr<type> _##var = std::make_shared<type>(__VA_ARGS__); \
    public: \
        std::shared_ptr<type> Get##var() const { return _##var; } \
        void Clone##var(const type *value) const { this->_##var = value != nullptr ? dynamic_pointer_cast<type>(value->Clone()) : nullptr; } \
        void Clone##var(std::shared_ptr<type> value) const { this->_##var = value != nullptr ? dynamic_pointer_cast<type>(value->Clone()) : nullptr; }

    // Cannot set type as current class, will throw Segmentation fault
    #define GETSET_SPTR(type, var, ...) \
    protected: \
        mutable std::shared_ptr<type> _##var = std::make_shared<type>(__VA_ARGS__); \
    public: \
        std::shared_ptr<type> Get##var() const { return _##var; } \
        void Set##var(std::shared_ptr<type> value) const { _##var = value; } \
        void Clone##var(const type *value) const { this->_##var = value != nullptr ? dynamic_pointer_cast<type>(value->Clone()) : nullptr; } \
        void Clone##var(std::shared_ptr<type> value) const { this->_##var = value != nullptr ? dynamic_pointer_cast<type>(value->Clone()) : nullptr; }
        
    #define GET_SPTR_NULL(type, var) \
    protected: \
        mutable std::shared_ptr<type> _##var = nullptr; \
    public: \
        std::shared_ptr<type> Get##var() const { return _##var; } \
        void Clone##var(const type *value) const { this->_##var = value != nullptr ? dynamic_pointer_cast<type>(value->Clone()) : nullptr; } \
        void Clone##var(std::shared_ptr<type> value) const { this->_##var = value != nullptr ? dynamic_pointer_cast<type>(value->Clone()) : nullptr; }

    #define GETSET_SPTR_NULL(type, var) \
    protected: \
        mutable std::shared_ptr<type> _##var = nullptr; \
    public: \
        std::shared_ptr<type> Get##var() const { return _##var; } \
        void Set##var(std::shared_ptr<type> value) const { _##var = value; } \
        void Clone##var(const type *value) const { this->_##var = value != nullptr ? dynamic_pointer_cast<type>(value->Clone()) : nullptr; } \
        void Clone##var(std::shared_ptr<type> value) const { this->_##var = value != nullptr ? dynamic_pointer_cast<type>(value->Clone()) : nullptr; }
        
    // std::vector
    
    #define VECTOR(type, var) \
    protected: \
        mutable std::vector<type> _##var; \
    public: \
        std::vector<type> &Get##var() const { return _##var; } \
        void Insert##var(type value) const { this->_##var.push_back(value); } \
        void Insert##var(size_t index, type value) const { this->_##var.insert(this->_##var.begin() + index, value); } \
        void Insert##var(const std::vector<type> &value) const { this->_##var.insert(this->_##var.end(), value.begin(), value.end()); } \
        void Insert##var(size_t index, std::vector<type> &value) const { this->_##var.insert(this->_##var.begin() + index, value.begin(), value.end()); } \
        void Remove##var(size_t index) const { this->_##var.erase(this->_##var.begin() + index); } \
        void Clone##var(const std::vector<type> &value) const { this->_##var.clear(); this->Insert##var(value); }\
        void Clear##var() const { this->_##var.clear(); }

    #define VECTOR_SPTR(type, var) \
    protected: \
        mutable std::vector<std::shared_ptr<type>> _##var; \
    public: \
        std::vector<std::shared_ptr<type>> &Get##var() const { return _##var; } \
        void Insert##var(std::shared_ptr<type> value) const { this->_##var.push_back(value); } \
        void Insert##var(size_t index, std::shared_ptr<type> value) const { this->_##var.insert(this->_##var.begin() + index, value); } \
        void Insert##var(std::vector<std::shared_ptr<type>> &value) { this->_##var.insert(this->_##var.end(), value.begin(), value.end()); } \
        void Insert##var(size_t index, std::vector<std::shared_ptr<type>> &value) { this->_##var.insert(this->_##var.begin() + index, value.begin(), value.end()); } \
        void Remove##var(size_t index) { this->_##var.erase(this->_##var.begin() + index); } \
        void Clone##var(const std::vector<std::shared_ptr<type>> &value) { this->_##var.clear(); for (std::shared_ptr<type> element : value) { this->Insert##var(element != nullptr ? dynamic_pointer_cast<type>(element->Clone()) : nullptr); } }\
        void Clear##var() { this->_##var.clear(); }

    // set

    #define SET(type, var) \
    protected: \
        mutable std::set<type> _##var; \
    public: \
        std::set<type> &Get##var() const { return _##var; } \
        void Insert##var(type value) {  this->_##var.insert(value); } \
        void Insert##var(std::set<type> value) { this->_##var.insert(value.begin(), value.end()); } \
        void Remove##var(type value) { this->_##var.erase(value); } \
        void Clone##var(const std::set<type> &value) { this->_##var.clear(); this->_##var.insert(value.begin(), value.end()); }\
        void Clear##var() { this->_##var.clear(); }
    
    #define SET_SPTR(type, var) \
    protected: \
        mutable std::set<std::shared_ptr<type>> _##var; \
    public: \
        std::set<std::shared_ptr<type>> &Get##var() const { return _##var; } \
        void Insert##var(std::shared_ptr<type> value) const {  this->_##var.insert(value); } \
        void Clone##var(const std::set<std::shared_ptr<type>> &value) const { this->_##var.clear(); for (std::shared_ptr<type> element : value) { this->Insert##var(element != nullptr ? dynamic_pointer_cast<type>(element->Clone()) : nullptr); } }\
        void Clear##var() const { this->_##var.clear(); }

    // map

    #define MAP(keyType, valueType, var) \
    protected: \
        mutable std::map<keyType, valueType> _##var; \
    public: \
        std::map<keyType, valueType> &Get##var() const { return _##var; } \
        valueType Get##var(keyType key) const { return _##var[key]; } \
        void Insert##var(keyType key, valueType value) const { _##var.insert(std::make_pair(key, value)); } \
        void Insert##var(const std::map<keyType, valueType> &value) const { _##var.insert(value.begin(), value.end()); } \
        void Clone##var(const std::map<keyType, valueType> &value) const { this->_##var.clear(); this->Insert##var(value); } \
        void Clear##var() const { this->_##var.clear(); }

    #define MAP_SPTR_R(keyType, valueType, var) \
    protected: \
        mutable std::map<keyType, std::shared_ptr<valueType>> _##var; \
    public: \
        std::map<keyType, std::shared_ptr<valueType>> &Get##var() const { return _##var; } \
        std::shared_ptr<valueType> Get##var(keyType key) const { return _##var[key]; } \
        void Insert##var(keyType key, std::shared_ptr<valueType> value) const { _##var.insert(std::make_pair(key, value)); } \
        void Insert##var(const std::map<keyType, std::shared_ptr<valueType>> &value) const { _##var.insert(value.begin(), value.end()); } \
        void Clone##var(const std::map<keyType, std::shared_ptr<valueType>> &value) const { this->_##var.clear(); for (auto const& element : value) { this->Insert##var(element.first, element.second != nullptr ? dynamic_pointer_cast<valueType>(element.second->Clone()) : nullptr); } }\
        void Clear##var() const { this->_##var.clear(); }

    #define ORDERED_MAP(keyType, valueType, var) \
    protected: \
        mutable std::vector<std::pair<keyType, valueType>> _##var; \
    public: \
        std::vector<std::pair<keyType, valueType>> &Get##var() const { return _##var; } \
        valueType Get##var(keyType key) const { for (auto const &pair : _##var) {  if (pair.first == key) return pair.second; } throw Exception(ExceptionType::CustomError, L"key not found");  } \
        void Insert##var(keyType key, valueType value) const { _##var.push_back(std::make_pair(key, value)); } \
        void Insert##var(const std::vector<std::pair<keyType, valueType>> &value) const { _##var.insert(_##var.end(), value.begin(), value.end()); } \
        void Clone##var(const std::vector<std::pair<keyType, valueType>> &value) const { this->_##var.clear(); this->Insert##var(value); } \
        void Clear##var() const { this->_##var.clear(); }

    #define ORDERED_MAP_SPTR_R(keyType, valueType, var) \
    protected: \
        mutable std::vector<std::pair<keyType, std::shared_ptr<valueType>>> _##var; \
    public: \
        std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &Get##var() const { return _##var; } \
        std::shared_ptr<valueType> Get##var(keyType key) const { for (auto const &pair : _##var) { if (pair.first == key) return pair.second; } throw Exception(ExceptionType::CustomError, L"key not found"); return nullptr; } \
        void Insert##var(keyType key, std::shared_ptr<valueType> value) const { this->_##var.push_back(std::make_pair(key, value)); } \
        void Insert##var(const std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &value) const { _##var.insert(_##var.end(),value.begin(), value.end()); } \
        void Clone##var(const std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &value) const { this->_##var.clear(); for (auto const& element : value) { this->Insert##var(element.first, element.second != nullptr ? dynamic_pointer_cast<valueType>(element.second->Clone()) : nullptr); } }\
        void Clear##var() const { this->_##var.clear(); }

    //------------------------------------------------------------------------------------------------------//
    //--------------------------------------------- VCC Object ---------------------------------------------//
    //------------------------------------------------------------------------------------------------------//
    
    #define MANAGER(type, var, ...) \
    protected: \
        std::shared_ptr<type> _##var = std::make_shared<type>(__VA_ARGS__); \
    public: \
        type* Get##var() const { return _##var.get(); }
        
}
