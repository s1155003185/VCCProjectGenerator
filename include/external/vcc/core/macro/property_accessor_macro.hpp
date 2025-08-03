#pragma once

#define THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND THROW_EXCEPTION_MSG(ExceptionType::KeyNotFound, L"Object Property Type " + std::to_wstring(objectProperty) + L" not found");

#define I_PROPERTY_ACCESSOR_HEADER(type, name) \
    public: \
        virtual type read##name(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual type read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const = 0; \
        virtual type read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const = 0; \
        virtual void write##name(const vcc::LockType lockType, const int64_t &objectProperty, const type &value) = 0; \
        virtual void write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) = 0; \
        virtual void write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const void *key) = 0; \
        virtual void insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) = 0;

#define I_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
    public: \
        virtual type read##name(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual type read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const = 0; \
        virtual type read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const = 0; \
        virtual void write##name(const vcc::LockType lockType, const int64_t &objectProperty, type value) = 0; \
        virtual void write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) = 0; \
        virtual void write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, type value, const void *key) = 0; \
        virtual void insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) = 0; \
        virtual type clone##name(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual type clone##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const = 0; \
        virtual type clone##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const = 0; \

#define I_PROPERTY_ACCESSOR_CONTAINER_HEADER \
    public: \
        virtual size_t getCount(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual std::set<void *> getMapKeys(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual bool isContainKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const = 0;\
        virtual void remove(const vcc::LockType lockType, const int64_t &objectProperty, const void *value) = 0; \
        virtual void removeObject(const vcc::LockType lockType, const int64_t &objectProperty, const vcc::IObject *value) = 0; \
        virtual void removeAtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index = -1) = 0; \
        virtual void removeAtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) = 0; \
        virtual void clear(const vcc::LockType lockType, const int64_t &objectProperty) = 0;

#define BASE_PROPERTY_ACCESSOR_HEADER(type, name) \
    protected: \
        virtual type _read##name(const int64_t &objectProperty) const; \
        virtual type _read##name##AtIndex(const int64_t &objectProperty, const int64_t &index = -1) const; \
        virtual type _read##name##AtKey(const int64_t &objectProperty, const void *key) const; \
        virtual void _write##name(const int64_t &objectProperty, const type &value); \
        virtual void _write##name##AtIndex(const int64_t &objectProperty, const type &value, const int64_t &index = -1); \
        virtual void _write##name##AtKey(const int64_t &objectProperty, const type &value, const void *key); \
        virtual void _insert##name##AtIndex(const int64_t &objectProperty, const type &value, const int64_t &index = -1); \
    public: \
        virtual type read##name(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual type read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
        virtual type read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const override; \
        virtual void write##name(const vcc::LockType lockType, const int64_t &objectProperty, const type &value) override; \
        virtual void write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) override; \
        virtual void write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const void *key) override; \
        virtual void insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) override;

#define BASE_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
    protected: \
        virtual type _read##name(const int64_t &objectProperty) const; \
        virtual type _read##name##AtIndex(const int64_t &objectProperty, const int64_t &index = -1) const; \
        virtual type _read##name##AtKey(const int64_t &objectProperty, const void *key) const; \
        virtual void _write##name(const int64_t &objectProperty, type value); \
        virtual void _write##name##AtIndex(const int64_t &objectProperty, type value, const int64_t &index = -1); \
        virtual void _write##name##AtKey(const int64_t &objectProperty, type value, const void *key); \
        virtual void _insert##name##AtIndex(const int64_t &objectProperty, type value, const int64_t &index = -1); \
        virtual type _clone##name(const int64_t &objectProperty) const; \
        virtual type _clone##name##AtIndex(const int64_t &objectProperty, const int64_t &index) const; \
        virtual type _clone##name##AtKey(const int64_t &objectProperty, const void *key) const; \
    public: \
        virtual type read##name(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual type read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
        virtual type read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const override; \
        virtual void write##name(const vcc::LockType lockType, const int64_t &objectProperty, type value) override; \
        virtual void write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) override; \
        virtual void write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, type value, const void *key) override; \
        virtual void insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) override; \
        virtual type clone##name(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual type clone##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const override; \
        virtual type clone##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const override;

