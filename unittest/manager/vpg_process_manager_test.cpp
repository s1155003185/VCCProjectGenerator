#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "vpg_global.hpp"
#include "vpg_process_manager.hpp"

class VPGProcessManagerTest : public testing::Test 
{
    MANAGER_SPTR_NULL(VPGProcessManager, Manager);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGProcessManagerTest/");

    GETSET(std::wstring, TestFolder, L"../VPGProcessManagerTest_VCCTestProject");
    GETSET(bool, IsCopyDebugFolderToTestFolder, false);

    public:

        void SetUp() override
        {
            //auto logConfig = std::make_shared<vcc::LogConfig>(LogConfigType::None);
            //logConfig->setIsConsoleLog(true);
            //this->_Manager->setLogConfig(logConfig);
            _Manager = std::make_shared<VPGProcessManager>(nullptr);
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->getWorkspace()));
        }
};

TEST_F(VPGProcessManagerTest, Empty)
{
    std::vector<std::wstring> cmds;
    this->getManager()->execute(cmds);
}

TEST_F(VPGProcessManagerTest, Version)
{
    this->getManager()->execute({L"vpg", L"-Version"});
}

// TEST_F(VPGProcessManagerTest, AddUpdateGenerate)
// {
//     this->getManager()->execute({L"vpg", L"-Add", L"-interface", L"CPPCOMPLEX", L"-workspace-destination", this->getWorkspace()});

//     if (this->getIsCopyDebugFolderToTestFolder()) {
//         std::wstring path = ConcatPaths({this->getTestFolder(), L"VCCComplex"});
//         removeDirectory(path);
//         copyDirectory(this->getWorkspace(), path);
//     }
// }
