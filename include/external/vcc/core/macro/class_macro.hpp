#pragma once
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "exception.hpp"
#include "i_result.hpp"
#include "map_helper.hpp"
#include "vector_helper.hpp"

namespace vcc
{
    //------------------------------------------------------------------------------------------------------//
    //----------------------------------------------- Module -----------------------------------------------//
    //------------------------------------------------------------------------------------------------------//
    
    // virtual

    #define GETCUSTOM(type, varName, customProcedcure) \
    public: \
        type Get##varName() const { customProcedcure }

    #define SETCUSTOM(varName, type, customProcedcure) \
    public: \
        void Set##varName(const type &value) { customProcedcure }

    #define GETCUSTOM_SPTR(type, varName, customProcedcure) \
    public: \
        std::shared_ptr<type> Get##varName() const { customProcedcure }

    #define SETCUSTOM_SPTR(varName, type,  customProcedcure) \
    public: \
        void Set##varName(std::shared_ptr<type> value) { customProcedcure }
    
    // general
    
    #define GETSET(type, varName, def) GETSET_VALIDATE(type, varName, def, (void)value;)

    #define GETSET_VALIDATE(type, varName, def, validate) \
    protected: \
        mutable type _##varName = def; \
        void Validate##varName(const type &value) const { validate } \
    public: \
        const type &Get##varName() const { return _##varName; } \
    public: \
        void Set##varName(const type &value) { Validate##varName(value); _##varName = value; }

    // object

    // Cannot set type as current class, will throw Segmentation fault
    // GETSET_SPTR is same as GETSET_SPTR_NULL, but Generator will generate initialization in class construction
    // Need to handle initialization manually if not using Generator
    #define GETSET_SPTR(type, varName, ...) GETSET_SPTR_NULL(type, varName)

    // #define GETSET_VALIDATE_SPTR(type, varName, validate, ...) GETSET_VALIDATE_SPTR_NULL(type, varName, validate)

    #define GETSET_SPTR_NULL(type, varName) GETSET_VALIDATE_SPTR_NULL(type, varName, (void)value;)

    #define GETSET_VALIDATE_SPTR_NULL(type, varName, validate) \
        protected: \
            mutable std::shared_ptr<type> _##varName = nullptr; \
            void Validate##varName(const std::shared_ptr<IObject> value) const { validate } \
        public: \
            std::shared_ptr<type> Get##varName() const { return _##varName; } \
            void Set##varName(std::shared_ptr<type> value) { Validate##varName(value); _##varName = value; } \
            void Clone##varName(const IObject *value) { _##varName = value != nullptr ? std::dynamic_pointer_cast<type>(value->Clone()) : nullptr; }
        
    // std::vector
    
    #define VECTOR(type, varName) VECTOR_VALIDATE(type, varName, (void)value; )

    #define VECTOR_VALIDATE(type, varName, validate) \
    protected: \
        mutable std::vector<type> _##varName; \
        void Validate##varName(const type &value) const { validate } \
    public: \
        std::vector<type> &Get##varName() const { return _##varName; } \
        type Get##varName##AtIndex(const int64_t &index) const { return _##varName[index]; } \
        void Set##varName##AtIndex(const int64_t &index, const type &value) { Validate##varName(value); Set(_##varName, value, index); } \
        int64_t Find##varName(const type &value) const { return Find(_##varName, value); } \
        void Insert##varName(const type &value) { Validate##varName(value); Insert(_##varName, value); } \
        void Insert##varName##AtIndex(const int64_t &index, const type &value) { Validate##varName(value); Insert(_##varName, value, index); } \
        void Insert##varName(const std::vector<type> &value) { for (auto &element : value) Validate##varName(element); Insert(_##varName, value); } \
        void Insert##varName##AtIndex(const int64_t &index, const std::vector<type> &value) { for (auto const &element : value) Validate##varName(element); Insert(_##varName, value, index); } \
        void Remove##varName(const type &value) { Remove(_##varName, value); } \
        void Remove##varName##AtIndex(const int64_t &index) { RemoveAtIndex(_##varName, index); } \
        std::vector<type> Clone##varName() const { return _##varName; }\
        void Clone##varName(const std::vector<type> &value) { _##varName.clear(); Insert##varName(value); }\
        void Clear##varName() { _##varName.clear(); }
    
