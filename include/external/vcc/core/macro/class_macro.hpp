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

//------------------------------------------------------------------------------------------------------//
//----------------------------------------------- Module -----------------------------------------------//
//------------------------------------------------------------------------------------------------------//

// virtual

#define GETCUSTOM(type, varName, customProcedcure) \
public: \
    type get##varName() const { customProcedcure }

#define SETCUSTOM(varName, type, customProcedcure) \
public: \
    void set##varName(const type &value) { customProcedcure }

#define GETCUSTOM_SPTR(type, varName, customProcedcure) \
public: \
    std::shared_ptr<type> get##varName() const { customProcedcure }

#define SETCUSTOM_SPTR(varName, type,  customProcedcure) \
public: \
    void set##varName(std::shared_ptr<type> value) { customProcedcure }

// general

#define GETSET(type, varName, def) GETSET_VALIDATE(type, varName, def, (void)value;)

#define GETSET_VALIDATE(type, varName, def, validateValue) \
protected: \
    mutable type _##varName = def; \
    void validate##varName(const type &value) const { validateValue; } \
public: \
    const type &get##varName() const { return _##varName; } \
public: \
    void set##varName(const type &value) { validate##varName(value); _##varName = value; }

// object

// Cannot set type as current class, will throw Segmentation fault
// GETSET_SPTR is same as GETSET_SPTR_NULL, but Generator will generate initialization in class construction
// Need to handle initialization manually if not using Generator
#define GETSET_SPTR(type, varName, ...) GETSET_SPTR_NULL(type, varName)

// #define GETSET_VALIDATE_SPTR(type, varName, validate, ...) GETSET_VALIDATE_SPTR_NULL(type, varName, validate)

#define GETSET_SPTR_NULL(type, varName) GETSET_VALIDATE_SPTR_NULL(type, varName, (void)value;)

#define GETSET_VALIDATE_SPTR_NULL(type, varName, validateValue) \
    protected: \
        mutable std::shared_ptr<type> _##varName = nullptr; \
        void validate##varName(const std::shared_ptr<vcc::IObject> value) const { validateValue; } \
    public: \
        std::shared_ptr<type> get##varName() const { return _##varName; } \
        void set##varName(std::shared_ptr<type> value) { validate##varName(value); _##varName = value; } \
        void clone##varName(const vcc::IObject *value) { _##varName = value != nullptr ? std::dynamic_pointer_cast<type>(value->clone()) : nullptr; }
    
// std::vector

#define VECTOR(type, varName) VECTOR_VALIDATE(type, varName, (void)value; )

#define VECTOR_VALIDATE(type, varName, validateValue) \
protected: \
    mutable std::vector<type> _##varName; \
    void validate##varName(const type &value) const { validateValue; } \
public: \
    std::vector<type> &get##varName() const { return _##varName; } \
    type get##varName##AtIndex(const int64_t &index) const { return _##varName[index]; } \
    void set##varName##AtIndex(const int64_t &index, const type &value) { validate##varName(value); vcc::set(_##varName, value, index); } \
    int64_t find##varName(const type &value) const { return vcc::find(_##varName, value); } \
    void insert##varName(const type &value) { validate##varName(value); vcc::insert(_##varName, value); } \
    void insert##varName##AtIndex(const int64_t &index, const type &value) { validate##varName(value); vcc::insert(_##varName, value, index); } \
    void insert##varName(const std::vector<type> &value) { for (auto &element : value) validate##varName(element); vcc::insert(_##varName, value); } \
    void insert##varName##AtIndex(const int64_t &index, const std::vector<type> &value) { for (auto const &element : value) validate##varName(element); vcc::insert(_##varName, value, index); } \
    void remove##varName(const type &value) { vcc::remove(_##varName, value); } \
    void remove##varName##AtIndex(const int64_t &index) { vcc::removeAtIndex(_##varName, index); } \
    std::vector<type> clone##varName() const { return _##varName; }\
    void clone##varName(const std::vector<type> &value) { _##varName.clear(); insert##varName(value); }\
    void clear##varName() { _##varName.clear(); }

#define VECTOR_SPTR(type, varName) VECTOR_VALIDATE_SPTR(type, varName, (void)value; )

