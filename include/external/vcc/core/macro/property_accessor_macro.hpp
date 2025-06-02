#pragma once

#define THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND THROW_EXCEPTION_MSG(ExceptionType::KeyNotFound, L"Object Property Type " + std::to_wstring(objectProperty) + L" not found");

#define I_PROPERTY_ACCESSOR_HEADER(type, name) \
    public: \
        virtual type Read##name(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual type Read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const = 0; \
        virtual type Read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const = 0; \
        virtual void Write##name(const vcc::LockType lockType, const int64_t &objectProperty, const type &value) = 0; \
        virtual void Write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) = 0; \
        virtual void Write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const void *key) = 0; \
        virtual void Insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) = 0;

#define I_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
    public: \
        virtual type Read##name(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual type Read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const = 0; \
        virtual type Read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const = 0; \
        virtual void Write##name(const vcc::LockType lockType, const int64_t &objectProperty, type value) = 0; \
        virtual void Write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) = 0; \
        virtual void Write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, type value, const void *key) = 0; \
        virtual void Insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) = 0; \
        virtual type Clone##name(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual type Clone##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const = 0; \
        virtual type Clone##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const = 0; \

#define I_PROPERTY_ACCESSOR_CONTAINER_HEADER \
    public: \
        virtual size_t GetCount(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual std::set<void *> GetMapKeys(const vcc::LockType lockType, const int64_t &objectProperty) const = 0; \
        virtual bool IsContainKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const = 0;\
        virtual void Remove(const vcc::LockType lockType, const int64_t &objectProperty, const void *value) = 0; \
        virtual void RemoveObject(const vcc::LockType lockType, const int64_t &objectProperty, const vcc::IObject *value) = 0; \
        virtual void RemoveAtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index = -1) = 0; \
        virtual void RemoveAtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) = 0; \
        virtual void Clear(const vcc::LockType lockType, const int64_t &objectProperty) = 0;

#define BASE_PROPERTY_ACCESSOR_HEADER(type, name) \
    protected: \
        virtual type _Read##name(const int64_t &objectProperty) const; \
        virtual type _Read##name##AtIndex(const int64_t &objectProperty, const int64_t &index = -1) const; \
        virtual type _Read##name##AtKey(const int64_t &objectProperty, const void *key) const; \
        virtual void _Write##name(const int64_t &objectProperty, const type &value); \
        virtual void _Write##name##AtIndex(const int64_t &objectProperty, const type &value, const int64_t &index = -1); \
        virtual void _Write##name##AtKey(const int64_t &objectProperty, const type &value, const void *key); \
        virtual void _Insert##name##AtIndex(const int64_t &objectProperty, const type &value, const int64_t &index = -1); \
    public: \
        virtual type Read##name(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual type Read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
        virtual type Read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const override; \
        virtual void Write##name(const vcc::LockType lockType, const int64_t &objectProperty, const type &value) override; \
        virtual void Write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) override; \
        virtual void Write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const void *key) override; \
        virtual void Insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) override;

#define BASE_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
    protected: \
        virtual type _Read##name(const int64_t &objectProperty) const; \
        virtual type _Read##name##AtIndex(const int64_t &objectProperty, const int64_t &index = -1) const; \
        virtual type _Read##name##AtKey(const int64_t &objectProperty, const void *key) const; \
        virtual void _Write##name(const int64_t &objectProperty, type value); \
        virtual void _Write##name##AtIndex(const int64_t &objectProperty, type value, const int64_t &index = -1); \
        virtual void _Write##name##AtKey(const int64_t &objectProperty, type value, const void *key); \
        virtual void _Insert##name##AtIndex(const int64_t &objectProperty, type value, const int64_t &index = -1); \
        virtual type _Clone##name(const int64_t &objectProperty) const; \
        virtual type _Clone##name##AtIndex(const int64_t &objectProperty, const int64_t &index) const; \
        virtual type _Clone##name##AtKey(const int64_t &objectProperty, const void *key) const; \
    public: \
        virtual type Read##name(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual type Read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
        virtual type Read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const override; \
        virtual void Write##name(const vcc::LockType lockType, const int64_t &objectProperty, type value) override; \
        virtual void Write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) override; \
        virtual void Write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, type value, const void *key) override; \
        virtual void Insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) override; \
        virtual type Clone##name(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual type Clone##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const override; \
        virtual type Clone##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const override;

