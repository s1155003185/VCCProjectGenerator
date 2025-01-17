#pragma once

namespace vcc
{   
    #define THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND THROW_EXCEPTION_MSG(ExceptionType::KeyNotFound, L"Object Property Type " + std::to_wstring(objectProperty) + L" not found");
    
    #define I_PROPERTY_ACCESSOR_HEADER(type, name) \
        public: \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const void *key) const = 0; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) const = 0; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const void *key) const = 0; \
            virtual void Insert##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) const = 0;

    #define I_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
        public: \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const void *key) const = 0; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) const = 0; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const void *key) const = 0; \
            virtual void Insert##name(const LockType &lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) const = 0; \
            virtual type Clone##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual type Clone##name(const LockType &lockType, const int64_t &objectProperty, const void *key) const = 0; \

    #define I_PROPERTY_ACCESSOR_CONTAINER_HEADER \
        public: \
            virtual size_t GetContainerCount(const LockType &lockType, const int64_t &objectProperty) const = 0; \
            virtual std::set<void *> GetMapKeys(const LockType &lockType, const int64_t &objectProperty) const = 0; \
            virtual bool IsContainKey(const LockType &lockType, const int64_t &objectProperty, const void *key) const = 0;\
            virtual void RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual void RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const void *key) const = 0; \
            virtual void ClearContainer(const LockType &lockType, const int64_t &objectProperty) const = 0;

    #define BASE_PROPERTY_ACCESSOR_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const int64_t &objectProperty, const int64_t &index = -1) const; \
            virtual type _Read##name(const int64_t &objectProperty, const void *key) const; \
            virtual void _Write##name(const int64_t &objectProperty, const type &value, const int64_t &index = -1) const; \
            virtual void _Write##name(const int64_t &objectProperty, const type &value, const void *key) const; \
            virtual void _Insert##name(const int64_t &objectProperty, const type &value, const int64_t &index = -1) const; \
        public: \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const void *key) const override; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) const override; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const void *key) const override; \
            virtual void Insert##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const int64_t &index = -1) const override;

    #define BASE_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const int64_t &objectProperty, const int64_t &index = -1) const; \
            virtual type _Read##name(const int64_t &objectProperty, const void *key) const; \
            virtual void _Write##name(const int64_t &objectProperty, type value, const int64_t &index = -1) const; \
            virtual void _Write##name(const int64_t &objectProperty, type value, const void *key) const; \
            virtual void _Insert##name(const int64_t &objectProperty, type value, const int64_t &index = -1) const; \
            virtual type _Clone##name(const int64_t &objectProperty, const int64_t &index = -1) const; \
            virtual type _Clone##name(const int64_t &objectProperty, const void *key) const; \
        public: \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type Read##name(const LockType &lockType, const int64_t &objectProperty, const void *key) const override; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) const override; \
            virtual void Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const void *key) const override; \
            virtual void Insert##name(const LockType &lockType, const int64_t &objectProperty, type value, const int64_t &index = -1) const override; \
            virtual type Clone##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type Clone##name(const LockType &lockType, const int64_t &objectProperty, const void *key) const override;
    
    #define BASE_PROPERTY_ACCESSOR_CONTAINER_HEADER \
        protected: \
            virtual size_t _GetContainerCount(const int64_t &objectProperty) const; \
            virtual std::set<void *> _GetMapKeys(const int64_t &objectProperty) const; \
            virtual bool _IsContainKey(const int64_t &objectProperty, const void *key) const;\
            virtual void _RemoveContainerElement(const int64_t &objectProperty, const int64_t &index = -1) const; \
            virtual void _RemoveContainerElement(const int64_t &objectProperty, const void *key) const; \
            virtual void _ClearContainer(const int64_t &objectProperty) const; \
        public: \
            virtual size_t GetContainerCount(const LockType &lockType, const int64_t &objectProperty) const override; \
            virtual std::set<void *> GetMapKeys(const LockType &lockType, const int64_t &objectProperty) const override; \
            virtual bool IsContainKey(const LockType &lockType, const int64_t &objectProperty, const void *key) const override;\
            virtual void RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual void RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const void *key) const override; \
            virtual void ClearContainer(const LockType &lockType, const int64_t &objectProperty) const override;

    #define BASE_PROPERTY_ACCESSOR_DETAIL(type, name, defaultValue) \
        type BasePropertyAccessor::_Read##name(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::_Read##name(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, const type &/*value*/, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, const type &/*value*/, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_Insert##name(const int64_t &objectProperty, const type &/*value*/, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, index); LOCK_END return result; } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const int64_t &objectProperty, const void *key) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, key); LOCK_END return result; } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const int64_t &index) const { LOCK_BEGIN _Write##name(objectProperty, value, index); LOCK_END } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const void *key) const { LOCK_BEGIN _Write##name(objectProperty, value, key); LOCK_END } \
        void BasePropertyAccessor::Insert##name(const LockType &lockType, const int64_t &objectProperty, const type &value, const int64_t &index) const { LOCK_BEGIN _Insert##name(objectProperty, value, index); LOCK_END }
    
    #define BASE_PROPERTY_ACCESSOR_OBJECT_DETAIL(type, name, defaultValue) \
        type BasePropertyAccessor::_Read##name(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::_Read##name(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, type /*value*/, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_Write##name(const int64_t &objectProperty, type /*value*/, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_Insert##name(const int64_t &objectProperty, type /*value*/, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        type BasePropertyAccessor::_Clone##name(const int64_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::_Clone##name(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, index); LOCK_END return result; } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const int64_t &objectProperty, const void *key) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, key); LOCK_END return result; } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const int64_t &index) const { LOCK_BEGIN _Write##name(objectProperty, value, index); LOCK_END } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const int64_t &objectProperty, type value, const void *key) const { LOCK_BEGIN _Write##name(objectProperty, value, key); LOCK_END } \
        void BasePropertyAccessor::Insert##name(const LockType &lockType, const int64_t &objectProperty, type value, const int64_t &index) const { LOCK_BEGIN _Insert##name(objectProperty, value, index); LOCK_END } \
        type BasePropertyAccessor::Clone##name(const LockType &lockType, const int64_t &objectProperty, const int64_t &index) const { type result = nullptr; LOCK_BEGIN result = _Clone##name(objectProperty, index); LOCK_END return result; } \
        type BasePropertyAccessor::Clone##name(const LockType &lockType, const int64_t &objectProperty, const void *key) const { type result = nullptr; LOCK_BEGIN result = _Clone##name(objectProperty, key); LOCK_END return result; } \

    #define BASE_PROPERTY_ACCESSOR_CONTAINER_DETAIL \
        size_t BasePropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return 0;} \
        std::set<void *> BasePropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND std::set<void *> result; return result;} \
        bool BasePropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return false;} \
        void BasePropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const void * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_ClearContainer(const int64_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        size_t BasePropertyAccessor::GetContainerCount(const LockType &lockType, const int64_t &objectProperty) const { size_t result = 0; LOCK_BEGIN result = _GetContainerCount(objectProperty); LOCK_END return result; } \
        std::set<void *> BasePropertyAccessor::GetMapKeys(const LockType &lockType, const int64_t &objectProperty) const { std::set<void *> result; LOCK_BEGIN result = _GetMapKeys(objectProperty); LOCK_END return result; } \
        bool BasePropertyAccessor::IsContainKey(const LockType &lockType, const int64_t &objectProperty, const void *key) const { bool result = false; LOCK_BEGIN result = _IsContainKey(objectProperty, key); LOCK_END return result; }\
        void BasePropertyAccessor::RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const int64_t &index) const { LOCK_BEGIN _RemoveContainerElement(objectProperty, index); LOCK_END } \
        void BasePropertyAccessor::RemoveContainerElement(const LockType &lockType, const int64_t &objectProperty, const void *key) const { LOCK_BEGIN _RemoveContainerElement(objectProperty, key); LOCK_END } \
        void BasePropertyAccessor::ClearContainer(const LockType &lockType, const int64_t &objectProperty) const { LOCK_BEGIN _ClearContainer(objectProperty); LOCK_END }

    #define PROPERTY_ACCESSOR_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type _Read##name(const int64_t &objectProperty, const void *key) const override; \
            virtual void _Write##name(const int64_t &objectProperty, const type &value, const int64_t &index = -1) const override; \
            virtual void _Write##name(const int64_t &objectProperty, const type &value, const void *key) const override; \
            virtual void _Insert##name(const int64_t &objectProperty, const type &value, const int64_t &index = -1) const override;

    #define PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type _Read##name(const int64_t &objectProperty, const void *key) const override; \
            virtual void _Write##name(const int64_t &objectProperty, type value, const int64_t &index = -1) const override; \
            virtual void _Write##name(const int64_t &objectProperty, type value, const void *key) const override; \
            virtual void _Insert##name(const int64_t &objectProperty, type value, const int64_t &index = -1) const override; \
            virtual type _Clone##name(const int64_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type _Clone##name(const int64_t &objectProperty, const void *key) const override; \
            
    #define PROPERTY_ACCESSOR_CONTAINER_HEADER \
        protected: \
            virtual size_t _GetContainerCount(const int64_t &objectProperty) const override; \
            virtual std::set<void *> _GetMapKeys(const int64_t &objectProperty) const override; \
            virtual bool _IsContainKey(const int64_t &objectProperty, const void *key) const override; \
            virtual void _RemoveContainerElement(const int64_t &objectProperty, const int64_t &index) const override; \
            virtual void _RemoveContainerElement(const int64_t &objectProperty, const void *key) const override; \
            virtual void _ClearContainer(const int64_t &objectProperty) const override;


    // ------------------------------------------------------------------------------------------ //
    //                                      DLL Interface                                         //
    // ------------------------------------------------------------------------------------------ //

    #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(exportType, typeName) \
        DLLEXPORT exportType Read##typeName(void *ref, int64_t property, int64_t index); \
        DLLEXPORT exportType Read##typeName##AtKey(void *ref, int64_t property, void *key); \
        DLLEXPORT void Write##typeName(void *ref, int64_t property, exportType value, int64_t index); \
        DLLEXPORT void Write##typeName##AtKey(void *ref, int64_t property, exportType value, void *key); \
        DLLEXPORT void Insert##typeName(void *ref, int64_t property, exportType value, int64_t index);

    #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING \
        DLLEXPORT void ReadString(void *ref, int64_t property, wchar_t **value, int64_t index); \
        DLLEXPORT void ReadStringAtKey(void *ref, int64_t property, wchar_t **value, void *key); \
        DLLEXPORT void WriteString(void *ref, int64_t property, wchar_t **value, int64_t index); \
        DLLEXPORT void WriteStringAtKey(void *ref, int64_t property, wchar_t **value, void *key); \
        DLLEXPORT void InsertString(void *ref, int64_t property, wchar_t **value, int64_t index);

    #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT \
        DLLEXPORT void *ReadObject(void *ref, int64_t property, int64_t index); \
        DLLEXPORT void *ReadObjectAtKey(void *ref, int64_t property, void *key); \
        DLLEXPORT void WriteObject(void *ref, int64_t property, void *value, int64_t index); \
        DLLEXPORT void WriteObjectAtKey(void *ref, int64_t property, void *value, void *key); \
        DLLEXPORT void *AddObject(void *ref, int64_t property, int64_t objectType, int64_t index); \
        DLLEXPORT void InsertObject(void *ref, int64_t property, void *value, int64_t index);

    #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER \
        DLLEXPORT long GetContainerCount(void *ref, int64_t property); \
        DLLEXPORT void **GetMapKeys(void *ref, int64_t property); \
        DLLEXPORT bool IsContainKey(void *ref, int64_t property, void *key); \
        DLLEXPORT void RemoveContainerElement(void *ref, int64_t property, int64_t index); \
        DLLEXPORT void RemoveContainerElementAtKey(void *ref, int64_t property, void *key); \
        DLLEXPORT void ClearContainer(void *ref, int64_t property);

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(exportType, typeName, defaultValue) \
    exportType Read##typeName(void *ref, int64_t property, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            return PropertyAccessorFactory::Create(object->GetSharedPtr())->Read##typeName(LockType::ReadLock, property, index); \
        CATCH \
        return defaultValue; \
    } \
    exportType Read##typeName##AtKey(void *ref, int64_t property, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            return PropertyAccessorFactory::Create(object->GetSharedPtr())->Read##typeName(LockType::ReadLock, property, key); \
        CATCH \
        return defaultValue; \
    } \
    void Write##typeName(void *ref, int64_t property, exportType value, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            PropertyAccessorFactory::Create(object->GetSharedPtr())->Write##typeName(LockType::WriteLock, property, value, index); \
        CATCH \
    } \
    void Write##typeName##AtKey(void *ref, int64_t property, exportType value, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            PropertyAccessorFactory::Create(object->GetSharedPtr())->Write##typeName(LockType::WriteLock, property, value, key); \
        CATCH \
    } \
    void Insert##typeName(void *ref, int64_t property, exportType value, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            PropertyAccessorFactory::Create(object->GetSharedPtr())->Insert##typeName(LockType::WriteLock, property, value, index); \
        CATCH \
    }

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING \
    void ReadString(void *ref, int64_t property, wchar_t **value, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::wstring result = PropertyAccessorFactory::Create(object->GetSharedPtr())->ReadString(LockType::ReadLock, property, index); \
            size_t size = (result.length() + 1) * sizeof(wchar_t); \
            *value = static_cast<wchar_t*>(malloc(size)); \
            wcscpy(*value, result.c_str()); \
        CATCH \
    } \
    void ReadStringAtKey(void *ref, int64_t property, wchar_t **value, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::wstring result = PropertyAccessorFactory::Create(object->GetSharedPtr())->ReadString(LockType::ReadLock, property, key); \
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
                auto ptr = object->GetSharedPtr();\
                PropertyAccessorFactory::Create(object->GetSharedPtr())->WriteString(LockType::WriteLock, property, *value, index); \
            } \
        CATCH \
    } \
    void WriteStringAtKey(void *ref, int64_t property, wchar_t **value, void *key) \
    { \
        TRY \
            if (value != nullptr && *value != nullptr) \
            { \
                IObject *object = static_cast<IObject *>(ref); \
                PropertyAccessorFactory::Create(object->GetSharedPtr())->WriteString(LockType::WriteLock, property, *value, key); \
            } \
        CATCH \
    } \
    void InsertString(void *ref, int64_t property, wchar_t **value, int64_t index) \
    { \
        TRY \
            if (value != nullptr && *value != nullptr) \
            { \
                IObject *object = static_cast<IObject *>(ref); \
                PropertyAccessorFactory::Create(object->GetSharedPtr())->InsertString(LockType::WriteLock, property, *value, index); \
            } \
        CATCH \
    }

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT \
    DLLEXPORT void *ReadObject(void *ref, int64_t property, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> result = PropertyAccessorFactory::Create(object->GetSharedPtr())->ReadObject(LockType::ReadLock, property, index); \
            return result != nullptr ? result.get() : nullptr; \
        CATCH \
        return nullptr; \
    } \
    DLLEXPORT void *ReadObjectAtKey(void *ref, int64_t property, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> result = PropertyAccessorFactory::Create(object->GetSharedPtr())->ReadObject(LockType::ReadLock, property, key); \
            return result != nullptr ? result.get() : nullptr; \
        CATCH \
        return nullptr; \
    } \
    DLLEXPORT void WriteObject(void *ref, int64_t property, void *value, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            IObject *result = static_cast<IObject *>(value); \
            std::shared_ptr<IObject> resultPtr(result); \
            PropertyAccessorFactory::Create(object->GetSharedPtr())->WriteObject(LockType::WriteLock, property, resultPtr, index); \
        CATCH \
    } \
    DLLEXPORT void WriteObjectAtKey(void *ref, int64_t property, void *value, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            IObject *result = static_cast<IObject *>(value); \
            std::shared_ptr<IObject> resultPtr(result); \
            PropertyAccessorFactory::Create(object->GetSharedPtr())->WriteObject(LockType::WriteLock, property, resultPtr, key); \
        CATCH \
    } \
    DLLEXPORT void *AddObject(void *ref, int64_t property, int64_t objectType, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::shared_ptr<IObject> newObject = ObjectFactory::Create(static_cast<ObjectType>(objectType)); \
            PropertyAccessorFactory::Create(object->GetSharedPtr())->InsertObject(LockType::WriteLock, property, newObject, index); \
            return newObject.get(); \
        CATCH \
        return nullptr; \
    } \
    DLLEXPORT void InsertObject(void *ref, int64_t property, void *value, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            IObject *result = static_cast<IObject *>(value); \
            std::shared_ptr<IObject> resultPtr(result); \
            PropertyAccessorFactory::Create(object->GetSharedPtr())->InsertObject(LockType::WriteLock, property, resultPtr, index); \
        CATCH \
    }

