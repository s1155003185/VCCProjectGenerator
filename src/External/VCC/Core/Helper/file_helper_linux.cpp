#ifndef __WIN32

#include "file_helper_linux.hpp"

#include "command_service.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"

namespace vcc
{
	std::wstring GetSystemFolderPathLinux(SystemFolderType fileType)
    {
        LogProperty logProperty(LogPropertyType::None);
        return CommandService::Execute(logProperty, L"", L"echo ~/Documents");
        
    }
};

#endif