#define BASE_PROPERTY_ACCESSOR_CONTAINER_HEADER \
    protected: \
        virtual size_t _GetCount(const int64_t &objectProperty) const; \
        virtual std::set<void *> _GetMapKeys(const int64_t &objectProperty) const; \
        virtual bool _IsContainKey(const int64_t &objectProperty, const void *key) const;\
        virtual void _Remove(const int64_t &objectProperty, const void *value); \
        virtual void _RemoveObject(const int64_t &objectProperty, const vcc::IObject *value); \
        virtual void _RemoveAtIndex(const int64_t &objectProperty, const int64_t &index = -1); \
        virtual void _RemoveAtKey(const int64_t &objectProperty, const void *key); \
        virtual void _Clear(const int64_t &objectProperty); \
    public: \
        virtual size_t GetCount(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual std::set<void *> GetMapKeys(const vcc::LockType lockType, const int64_t &objectProperty) const override; \
        virtual bool IsContainKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const override;\
        virtual void Remove(const vcc::LockType lockType, const int64_t &objectProperty, const void *value) override; \
        virtual void RemoveObject(const vcc::LockType lockType, const int64_t &objectProperty, const vcc::IObject *value) override; \
        virtual void RemoveAtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index = -1) override; \
        virtual void RemoveAtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) override; \
        virtual void Clear(const vcc::LockType lockType, const int64_t &objectProperty) override;

