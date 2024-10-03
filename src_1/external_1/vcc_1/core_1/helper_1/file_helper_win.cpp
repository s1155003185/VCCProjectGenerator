#ifdef __WIN32

#include "file_helper_win.hpp"

#include <windows.h>
#include <shlobj.h>
#include <stdexcept>

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "string_helper.hpp"

namespace vcc
{
	std::wstring GetSystemFolderPathWindow(SystemFolderType fileType)
    {
        std::wstring result = L"";
        try {
			int csidl = 0;
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
				csidl = CSIDL_PERSONAL;
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
			case SystemFolderType::UserHome:
				csidl = CSIDL_PROFILE;
				break;
			// case SystemFolderType::Windows:
			// 	csidl = CSIDL_WINDOWS;
			// 	break;
			default:
				return 0;
				break;
			}

			CHAR path[MAX_PATH];
			HRESULT hResult = SHGetFolderPath(NULL, csidl, NULL, SHGFP_TYPE_CURRENT, path);

			if (hResult != S_OK)
                throw std::runtime_error("GetSystemFolderPathWindow: " + std::to_string(GetLastError()));
			else
                result = str2wstr(path);
        } catch (const std::exception &e) {
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, str2wstr(e.what()));
        }
        return result;
    }
};

#endif