#define VECTOR_VALIDATE_SPTR(type, varName, validateValue) \
protected: \
    mutable std::vector<std::shared_ptr<type>> _##varName; \
    void validate##varName(const std::shared_ptr<vcc::IObject> value) const { validateValue; } \
public: \
    std::vector<std::shared_ptr<type>> &get##varName() const { return _##varName; } \
    std::shared_ptr<type> get##varName##AtIndex(int64_t index) const { return _##varName[index]; } \
    void set##varName##AtIndex(int64_t index, std::shared_ptr<vcc::IObject> value) { validate##varName(value); vcc::setIObject(_##varName, value, index); } \
    int64_t find##varName(const std::shared_ptr<vcc::IObject> value) const { return vcc::findIObject(_##varName, value.get()); } \
    void insert##varName(std::shared_ptr<vcc::IObject> value) { validate##varName(value); vcc::insertIObject(_##varName, value); } \
    void insert##varName##AtIndex(int64_t index, std::shared_ptr<vcc::IObject> value) { validate##varName(value); vcc::insertIObject(_##varName, value, index); } \
    void insert##varName(std::vector<std::shared_ptr<type>> &value) { for (auto &element : value) validate##varName(element); vcc::insertIObjects(_##varName, value); } \
    void insert##varName##AtIndex(int64_t index, std::vector<std::shared_ptr<type>> &value) { for (auto const &element : value) validate##varName(element); vcc::insertIObjects(_##varName, value, index); } \
    void remove##varName(const IObject* value) { vcc::removeIObject(_##varName, value); } \
    void remove##varName##AtIndex(int64_t index) { vcc::removeIObjecttAtIndex(_##varName, index); } \
    std::shared_ptr<type> clone##varName##AtIndex(int64_t index) const { return std::static_pointer_cast<type>(_##varName[index]->clone()); } \
    std::vector<std::shared_ptr<type>> clone##varName() const { std::vector<std::shared_ptr<type>> result; for (auto &element : _##varName) { result.push_back(std::static_pointer_cast<type>(element->clone())); } return result; } \
    void clone##varName(const std::vector<std::shared_ptr<type>> &value) { _##varName.clear(); for (std::shared_ptr<type> element : value) { insert##varName(element != nullptr ? std::dynamic_pointer_cast<type>(element->clone()) : nullptr); } }\
    void clear##varName() { _##varName.clear(); }

// Set

#define SET(type, varName) SET_VALIDATE(type, varName, (void) value;)

#define SET_VALIDATE(type, varName, validateValue) \
protected: \
    mutable std::set<type> _##varName; \
    void validate##varName(const type &value) const { validateValue; } \
public: \
    std::set<type> &get##varName() const { return _##varName; } \
    void insert##varName(type value) { validate##varName(value); _##varName.insert(value); } \
    void insert##varName(std::set<type> value) { for (auto const &element : value) validate##varName(element); _##varName.insert(value.begin(), value.end()); } \
    void remove##varName(type value) { _##varName.erase(value); } \
    std::set<type> clone##varName() {return _##varName; }\
    void clone##varName(const std::set<type> &value) { _##varName.clear(); _##varName.insert(value.begin(), value.end()); }\
    void clear##varName() { _##varName.clear(); }

#define SET_SPTR(type, varName) SET_VALIDATE_SPTR(type, varName, (void)value;)

#define SET_VALIDATE_SPTR(type, varName, validateValue) \
protected: \
    mutable std::set<std::shared_ptr<type>> _##varName; \
    void validate##varName(const std::shared_ptr<vcc::IObject> value) const { validateValue; } \
public: \
    std::set<std::shared_ptr<type>> &get##varName() const { return _##varName; } \
    void insert##varName(std::shared_ptr<type> value) { validate##varName(value); _##varName.insert(value); } \
    void insert##varName(std::set<std::shared_ptr<type>> value) { for (auto const &element : value) validate##varName(element); _##varName.insert(value.begin(), value.end()); } \
    std::set<std::shared_ptr<type>> clone##varName() const { std::set<std::shared_ptr<type>> result; for (auto const & element : _##varName) result.insert(std::dynamic_pointer_cast<type>(element->clone())); return result; } \
    void clone##varName(const std::set<std::shared_ptr<type>> &value) { _##varName.clear(); for (std::shared_ptr<type> element : value) { insert##varName(element != nullptr ? std::dynamic_pointer_cast<type>(element->clone()) : nullptr); } }\
    void clear##varName() { _##varName.clear(); }