#define BASE_PROPERTY_ACCESSOR_DETAIL(type, name, defaultValue) \
    type BasePropertyAccessor::_Read##name(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_Read##name##AtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_Read##name##AtKey(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, const type &/*value*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_Write##name##AtIndex(const int64_t &objectProperty, const type &/*value*/, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_Write##name##AtKey(const int64_t &objectProperty, const type &/*value*/, const void * /*key*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_Insert##name##AtIndex(const int64_t &objectProperty, const type &/*value*/, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    type BasePropertyAccessor::Read##name(const vcc::LockType lockType, const int64_t &objectProperty) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty); LOCK_END return result; } \
    type BasePropertyAccessor::Read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _Read##name##AtIndex(objectProperty, index); LOCK_END return result; } \
    type BasePropertyAccessor::Read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const { type result = defaultValue; LOCK_BEGIN result = _Read##name##AtKey(objectProperty, key); LOCK_END return result; } \
    void BasePropertyAccessor::Write##name(const vcc::LockType lockType, const int64_t &objectProperty, const type &value) { LOCK_BEGIN _Write##name(objectProperty, value); LOCK_END } \
    void BasePropertyAccessor::Write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index) { LOCK_BEGIN _Write##name##AtIndex(objectProperty, value, index); LOCK_END } \
    void BasePropertyAccessor::Write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const void *key) { LOCK_BEGIN _Write##name##AtKey(objectProperty, value, key); LOCK_END } \
    void BasePropertyAccessor::Insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const type &value, const int64_t &index) { LOCK_BEGIN _Insert##name##AtIndex(objectProperty, value, index); LOCK_END }

#define BASE_PROPERTY_ACCESSOR_OBJECT_DETAIL(type, name, defaultValue) \
    type BasePropertyAccessor::_Read##name(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_Read##name##AtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_Read##name##AtKey(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, type /*value*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_Write##name##AtIndex(const int64_t &objectProperty, type /*value*/, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_Write##name##AtKey(const int64_t &objectProperty, type /*value*/, const void * /*key*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_Insert##name##AtIndex(const int64_t &objectProperty, type /*value*/, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    type BasePropertyAccessor::_Clone##name(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_Clone##name##AtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::_Clone##name##AtKey(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
    type BasePropertyAccessor::Read##name(const vcc::LockType lockType, const int64_t &objectProperty) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty); LOCK_END return result; } \
    type BasePropertyAccessor::Read##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _Read##name##AtIndex(objectProperty, index); LOCK_END return result; } \
    type BasePropertyAccessor::Read##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const { type result = defaultValue; LOCK_BEGIN result = _Read##name##AtKey(objectProperty, key); LOCK_END return result; } \
    void BasePropertyAccessor::Write##name(const vcc::LockType lockType, const int64_t &objectProperty, type value) { LOCK_BEGIN _Write##name(objectProperty, value); LOCK_END } \
    void BasePropertyAccessor::Write##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index) { LOCK_BEGIN _Write##name##AtIndex(objectProperty, value, index); LOCK_END } \
    void BasePropertyAccessor::Write##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, type value, const void *key) { LOCK_BEGIN _Write##name##AtKey(objectProperty, value, key); LOCK_END } \
    void BasePropertyAccessor::Insert##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, type value, const int64_t &index) { LOCK_BEGIN _Insert##name##AtIndex(objectProperty, value, index); LOCK_END } \
    type BasePropertyAccessor::Clone##name(const vcc::LockType lockType, const int64_t &objectProperty) const { type result = nullptr; LOCK_BEGIN result = _Clone##name(objectProperty); LOCK_END return result; } \
    type BasePropertyAccessor::Clone##name##AtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) const { type result = nullptr; LOCK_BEGIN result = _Clone##name##AtIndex(objectProperty, index); LOCK_END return result; } \
    type BasePropertyAccessor::Clone##name##AtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const { type result = nullptr; LOCK_BEGIN result = _Clone##name##AtKey(objectProperty, key); LOCK_END return result; } \

#define BASE_PROPERTY_ACCESSOR_CONTAINER_DETAIL \
    size_t BasePropertyAccessor::_GetCount(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return 0;} \
    std::set<void *> BasePropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND std::set<void *> result; return result;} \
    bool BasePropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return false;} \
    void BasePropertyAccessor::_Remove(const int64_t &objectProperty, const void * /*value*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_RemoveObject(const int64_t &objectProperty, const vcc::IObject * /*value*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_RemoveAtIndex(const int64_t &objectProperty, const int64_t & /*index*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_RemoveAtKey(const int64_t &objectProperty, const void * /*key*/) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    void BasePropertyAccessor::_Clear(const int64_t &objectProperty) { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
    size_t BasePropertyAccessor::GetCount(const vcc::LockType lockType, const int64_t &objectProperty) const { size_t result = 0; LOCK_BEGIN result = _GetCount(objectProperty); LOCK_END return result; } \
    std::set<void *> BasePropertyAccessor::GetMapKeys(const vcc::LockType lockType, const int64_t &objectProperty) const { std::set<void *> result; LOCK_BEGIN result = _GetMapKeys(objectProperty); LOCK_END return result; } \
    bool BasePropertyAccessor::IsContainKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) const { bool result = false; LOCK_BEGIN result = _IsContainKey(objectProperty, key); LOCK_END return result; }\
    void BasePropertyAccessor::Remove(const vcc::LockType lockType, const int64_t &objectProperty, const void *value) { LOCK_BEGIN _Remove(objectProperty, value); LOCK_END } \
    void BasePropertyAccessor::RemoveObject(const vcc::LockType lockType, const int64_t &objectProperty, const vcc::IObject *value) { LOCK_BEGIN _RemoveObject(objectProperty, value); LOCK_END } \
    void BasePropertyAccessor::RemoveAtIndex(const vcc::LockType lockType, const int64_t &objectProperty, const int64_t &index) { LOCK_BEGIN _RemoveAtIndex(objectProperty, index); LOCK_END } \
    void BasePropertyAccessor::RemoveAtKey(const vcc::LockType lockType, const int64_t &objectProperty, const void *key) { LOCK_BEGIN _RemoveAtKey(objectProperty, key); LOCK_END } \
    void BasePropertyAccessor::Clear(const vcc::LockType lockType, const int64_t &objectProperty) { LOCK_BEGIN _Clear(objectProperty); LOCK_END }

