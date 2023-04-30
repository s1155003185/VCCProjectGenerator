#pragma once
#include <mutex>
#include <map>
#include <set>
#include <vector>

namespace vcc
{
    #define THREAD_SAFE \
    protected: \
        std::mutex __mutex; \
    public: \
        std::mutex *GetMutex() { return &__mutex; }

    #define LOCK_GUAND std::lock_guard<std::mutex> lock(__mutex)

    // general
    #define GETSET(type, var, def) \
    private: \
        type _##var = def; \
    public: \
        type Get##var() { std::lock_guard<std::mutex> lock(__mutex); return _##var; }\
        void Set##var(type val) { std::lock_guard<std::mutex> lock(__mutex); _##var = val; }

    #define GET(type, var, def) \
    private: \
        type _##var = def; \
    public: \
        type Get##var() { std::lock_guard<std::mutex> lock(__mutex); return _##var; }\

    #define STATICGET(type, var, def) \
    public: \
        static type Get##var() { return def; }

    #define GETOBJ(type, var) \
    private: \
        type _##var; \
    public: \
        type *Get##var() { std::lock_guard<std::mutex> lock(__mutex); return &_##var; }\

    // std::vector
    #define VECTOR(type, var) \
    private: \
        std::vector<type> _##var; \
    public: \
        std::vector<type> *Get##var() { std::lock_guard<std::mutex> lock(__mutex); return &_##var; } \
        void Insert##var(type value) { std::lock_guard<std::mutex> lock(__mutex); this->_##var.push_back(value); } \
        void Insert##var(size_t index, type value) { std::lock_guard<std::mutex> lock(__mutex); this->_##var.insert(this->_##var.begin() + index, value); } \
        void Insert##var(std::vector<type> &value) { this-> Insert##var(0, value); } \
        void Insert##var(size_t index, std::vector<type> &value) { std::lock_guard<std::mutex> lock(__mutex); this->_##var.insert(this->_##var.begin() + index, value.begin(), value.end()); } \
        void Remove##var(size_t index) { std::lock_guard<std::mutex> lock(__mutex); this->_##var.erase(this->_##var.begin() + index); } \
        void Clear##var() { std::lock_guard<std::mutex> lock(__mutex); this->_##var.clear(); }

    #define SET(type, var) \
    private: \
        std::set<type> _##var; \
    public: \
        std::set<type> *Get##var() { std::lock_guard<std::mutex> lock(__mutex); return &_##var; } \
        void Insert##var(type value) { std::lock_guard<std::mutex> lock(__mutex);  this->_##var.insert(value); } \
        void Insert##var(std::set<type> value) { std::lock_guard<std::mutex> lock(__mutex); this->_##var.insert(value.begin(), value.end()); } \
        void Remove##var(type value) { std::lock_guard<std::mutex> lock(__mutex); this->_##var.erase(value); } \
        void Clear##var() { std::lock_guard<std::mutex> lock(__mutex); this->_##var.clear(); }

    #define MAP(keyType, valueType, var) \
    private: \
        std::map<keyType, valueType> _##var; \
    public: \
        std::map<keyType, valueType> *Get##var() { std::lock_guard<std::mutex> lock(__mutex); return &_##var; }
        
}