// map

#define MAP(keyType, valueType, varName) MAP_VALIDATE(keyType, valueType, varName, (void)value; )

#define MAP_VALIDATE(keyType, valueType, varName, validateValue) \
protected: \
    mutable std::map<keyType, valueType> _##varName; \
    void validate##varName(const valueType &value) const { validateValue; } \
public: \
    std::map<keyType, valueType> &get##varName() const { return _##varName; } \
    std::set<keyType> get##varName##Keys() const { return vcc::getKeys(_##varName); } \
    std::set<void *> get##varName##VoidKeys() const { return vcc::getVoidKeys(_##varName); } \
    bool is##varName##ContainKey(keyType key) const { return vcc::isContain(_##varName, key); } \
    valueType get##varName##AtKey(keyType key) const { return _##varName[key]; } \
    void set##varName##AtKey(keyType key, valueType value) { validate##varName(value); vcc::set(_##varName, key, value); } \
    void insert##varName##AtKey(keyType key, valueType value) { validate##varName(value); vcc::set(_##varName, key, value); } \
    void insert##varName(const std::pair<keyType, valueType> &value) { validate##varName(value.second); vcc::set(_##varName, value.first, value.second); } \
    void insert##varName(const std::map<keyType, valueType> &value) { for (auto const &pair : value) validate##varName(pair.second); vcc::set(_##varName, value); } \
    std::map<keyType, valueType> clone##varName() const { return _##varName; } \
    void clone##varName(const std::map<keyType, valueType> &value) { _##varName.clear(); insert##varName(value); } \
    void remove##varName##AtKey(keyType key) { vcc::removeAtKey(_##varName, key); } \
    void clear##varName() { _##varName.clear(); }

#define MAP_SPTR_R(keyType, valueType, varName) MAP_VALIDATE_SPTR_R(keyType, valueType, varName, (void)value; )

#define MAP_VALIDATE_SPTR_R(keyType, valueType, varName, validateValue) \
protected: \
    mutable std::map<keyType, std::shared_ptr<valueType>> _##varName; \
    void validate##varName(const std::shared_ptr<vcc::IObject> value) const { validateValue; } \
public: \
    std::map<keyType, std::shared_ptr<valueType>> &get##varName() const { return _##varName; } \
    std::set<keyType> get##varName##Keys() const { std::set<keyType> result; for (auto const &pair : _##varName) result.insert(pair.first); return result; } \
    std::set<void *> get##varName##VoidKeys() const { std::set<void *> result; for (auto const &pair : _##varName) result.insert((void *)&pair.first); return result; } \
    bool is##varName##ContainKey(keyType key) const { return vcc::isContain(_##varName, key); } \
    std::shared_ptr<valueType> get##varName##AtKey(keyType key) const { return _##varName[key]; } \
    void set##varName##AtKey(keyType key, std::shared_ptr<valueType> value) { validate##varName(value); vcc::setIObject(_##varName, key, value); } \
    void insert##varName##AtKey(keyType key, std::shared_ptr<valueType> value) { validate##varName(value); vcc::setIObject(_##varName, key, value); } \
    void insert##varName(const std::pair<keyType, std::shared_ptr<valueType>> &value) { validate##varName(value.second); vcc::setIObject(_##varName, value.first, value.second); } \
    void insert##varName(const std::map<keyType, std::shared_ptr<valueType>> &value) { for (auto const &pair : value) validate##varName(pair.second); vcc::setIObjects(_##varName, value); } \
    std::shared_ptr<valueType> clone##varName##AtKey(keyType key) const { return std::static_pointer_cast<valueType>(get##varName##AtKey(key)->clone()); } \
    std::map<keyType, std::shared_ptr<valueType>> clone##varName() const { std::map<keyType, std::shared_ptr<valueType>> result; for (auto &pair : _##varName) { result.insert(std::make_pair(pair.first, std::dynamic_pointer_cast<valueType>(pair.second->clone()))); } return result; } \
    void clone##varName(const std::map<keyType, std::shared_ptr<valueType>> &value) { _##varName.clear(); for (auto const& element : value) { insert##varName##AtKey(element.first, element.second != nullptr ? std::dynamic_pointer_cast<valueType>(element.second->clone()) : nullptr); } }\
    void remove##varName##AtKey(keyType key) { vcc::removeIObjectAtKey(_##varName, key); } \
    void clear##varName() { _##varName.clear(); }

