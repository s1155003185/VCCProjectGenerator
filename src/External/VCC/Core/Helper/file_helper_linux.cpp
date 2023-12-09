#ifndef __WIN32

#include "file_helper_linux.hpp"
#include "file_helper.hpp"

namespace vcc
{
	std::wstring GetSystemFolderPathLinux(SystemFolderType fileType)
    {
        LogProperty logProperty(LogPropertyType::None);
        return CommandService::Execute(logProperty, L"", L"echo ~/Documents");
        
    }
};

#endif