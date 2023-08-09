#include "vpg_file_converse_service.hpp"

#include <string>
#include <vector>

#include "file_helper.hpp"
#include "log_property.hpp"
#include "log_service.hpp"

using namespace std;
using namespace vcc;

void VPGFileConverseService::CheckAndCreateDirectory(LogProperty &logProperty, std::wstring workspace)
{
    // All type has same project structure
    vector<wstring> checkList;
    // bin
    checkList.push_back(L"bin");
    checkList.push_back(L"bin/Debug");
    checkList.push_back(L"bin/Release");

    // include
    checkList.push_back(L"include");

    // src
    checkList.push_back(L"src");

    // unittest
    checkList.push_back(L"unittest");
    
    for (auto path : checkList) {
        std::wstring absPath = ConcatPath(workspace, path);
        if (!IsDirectoryExists(absPath)) {
            CreateDirectory(absPath);
            LogService::LogInfo(logProperty, L"", L"Create Directory: " + path);
        }        
    }
}