    #define VECTOR_SPTR(type, varName) VECTOR_VALIDATE_SPTR(type, varName, (void)value; )

    #define VECTOR_VALIDATE_SPTR(type, varName, validate) \
    protected: \
        mutable std::vector<std::shared_ptr<type>> _##varName; \
        void Validate##varName(const std::shared_ptr<IObject> value) const { validate } \
    public: \
        std::vector<std::shared_ptr<type>> &Get##varName() const { return _##varName; } \
        std::shared_ptr<type> Get##varName##AtIndex(int64_t index) const { return _##varName[index]; } \
        void Set##varName##AtIndex(int64_t index, std::shared_ptr<IObject> value) { Validate##varName(value); SetIObject(_##varName, value, index); } \
        int64_t Find##varName(const std::shared_ptr<IObject> value) const { return FindIObject(_##varName, value.get()); } \
        void Insert##varName(std::shared_ptr<IObject> value) { Validate##varName(value); InsertIObject(_##varName, value); } \
        void Insert##varName##AtIndex(int64_t index, std::shared_ptr<IObject> value) { Validate##varName(value); InsertIObject(_##varName, value, index); } \
        void Insert##varName(std::vector<std::shared_ptr<type>> &value) { for (auto &element : value) Validate##varName(element); InsertIObjects(_##varName, value); } \
        void Insert##varName##AtIndex(int64_t index, std::vector<std::shared_ptr<type>> &value) { for (auto const &element : value) Validate##varName(element); InsertIObjects(_##varName, value, index); } \
        void Remove##varName(const IObject* value) { RemoveIObject(_##varName, value); } \
        void Remove##varName##AtIndex(int64_t index) { RemoveIObjecttAtIndex(_##varName, index); } \
        std::shared_ptr<type> Clone##varName##AtIndex(int64_t index) const { return std::static_pointer_cast<type>(_##varName[index]->Clone()); } \
        std::vector<std::shared_ptr<type>> Clone##varName() const { std::vector<std::shared_ptr<type>> result; for (auto &element : _##varName) { result.push_back(std::static_pointer_cast<type>(element->Clone())); } return result; } \
        void Clone##varName(const std::vector<std::shared_ptr<type>> &value) { _##varName.clear(); for (std::shared_ptr<type> element : value) { Insert##varName(element != nullptr ? std::dynamic_pointer_cast<type>(element->Clone()) : nullptr); } }\
        void Clear##varName() { _##varName.clear(); }

    // set

    #define SET(type, varName) SET_VALIDATE(type, varName, (void) value;)
    
    #define SET_VALIDATE(type, varName, validate) \
    protected: \
        mutable std::set<type> _##varName; \
        void Validate##varName(const type &value) const { validate } \
    public: \
        std::set<type> &Get##varName() const { return _##varName; } \
        void Insert##varName(type value) { Validate##varName(value); _##varName.insert(value); } \
        void Insert##varName(std::set<type> value) { for (auto const &element : value) Validate##varName(element); _##varName.insert(value.begin(), value.end()); } \
        void Remove##varName(type value) { _##varName.erase(value); } \
        std::set<type> Clone##varName() {return _##varName; }\
        void Clone##varName(const std::set<type> &value) { _##varName.clear(); _##varName.insert(value.begin(), value.end()); }\
        void Clear##varName() { _##varName.clear(); }
    
    #define SET_SPTR(type, varName) SET_VALIDATE_SPTR(type, varName, (void)value;)

    #define SET_VALIDATE_SPTR(type, varName, validate) \
    protected: \
        mutable std::set<std::shared_ptr<type>> _##varName; \
        void Validate##varName(const std::shared_ptr<IObject> value) const { validate } \
    public: \
        std::set<std::shared_ptr<type>> &Get##varName() const { return _##varName; } \
        void Insert##varName(std::shared_ptr<type> value) { Validate##varName(value); _##varName.insert(value); } \
        void Insert##varName(std::set<std::shared_ptr<type>> value) { for (auto const &element : value) Validate##varName(element); _##varName.insert(value.begin(), value.end()); } \
        std::set<std::shared_ptr<type>> Clone##varName() const { std::set<std::shared_ptr<type>> result; for (auto const & element : _##varName) result.insert(std::dynamic_pointer_cast<type>(element->Clone())); return result; } \
        void Clone##varName(const std::set<std::shared_ptr<type>> &value) { _##varName.clear(); for (std::shared_ptr<type> element : value) { Insert##varName(element != nullptr ? std::dynamic_pointer_cast<type>(element->Clone()) : nullptr); } }\
        void Clear##varName() { _##varName.clear(); }

