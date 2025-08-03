#pragma once
#ifndef __WIN32

#include <string>

#include "file_helper.hpp"

namespace vcc
{
	std::wstring getSystemFolderPathLinux(const SystemFolderType &fileType);
};

#endif