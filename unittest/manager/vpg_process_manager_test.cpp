#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "vpg_global.hpp"
#include "vpg_process_manager.hpp"

using namespace vcc;

class VPGProcessManagerTest : public testing::Test 
{
    MANAGER_SPTR_NULL(VPGProcessManager, Manager);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGProcessManagerTest/");

    GETSET(std::wstring, TestFolder, L"../VPGProcessManagerTest_VCCTestProject");
    GETSET(bool, IsCopyDebugFolderToTestFolder, false);

    public:

        void SetUp() override
        {
            //auto logConfig = std::make_shared<LogConfig>(LogConfigType::None);
            //logConfig->SetIsConsoleLog(true);
            //this->_Manager->SetLogConfig(logConfig);
            _Manager = std::make_shared<VPGProcessManager>(nullptr);
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGProcessManagerTest, Empty)
{
    std::vector<std::wstring> cmds;
    this->GetManager()->Execute(cmds);
}

TEST_F(VPGProcessManagerTest, Version)
{
    this->GetManager()->Execute({L"vpg", L"-Version"});
}

// TEST_F(VPGProcessManagerTest, AddUpdateGenerate)
// {
//     this->GetManager()->Execute({L"vpg", L"-Add", L"-interface", L"CPPCOMPLEX", L"-workspace-destination", this->GetWorkspace()});

//     if (this->GetIsCopyDebugFolderToTestFolder()) {
//         std::wstring path = ConcatPaths({this->GetTestFolder(), L"VCCComplex"});
//         RemoveDirectory(path);
//         CopyDirectory(this->GetWorkspace(), path);
//     }
// }