#define PROPERTY_ACCESSOR_HEADER(type, name) \
    protected: \
        virtual type _Read##name(const int64_t &objectProperty) const override; \
        virtual type _Read##name##AtIndex(const int64_t &objectProperty, const int64_t &index = -1) const override; \
        virtual type _Read##name##AtKey(const int64_t &objectProperty, const void *key) const override; \
        virtual void _Write##name(const int64_t &objectProperty, const type &value) override; \
        virtual void _Write##name##AtIndex(const int64_t &objectProperty, const type &value, const int64_t &index = -1) override; \
        virtual void _Write##name##AtKey(const int64_t &objectProperty, const type &value, const void *key) override; \
        virtual void _Insert##name##AtIndex(const int64_t &objectProperty, const type &value, const int64_t &index = -1) override;

#define PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
    protected: \
        virtual type _Read##name(const int64_t &objectProperty) const override; \
        virtual type _Read##name##AtIndex(const int64_t &objectProperty, const int64_t &index = -1) const override; \
        virtual type _Read##name##AtKey(const int64_t &objectProperty, const void *key) const override; \
        virtual void _Write##name(const int64_t &objectProperty, type value) override; \
        virtual void _Write##name##AtIndex(const int64_t &objectProperty, type value, const int64_t &index = -1) override; \
        virtual void _Write##name##AtKey(const int64_t &objectProperty, type value, const void *key) override; \
        virtual void _Insert##name##AtIndex(const int64_t &objectProperty, type value, const int64_t &index = -1) override; \
        virtual type _Clone##name(const int64_t &objectProperty) const override; \
        virtual type _Clone##name##AtIndex(const int64_t &objectProperty, const int64_t &index) const override; \
        virtual type _Clone##name##AtKey(const int64_t &objectProperty, const void *key) const override; \
        
#define PROPERTY_ACCESSOR_CONTAINER_HEADER \
    protected: \
        virtual size_t _GetCount(const int64_t &objectProperty) const override; \
        virtual std::set<void *> _GetMapKeys(const int64_t &objectProperty) const override; \
        virtual bool _IsContainKey(const int64_t &objectProperty, const void *key) const override; \
        void _Remove(const int64_t &objectProperty, const void *value) override; \
        void _RemoveObject(const int64_t &objectProperty, const vcc::IObject *value) override; \
        virtual void _RemoveAtIndex(const int64_t &objectProperty, const int64_t &index) override; \
        virtual void _RemoveAtKey(const int64_t &objectProperty, const void *key) override; \
        virtual void _Clear(const int64_t &objectProperty) override;


