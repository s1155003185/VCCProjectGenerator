#pragma once
#include <map>
#include <set>
#include <vector>

namespace vcc
{
    // general
    #define GETSET(type, var, def) \
    private: \
        type _##var = def; \
    public: \
        type Get##var() { return _##var; }\
        void Set##var(type val) { _##var = val; }

    #define GET(type, var, def) \
    private: \
        type _##var = def; \
    public: \
        type Get##var() { return _##var; }\

    #define STATICGET(type, var, def) \
    public: \
        static type Get##var() { return def; }

    // object
    #define GETOBJ(type, var) \
    private: \
        type _##var; \
    public: \
        type *Get##var() { return &_##var; }
        
    #define GETOBJU(type, var, ...) \
    private: \
        std::unique_ptr<type> _##var = std::make_unique<type>(__VA_ARGS__); \
    public: \
        type* Get##var() { return _##var.get(); } \
        void Set##var(std::unique_ptr<type>&& ptr) { _##var = std::move(ptr); }

    #define GETUPTR(type, var, ...) GETOBJU(type, var, ...)

    #define GETOBJS(type, var, ...) \
    private: \
        std::shared_ptr<type> _##var = std::make_shared<type>(__VA_ARGS__); \
    public: \
        std::shared_ptr<type> Get##var() { return _##var; }

    #define GETSPTR(type, var, ...) GETOBJS(type, var, ...)

    // std::vector
    #define VECTOR(type, var) \
    private: \
        std::vector<type> _##var; \
    public: \
        std::vector<type> *Get##var() { return &_##var; } \
        void Insert##var(type value) { this->_##var.push_back(value); } \
        void Insert##var(size_t index, type value) { this->_##var.insert(this->_##var.begin() + index, value); } \
        void Insert##var(std::vector<type> &value) { this-> Insert##var(0, value); } \
        void Insert##var(size_t index, std::vector<type> &value) { this->_##var.insert(this->_##var.begin() + index, value.begin(), value.end()); } \
        void Remove##var(size_t index) { this->_##var.erase(this->_##var.begin() + index); } \
        void Clear##var() { this->_##var.clear(); }

    #define SET(type, var) \
    private: \
        std::set<type> _##var; \
    public: \
        std::set<type> *Get##var() { return &_##var; } \
        void Insert##var(type value) {  this->_##var.insert(value); } \
        void Insert##var(std::set<type> value) { this->_##var.insert(value.begin(), value.end()); } \
        void Remove##var(type value) { this->_##var.erase(value); } \
        void Clear##var() { this->_##var.clear(); }

    #define MAP(keyType, valueType, var) \
    private: \
        std::map<keyType, valueType> _##var; \
    public: \
        std::map<keyType, valueType> *Get##var() { return &_##var; }
        
}
