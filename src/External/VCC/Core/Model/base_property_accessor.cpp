#include "base_property_accessor.hpp"

#include <shared_mutex>
#include <string>

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "property_accessor_macro.hpp"
#include "lock_type.hpp"

namespace vcc
{

    #define LOCK_BEGIN switch(lockType) \
        {\
            case LockType::NoLock:\
                break;\
            case LockType::ReadLock:\
                ReadLock();\
                break;\
            case LockType::WriteLock:\
            case LockType::ReadWriteLock:\
                WriteLock();\
                break;\
            default:\
                assert(false);\
        }\
        TRY

    #define LOCK_END Unlock(); CATCH

    void BasePropertyAccessor::ReadLock() const 
    { 
        _Mutex.lock_shared();
    }

    void BasePropertyAccessor::WriteLock() const
    {
        _Mutex.lock();
    }

    void BasePropertyAccessor::ReadWriteLock() const 
    {
        WriteLock();
    }

    void BasePropertyAccessor::Unlock() const
    {
        _Mutex.unlock();
    }

    BASE_PROPERTY_ACCESSOR_DETAIL(bool, Bool, false)
    BASE_PROPERTY_ACCESSOR_DETAIL(char, Char, '\0')
    BASE_PROPERTY_ACCESSOR_DETAIL(wchar_t, Wchar, L'\0')
    BASE_PROPERTY_ACCESSOR_DETAIL(int8_t, Int8, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(uint8_t, Uint8, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(short, Short, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(uint16_t, UnsignedShort, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(int, Int, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(uint32_t, UnsignedInt, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(long, Long, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(size_t, UnsignedLong, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(float, Float, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(double, Double, 0)
    BASE_PROPERTY_ACCESSOR_DETAIL(std::string, String, "")
    BASE_PROPERTY_ACCESSOR_DETAIL(std::wstring, Wstring, L"")
    BASE_PROPERTY_ACCESSOR_OBJECT_DETAIL(std::shared_ptr<IObject>, Object, nullptr)
    
    BASE_PROPERTY_ACCESSOR_CONTAINER_DETAIL
}
