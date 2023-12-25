#include "vpg_process_manager.hpp"

#include <iostream>

#include "exception_macro.hpp"
#include "string_helper.hpp"
#include "vpg_global.hpp"

using namespace vcc;

void VPGProcessManager::Add(const std::vector<std::wstring> &cmds)
{
    try {
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

void VPGProcessManager::Update(const std::vector<std::wstring> &cmds)
{
    try {
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

void VPGProcessManager::Generate(const std::vector<std::wstring> &cmds)
{
    try {
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

void VPGProcessManager::Execute(const std::vector<std::wstring> &cmds)
{
    if (cmds.size() < 2)
        return;

    try {
        std::wstring mode = cmds[1];
        
        if (mode == L"-v")
            std::wcout << L"VCCProjectGenerator version " << VPGGlobal::GetVersion() << std::endl;
        else if (mode == L"-A")
            this->Add(cmds);
        else if (mode == L"-U")
            this->Update(cmds);
        else if (mode == L"-G")
            this->Generate(cmds);
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknow Mode " + mode);
        
        std::wcout << L"Process Complete Successfully!" << std::endl;
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}
