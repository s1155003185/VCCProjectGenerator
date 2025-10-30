#pragma once

// win only
// need to be sepearate as cannot use namespace std
#ifdef _WIN32
#include <tchar.h>

#include <string>

namespace vcc {
std::wstring ProcessServiceWin(const std::wstring& command);
}
#endif
