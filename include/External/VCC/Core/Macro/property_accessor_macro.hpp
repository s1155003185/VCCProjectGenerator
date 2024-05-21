#pragma once
namespace vcc
{   
    #define THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND THROW_EXCEPTION_MSG(ExceptionType::KeyNotFound, L"Object Property Type " + std::to_wstring(objectProperty) + L" not found");
    
    #define I_PROPERTY_ACCESSOR_HEADER(type, name) \
        public: \
            virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0; \
            virtual void Write##name(const LockType &lockType, const size_t &objectProperty, const type &value, const int64_t &index = -1) const = 0; \
            virtual void Write##name(const LockType &lockType, const size_t &objectProperty, const type &value, const ITypeUnion *key) const = 0; \

    #define I_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
        public: \
            virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0; \
            virtual void Write##name(const LockType &lockType, const size_t &objectProperty, type value, const int64_t &index = -1) const = 0; \
            virtual void Write##name(const LockType &lockType, const size_t &objectProperty, type value, const ITypeUnion *key) const = 0; \
            virtual type Clone##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual type Clone##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0; \

    #define I_PROPERTY_ACCESSOR_CONTAINER_HEADER \
        public: \
            virtual size_t GetContainerCount(const LockType &lockType, const size_t &objectProperty) const = 0; \
            virtual bool IsContainKey(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;\
            virtual void RemoveContainerElement(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0; \
            virtual void RemoveContainerElement(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0; \
            virtual void ClearContainer(const LockType &lockType, const size_t &objectProperty) const = 0;

    #define BASE_PROPERTY_ACCESSOR_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const size_t &objectProperty, const int64_t &index = -1) const; \
            virtual type _Read##name(const size_t &objectProperty, const ITypeUnion *key) const; \
            virtual void _Write##name(const size_t &objectProperty, const type &value, const int64_t &index = -1) const; \
            virtual void _Write##name(const size_t &objectProperty, const type &value, const ITypeUnion *key) const; \
        public: \
            virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void Write##name(const LockType &lockType, const size_t &objectProperty, const type &value, const int64_t &index = -1) const override; \
            virtual void Write##name(const LockType &lockType, const size_t &objectProperty, const type &value, const ITypeUnion *key) const override; \

    #define BASE_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const size_t &objectProperty, const int64_t &index = -1) const; \
            virtual type _Read##name(const size_t &objectProperty, const ITypeUnion *key) const; \
            virtual void _Write##name(const size_t &objectProperty, type value, const int64_t &index = -1) const; \
            virtual void _Write##name(const size_t &objectProperty, type value, const ITypeUnion *key) const; \
            virtual type _Clone##name(const size_t &objectProperty, const int64_t &index = -1) const; \
            virtual type _Clone##name(const size_t &objectProperty, const ITypeUnion *key) const; \
        public: \
            virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void Write##name(const LockType &lockType, const size_t &objectProperty, type value, const int64_t &index = -1) const override; \
            virtual void Write##name(const LockType &lockType, const size_t &objectProperty, type value, const ITypeUnion *key) const override; \
            virtual type Clone##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type Clone##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override; \
    
    #define BASE_PROPERTY_ACCESSOR_CONTAINER_HEADER \
        protected: \
            virtual size_t _GetContainerCount(const size_t &objectProperty) const; \
            virtual bool _IsContainKey(const size_t &objectProperty, const ITypeUnion *key) const;\
            virtual void _RemoveContainerElement(const size_t &objectProperty, const int64_t &index = -1) const; \
            virtual void _RemoveContainerElement(const size_t &objectProperty, const ITypeUnion *key) const; \
            virtual void _ClearContainer(const size_t &objectProperty) const; \
        public: \
            virtual size_t GetContainerCount(const LockType &lockType, const size_t &objectProperty) const override; \
            virtual bool IsContainKey(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;\
            virtual void RemoveContainerElement(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override; \
            virtual void RemoveContainerElement(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void ClearContainer(const LockType &lockType, const size_t &objectProperty) const override;

    #define BASE_PROPERTY_ACCESSOR_DETAIL(type, name, defaultValue) \
        type BasePropertyAccessor::_Read##name(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::_Read##name(const size_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        void BasePropertyAccessor::_Write##name(const size_t &objectProperty, const type &/*value*/, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_Write##name(const size_t &objectProperty, const type &/*value*/, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, index); LOCK_END return result; } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, key); LOCK_END return result; } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const size_t &objectProperty, const type &value, const int64_t &index) const { LOCK_BEGIN _Write##name(objectProperty, value, index); LOCK_END } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const size_t &objectProperty, const type &value, const ITypeUnion *key) const { LOCK_BEGIN _Write##name(objectProperty, value, key); LOCK_END } \
    
    #define BASE_PROPERTY_ACCESSOR_OBJECT_DETAIL(type, name, defaultValue) \
        type BasePropertyAccessor::_Read##name(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::_Read##name(const size_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        void BasePropertyAccessor::_Write##name(const size_t &objectProperty, type /*value*/, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_Write##name(const size_t &objectProperty, type /*value*/, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        type BasePropertyAccessor::_Clone##name(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::_Clone##name(const size_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return defaultValue; } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, index); LOCK_END return result; } \
        type BasePropertyAccessor::Read##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { type result = defaultValue; LOCK_BEGIN result = _Read##name(objectProperty, key); LOCK_END return result; } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const size_t &objectProperty, type value, const int64_t &index) const { LOCK_BEGIN _Write##name(objectProperty, value, index); LOCK_END } \
        void BasePropertyAccessor::Write##name(const LockType &lockType, const size_t &objectProperty, type value, const ITypeUnion *key) const { LOCK_BEGIN _Write##name(objectProperty, value, key); LOCK_END } \
        type BasePropertyAccessor::Clone##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { type result = nullptr; LOCK_BEGIN result = _Clone##name(objectProperty, index); LOCK_END return result; } \
        type BasePropertyAccessor::Clone##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { type result = nullptr; LOCK_BEGIN result = _Clone##name(objectProperty, key); LOCK_END return result; } \

    #define BASE_PROPERTY_ACCESSOR_CONTAINER_DETAIL \
        size_t BasePropertyAccessor::_GetContainerCount(const size_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return 0;} \
        bool BasePropertyAccessor::_IsContainKey(const size_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND return false;} \
        void BasePropertyAccessor::_RemoveContainerElement(const size_t &objectProperty, const int64_t & /*index*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_RemoveContainerElement(const size_t &objectProperty, const ITypeUnion * /*key*/) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        void BasePropertyAccessor::_ClearContainer(const size_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND } \
        size_t BasePropertyAccessor::GetContainerCount(const LockType &lockType, const size_t &objectProperty) const { size_t result = 0; LOCK_BEGIN result = _GetContainerCount(objectProperty); LOCK_END return result; } \
        bool BasePropertyAccessor::IsContainKey(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { bool result = false; LOCK_BEGIN result = _IsContainKey(objectProperty, key); LOCK_END return result; }\
        void BasePropertyAccessor::RemoveContainerElement(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { LOCK_BEGIN _RemoveContainerElement(objectProperty, index); LOCK_END } \
        void BasePropertyAccessor::RemoveContainerElement(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { LOCK_BEGIN _RemoveContainerElement(objectProperty, key); LOCK_END } \
        void BasePropertyAccessor::ClearContainer(const LockType &lockType, const size_t &objectProperty) const { LOCK_BEGIN _ClearContainer(objectProperty); LOCK_END }

    #define PROPERTY_ACCESSOR_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const size_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type _Read##name(const size_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void _Write##name(const size_t &objectProperty, const type &value, const int64_t &index = -1) const override; \
            virtual void _Write##name(const size_t &objectProperty, const type &value, const ITypeUnion *key) const override; \

    #define PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
        protected: \
            virtual type _Read##name(const size_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type _Read##name(const size_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void _Write##name(const size_t &objectProperty, type value, const int64_t &index = -1) const override; \
            virtual void _Write##name(const size_t &objectProperty, type value, const ITypeUnion *key) const override; \
            virtual type _Clone##name(const size_t &objectProperty, const int64_t &index = -1) const override; \
            virtual type _Clone##name(const size_t &objectProperty, const ITypeUnion *key) const override; \
            
    #define PROPERTY_ACCESSOR_CONTAINER_HEADER \
        protected: \
            virtual size_t _GetContainerCount(const size_t &objectProperty) const override; \
            virtual bool _IsContainKey(const size_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void _RemoveContainerElement(const size_t &objectProperty, const int64_t &index) const override; \
            virtual void _RemoveContainerElement(const size_t &objectProperty, const ITypeUnion *key) const override; \
            virtual void _ClearContainer(const size_t &objectProperty) const override;
}