    // map

    #define MAP(keyType, valueType, varName) MAP_VALIDATE(keyType, valueType, varName, (void)value; )
    
    #define MAP_VALIDATE(keyType, valueType, varName, validate) \
    protected: \
        mutable std::map<keyType, valueType> _##varName; \
        void Validate##varName(const valueType &value) const { validate } \
    public: \
        std::map<keyType, valueType> &Get##varName() const { return _##varName; } \
        std::set<keyType> Get##varName##Keys() const { return GetKeys(_##varName); } \
        std::set<void *> Get##varName##VoidKeys() const { return GetVoidKeys(_##varName); } \
        bool Is##varName##ContainKey(keyType key) const { return IsContain(_##varName, key); } \
        valueType Get##varName##AtKey(keyType key) const { return _##varName[key]; } \
        void Set##varName##AtKey(keyType key, valueType value) { Validate##varName(value); Set(_##varName, key, value); } \
        void Insert##varName##AtKey(keyType key, valueType value) { Validate##varName(value); Set(_##varName, key, value); } \
        void Insert##varName(const std::pair<keyType, valueType> &value) { Validate##varName(value.second); Set(_##varName, value.first, value.second); } \
        void Insert##varName(const std::map<keyType, valueType> &value) { for (auto const &pair : value) Validate##varName(pair.second); Set(_##varName, value); } \
        std::map<keyType, valueType> Clone##varName() const { return _##varName; } \
        void Clone##varName(const std::map<keyType, valueType> &value) { _##varName.clear(); Insert##varName(value); } \
        void Remove##varName##AtKey(keyType key) { RemoveAtKey(_##varName, key); } \
        void Clear##varName() { _##varName.clear(); }

    #define MAP_SPTR_R(keyType, valueType, varName) MAP_VALIDATE_SPTR_R(keyType, valueType, varName, (void)value; )

    #define MAP_VALIDATE_SPTR_R(keyType, valueType, varName, validate) \
    protected: \
        mutable std::map<keyType, std::shared_ptr<valueType>> _##varName; \
        void Validate##varName(const std::shared_ptr<IObject> value) const { validate } \
    public: \
        std::map<keyType, std::shared_ptr<valueType>> &Get##varName() const { return _##varName; } \
        std::set<keyType> Get##varName##Keys() const { std::set<keyType> result; for (auto const &pair : _##varName) result.insert(pair.first); return result; } \
        std::set<void *> Get##varName##VoidKeys() const { std::set<void *> result; for (auto const &pair : _##varName) result.insert((void *)&pair.first); return result; } \
        bool Is##varName##ContainKey(keyType key) const { return IsContain(_##varName, key); } \
        std::shared_ptr<valueType> Get##varName##AtKey(keyType key) const { return _##varName[key]; } \
        void Set##varName##AtKey(keyType key, std::shared_ptr<valueType> value) { Validate##varName(value); SetIObject(_##varName, key, value); } \
        void Insert##varName##AtKey(keyType key, std::shared_ptr<valueType> value) { Validate##varName(value); SetIObject(_##varName, key, value); } \
        void Insert##varName(const std::pair<keyType, std::shared_ptr<valueType>> &value) { Validate##varName(value.second); SetIObject(_##varName, value.first, value.second); } \
        void Insert##varName(const std::map<keyType, std::shared_ptr<valueType>> &value) { for (auto const &pair : value) Validate##varName(pair.second); SetIObjects(_##varName, value); } \
        std::shared_ptr<valueType> Clone##varName##AtKey(keyType key) const { return std::static_pointer_cast<valueType>(Get##varName##AtKey(key)->Clone()); } \
        std::map<keyType, std::shared_ptr<valueType>> Clone##varName() const { std::map<keyType, std::shared_ptr<valueType>> result; for (auto &pair : _##varName) { result.insert(std::make_pair(pair.first, std::dynamic_pointer_cast<valueType>(pair.second->Clone()))); } return result; } \
        void Clone##varName(const std::map<keyType, std::shared_ptr<valueType>> &value) { _##varName.clear(); for (auto const& element : value) { Insert##varName##AtKey(element.first, element.second != nullptr ? std::dynamic_pointer_cast<valueType>(element.second->Clone()) : nullptr); } }\
        void Remove##varName##AtKey(keyType key) { RemoveIObjectAtKey(_##varName, key); } \
        void Clear##varName() { _##varName.clear(); }

