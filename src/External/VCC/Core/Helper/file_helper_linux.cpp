#ifndef __WIN32

#include "file_helper_linux.hpp"

#include "terminal_service.hpp"
#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"

namespace vcc
{
	std::wstring GetSystemFolderPathLinux(SystemFolderType fileType)
    {
        std::wstring result = L"";
        try {
            std::wstring folderName = L"";
			switch (fileType)
			{
			// case SystemFolderType::AdminTools:
			// 	csidl = CSIDL_ADMINTOOLS;
			// 	break;
			// case SystemFolderType::AppData:
			// 	csidl = CSIDL_APPDATA;
			// 	break;
			// case SystemFolderType::CommonAdminTools:
			// 	csidl = CSIDL_COMMON_ADMINTOOLS;
			// 	break;
			// case SystemFolderType::CommonAppdata:
			// 	csidl = CSIDL_COMMON_APPDATA;
			// 	break;
			// case SystemFolderType::CommonDocuments:
			// 	csidl = CSIDL_COMMON_DOCUMENTS;
			// 	break;
			// case SystemFolderType::Cookies:
			// 	csidl = CSIDL_COOKIES;
			// 	break;
			// case SystemFolderType::FlagCreate:
			// 	csidl = CSIDL_FLAG_CREATE;
			// 	break;
			// case SystemFolderType::FlagDontVerify:
			// 	csidl = CSIDL_FLAG_DONT_VERIFY;
			// 	break;
			// case SystemFolderType::History:
			// 	csidl = CSIDL_HISTORY;
			// 	break;
			// case SystemFolderType::InternetCache:
			// 	csidl = CSIDL_INTERNET_CACHE;
			// 	break;
			// case SystemFolderType::LocalAppData:
			// 	csidl = CSIDL_LOCAL_APPDATA;
			// 	break;
			// case SystemFolderType::LocalPictures:
			// 	csidl = CSIDL_MYPICTURES;
			// 	break;
			case SystemFolderType::LocalDocuments:
				folderName = L"Documents";
				break;
			// case SystemFolderType::ProgramFiles:
			// 	csidl = CSIDL_PROGRAM_FILES;
			// 	break;
			// case SystemFolderType::ProgramFilesCommon:
			// 	csidl = CSIDL_PROGRAM_FILES_COMMON;
			// 	break;
			// case SystemFolderType::System:
			// 	csidl = CSIDL_SYSTEM;
			// 	break;
			// case SystemFolderType::Windows:
			// 	csidl = CSIDL_WINDOWS;
			// 	break;
			default:
				assert(false);
				break;
			}
            result = TerminalService::Execute(nullptr, L"", L"echo ~/" + folderName);
        } catch (const std::exception &e) {
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, str2wstr(e.what()));
        }
        return result;
    }
};

#endif