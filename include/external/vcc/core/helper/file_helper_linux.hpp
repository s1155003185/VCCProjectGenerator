#pragma once
#ifndef __WIN32

#include <string>

#include "file_helper.hpp"

namespace vcc
{
	std::wstring GetSystemFolderPathLinux(const SystemFolderType &fileType);
};

#endif