#define BASE_PROPERTY_ACCESSOR_CONTAINER_HEADER \
    protected: \
        virtual size_t _getCount(const int64_t &objectProperty) const; \
        virtual std::set<void *> _getMapKeys(const int64_t &objectProperty) const; \
        virtual bool _isContainKey(const int64_t &objectProperty, const void *key) const;\
        virtual void _remove(const int64_t &objectProperty, const void *value); \
        virtual void _removeObject(const int64_t &objectProperty, const vcc::IObject *value); \
        virtual void _removeAtIndex(const int64_t &objectProperty, const int64_t &index = -1); \
        virtual void _removeAtKey(const int64_t &objectProperty, const void *key); \
        virtual void _clear(const int64_t &objectProperty); \
    public: \
        virtual size_t getCount(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual std::set<void *> getMapKeys(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual bool isContainKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const override;\
        virtual void remove(const vcc::LockType lockType, const int64_t &objectProperty, const void *value) override; \
        virtual void removeObject(const vcc::LockType lockType, const int64_t &objectProperty, const vcc::IObject *value) override; \
        virtual void removeAtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index = -1) override; \
        virtual void removeAtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) override; \
        virtual void clear(const vcc::LockType lockType, const int64_t &objectProperty) override;

#define BASE_PROPERTY_ACCESSOR_DETAIL(type, name, defaultValue) \
    type BasePropertyAccessor::_read##name(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_read##name##AtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_read##name##AtKey(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    void BasePropertyAccessor::_write##name(const int64_t &objectProperty, const type &/*value*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_write##name##AtIndex(const int64_t &objectProperty, const type &/*value*/, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_write##name##AtKey(const int64_t &objectProperty, const type &/*value*/, const void * /*key*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_insert##name##AtIndex(const int64_t &objectProperty, const type &/*value*/, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    type BasePropertyAccessor::read##name(const vcc::LockType lockType, const int64_t &objectProperty) const { type result = defaultValue; LOCK_BEGIN result = _read##name(objectProperty); LOCK_END return result; } \
    type BasePropertyAccessor::read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _read##name##AtIndex(objectProperty, index); LOCK_END return result; } \
    type BasePropertyAccessor::read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const { type result = defaultValue; LOCK_BEGIN result = _read##name##AtKey(objectProperty, key); LOCK_END return result; } \
    void BasePropertyAccessor::write##name(const vcc::LockType lockType, const int64_t &objectProperty, const type &value) { LOCK_BEGIN _write##name(objectProperty, value); LOCK_END } \
    void BasePropertyAccessor::write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index) { LOCK_BEGIN _write##name##AtIndex(objectProperty, value, index); LOCK_END } \
    void BasePropertyAccessor::write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const void *key) { LOCK_BEGIN _write##name##AtKey(objectProperty, value, key); LOCK_END } \
    void BasePropertyAccessor::insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index) { LOCK_BEGIN _insert##name##AtIndex(objectProperty, value, index); LOCK_END }

#define BASE_PROPERTY_ACCESSOR_OBJECT_DETAIL(type, name, defaultValue) \
    type BasePropertyAccessor::_read##name(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_read##name##AtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_read##name##AtKey(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    void BasePropertyAccessor::_write##name(const int64_t &objectProperty, type /*value*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_write##name##AtIndex(const int64_t &objectProperty, type /*value*/, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_write##name##AtKey(const int64_t &objectProperty, type /*value*/, const void * /*key*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_insert##name##AtIndex(const int64_t &objectProperty, type /*value*/, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    type BasePropertyAccessor::_clone##name(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_clone##name##AtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_clone##name##AtKey(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::read##name(const vcc::LockType lockType, const int64_t &objectProperty) const { type result = defaultValue; LOCK_BEGIN result = _read##name(objectProperty); LOCK_END return result; } \
    type BasePropertyAccessor::read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _read##name##AtIndex(objectProperty, index); LOCK_END return result; } \
    type BasePropertyAccessor::read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const { type result = defaultValue; LOCK_BEGIN result = _read##name##AtKey(objectProperty, key); LOCK_END return result; } \
    void BasePropertyAccessor::write##name(const vcc::LockType lockType, const int64_t &objectProperty, type value) { LOCK_BEGIN _write##name(objectProperty, value); LOCK_END } \
    void BasePropertyAccessor::write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index) { LOCK_BEGIN _write##name##AtIndex(objectProperty, value, index); LOCK_END } \
    void BasePropertyAccessor::write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, type value, const void *key) { LOCK_BEGIN _write##name##AtKey(objectProperty, value, key); LOCK_END } \
    void BasePropertyAccessor::insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index) { LOCK_BEGIN _insert##name##AtIndex(objectProperty, value, index); LOCK_END } \
    type BasePropertyAccessor::clone##name(const vcc::LockType lockType, const int64_t &objectProperty) const { type result = nullptr; LOCK_BEGIN result = _clone##name(objectProperty); LOCK_END return result; } \
    type BasePropertyAccessor::clone##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const { type result = nullptr; LOCK_BEGIN result = _clone##name##AtIndex(objectProperty, index); LOCK_END return result; } \
    type BasePropertyAccessor::clone##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const { type result = nullptr; LOCK_BEGIN result = _clone##name##AtKey(objectProperty, key); LOCK_END return result; } \

#define BASE_PROPERTY_ACCESSOR_CONTAINER_DETAIL \
    size_t BasePropertyAccessor::_getCount(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return 0;} \
    std::set<void *> BasePropertyAccessor::_getMapKeys(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND std::set<void *> result; return result;} \
    bool BasePropertyAccessor::_isContainKey(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return false;} \
    void BasePropertyAccessor::_remove(const int64_t &objectProperty, const void * /*value*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_removeObject(const int64_t &objectProperty, const vcc::IObject * /*value*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_removeAtIndex(const int64_t &objectProperty, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_removeAtKey(const int64_t &objectProperty, const void * /*key*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_clear(const int64_t &objectProperty) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    size_t BasePropertyAccessor::getCount(const vcc::LockType lockType, const int64_t &objectProperty) const { size_t result = 0; LOCK_BEGIN result = _getCount(objectProperty); LOCK_END return result; } \
    std::set<void *> BasePropertyAccessor::getMapKeys(const vcc::LockType lockType, const int64_t &objectProperty) const { std::set<void *> result; LOCK_BEGIN result = _getMapKeys(objectProperty); LOCK_END return result; } \
    bool BasePropertyAccessor::isContainKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const { bool result = false; LOCK_BEGIN result = _isContainKey(objectProperty, key); LOCK_END return result; }\
    void BasePropertyAccessor::remove(const vcc::LockType lockType, const int64_t &objectProperty, const void *value) { LOCK_BEGIN _remove(objectProperty, value); LOCK_END } \
    void BasePropertyAccessor::removeObject(const vcc::LockType lockType, const int64_t &objectProperty, const vcc::IObject *value) { LOCK_BEGIN _removeObject(objectProperty, value); LOCK_END } \
    void BasePropertyAccessor::removeAtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) { LOCK_BEGIN _removeAtIndex(objectProperty, index); LOCK_END } \
    void BasePropertyAccessor::removeAtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) { LOCK_BEGIN _removeAtKey(objectProperty, key); LOCK_END } \
    void BasePropertyAccessor::clear(const vcc::LockType lockType, const int64_t &objectProperty) { LOCK_BEGIN _clear(objectProperty); LOCK_END }

#define PROPERTY_ACCESSOR_HEADER(type, name) \
    protected: \
        virtual type _read##name(const int64_t &objectProperty) const override; \
        virtual type _read##name##AtIndex(const int64_t &objectProperty, const int64_t &index = -1) const override; \
        virtual type _read##name##AtKey(const int64_t &objectProperty, const void *key) const override; \
        virtual void _write##name(const int64_t &objectProperty, const type &value) override; \
        virtual void _write##name##AtIndex(const int64_t &objectProperty, const type &value, const int64_t &index = -1) override; \
        virtual void _write##name##AtKey(const int64_t &objectProperty, const type &value, const void *key) override; \
        virtual void _insert##name##AtIndex(const int64_t &objectProperty, const type &value, const int64_t &index = -1) override;

#define PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
    protected: \
        virtual type _read##name(const int64_t &objectProperty) const override; \
        virtual type _read##name##AtIndex(const int64_t &objectProperty, const int64_t &index = -1) const override; \
        virtual type _read##name##AtKey(const int64_t &objectProperty, const void *key) const override; \
        virtual void _write##name(const int64_t &objectProperty, type value) override; \
        virtual void _write##name##AtIndex(const int64_t &objectProperty, type value, const int64_t &index = -1) override; \
        virtual void _write##name##AtKey(const int64_t &objectProperty, type value, const void *key) override; \
        virtual void _insert##name##AtIndex(const int64_t &objectProperty, type value, const int64_t &index = -1) override; \
        virtual type _clone##name(const int64_t &objectProperty) const override; \
        virtual type _clone##name##AtIndex(const int64_t &objectProperty, const int64_t &index) const override; \
        virtual type _clone##name##AtKey(const int64_t &objectProperty, const void *key) const override; \
        
#define PROPERTY_ACCESSOR_CONTAINER_HEADER \
    protected: \
        virtual size_t _getCount(const int64_t &objectProperty) const override; \
        virtual std::set<void *> _getMapKeys(const int64_t &objectProperty) const override; \
        virtual bool _isContainKey(const int64_t &objectProperty, const void *key) const override; \
        void _remove(const int64_t &objectProperty, const void *value) override; \
        void _removeObject(const int64_t &objectProperty, const vcc::IObject *value) override; \
        virtual void _removeAtIndex(const int64_t &objectProperty, const int64_t &index) override; \
        virtual void _removeAtKey(const int64_t &objectProperty, const void *key) override; \
        virtual void _clear(const int64_t &objectProperty) override;


// ------------------------------------------------------------------------------------------ //
//                                      DLL Interface                                         //
// ------------------------------------------------------------------------------------------ //

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(exportType, typeName) \
    DLLEXPORT exportType read##typeName(void *ref, int64_t property); \
    DLLEXPORT exportType read##typeName##AtIndex(void *ref, int64_t property, int64_t index); \
    DLLEXPORT exportType read##typeName##AtKey(void *ref, int64_t property, void *key); \
    DLLEXPORT void write##typeName(void *ref, int64_t property, exportType value); \
    DLLEXPORT void write##typeName##AtIndex(void *ref, int64_t property, exportType value, int64_t index); \
    DLLEXPORT void write##typeName##AtKey(void *ref, int64_t property, exportType value, void *key); \
    DLLEXPORT void insert##typeName##AtIndex(void *ref, int64_t property, exportType value, int64_t index);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING \
    DLLEXPORT void readString(void *ref, int64_t property, wchar_t **value); \
    DLLEXPORT void readStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index); \
    DLLEXPORT void readStringAtKey(void *ref, int64_t property, wchar_t **value, void *key); \
    DLLEXPORT void writeString(void *ref, int64_t property, wchar_t **value); \
    DLLEXPORT void writeStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index); \
    DLLEXPORT void writeStringAtKey(void *ref, int64_t property, wchar_t **value, void *key); \
    DLLEXPORT void insertStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT \
    DLLEXPORT void *readObject(void *ref, int64_t property); \
    DLLEXPORT void *readObjectAtIndex(void *ref, int64_t property, int64_t index); \
    DLLEXPORT void *readObjectAtKey(void *ref, int64_t property, void *key); \
    DLLEXPORT void writeObject(void *ref, int64_t property, void *value); \
    DLLEXPORT void writeObjectAtIndex(void *ref, int64_t property, void *value, int64_t index); \
    DLLEXPORT void writeObjectAtKey(void *ref, int64_t property, void *value, void *key); \
    DLLEXPORT void *addObjectAtIndex(void *ref, int64_t property, int64_t objectType, int64_t index); \
    DLLEXPORT void insertObjectAtIndex(void *ref, int64_t property, void *value, int64_t index);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER \
    DLLEXPORT long getCount(void *ref, int64_t property); \
    DLLEXPORT void **getMapKeys(void *ref, int64_t property); \
    DLLEXPORT bool isContainKey(void *ref, int64_t property, void *key); \
    DLLEXPORT void removeValue(void *ref, int64_t property, void *value); \
    DLLEXPORT void removeObject(void *ref, int64_t property, void *value); \
    DLLEXPORT void removeAtIndex(void *ref, int64_t property, int64_t index); \
    DLLEXPORT void removeAtKey(void *ref, int64_t property, void *key); \
    DLLEXPORT void clear(void *ref, int64_t property);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(exportType, typeName, defaultValue) \
exportType read##typeName(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::create(object->sharedPtr())->read##typeName(vcc::LockType::ReadLock, property); \
    CATCH \
    return defaultValue; \
} \
exportType read##typeName##AtIndex(void *ref, int64_t property, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::create(object->sharedPtr())->read##typeName##AtIndex(vcc::LockType::ReadLock, property, index); \
    CATCH \
    return defaultValue; \
} \
exportType read##typeName##AtKey(void *ref, int64_t property, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::create(object->sharedPtr())->read##typeName##AtKey(vcc::LockType::ReadLock, property, key); \
    CATCH \
    return defaultValue; \
} \
void write##typeName(void *ref, int64_t property, exportType value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::create(object->sharedPtr())->write##typeName(vcc::LockType::WriteLock, property, value); \
    CATCH \
} \
void write##typeName##AtIndex(void *ref, int64_t property, exportType value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::create(object->sharedPtr())->write##typeName##AtIndex(vcc::LockType::WriteLock, property, value, index); \
    CATCH \
} \
void write##typeName##AtKey(void *ref, int64_t property, exportType value, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::create(object->sharedPtr())->write##typeName##AtKey(vcc::LockType::WriteLock, property, value, key); \
    CATCH \
} \
void insert##typeName##AtIndex(void *ref, int64_t property, exportType value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::create(object->sharedPtr())->insert##typeName##AtIndex(vcc::LockType::WriteLock, property, value, index); \
    CATCH \
}

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING \
void readString(void *ref, int64_t property, wchar_t **value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::wstring result = PropertyAccessorFactory::create(object->sharedPtr())->readString(vcc::LockType::ReadLock, property); \
        size_t size = (result.length() + 1) * sizeof(wchar_t); \
        *value = static_cast<wchar_t*>(malloc(size)); \
        wcscpy(*value, result.c_str()); \
    CATCH \
} \
void readStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::wstring result = PropertyAccessorFactory::create(object->sharedPtr())->readStringAtIndex(vcc::LockType::ReadLock, property, index); \
        size_t size = (result.length() + 1) * sizeof(wchar_t); \
        *value = static_cast<wchar_t*>(malloc(size)); \
        wcscpy(*value, result.c_str()); \
    CATCH \
} \
void readStringAtKey(void *ref, int64_t property, wchar_t **value, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::wstring result = PropertyAccessorFactory::create(object->sharedPtr())->readStringAtKey(vcc::LockType::ReadLock, property, key); \
        size_t size = (result.length() + 1) * sizeof(wchar_t); \
        *value = static_cast<wchar_t*>(malloc(size)); \
        wcscpy(*value, result.c_str()); \
    CATCH \
} \
void writeString(void *ref, int64_t property, wchar_t **value) \
{ \
    TRY \
        if (value != nullptr && *value != nullptr) \
        { \
            vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
            auto ptr = object->sharedPtr();\
            PropertyAccessorFactory::create(object->sharedPtr())->writeString(vcc::LockType::WriteLock, property, *value); \
        } \
    CATCH \
} \
void writeStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index) \
{ \
    TRY \
        if (value != nullptr && *value != nullptr) \
        { \
            vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
            auto ptr = object->sharedPtr();\
            PropertyAccessorFactory::create(object->sharedPtr())->writeStringAtIndex(vcc::LockType::WriteLock, property, *value, index); \
        } \
    CATCH \
} \
void writeStringAtKey(void *ref, int64_t property, wchar_t **value, void *key) \
{ \
    TRY \
        if (value != nullptr && *value != nullptr) \
        { \
            vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
            PropertyAccessorFactory::create(object->sharedPtr())->writeStringAtKey(vcc::LockType::WriteLock, property, *value, key); \
        } \
    CATCH \
} \
void insertStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index) \
{ \
    TRY \
        if (value != nullptr && *value != nullptr) \
        { \
            vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
            PropertyAccessorFactory::create(object->sharedPtr())->insertStringAtIndex(vcc::LockType::WriteLock, property, *value, index); \
        } \
    CATCH \
}

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT \
DLLEXPORT void *readObject(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::shared_ptr<vcc::IObject> result = PropertyAccessorFactory::create(object->sharedPtr())->readObject(vcc::LockType::ReadLock, property); \
        return result != nullptr ? result.get() : nullptr; \
    CATCH \
    return nullptr; \
} \
DLLEXPORT void *readObjectAtIndex(void *ref, int64_t property, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::shared_ptr<vcc::IObject> result = PropertyAccessorFactory::create(object->sharedPtr())->readObjectAtIndex(vcc::LockType::ReadLock, property, index); \
        return result != nullptr ? result.get() : nullptr; \
    CATCH \
    return nullptr; \
} \
DLLEXPORT void *readObjectAtKey(void *ref, int64_t property, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::shared_ptr<vcc::IObject> result = PropertyAccessorFactory::create(object->sharedPtr())->readObjectAtKey(vcc::LockType::ReadLock, property, key); \
        return result != nullptr ? result.get() : nullptr; \
    CATCH \
    return nullptr; \
} \
DLLEXPORT void writeObject(void *ref, int64_t property, void *value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        vcc::IObject *result = static_cast<vcc::IObject *>(value); \
        std::shared_ptr<vcc::IObject> resultPtr(result); \
        PropertyAccessorFactory::create(object->sharedPtr())->writeObject(vcc::LockType::WriteLock, property, resultPtr); \
    CATCH \
} \
DLLEXPORT void writeObjectAtIndex(void *ref, int64_t property, void *value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        vcc::IObject *result = static_cast<vcc::IObject *>(value); \
        std::shared_ptr<vcc::IObject> resultPtr(result); \
        PropertyAccessorFactory::create(object->sharedPtr())->writeObjectAtIndex(vcc::LockType::WriteLock, property, resultPtr, index); \
    CATCH \
} \
DLLEXPORT void writeObjectAtKey(void *ref, int64_t property, void *value, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        vcc::IObject *result = static_cast<vcc::IObject *>(value); \
        std::shared_ptr<vcc::IObject> resultPtr(result); \
        PropertyAccessorFactory::create(object->sharedPtr())->writeObjectAtKey(vcc::LockType::WriteLock, property, resultPtr, key); \
    CATCH \
} \
DLLEXPORT void *addObjectAtIndex(void *ref, int64_t property, int64_t objectType, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::shared_ptr<vcc::IObject> newObject = ObjectFactory::create(static_cast<ObjectType>(objectType)); \
        PropertyAccessorFactory::create(object->sharedPtr())->insertObjectAtIndex(vcc::LockType::WriteLock, property, newObject, index); \
        return newObject.get(); \
    CATCH \
    return nullptr; \
} \
DLLEXPORT void insertObjectAtIndex(void *ref, int64_t property, void *value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        vcc::IObject *result = static_cast<vcc::IObject *>(value); \
        std::shared_ptr<vcc::IObject> resultPtr(result); \
        PropertyAccessorFactory::create(object->sharedPtr())->insertObjectAtIndex(vcc::LockType::WriteLock, property, resultPtr, index); \
    CATCH \
}

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER \
long getCount(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::create(object->sharedPtr())->getCount(vcc::LockType::ReadLock, property); \
    CATCH \
    return 0; \
} \
void **getMapKeys(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::set<void *> result = PropertyAccessorFactory::create(object->sharedPtr())->getMapKeys(vcc::LockType::ReadLock, property); \
        void **array = new void *[result.size() + 1]; \
        std::fill(array, array + (result.size() + 1), nullptr); \
        size_t index = 0; \
        for (const auto& item : result) \
            array[index++] = item; \
        return array; \
    CATCH \
    return nullptr; \
} \
bool isContainKey(void *ref, int64_t property, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::create(object->sharedPtr())->isContainKey(vcc::LockType::ReadLock, property, key); \
    CATCH \
    return false; \
} \
void removeValue(void *ref, int64_t property, void *value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::create(object->sharedPtr())->remove(vcc::LockType::WriteLock, property, value); \
    CATCH \
} \
void removeObject(void *ref, int64_t property, void *value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::create(object->sharedPtr())->removeObject(vcc::LockType::WriteLock, property, static_cast<vcc::IObject *>(value)); \
    CATCH \
} \
void removeAtIndex(void *ref, int64_t property, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::create(object->sharedPtr())->removeAtIndex(vcc::LockType::WriteLock, property, index); \
    CATCH \
} \
void removeAtKey(void *ref, int64_t property, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::create(object->sharedPtr())->removeAtKey(vcc::LockType::WriteLock, property, key); \
    CATCH \
} \
void clear(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::create(object->sharedPtr())->clear(vcc::LockType::WriteLock, property); \
    CATCH \
}
