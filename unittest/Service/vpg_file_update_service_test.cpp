#include <gtest/gtest.h>

#include "log_property.hpp"
#include "vpg_file_update_service.hpp"
#include "vpg_file_update_type.hpp"

// command the test because require connection

// TEST(FileUpdateService, Download_ForceUpdate)
// {
//     LogProperty logProperty(LogPropertyType::None);

//     VPGFileUpdateService::DownloadVCCResource(logProperty, VPGDllType::VCC, L"", true);
//     VPGFileUpdateService::DownloadVCCResource(logProperty, VPGDllType::CPP, L"", true);
//     VPGFileUpdateService::DownloadVCCResource(logProperty, VPGInterfaceType::VCC, L"", true);
//     VPGFileUpdateService::DownloadVCCResource(logProperty, VPGInterfaceType::CPP, L"", true);
// }

// TEST(FileUpdateService, Download_ProjectExist)
// {
//     LogProperty logProperty(LogPropertyType::None);

//     VPGFileUpdateService::DownloadVCCResource(logProperty, VPGDllType::VCC, L"", false);
//     VPGFileUpdateService::DownloadVCCResource(logProperty, VPGDllType::CPP, L"", false);
//     VPGFileUpdateService::DownloadVCCResource(logProperty, VPGDllType::VCC, L"",  false);
//     VPGFileUpdateService::DownloadVCCResource(logProperty, VPGDllType::CPP, L"", false);
// }
