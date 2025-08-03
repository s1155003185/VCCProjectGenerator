#pragma once
#ifdef __WIN32

#include "file_helper.hpp"

namespace vcc
{
	std::wstring getSystemFolderPathWindow(const SystemFolderType &fileType);
};

#endif