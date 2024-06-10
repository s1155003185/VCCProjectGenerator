#include "DllFunctions.h"

#include <locale.h>
#include <stdio.h>
#include <wchar.h>

int GetVersion(wchar_t **str)
{
    std::wstring versionString = L"v0.0.1";
    size_t size = (versionString.length() + 1) * sizeof(wchar_t);
    *str = static_cast<wchar_t*>(malloc(size));
    if (*str == nullptr) {
        return -1;
    }
    wcscpy(*str, versionString.c_str());
    return 0;
}
