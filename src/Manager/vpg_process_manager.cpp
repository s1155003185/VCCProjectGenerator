#include "vpg_process_manager.hpp"

#include <iostream>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "git_service.hpp"
#include "i_vpg_generation_manager.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"
#include "vpg_global.hpp"
#include "vpg_base_generation_manager.hpp"
#include "vpg_cpp_generation_manager.hpp"
#include "vpg_project_type.hpp"
#include "vpg_vcc_generation_manager.hpp"

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
        std::wstring localResponseDirectory = _Option->GetWorkspaceSource();
        std::wstring gitUrl = _Option->GetWorkspaceSourceGitUrl();

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
            GitService::Clone(this->GetLogProperty().get(),  VPGGlobal::GetVccLocalResponseFolder(), gitUrl, _Option->GetVersion());
            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Done.");
        }
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

bool VPGProcessManager::IsUpdateAvaliable()
{
    switch (_Option->GetProjectType())
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

std::shared_ptr<IVPGGenerationManager> VPGProcessManager::GetGenerationManager()
{
    switch (_Option->GetProjectType())
    {
    case VPGProjectType::VccComplex:
    case VPGProjectType::VccDll:
    case VPGProjectType::VccExe:
        return std::make_shared<VPGVccGenerationManager>(this->GetLogProperty(), _Option);
    case VPGProjectType::CppComplex:
    case VPGProjectType::CppDll:
    case VPGProjectType::CppExe:
        return std::make_shared<VPGCppGenerationManager>(this->GetLogProperty(), _Option);
    default:
        assert(false);
        break;
    }
    return nullptr;
}

void VPGProcessManager::Add()
{
    TRY_CATCH(
        this->VerifyLocalResponse();
        GetGenerationManager()->Add();
    )
}

void VPGProcessManager::Update()
{
    TRY_CATCH(
        if (!IsUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        this->VerifyLocalResponse();
        GetGenerationManager()->Update();
    )
}

void VPGProcessManager::Generate()
{
    TRY_CATCH(
        if (!IsUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        this->VerifyLocalResponse();
        GetGenerationManager()->Generate();
    )
}

void VPGProcessManager::Execute(const std::vector<std::wstring> &cmds)
{
    if (cmds.size() < 2)
        return;

    this->InitLogProperty();

    try {
        std::wstring mode = cmds[1];
        if (mode == L"-V") {
            std::wcout << VPGGlobal::GetVersion() << std::endl;
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
                        _Option->SetWorkspaceDestination(cmd2);
                    } else if (cmd == L"-i") {
                        ToUpper(cmd2);
                        if (HasPrefix(cmd2, L"CPP")) {
                            if (HasSuffix(cmd2, L"DLL"))
                                _Option->SetProjectType(VPGProjectType::CppDll);
                            else if (HasSuffix(cmd2, L"EXE"))
                                _Option->SetProjectType(VPGProjectType::CppExe);
                            else if (HasSuffix(cmd2, L"COMPLEX"))
                                _Option->SetProjectType(VPGProjectType::CppComplex);
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        } else if (HasPrefix(cmd2, L"VCC")) {
                            if (HasSuffix(cmd2, L"DLL"))
                                _Option->SetProjectType(VPGProjectType::VccDll);
                            else if (HasSuffix(cmd2, L"EXE"))
                                _Option->SetProjectType(VPGProjectType::VccExe);
                            else if (HasSuffix(cmd2, L"COMPLEX"))
                                _Option->SetProjectType(VPGProjectType::VccComplex);
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        }
                    } else if (cmd == L"-v") {
                        _Option->SetVersion(cmd2);
                    } else if (cmd == L"-p") {
                        _Option->InsertPlugins(cmd2);
                    } else
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd);
                } else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Argument missing for " + cmd);
            } else {
                // double tag, no second argument
                if (cmd == L"--ExcludeUnitTest")
                    _Option->SetProjectNameGtest(L"");
                else if (cmd == L"--ExcludeExternalUnitTest")
                    _Option->SetIsExcludeVCCUnitTest(true);
                else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd);
            }
        }

        // validation
        if (this->_Option->GetProjectType() == VPGProjectType::NA)
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Interface Type missing");
        
        _Option->SetWorkspaceSource(VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType()));
        _Option->SetWorkspaceSourceGitUrl(VPGGlobal::GetProjecURL(_Option->GetProjectType()));

        // process
        if (mode == L"-A")
            this->Add();
        else if (mode == L"-U")
            this->Update();
        else if (mode == L"-G")
            this->Generate();
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknow Mode " + mode);
        
        LogService::LogInfo(this->_LogProperty.get(), L"VPGProcessManager", L"Process Complete Successfully!");
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}