#define ORDERED_MAP(keyType, valueType, varName) ORDERED_MAP_VALIDATE(keyType, valueType, varName, (void)value; )

#define ORDERED_MAP_VALIDATE(keyType, valueType, varName, validateValue) \
protected: \
    mutable std::vector<std::pair<keyType, valueType>> _##varName; \
    void validate##varName(const valueType &value) const { validateValue; } \
public: \
    std::vector<std::pair<keyType, valueType>> &get##varName() const { return _##varName; } \
    std::set<keyType> get##varName##Keys() const { std::set<keyType> result; for (auto const &pair : _##varName) result.insert(pair.first); return result; } \
    std::set<void *> get##varName##VoidKeys() const { std::set<void *> result; for (auto const &pair : _##varName) result.insert((void *)&pair.first); return result; } \
    std::pair<keyType, valueType> get##varName##AtIndex(int64_t index) const { return _##varName[index]; } \
    valueType get##varName##AtKey(keyType key) const { for (auto const &pair : _##varName) {  if (pair.first == key) return pair.second; } throw vcc::Exception(ExceptionType::CustomError, L"key not found");  } \
    void set##varName##AtIndex(int64_t index, std::pair<keyType, valueType> value) { validate##varName(value.second); _##varName[index] = value; } \
    void set##varName##AtKey(keyType key, valueType value) { for (auto &pair : _##varName) {  validate##varName(value); if (pair.first == key) { pair.second = value; return; } } insert##varName##AtKey(key, value); } \
    int64_t find##varName(keyType key) const { for (size_t i = 0; i < _##varName.size(); i++) { if (_##varName[i].first == key) return i; } return -1; } \
    bool is##varName##ContainKey(keyType key) const { return find##varName(key) > -1; } \
    void insert##varName##AtIndex(int64_t index, std::pair<keyType, valueType> value) { validate##varName(value.second); vcc::insert(_##varName, value, index); } \
    void insert##varName##AtKey(keyType key, valueType value) { validate##varName(value); _##varName.push_back(std::make_pair(key, value)); } \
    void insert##varName(const std::pair<keyType, valueType> value) { validate##varName(value.second); _##varName.push_back(value); } \
    void insert##varName(const std::vector<std::pair<keyType, valueType>> &value) { for (auto const &pair : value) validate##varName(pair.second); _##varName.insert(_##varName.end(), value.begin(), value.end()); } \
    std::vector<std::pair<keyType, valueType>> clone##varName() const { return _##varName; } \
    void clone##varName(const std::vector<std::pair<keyType, valueType>> &value) { _##varName.clear(); insert##varName(value); } \
    void remove##varName##AtIndex(int64_t index) { if (index >= 0) { _##varName.erase(_##varName.begin() + (size_t)index); } } \
    void remove##varName##AtKey(keyType key) { int64_t index = find##varName(key); if (index >= 0) (_##varName.erase(_##varName.begin() + (size_t)(find##varName(key)))); } \
    void clear##varName() { _##varName.clear(); }

#define ORDERED_MAP_SPTR_R(keyType, valueType, varName) ORDERED_MAP_VALIDATE_SPTR_R(keyType, valueType, varName, (void)value; )

#define ORDERED_MAP_VALIDATE_SPTR_R(keyType, valueType, varName, validateValue) \
protected: \
    mutable std::vector<std::pair<keyType, std::shared_ptr<valueType>>> _##varName; \
    void validate##varName(const std::shared_ptr<vcc::IObject> value) const { validateValue; } \