    #define ORDERED_MAP(keyType, valueType, varName) ORDERED_MAP_VALIDATE(keyType, valueType, varName, (void)value; )

    #define ORDERED_MAP_VALIDATE(keyType, valueType, varName, validate) \
    protected: \
        mutable std::vector<std::pair<keyType, valueType>> _##varName; \
        void Validate##varName(const valueType &value) const { validate } \
    public: \
        std::vector<std::pair<keyType, valueType>> &Get##varName() const { return _##varName; } \
        std::set<keyType> Get##varName##Keys() const { std::set<keyType> result; for (auto const &pair : _##varName) result.insert(pair.first); return result; } \
        std::set<void *> Get##varName##VoidKeys() const { std::set<void *> result; for (auto const &pair : _##varName) result.insert((void *)&pair.first); return result; } \
        std::pair<keyType, valueType> Get##varName##AtIndex(int64_t index) const { return _##varName[index]; } \
        valueType Get##varName##AtKey(keyType key) const { for (auto const &pair : _##varName) {  if (pair.first == key) return pair.second; } throw Exception(ExceptionType::CustomError, L"key not found");  } \
        void Set##varName##AtIndex(int64_t index, std::pair<keyType, valueType> value) { Validate##varName(value.second); _##varName[index] = value; } \
        void Set##varName##AtKey(keyType key, valueType value) { for (auto &pair : _##varName) {  Validate##varName(value); if (pair.first == key) { pair.second = value; return; } } Insert##varName##AtKey(key, value); } \
        int64_t Find##varName(keyType key) const { for (size_t i = 0; i < _##varName.size(); i++) { if (_##varName[i].first == key) return i; } return -1; } \
        bool Is##varName##ContainKey(keyType key) const { return Find##varName(key) > -1; } \
        void Insert##varName##AtIndex(int64_t index, std::pair<keyType, valueType> value) { Validate##varName(value.second); Insert(_##varName, value, index); } \
        void Insert##varName##AtKey(keyType key, valueType value) { Validate##varName(value); _##varName.push_back(std::make_pair(key, value)); } \
        void Insert##varName(const std::pair<keyType, valueType> value) { Validate##varName(value.second); _##varName.push_back(value); } \
        void Insert##varName(const std::vector<std::pair<keyType, valueType>> &value) { for (auto const &pair : value) Validate##varName(pair.second); _##varName.insert(_##varName.end(), value.begin(), value.end()); } \
        std::vector<std::pair<keyType, valueType>> Clone##varName() const { return _##varName; } \
        void Clone##varName(const std::vector<std::pair<keyType, valueType>> &value) { _##varName.clear(); Insert##varName(value); } \
        void Remove##varName##AtIndex(int64_t index) { if (index >= 0) { _##varName.erase(_##varName.begin() + (size_t)index); } } \
        void Remove##varName##AtKey(keyType key) { int64_t index = Find##varName(key); if (index >= 0) (_##varName.erase(_##varName.begin() + (size_t)(Find##varName(key)))); } \
        void Clear##varName() { _##varName.clear(); }

    #define ORDERED_MAP_SPTR_R(keyType, valueType, varName) ORDERED_MAP_VALIDATE_SPTR_R(keyType, valueType, varName, (void)value; )