#define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER \
    long GetContainerCount(void *ref, int64_t property) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            return PropertyAccessorFactory::Create(object->GetSharedPtr())->GetContainerCount(LockType::ReadLock, property); \
        CATCH \
        return 0; \
    } \
    void **GetMapKeys(void *ref, int64_t property) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            std::set<void *> result = PropertyAccessorFactory::Create(object->GetSharedPtr())->GetMapKeys(LockType::ReadLock, property); \
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
            IObject *object = static_cast<IObject *>(ref); \
            return PropertyAccessorFactory::Create(object->GetSharedPtr())->IsContainKey(LockType::ReadLock, property, key); \
        CATCH \
        return false; \
    } \
    void RemoveContainerElement(void *ref, int64_t property, int64_t index) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            PropertyAccessorFactory::Create(object->GetSharedPtr())->RemoveContainerElement(LockType::WriteLock, property, index); \
        CATCH \
    } \
    void RemoveContainerElementAtKey(void *ref, int64_t property, void *key) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            PropertyAccessorFactory::Create(object->GetSharedPtr())->RemoveContainerElement(LockType::WriteLock, property, key); \
        CATCH \
    } \
    void ClearContainer(void *ref, int64_t property) \
    { \
        TRY \
            IObject *object = static_cast<IObject *>(ref); \
            return PropertyAccessorFactory::Create(object->GetSharedPtr())->ClearContainer(LockType::WriteLock, property); \
        CATCH \
    }

}