public: \
    std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &get##varName() const { return _##varName; } \
    std::set<keyType> get##varName##Keys() const { std::set<keyType> result; for (auto const &pair : _##varName) result.insert(pair.first); return result; } \
    std::set<void *> get##varName##VoidKeys() const { std::set<void *> result; for (auto const &pair : _##varName) result.insert((void *)&pair.first); return result; } \
    std::pair<keyType, std::shared_ptr<valueType>> get##varName##AtIndex(int64_t index) const { return _##varName[index]; } \
    std::shared_ptr<valueType> get##varName##AtKey(keyType key) const { for (auto const &pair : _##varName) { if (pair.first == key) return pair.second; } throw vcc::Exception(ExceptionType::CustomError, L"key not found"); return nullptr; } \
    int64_t find##varName(keyType key) const { for (size_t i = 0; i < _##varName.size(); i++) { if (_##varName[i].first == key) return i; } return -1; } \
    bool is##varName##ContainKey(keyType key) const { return find##varName(key) > -1; } \
    void set##varName##AtIndex(int64_t index, std::pair<keyType, std::shared_ptr<valueType>> value) { validate##varName(value.second); _##varName[index] = value; } \
    void set##varName##AtKey(keyType key, std::shared_ptr<valueType> value) { for (auto &pair : _##varName) { validate##varName(value); if (pair.first == key) { pair.second = value; return; } } insert##varName##AtKey(key, value); } \
    void insert##varName##AtIndex(int64_t index, std::pair<keyType, std::shared_ptr<valueType>> value) { validate##varName(value.second); vcc::insert(_##varName, value, index); } \
    void insert##varName##AtKey(keyType key, std::shared_ptr<valueType> value) { validate##varName(value); _##varName.push_back(std::make_pair(key, value)); } \
    void insert##varName(const std::pair<keyType, std::shared_ptr<valueType>> value) { validate##varName(value.second); _##varName.push_back(value); } \
    void insert##varName(const std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &value) { for (auto const &pair : value) validate##varName(pair.second); _##varName.insert(_##varName.end(),value.begin(), value.end()); } \
    std::shared_ptr<valueType> clone##varName##AtIndex(int64_t index) const { return std::static_pointer_cast<valueType>(get##varName##AtIndex(index).second->clone()); } \
    std::shared_ptr<valueType> clone##varName##AtKey(keyType key) const { return std::static_pointer_cast<valueType>(get##varName##AtKey(key)->clone()); } \
    void clone##varName(const std::vector<std::pair<keyType, std::shared_ptr<valueType>>> &value) { _##varName.clear(); for (auto const& element : value) { insert##varName##AtKey(element.first, element.second != nullptr ? std::dynamic_pointer_cast<valueType>(element.second->clone()) : nullptr); } }\
    void remove##varName##AtIndex(int64_t index) { if (index >= 0) { _##varName.erase(_##varName.begin() + (size_t)index); } } \
    void remove##varName##AtKey(keyType key) { int64_t index = find##varName(key); if (index >= 0) (_##varName.erase(_##varName.begin() + (size_t)(find##varName(key)))); } \
    void clear##varName() { _##varName.clear(); }

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
    std::shared_ptr<type> get##varName() const { return _##varName; } \
    void set##varName(std::shared_ptr<type> value) { _##varName = value; }
    
#define MANAGER_SPTR_PARENT(type, varName, parentClass) \
protected: \
    mutable std::shared_ptr<type> _##varName = nullptr; \
public: \
    std::shared_ptr<type> get##varName() const { auto baseForm = std::dynamic_pointer_cast<parentClass>(_ParentObject); return (_##varName == nullptr && baseForm != nullptr) ? baseForm->get##varName() : _##varName; } \
    void set##varName(std::shared_ptr<type> value) { _##varName = value; }

//------------------------------------------------------------------------------------------------------//
//--------------------------------------------- ACTION -------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
#define ACTION(name) \
    public: \
        virtual std::shared_ptr<vcc::IResult> do##name();
        
#define ACTION_WITH_ARG_SPTR(name, type) \
    public: \
        virtual std::shared_ptr<vcc::IResult> do##name(std::shared_ptr<type> value);