    #define ORDERED_MAP_VALIDATE_SPTR_R(keyType, valueType, varName, validate) \
    protected: \
        mutable std::vector<std::pair<keyType, std::shared_ptr<valueType>>> _##varName; \
        void Validate##varName(const std::shared_ptr<IObject> value) const { validate } \
    public: \
        std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &Get##varName() const { return _##varName; } \
        std::set<keyType> Get##varName##Keys() const { std::set<keyType> result; for (auto const &pair : _##varName) result.insert(pair.first); return result; } \
        std::set<void *> Get##varName##VoidKeys() const { std::set<void *> result; for (auto const &pair : _##varName) result.insert((void *)&pair.first); return result; } \
        std::pair<keyType, std::shared_ptr<valueType>> Get##varName##AtIndex(int64_t index) const { return _##varName[index]; } \
        std::shared_ptr<valueType> Get##varName##AtKey(keyType key) const { for (auto const &pair : _##varName) { if (pair.first == key) return pair.second; } throw Exception(ExceptionType::CustomError, L"key not found"); return nullptr; } \
        int64_t Find##varName(keyType key) const { for (size_t i = 0; i < _##varName.size(); i++) { if (_##varName[i].first == key) return i; } return -1; } \
        bool Is##varName##ContainKey(keyType key) const { return Find##varName(key) > -1; } \
        void Set##varName##AtIndex(int64_t index, std::pair<keyType, std::shared_ptr<valueType>> value) { Validate##varName(value.second); _##varName[index] = value; } \
        void Set##varName##AtKey(keyType key, std::shared_ptr<valueType> value) { for (auto &pair : _##varName) { Validate##varName(value); if (pair.first == key) { pair.second = value; return; } } Insert##varName##AtKey(key, value); } \
        void Insert##varName##AtIndex(int64_t index, std::pair<keyType, std::shared_ptr<valueType>> value) { Validate##varName(value.second); Insert(_##varName, value, index); } \
        void Insert##varName##AtKey(keyType key, std::shared_ptr<valueType> value) { Validate##varName(value); _##varName.push_back(std::make_pair(key, value)); } \
        void Insert##varName(const std::pair<keyType, std::shared_ptr<valueType>> value) { Validate##varName(value.second); _##varName.push_back(value); } \
        void Insert##varName(const std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &value) { for (auto const &pair : value) Validate##varName(pair.second); _##varName.insert(_##varName.end(),value.begin(), value.end()); } \
        std::shared_ptr<valueType> Clone##varName##AtIndex(int64_t index) const { return std::static_pointer_cast<valueType>(Get##varName##AtIndex(index).second->Clone()); } \
        std::shared_ptr<valueType> Clone##varName##AtKey(keyType key) const { return std::static_pointer_cast<valueType>(Get##varName##AtKey(key)->Clone()); } \
        void Clone##varName(const std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &value) { _##varName.clear(); for (auto const& element : value) { Insert##varName##AtKey(element.first, element.second != nullptr ? std::dynamic_pointer_cast<valueType>(element.second->Clone()) : nullptr); } }\
        void Remove##varName##AtIndex(int64_t index) { if (index >= 0) { _##varName.erase(_##varName.begin() + (size_t)index); } } \
        void Remove##varName##AtKey(keyType key) { int64_t index = Find##varName(key); if (index >= 0) (_##varName.erase(_##varName.begin() + (size_t)(Find##varName(key)))); } \
        void Clear##varName() { _##varName.clear(); }

    //------------------------------------------------------------------------------------------------------//
    //--------------------------------------------- MANAGER ------------------------------------------------//
    //------------------------------------------------------------------------------------------------------//
    
    // MANAGER_SPTR is same as MANAGER_SPTR_NULL, but Generator will generate initialization in class construction
    // Need to handle initialization manually if not using Generator
    #define MANAGER_SPTR(type, varName, ...) MANAGER_SPTR_NULL(type, varName, __VA_ARGS__)
        
    #define MANAGER_SPTR_NULL(type, varName, ...) \
    protected: \
        mutable std::shared_ptr<type> _##varName = nullptr; \
    public: \
        std::shared_ptr<type> Get##varName() const { return _##varName; } \
        void Set##varName(std::shared_ptr<type> value) { _##varName = value; }
        
    #define MANAGER_SPTR_PARENT(type, varName, parentClass) \
    protected: \
        mutable std::shared_ptr<type> _##varName = nullptr; \
    public: \
        std::shared_ptr<type> Get##varName() const { auto baseForm = std::dynamic_pointer_cast<parentClass>(_ParentObject); return (_##varName == nullptr && baseForm != nullptr) ? baseForm->Get##varName() : _##varName; } \
        void Set##varName(std::shared_ptr<type> value) { _##varName = value; }

    //------------------------------------------------------------------------------------------------------//
    //--------------------------------------------- ACTION -------------------------------------------------//
    //------------------------------------------------------------------------------------------------------//
    #define ACTION(name) \
        public: \
            virtual std::shared_ptr<IResult> Do##name();
            
    #define ACTION_WITH_ARG_SPTR(name, type) \
        public: \
            virtual std::shared_ptr<IResult> Do##name(std::shared_ptr<type> value);
}
