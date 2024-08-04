#pragma once

namespace vcc
{   
    #define THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND THROW_EXCEPTION_MSG(ExceptionType::KeyNotFound, L"Object Property Type " + std::to_wstring(objectProperty) + L" not found");
    
    #define I_PROPERTY_ACCESSOR_HEADER(type, name) \
        public: \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const = 0; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) const = 0; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const ITypeUnion *key) const = 0; \

    #define I_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
        public: \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const = 0; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) const = 0; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const ITypeUnion *key) const = 0; \
            virtual type Clone##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual type Clone##name(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const = 0; \

    #define I_PROPERTY_ACCESSOR_CONTAINER_HEADER \
        public: \
            virtual size_t GetContainerCount(const LockType &lockType, const int64_t &objectProperty) const = 0; \
            virtual bool IsContainKey(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const = 0;\
            virtual void RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual void RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const = 0; \
            virtual void ClearContainer(const LockType &lockType, const int64_t &objectProperty) const = 0;

    #define BASE_PROPERTY_ACCESSOR_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const int64_t &objectProperty, const int64_t &index = -1) const; \
            virtual type _Read##name(const int64_t &objectProperty, const ITypeUnion *key) const; \
            virtual void _Write##name(const int64_t &objectProperty, const type &value, const int64_t &index = -1) const; \
            virtual void _Write##name(const int64_t &objectProperty, const type &value, const ITypeUnion *key) const; \
        public: \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) const override; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const ITypeUnion *key) const override; \

    #define BASE_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const int64_t &objectProperty, const int64_t &index = -1) const; \
            virtual type _Read##name(const int64_t &objectProperty, const ITypeUnion *key) const; \
            virtual void _Write##name(const int64_t &objectProperty, type value, const int64_t &index = -1) const; \
            virtual void _Write##name(const int64_t &objectProperty, type value, const ITypeUnion *key) const; \
            virtual type _Clone##name(const int64_t &objectProperty, const int64_t &index = -1) const; \
            virtual type _Clone##name(const int64_t &objectProperty, const ITypeUnion *key) const; \
        public: \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) const override; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const ITypeUnion *key) const override; \
            virtual type Clone##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type Clone##name(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const override; \
    
    #define BASE_PROPERTY_ACCESSOR_CONTAINER_HEADER \
        protected: \
            virtual size_t _GetContainerCount(const int64_t &objectProperty) const; \
            virtual bool _IsContainKey(const int64_t &objectProperty, const ITypeUnion *key) const;\
            virtual void _RemoveContainerElement(const int64_t &objectProperty, const int64_t &index = -1) const; \
            virtual void _RemoveContainerElement(const int64_t &objectProperty, const ITypeUnion *key) const; \
            virtual void _ClearContainer(const int64_t &objectProperty) const; \
        public: \
            virtual size_t GetContainerCount(const LockType &lockType, const int64_t &objectProperty) const override; \
            virtual bool IsContainKey(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const override;\
            virtual void RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual void RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void ClearContainer(const LockType &lockType, const int64_t &objectProperty) const override;

    #define BASE_PROPERTY_ACCESSOR_DETAIL(type, name, defaultValue) \
        type BasePropertyAccessor::_Read##name(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::_Read##name(const int64_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, const type &/*value*/, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, const type &/*value*/, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, index); LOCK_END return result; } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, key); LOCK_END return result; } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const int64_t &index) const { LOCK_BEGIN _Write##name(objectProperty, value, index); LOCK_END } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const ITypeUnion *key) const { LOCK_BEGIN _Write##name(objectProperty, value, key); LOCK_END } \
    
    #define BASE_PROPERTY_ACCESSOR_OBJECT_DETAIL(type, name, defaultValue) \
        type BasePropertyAccessor::_Read##name(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::_Read##name(const int64_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, type /*value*/, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, type /*value*/, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        type BasePropertyAccessor::_Clone##name(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::_Clone##name(const int64_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, index); LOCK_END return result; } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, key); LOCK_END return result; } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const int64_t &index) const { LOCK_BEGIN _Write##name(objectProperty, value, index); LOCK_END } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const ITypeUnion *key) const { LOCK_BEGIN _Write##name(objectProperty, value, key); LOCK_END } \
        type BasePropertyAccessor::Clone##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index) const { type result = nullptr; LOCK_BEGIN result = _Clone##name(objectProperty, index); LOCK_END return result; } \
        type BasePropertyAccessor::Clone##name(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const { type result = nullptr; LOCK_BEGIN result = _Clone##name(objectProperty, key); LOCK_END return result; } \

    #define BASE_PROPERTY_ACCESSOR_CONTAINER_DETAIL \
        size_t BasePropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return 0;} \
        bool BasePropertyAccessor::_IsContainKey(const int64_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return false;} \
        void BasePropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_ClearContainer(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        size_t BasePropertyAccessor::GetContainerCount(const LockType &lockType, const int64_t &objectProperty) const { size_t result = 0; LOCK_BEGIN result = _GetContainerCount(objectProperty); LOCK_END return result; } \
        bool BasePropertyAccessor::IsContainKey(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const { bool result = false; LOCK_BEGIN result = _IsContainKey(objectProperty, key); LOCK_END return result; }\
        void BasePropertyAccessor::RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const int64_t &index) const { LOCK_BEGIN _RemoveContainerElement(objectProperty, index); LOCK_END } \
        void BasePropertyAccessor::RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const ITypeUnion *key) const { LOCK_BEGIN _RemoveContainerElement(objectProperty, key); LOCK_END } \
        void BasePropertyAccessor::ClearContainer(const LockType &lockType, const int64_t &objectProperty) const { LOCK_BEGIN _ClearContainer(objectProperty); LOCK_END }

    #define PROPERTY_ACCESSOR_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type _Read##name(const int64_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void _Write##name(const int64_t &objectProperty, const type &value, const int64_t &index = -1) const override; \
            virtual void _Write##name(const int64_t &objectProperty, const type &value, const ITypeUnion *key) const override; \

    #define PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type _Read##name(const int64_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void _Write##name(const int64_t &objectProperty, type value, const int64_t &index = -1) const override; \
            virtual void _Write##name(const int64_t &objectProperty, type value, const ITypeUnion *key) const override; \
            virtual type _Clone##name(const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type _Clone##name(const int64_t &objectProperty, const ITypeUnion *key) const override; \
            
    #define PROPERTY_ACCESSOR_CONTAINER_HEADER \
        protected: \
            virtual size_t _GetContainerCount(const int64_t &objectProperty) const override; \
            virtual bool _IsContainKey(const int64_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void _RemoveContainerElement(const int64_t &objectProperty, const int64_t &index) const override; \
            virtual void _RemoveContainerElement(const int64_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void _ClearContainer(const int64_t &objectProperty) const override;


    // ------------------------------------------------------------------------------------------ //
    //                                      DLL Interface                                         //
    // ------------------------------------------------------------------------------------------ //

    #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(exportType, typeName) \
        DLLEXPORT exportType Read##typeName(void *ref, int64_t property, int64_t index); \
        DLLEXPORT exportType Read##typeName##ByKey(void *ref, int64_t property, void *key); \
        DLLEXPORT void Write##typeName(void *ref, int64_t property, exportType value, int64_t index); \
        DLLEXPORT void Write##typeName##ByKey(void *ref, int64_t property, exportType value, void *key);

    #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING \
        DLLEXPORT void ReadString(void *ref, int64_t property, wchar_t **value, int64_t index); \
        DLLEXPORT void ReadStringByKey(void *ref, int64_t property, wchar_t **value, void *key); \
        DLLEXPORT void WriteString(void *ref, int64_t property, wchar_t **value, int64_t index); \
        DLLEXPORT void WriteStringByKey(void *ref, int64_t property, wchar_t **value, void *key);

    #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT \
        DLLEXPORT void *ReadObject(void *ref, int64_t property, int64_t index); \
        DLLEXPORT void *ReadObjectByKey(void *ref, int64_t property, void *key); \
        DLLEXPORT void WriteObject(void *ref, int64_t property, void *value, int64_t index); \
        DLLEXPORT void WriteObjectByKey(void *ref, int64_t property, void *value, void *key);

    #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER \
        DLLEXPORT long GetContainerCount(void *ref, int64_t property); \
        DLLEXPORT bool IsContainKey(void *ref, int64_t property, void *key); \
        DLLEXPORT void RemoveContainerElement(void *ref, int64_t property, int64_t index); \
        DLLEXPORT void RemoveContainerElementByKey(void *ref, int64_t property, void *key); \
        DLLEXPORT void ClearContainer(void *ref, int64_t property);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(exportType, typeName, defaultValue) \
    exportType Read##typeName(void *ref, int64_t property, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            return PropertyAccessorFactory::Create(ptr)->Read##typeName(LockType::ReadLock, property, index); \
        CATCH \
        return defaultValue; \
    } \
    exportType Read##typeName(void *ref, int64_t property, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            ITypeUnion *typeUnion = static_cast<ITypeUnion *>(key); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            return PropertyAccessorFactory::Create(ptr)->Read##typeName(LockType::ReadLock, property, typeUnion); \
        CATCH \
        return defaultValue; \
    } \
    void Write##typeName(void *ref, int64_t property, exportType value, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            PropertyAccessorFactory::Create(ptr)->Write##typeName(LockType::WriteLock, property, value, index); \
        CATCH \
    } \
    void Write##typeName(void *ref, int64_t property, exportType value, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            ITypeUnion *typeUnion = static_cast<ITypeUnion *>(key); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            PropertyAccessorFactory::Create(ptr)->Write##typeName(LockType::WriteLock, property, value, typeUnion); \
        CATCH \
    }

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING \
    void ReadString(void *ref, int64_t property, wchar_t **value, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            std::wstring result = PropertyAccessorFactory::Create(ptr)->ReadString(LockType::ReadLock, property, index); \
            size_t size = (result.length() + 1) * sizeof(wchar_t); \
            *value = static_cast<wchar_t*>(malloc(size)); \
            wcscpy(*value, result.c_str()); \
        CATCH \
    } \
    void ReadString(void *ref, int64_t property, wchar_t **value, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            ITypeUnion *typeUnion = static_cast<ITypeUnion *>(key); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            std::wstring result = PropertyAccessorFactory::Create(ptr)->ReadString(LockType::ReadLock, property, typeUnion); \
            size_t size = (result.length() + 1) * sizeof(wchar_t); \
            *value = static_cast<wchar_t*>(malloc(size)); \
            wcscpy(*value, result.c_str()); \
        CATCH \
    } \
    void WriteString(void *ref, int64_t property, wchar_t **value, int64_t index) \
    { \
        TRY \
            if (value != nullptr && *value != nullptr) \
            { \
                IObject *object = static_cast<IObject *>(ref); \
                std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
                PropertyAccessorFactory::Create(ptr)->WriteString(LockType::WriteLock, property, *value, index); \
            } \
        CATCH \
    } \
    void WriteString(void *ref, int64_t property, wchar_t **value, void *key) \
    { \
        TRY \
            if (value != nullptr && *value != nullptr) \
            { \
                IObject *object = static_cast<IObject *>(ref); \
                ITypeUnion *typeUnion = static_cast<ITypeUnion *>(key); \
                std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
                PropertyAccessorFactory::Create(ptr)->WriteString(LockType::WriteLock, property, *value, typeUnion); \
                free(*value); \
                *value = nullptr; \
            } \
        CATCH \
    }

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT \
    DLLEXPORT void *ReadObject(void *ref, int64_t property, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            std::shared_ptr<IObject> result = PropertyAccessorFactory::Create(ptr)->ReadObject(LockType::ReadLock, property, index); \
            return result != nullptr ? result.get() : nullptr; \
        CATCH \
        return nullptr; \
    } \
    DLLEXPORT void *ReadObjectByKey(void *ref, int64_t property, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            ITypeUnion *typeUnion = static_cast<ITypeUnion *>(key); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            std::shared_ptr<IObject> result = PropertyAccessorFactory::Create(ptr)->ReadObject(LockType::ReadLock, property, typeUnion); \
            return result != nullptr ? result.get() : nullptr; \
        CATCH \
        return nullptr; \
    } \
    DLLEXPORT void WriteObject(void *ref, int64_t property, void *value, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            IObject *result = static_cast<IObject *>(value); \
            std::shared_ptr<IObject> resultPtr(result); \
            PropertyAccessorFactory::Create(ptr)->WriteObject(LockType::WriteLock, property, resultPtr, index); \
        CATCH \
    } \
    DLLEXPORT void WriteObjectByKey(void *ref, int64_t property, void *value, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            ITypeUnion *typeUnion = static_cast<ITypeUnion *>(key); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            IObject *result = static_cast<IObject *>(value); \
            std::shared_ptr<IObject> resultPtr(result); \
            PropertyAccessorFactory::Create(ptr)->WriteObject(LockType::WriteLock, property, resultPtr, typeUnion); \
        CATCH \
    }

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER \
    long GetContainerCount(void *ref, int64_t property) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            return PropertyAccessorFactory::Create(ptr)->GetContainerCount(LockType::ReadLock, property); \
        CATCH \
        return 0; \
    } \
    bool IsContainKey(void *ref, int64_t property, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            ITypeUnion *typeUnion = static_cast<ITypeUnion *>(key); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            return PropertyAccessorFactory::Create(ptr)->IsContainKey(LockType::ReadLock, property, typeUnion); \
        CATCH \
        return false; \
    } \
    void RemoveContainerElement(void *ref, int64_t property, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            PropertyAccessorFactory::Create(ptr)->RemoveContainerElement(LockType::WriteLock, property, index); \
        CATCH \
    } \
    void RemoveContainerElementByKey(void *ref, int64_t property, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            ITypeUnion *typeUnion = static_cast<ITypeUnion *>(key); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            PropertyAccessorFactory::Create(ptr)->RemoveContainerElement(LockType::WriteLock, property, typeUnion); \
        CATCH \
    } \
    void ClearContainer(void *ref, int64_t property) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> ptr(object, [](IObject*){}); \
            return PropertyAccessorFactory::Create(ptr)->ClearContainer(LockType::WriteLock, property); \
        CATCH \
    }

}