// ------------------------------------------------------------------------------------------ //
//                                      DLL Interface                                         //
// ------------------------------------------------------------------------------------------ //

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(exportType, typeName) \
    DLLEXPORT exportType Read##typeName(void *ref, int64_t property); \
    DLLEXPORT exportType Read##typeName##AtIndex(void *ref, int64_t property, int64_t index); \
    DLLEXPORT exportType Read##typeName##AtKey(void *ref, int64_t property, void *key); \
    DLLEXPORT void Write##typeName(void *ref, int64_t property, exportType value); \
    DLLEXPORT void Write##typeName##AtIndex(void *ref, int64_t property, exportType value, int64_t index); \
    DLLEXPORT void Write##typeName##AtKey(void *ref, int64_t property, exportType value, void *key); \
    DLLEXPORT void Insert##typeName##AtIndex(void *ref, int64_t property, exportType value, int64_t index);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING \
    DLLEXPORT void ReadString(void *ref, int64_t property, wchar_t **value); \
    DLLEXPORT void ReadStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index); \
    DLLEXPORT void ReadStringAtKey(void *ref, int64_t property, wchar_t **value, void *key); \
    DLLEXPORT void WriteString(void *ref, int64_t property, wchar_t **value); \
    DLLEXPORT void WriteStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index); \
    DLLEXPORT void WriteStringAtKey(void *ref, int64_t property, wchar_t **value, void *key); \
    DLLEXPORT void InsertStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT \
    DLLEXPORT void *ReadObject(void *ref, int64_t property); \
    DLLEXPORT void *ReadObjectAtIndex(void *ref, int64_t property, int64_t index); \
    DLLEXPORT void *ReadObjectAtKey(void *ref, int64_t property, void *key); \
    DLLEXPORT void WriteObject(void *ref, int64_t property, void *value); \
    DLLEXPORT void WriteObjectAtIndex(void *ref, int64_t property, void *value, int64_t index); \
    DLLEXPORT void WriteObjectAtKey(void *ref, int64_t property, void *value, void *key); \
    DLLEXPORT void *AddObjectAtIndex(void *ref, int64_t property, int64_t objectType, int64_t index); \
    DLLEXPORT void InsertObjectAtIndex(void *ref, int64_t property, void *value, int64_t index);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER \
    DLLEXPORT long GetCount(void *ref, int64_t property); \
    DLLEXPORT void **GetMapKeys(void *ref, int64_t property); \
    DLLEXPORT bool IsContainKey(void *ref, int64_t property, void *key); \
    DLLEXPORT void Remove(void *ref, int64_t property, void *value); \
    DLLEXPORT void RemoveObject(void *ref, int64_t property, void *value); \
    DLLEXPORT void RemoveAtIndex(void *ref, int64_t property, int64_t index); \
    DLLEXPORT void RemoveAtKey(void *ref, int64_t property, void *key); \
    DLLEXPORT void Clear(void *ref, int64_t property);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(exportType, typeName, defaultValue) \
