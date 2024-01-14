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
        void Insert##var(std::vector<type> &value) { this-> Insert##var(0, value); } \
        void Insert##var(size_t index, std::vector<type> &value) { this->_##var->insert(this->_##var->begin() + index, value.begin(), value.end()); } \
        void Remove##var(size_t index) { this->_##var->erase(this->_##var->begin() + index); } \
        void Clear##var() { this->_##var->clear(); }

    #define VECTOR_SPTR(type, var) \
    protected: \
        mutable std::shared_ptr<std::vector<std::shared_ptr<type>>> _##var = std::make_shared<std::vector<std::shared_ptr<type>>>(); \
    public: \
        const std::shared_ptr<std::vector<std::shared_ptr<type>>> Get##var() const { return _##var; } \
        void Insert##var(std::shared_ptr<type> value) { this->_##var->push_back(value); } \
        void Insert##var(size_t index, std::shared_ptr<type> value) { this->_##var->insert(this->_##var->begin() + index, value); } \
        void Insert##var(std::vector<std::shared_ptr<type>> &value) { this-> Insert##var(0, value); } \
        void Insert##var(size_t index, std::vector<std::shared_ptr<type>> &value) { this->_##var->insert(this->_##var->begin() + index, value.begin(), value.end()); } \
        void Remove##var(size_t index) { this->_##var->erase(this->_##var->begin() + index); } \
        void Clear##var() { this->_##var->clear(); }


    #define SET(type, var) \
    protected: \
        mutable std::set<type> _##var; \
    public: \
        const std::set<type> *Get##var() const { return &_##var; } \
        void Insert##var(type value) {  this->_##var.insert(value); } \
        void Insert##var(std::set<type> value) { this->_##var.insert(value.begin(), value.end()); } \
        void Remove##var(type value) { this->_##var.erase(value); } \
        void Clear##var() { this->_##var.clear(); }

    #define MAP(keyType, valueType, var) \
    protected: \
        mutable std::map<keyType, valueType> _##var; \
    public: \
        const std::map<keyType, valueType>* Get##var() const { return &_##var; } \
        void Insert##var(keyType key, valueType value) const { _##var.insert(std::make_pair(key, value)); }

    //------------------------------------------------------------------------------------------------------//
    //--------------------------------------------- VCC Object ---------------------------------------------//
    //------------------------------------------------------------------------------------------------------//
    
    #define MANAGER(type, var, ...) \
    protected: \
        std::shared_ptr<type> _##var = std::make_shared<type>(__VA_ARGS__); \
    public: \
        type* Get##var() { return _##var.get(); }
        
}
