#pragma once
#include "i_proeprty_accessor_key.hpp"

#include <string>

#include "class_macro.hpp"
#include "i_object.hpp"

namespace vcc
{
    class BaseTypeUnion : public ITypeUnion
    {
        GETSET(bool, KeyBool, false);
        GETSET(char, KeyChar, '');
        GETSET(wchar_t, KeyWchar, L'');
        GETSET(short, KeyShort, 0);
        GETSET(int, KeyInt, 0);
        GETSET(long, KeyLong, 0);
        GETSET(float, KeyFloat, 0);
        GETSET(double, KeyDouble, 0);
        GETSET(std::string, KeyString, "");
        GETSET(std::wstring, KeyWstring, L"");
        GETSET(uintptr_t, KeyPointer, 0);
        GET_SPTR_NULL(IObject, KeySharedPtr);

        protected:
            BaseTypeUnion() {}
            ~BaseTypeUnion() {}
    };
}
