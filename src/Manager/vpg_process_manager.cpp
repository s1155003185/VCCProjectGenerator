#include "vpg_process_manager.hpp"

#include <iostream>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "git_service.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"
#include "vpg_global.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

void VPGProcessManager::InitLogProperty()
{
    this->GetLogProperty().get()->SetIsConsoleLog(true);
}

void VPGProcessManager::VerifyLocalResponse()
{
    try {
        // 1. Check if source file exists, if not exist then clone
        // 2. Check if version != branch, then checkout
        std::wstring localResponseDirectory = VPGGlobal::GetVccProjectLocalResponseDirectory(this->GetVPGProjectType());
        std::wstring gitUrl = VPGGlobal::GetProjecURL(this->GetVPGProjectType());

        LogService::LogInfo(this->GetLogProperty().get(), L"", L"Check VCC Local response existance: " + localResponseDirectory);
        if (IsDirectoryExists(localResponseDirectory)) {
            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Done.");
            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Check Version.");
            // check tag version
            // if same as current version of generator, no action
            // if not same, then check verison of genertor exists, if not exists, then master, else switch to correct branch
            DECLARE_SPTR(GitLog, currentLog);
            GitService::GetCurrentLog(this->GetLogProperty().get(), VPGGlobal::GetVccLocalResponseFolder(), currentLog);
            if (!Contains(currentLog->GetTags(), VPGGlobal::GetVersion())) {
                try
                {
                    GitService::SwitchTag(this->GetLogProperty().get(), VPGGlobal::GetVccLocalResponseFolder(), VPGGlobal::GetVersion());
                    LogService::LogInfo(this->GetLogProperty().get(), L"", L"Done.");
                }
                catch(const std::exception& e)
                {
                    LogService::LogError(this->GetLogProperty().get(), L"", L"VCC Project Generator version Not Exists. Switch to main.");
                    GitService::SwitchTag(this->GetLogProperty().get(), VPGGlobal::GetVccLocalResponseFolder(), L"main");
                    GitService::Pull(this->GetLogProperty().get(), VPGGlobal::GetVccLocalResponseFolder());
                    LogService::LogInfo(this->GetLogProperty().get(), L"", L"Done.");
                }
            }
        } else {
            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Not Exists.");
            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Clone from " + gitUrl);
            GitService::Clone(this->GetLogProperty().get(),  VPGGlobal::GetVccLocalResponseFolder(), gitUrl, this->GetTag(), this->GetLogDepth());
            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Done.");
        }
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

bool VPGProcessManager::IsUpdateAvaliable()
{
    switch (this->GetVPGProjectType())
    {
    case VPGProjectType::VccComplex:
    case VPGProjectType::VccDll:
    case VPGProjectType::VccExe:
        return true;
    default:
        break;
    }
    return false;
}

void VPGProcessManager::Add()
{
    try {
        this->VerifyLocalResponse();

        // Copy all nessary files to dest


    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

void VPGProcessManager::Update()
{
    try {
        if (!IsUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        this->VerifyLocalResponse();

        // Compare the files with dist, sync files

    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

void VPGProcessManager::Generate()
{
    try {
        if (!IsUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        this->VerifyLocalResponse();

    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

void VPGProcessManager::Execute(const std::vector<std::wstring> &cmds)
{
    if (cmds.size() < 2)
        return;

    this->InitLogProperty();

    try {
        std::wstring mode = cmds[1];
        if (mode == L"-V") {
            std::wcout << L"VCCProjectGenerator version " << VPGGlobal::GetVersion() << std::endl;
            return;        
        }

        for (size_t i = 2; i < cmds.size(); i++) {
            std::wstring cmd = cmds[i];
            if (!HasPrefix(cmd, L"-")) {
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Argument missing - for " + cmd);
            }
            // single tag
            if (!HasPrefix(cmd, L"--")) {
                // must have second argument
                if (i + 1 < cmds.size()) {
                    i++;
                    std::wstring cmd2 = cmds[i];
                    if (cmd == L"-w") {
                        this->_Workspace = cmd2;
                    } else if (cmd == L"-i") {
                        ToUpper(cmd2);
                        if (HasPrefix(cmd2, L"CPP")) {
                            if (HasSuffix(cmd2, L"DLL"))
                                this->_VPGProjectType = VPGProjectType::CppDll;
                            else if (HasSuffix(cmd2, L"EXE"))
                                this->_VPGProjectType = VPGProjectType::CppExe;
                            else if (HasSuffix(cmd2, L"COMPLEX"))
                                this->_VPGProjectType = VPGProjectType::CppComplex;
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        } else if (HasPrefix(cmd2, L"VCC")) {
                            if (HasSuffix(cmd2, L"DLL"))
                                this->_VPGProjectType = VPGProjectType::VccDll;
                            else if (HasSuffix(cmd2, L"EXE"))
                                this->_VPGProjectType = VPGProjectType::VccExe;
                            else if (HasSuffix(cmd2, L"COMPLEX"))
                                this->_VPGProjectType = VPGProjectType::VccComplex;
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        }
                    } else if (cmd == L"-v") {
                        this->_Tag = cmd2;
                    } else if (cmd == L"-d") {
                        this->_LogDepth = std::stoi(cmd2);
                    } else if (cmd == L"-p") {
                        this->_Plugins.push_back(cmd2);
                    } else
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd);
                } else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Argument missing for " + cmd);
            } else {
                // double tag, no second argument
                if (cmd == L"--ExcludeUnitTest")
                    this->_IsExcludeUnitTest = true;
                else if (cmd == L"--ExcludeExternalUnitTest")
                    this->_IsExcludeExternalUnitTest = true;
                else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd);
            }
        }

        // validation
        if (this->_VPGProjectType == VPGProjectType::NA)
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Interface Type missing");
        
        // process
        if (mode == L"-A")
            this->Add();
        else if (mode == L"-U")
            this->Update();
        else if (mode == L"-G")
            this->Generate();
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknow Mode " + mode);
        
        std::wcout << L"Process Complete Successfully!" << std::endl;
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}
