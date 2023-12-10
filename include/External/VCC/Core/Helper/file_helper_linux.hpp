#pragma once
#ifndef __WIN32

#include <string>

#include "file_helper.hpp"

using namespace std;

namespace vcc
{
	std::wstring GetSystemFolderPathLinux(SystemFolderType fileType);
};

#endif