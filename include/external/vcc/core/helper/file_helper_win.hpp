#pragma once
#ifdef __WIN32

#include "file_helper.hpp"

namespace vcc
{
	std::wstring GetSystemFolderPathWindow(const SystemFolderType &fileType);
};

#endif