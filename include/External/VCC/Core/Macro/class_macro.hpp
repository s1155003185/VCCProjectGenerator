#pragma once
#include <map>
#include <memory>
#include <set>
#include <vector>

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

    #define GETSPTR(type, var, ...) \
    protected: \
        mutable std::shared_ptr<type> _##var = std::make_shared<type>(__VA_ARGS__); \
    public: \
        std::shared_ptr<type> Get##var() { return _##var; }

    // std::vector
    
    #define VECTOR(type, var) \
    protected: \
        mutable std::shared_ptr<std::vector<type>> _##var = std::make_shared<std::vector<type>>(); \
    public: \
        const std::shared_ptr<std::vector<type>> Get##var() const { return _##var; } \
        void Insert##var(type value) { this->_##var->push_back(value); } \
        void Insert##var(size_t index, type value) { this->_##var->insert(this->_##var->begin() + index, value); } \
        void Insert##var(const std::vector<type> *value) { this->_##var->insert(this->_##var->end(), value->begin(), value->end()); } \
        void Insert##var(size_t index, std::vector<type> *value) { this->_##var->insert(this->_##var->begin() + index, value->begin(), value->end()); } \
        void Remove##var(size_t index) { this->_##var->erase(this->_##var->begin() + index); } \
        void Clone##var(const std::vector<type> *value) { this->_##var = std::make_shared<std::vector<type>>(); this->Insert##var(value); }\
        void Clear##var() { this->_##var->clear(); }

    #define VECTOR_SPTR(type, var) \
    protected: \
        mutable std::shared_ptr<std::vector<std::shared_ptr<type>>> _##var = std::make_shared<std::vector<std::shared_ptr<type>>>(); \
    public: \
        const std::shared_ptr<std::vector<std::shared_ptr<type>>> Get##var() const { return _##var; } \
        void Insert##var(std::shared_ptr<type> value) { this->_##var->push_back(value); } \
        void Insert##var(size_t index, std::shared_ptr<type> value) { this->_##var->insert(this->_##var->begin() + index, value); } \
        void Insert##var(std::vector<std::shared_ptr<type>> *value) { this->_##var->insert(this->_##var->end(), value->begin(), value->end()); } \
        void Insert##var(size_t index, std::vector<std::shared_ptr<type>> *value) { this->_##var->insert(this->_##var->begin() + index, value->begin(), value->end()); } \
        void Remove##var(size_t index) { this->_##var->erase(this->_##var->begin() + index); } \
        void Clone##var(const std::vector<std::shared_ptr<type>> *value) { this->_##var = std::make_shared<std::vector<std::shared_ptr<type>>>(); for (std::shared_ptr<type> element : *value) { this->Insert##var(dynamic_pointer_cast<type>(element->Clone())); } }\
        void Clear##var() { this->_##var->clear(); }

    // set

    #define SET(type, var) \
    protected: \
        mutable std::shared_ptr<std::set<type>> _##var = std::make_shared<std::set<type>>(); \
    public: \
        const std::shared_ptr<std::set<type>> Get##var() const { return _##var; } \
        void Insert##var(type value) {  this->_##var->insert(value); } \
        void Insert##var(std::set<type> *value) { this->_##var->insert(value->begin(), value->end()); } \
        void Remove##var(type value) { this->_##var->erase(value); } \
        void Clone##var(const std::set<type> *value) { this->_##var = std::make_shared<std::set<type>>(); this->_##var->insert(value->begin(), value->end()); }\
        void Clear##var() { this->_##var->clear(); }
    
    #define SET_SPTR(type, var) \
    protected: \
        mutable std::shared_ptr<std::set<std::shared_ptr<type>>> _##var = std::make_shared<std::set<std::shared_ptr<type>>>(); \
    public: \
        const std::shared_ptr<std::set<std::shared_ptr<type>>> Get##var() const { return _##var; } \
        void Insert##var(std::shared_ptr<type> value) {  this->_##var->insert(value); } \
        void Clone##var(const std::set<std::shared_ptr<type>> *value) { this->_##var = std::make_shared<std::set<std::shared_ptr<type>>>(); for (std::shared_ptr<type> element : *value) { this->Insert##var(dynamic_pointer_cast<type>(element->Clone())); } }\
        void Clear##var() { this->_##var->clear(); }

    // map

    #define MAP(keyType, valueType, var) \
    protected: \
        mutable std::shared_ptr<std::map<keyType, valueType>> _##var = std::make_shared<std::map<keyType, valueType>>(); \
    public: \
        const std::shared_ptr<std::map<keyType, valueType>> Get##var() const { return _##var; } \
        void Insert##var(keyType key, valueType value) const { _##var->insert(std::make_pair(key, value)); } \
        void Insert##var(const std::map<keyType, valueType> *value) const { _##var->insert(value->begin(), value->end()); } \
        void Clone##var(const std::map<keyType, valueType> *value) { this->_##var = std::make_shared<std::map<keyType, valueType>>(); this->Insert##var(value); } \
        void Clear##var() { this->_##var->clear(); }

    //------------------------------------------------------------------------------------------------------//
    //--------------------------------------------- VCC Object ---------------------------------------------//
    //------------------------------------------------------------------------------------------------------//
    
    #define MANAGER(type, var, ...) \
    protected: \
        std::shared_ptr<type> _##var = std::make_shared<type>(__VA_ARGS__); \
    public: \
        type* Get##var() { return _##var.get(); }
        
}