exportType Read##typeName(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::Create(object->SharedPtr())->Read##typeName(vcc::LockType::ReadLock, property); \
    CATCH \
    return defaultValue; \
} \
exportType Read##typeName##AtIndex(void *ref, int64_t property, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::Create(object->SharedPtr())->Read##typeName##AtIndex(vcc::LockType::ReadLock, property, index); \
    CATCH \
    return defaultValue; \
} \
exportType Read##typeName##AtKey(void *ref, int64_t property, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::Create(object->SharedPtr())->Read##typeName##AtKey(vcc::LockType::ReadLock, property, key); \
    CATCH \
    return defaultValue; \
} \
void Write##typeName(void *ref, int64_t property, exportType value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::Create(object->SharedPtr())->Write##typeName(vcc::LockType::WriteLock, property, value); \
    CATCH \
} \
void Write##typeName##AtIndex(void *ref, int64_t property, exportType value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::Create(object->SharedPtr())->Write##typeName##AtIndex(vcc::LockType::WriteLock, property, value, index); \
    CATCH \
} \
void Write##typeName##AtKey(void *ref, int64_t property, exportType value, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::Create(object->SharedPtr())->Write##typeName##AtKey(vcc::LockType::WriteLock, property, value, key); \
    CATCH \
} \
void Insert##typeName##AtIndex(void *ref, int64_t property, exportType value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::Create(object->SharedPtr())->Insert##typeName##AtIndex(vcc::LockType::WriteLock, property, value, index); \
    CATCH \
}

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING \
void ReadString(void *ref, int64_t property, wchar_t **value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::wstring result = PropertyAccessorFactory::Create(object->SharedPtr())->ReadString(vcc::LockType::ReadLock, property); \
        size_t size = (result.length() + 1) * sizeof(wchar_t); \
        *value = static_cast<wchar_t*>(malloc(size)); \
        wcscpy(*value, result.c_str()); \
    CATCH \
} \
void ReadStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::wstring result = PropertyAccessorFactory::Create(object->SharedPtr())->ReadStringAtIndex(vcc::LockType::ReadLock, property, index); \
        size_t size = (result.length() + 1) * sizeof(wchar_t); \
        *value = static_cast<wchar_t*>(malloc(size)); \
        wcscpy(*value, result.c_str()); \
    CATCH \
} \
void ReadStringAtKey(void *ref, int64_t property, wchar_t **value, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::wstring result = PropertyAccessorFactory::Create(object->SharedPtr())->ReadStringAtKey(vcc::LockType::ReadLock, property, key); \
        size_t size = (result.length() + 1) * sizeof(wchar_t); \
        *value = static_cast<wchar_t*>(malloc(size)); \
        wcscpy(*value, result.c_str()); \
    CATCH \
} \
void WriteString(void *ref, int64_t property, wchar_t **value) \
{ \
    TRY \
        if (value != nullptr && *value != nullptr) \
        { \
            vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
            auto ptr = object->SharedPtr();\
            PropertyAccessorFactory::Create(object->SharedPtr())->WriteString(vcc::LockType::WriteLock, property, *value); \
        } \
    CATCH \
} \
void WriteStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index) \
{ \
    TRY \
        if (value != nullptr && *value != nullptr) \
        { \
            vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
            auto ptr = object->SharedPtr();\
            PropertyAccessorFactory::Create(object->SharedPtr())->WriteStringAtIndex(vcc::LockType::WriteLock, property, *value, index); \
        } \
    CATCH \
} \
void WriteStringAtKey(void *ref, int64_t property, wchar_t **value, void *key) \
{ \
    TRY \
        if (value != nullptr && *value != nullptr) \
        { \
            vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
            PropertyAccessorFactory::Create(object->SharedPtr())->WriteStringAtKey(vcc::LockType::WriteLock, property, *value, key); \
        } \
    CATCH \
} \
void InsertStringAtIndex(void *ref, int64_t property, wchar_t **value, int64_t index) \
{ \
    TRY \
        if (value != nullptr && *value != nullptr) \
        { \
            vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
            PropertyAccessorFactory::Create(object->SharedPtr())->InsertStringAtIndex(vcc::LockType::WriteLock, property, *value, index); \
        } \
    CATCH \
}

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT \
DLLEXPORT void *ReadObject(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::shared_ptr<vcc::IObject> result = PropertyAccessorFactory::Create(object->SharedPtr())->ReadObject(vcc::LockType::ReadLock, property); \
        return result != nullptr ? result.get() : nullptr; \
    CATCH \
    return nullptr; \
} \
DLLEXPORT void *ReadObjectAtIndex(void *ref, int64_t property, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::shared_ptr<vcc::IObject> result = PropertyAccessorFactory::Create(object->SharedPtr())->ReadObjectAtIndex(vcc::LockType::ReadLock, property, index); \
        return result != nullptr ? result.get() : nullptr; \
    CATCH \
    return nullptr; \
} \
DLLEXPORT void *ReadObjectAtKey(void *ref, int64_t property, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::shared_ptr<vcc::IObject> result = PropertyAccessorFactory::Create(object->SharedPtr())->ReadObjectAtKey(vcc::LockType::ReadLock, property, key); \
        return result != nullptr ? result.get() : nullptr; \
    CATCH \
    return nullptr; \
} \
DLLEXPORT void WriteObject(void *ref, int64_t property, void *value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        vcc::IObject *result = static_cast<vcc::IObject *>(value); \
        std::shared_ptr<vcc::IObject> resultPtr(result); \
        PropertyAccessorFactory::Create(object->SharedPtr())->WriteObject(vcc::LockType::WriteLock, property, resultPtr); \
    CATCH \
} \
DLLEXPORT void WriteObjectAtIndex(void *ref, int64_t property, void *value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        vcc::IObject *result = static_cast<vcc::IObject *>(value); \
        std::shared_ptr<vcc::IObject> resultPtr(result); \
        PropertyAccessorFactory::Create(object->SharedPtr())->WriteObjectAtIndex(vcc::LockType::WriteLock, property, resultPtr, index); \
    CATCH \
} \
DLLEXPORT void WriteObjectAtKey(void *ref, int64_t property, void *value, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        vcc::IObject *result = static_cast<vcc::IObject *>(value); \
        std::shared_ptr<vcc::IObject> resultPtr(result); \
        PropertyAccessorFactory::Create(object->SharedPtr())->WriteObjectAtKey(vcc::LockType::WriteLock, property, resultPtr, key); \
    CATCH \
} \
DLLEXPORT void *AddObjectAtIndex(void *ref, int64_t property, int64_t objectType, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::shared_ptr<vcc::IObject> newObject = ObjectFactory::Create(static_cast<ObjectType>(objectType)); \
        PropertyAccessorFactory::Create(object->SharedPtr())->InsertObjectAtIndex(vcc::LockType::WriteLock, property, newObject, index); \
        return newObject.get(); \
    CATCH \
    return nullptr; \
} \
DLLEXPORT void InsertObjectAtIndex(void *ref, int64_t property, void *value, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        vcc::IObject *result = static_cast<vcc::IObject *>(value); \
        std::shared_ptr<vcc::IObject> resultPtr(result); \
        PropertyAccessorFactory::Create(object->SharedPtr())->InsertObjectAtIndex(vcc::LockType::WriteLock, property, resultPtr, index); \
    CATCH \
}

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER \
long GetCount(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::Create(object->SharedPtr())->GetCount(vcc::LockType::ReadLock, property); \
    CATCH \
    return 0; \
} \
void **GetMapKeys(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        std::set<void *> result = PropertyAccessorFactory::Create(object->SharedPtr())->GetMapKeys(vcc::LockType::ReadLock, property); \
        void **array = new void *[result.size() + 1]; \
        std::fill(array, array + (result.size() + 1), nullptr); \
        size_t index = 0; \
        for (const auto& item : result) \
            array[index++] = item; \
        return array; \
    CATCH \
    return nullptr; \
} \
bool IsContainKey(void *ref, int64_t property, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::Create(object->SharedPtr())->IsContainKey(vcc::LockType::ReadLock, property, key); \
    CATCH \
    return false; \
} \
void Remove(void *ref, int64_t property, void *value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::Create(object->SharedPtr())->Remove(vcc::LockType::WriteLock, property, value); \
    CATCH \
} \
void RemoveObject(void *ref, int64_t property, void *value) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::Create(object->SharedPtr())->RemoveObject(vcc::LockType::WriteLock, property, static_cast<vcc::IObject *>(value)); \
    CATCH \
} \
void RemoveAtIndex(void *ref, int64_t property, int64_t index) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::Create(object->SharedPtr())->RemoveAtIndex(vcc::LockType::WriteLock, property, index); \
    CATCH \
} \
void RemoveAtKey(void *ref, int64_t property, void *key) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        PropertyAccessorFactory::Create(object->SharedPtr())->RemoveAtKey(vcc::LockType::WriteLock, property, key); \
    CATCH \
} \
void Clear(void *ref, int64_t property) \
{ \
    TRY \
        vcc::IObject *object = static_cast<vcc::IObject *>(ref); \
        return PropertyAccessorFactory::Create(object->SharedPtr())->Clear(vcc::LockType::WriteLock, property); \
    CATCH \
}
