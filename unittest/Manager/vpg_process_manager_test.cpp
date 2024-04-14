#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "class_macro.hpp"
#include "memory_macro.hpp"
#include "log_property.hpp"
#include "vpg_global.hpp"
#include "vpg_process_manager.hpp"

using namespace vcc;

class VPGProcessManagerTest : public testing::Test 
{
    MANAGER(VPGProcessManager, Manager, nullptr);
    GET(std::wstring, Workspace, L"bin/Debug/VPGProcessManagerTest/");

    GET(std::wstring, TestFolder, L"../VPGProcessManagerTest_VCCTestProject");
    GET(bool, IsCopyDebugFolderToTestFolder, false);

    public:

        void SetUp() override
        {
            //DECLARE_SPTR(LogProperty, logProperty, LogPropertyType::None);
            //logProperty->SetIsConsoleLog(true);
            //this->_Manager->SetLogProperty(logProperty);
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
