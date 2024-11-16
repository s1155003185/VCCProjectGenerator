#include "vpg_process_manager.hpp"

#include <iostream>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "git_service.hpp"
#include "i_vpg_generation_manager.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"
#include "vpg_global.hpp"
#include "vpg_cpp_generation_manager.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_project_type.hpp"
#include "vpg_vcc_generation_manager.hpp"

using namespace vcc;

void VPGProcessManager::InitLogConfig()
{
    if (this->_LogConfig != nullptr)
        this->_LogConfig->SetIsConsoleLog(true);
}

void VPGProcessManager::VerifyLocalResponse()
{
    TRY
        // 1. Check if source file exists, if not exist then clone
        // 2. Check if version != branch, then checkout
        // std::wstring localResponseDirectory = VPGGlobal::GetConvertedPath(_Option->GetTemplateWorkspace());
        // if (IsBlank(localResponseDirectory))
        //     localResponseDirectory = VPGGlobal::GetConvertedPath(VPGGlobal::GetVccLocalResponseFolder());
        std::wstring localResponseDirectoryBase = VPGGlobal::GetConvertedPath(VPGGlobal::GetVccLocalResponseFolder());
        std::wstring localResponseDirectoryProject = VPGGlobal::GetConvertedPath(VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType()));
        std::wstring gitUrl = _Option->GetTemplateGitUrl();

        LogService::LogInfo(this->GetLogConfig().get(), L"", L"Check VCC Local response existance: " + localResponseDirectoryProject);
        bool isNeedToCloneGitResponse = false;
        if (IsDirectoryExists(localResponseDirectoryProject)) {
            if (GitService::IsGitResponse(this->GetLogConfig().get(), localResponseDirectoryProject)) {
                LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");

                // Cannot push for tag
                try
                {            
                    GitService::Pull(this->GetLogConfig().get(), localResponseDirectoryProject);
                }
                catch(...)
                {
                }
                if (_Option->GetProjectType() == VPGProjectType::VccComplex
                    || _Option->GetProjectType() == VPGProjectType::VccDll
                    || _Option->GetProjectType() == VPGProjectType::VccExe) {

                    LogService::LogInfo(this->GetLogConfig().get(), L"", L"Check Version.");
                    // check tag version
                    // if same as current version of generator, no action
                    // if not same, then check verison of genertor exists, if not exists, then master, else switch to correct branch
                    DECLARE_SPTR(GitLog, currentLog);
                    GitService::GetCurrentLog(this->GetLogConfig().get(), localResponseDirectoryProject, currentLog);
                    if (!IsContain(currentLog->GetTags(), VPGGlobal::GetVersion())) {
                        std::wstring currentBranchName = L"";
                        TRY
                            GitTagCurrentTag currentTag = GitService::GetCurrentTag(this->GetLogConfig().get(), localResponseDirectoryProject);
                            if (IsBlank(currentTag.GetTagName()))
                                currentBranchName = GitService::GetCurrentBranchName(this->GetLogConfig().get(), localResponseDirectoryProject);
                        CATCH_SLIENT
                        // If version is main and current tag version not exists, then no switch
                        std::wstring mainBranch = L"main";
                        std::vector<std::wstring> allTags;
                        GitService::GetTags(this->GetLogConfig().get(), localResponseDirectoryProject, nullptr, allTags);
                        if (currentBranchName == L"main" && !IsContain(allTags, VPGGlobal::GetVersion())) {
                            LogService::LogInfo(this->GetLogConfig().get(), L"", L"Currently in main branch and " + VPGGlobal::GetVersion() + L" is not found. Keep in main branch.");
                        } else {
                            isNeedToCloneGitResponse = true;
                            LogService::LogInfo(this->GetLogConfig().get(), L"", L"Outdated.");
                        }
                    }
                }
            } else {
                isNeedToCloneGitResponse = true;
                LogService::LogInfo(this->GetLogConfig().get(), L"", localResponseDirectoryProject + L" already exists.");
            }            
        } else {
            isNeedToCloneGitResponse = true;
            LogService::LogInfo(this->GetLogConfig().get(), L"", localResponseDirectoryProject + L" not Exists.");
        }
        if (isNeedToCloneGitResponse) {
            // Try to drop and create
            // Note: Window version will hold folder for a while because of some threads of project have not been terminated. Cannot use drop create
            try
            {
                if (IsDirectoryExists(localResponseDirectoryProject)) {
                    LogService::LogInfo(this->GetLogConfig().get(), L"", L"Remove current response.");
                    RemoveDirectory(localResponseDirectoryProject);
                    LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");
                }
            }
            catch(const std::exception& e)
            {
                std::string msg(e.what());
                LogService::LogWarning(this->GetLogConfig().get(), L"", str2wstr(msg));
            }
            try
            {            
                LogService::LogInfo(this->GetLogConfig().get(), L"", L"Clone from " + gitUrl);
                GitCloneOption cloneOption;
                cloneOption.SetIsQuiet(true);
                GitService::Clone(this->GetLogConfig().get(), localResponseDirectoryBase, gitUrl, &cloneOption);
                LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");
            }
            catch(const std::exception& e)
            {
                std::string msg(e.what());
                LogService::LogWarning(this->GetLogConfig().get(), L"", str2wstr(msg));
            }
            
            // Switch to correct version
            LogService::LogInfo(this->GetLogConfig().get(), L"", L"Switch to current version " + VPGGlobal::GetVersion());
            try
            {
                GitService::Switch(this->GetLogConfig().get(), localResponseDirectoryProject, VPGGlobal::GetVersion());
                LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");
            }
            catch(const std::exception& e)
            {
                try {
                    LogService::LogError(this->GetLogConfig().get(), L"", L"VCC Project Generator version Not Exists. Switch to main");
                    GitService::Switch(this->GetLogConfig().get(), localResponseDirectoryProject, L"main");
                    GitService::Pull(this->GetLogConfig().get(), localResponseDirectoryProject);
                    LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");
                } catch (const std::exception &e) {
                    LogService::LogWarning(this->GetLogConfig().get(), L"", str2wstr(e.what()));
                }
            }
        }
    CATCH
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
        return std::make_shared<VPGVccGenerationManager>(this->GetLogConfig(), _Workspace, _Option);
    case VPGProjectType::CppComplex:
    case VPGProjectType::CppDll:
    case VPGProjectType::CppExe:
        return std::make_shared<VPGCppGenerationManager>(this->GetLogConfig(), _Workspace, _Option);
    default:
        assert(false);
        break;
    }
    return nullptr;
}

void VPGProcessManager::Add()
{
    TRY
        this->VerifyLocalResponse();
        GetGenerationManager()->Add();
    CATCH
}

void VPGProcessManager::Update()
{
    TRY
        if (!IsUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        this->VerifyLocalResponse();
        GetGenerationManager()->Update();
    CATCH
}

void VPGProcessManager::Generate()
{
    TRY
        if (!IsUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        // No need to varify response for generate
        // this->VerifyLocalResponse();
        GetGenerationManager()->Generate();
    CATCH
}

void VPGProcessManager::Execute(const std::vector<std::wstring> &cmds)
{
    if (cmds.size() < 2)
        return;

    this->InitLogConfig();

    _Workspace = GetCurrentFolderPath();
    try {
        std::wstring mode = cmds[1];
        if (mode == L"-Version") {
            std::wcout << VPGGlobal::GetVersion() << std::endl;
            return;        
        }

        for (size_t i = 2; i < cmds.size(); i++) {
            std::wstring cmd = cmds[i];
            if (!IsStartWith(cmd, L"-")) {
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Argument missing - for " + cmd);
            }
            // single tag
            if (!IsStartWith(cmd, L"--")) {
                // must have second argument
                if (i + 1 < cmds.size()) {
                    i++;
                    std::wstring cmd2 = cmds[i];
                    if (cmd == L"-workspace-destination") {
                        _Workspace = cmd2;
                    } else if (cmd == L"-interface") {
                        ToUpper(cmd2);
                        if (IsStartWith(cmd2, L"CPP")) {
                            if (IsEndWith(cmd2, L"DLL"))
                                _Option->SetProjectType(VPGProjectType::CppDll);
                            else if (IsEndWith(cmd2, L"EXE"))
                                _Option->SetProjectType(VPGProjectType::CppExe);
                            else if (IsEndWith(cmd2, L"COMPLEX"))
                                _Option->SetProjectType(VPGProjectType::CppComplex);
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        } else if (IsStartWith(cmd2, L"VCC")) {
                            if (IsEndWith(cmd2, L"DLL"))
                                _Option->SetProjectType(VPGProjectType::VccDll);
                            else if (IsEndWith(cmd2, L"EXE"))
                                _Option->SetProjectType(VPGProjectType::VccExe);
                            else if (IsEndWith(cmd2, L"COMPLEX"))
                                _Option->SetProjectType(VPGProjectType::VccComplex);
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        }
                    } else if (cmd == L"-version") 
                        _Option->SetVersion(cmd2);
                    else if (cmd == L"-project-prefix")
                        _Option->SetProjectPrefix(cmd2);
                    else if (cmd == L"-project-name")
                        _Option->SetProjectName(cmd2);
                    else if (cmd == L"-exe-name")
                        _Option->SetProjectNameExe(cmd2);
                    else if (cmd == L"-dll-name")
                        _Option->SetProjectNameDll(cmd2);
                    else if (cmd == L"-plugins")
                        _Option->InsertPlugins(cmd2);
                    else
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd);
                } else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Argument missing for " + cmd);
            } else {
                // double tag, no second argument
                if (cmd == L"--ExcludeUnitTest")
                    _Option->SetIsExcludeUnittest(true);
                else if (cmd == L"--ExcludeExternalUnitTest")
                    _Option->SetIsExcludeVCCUnitTest(true);
                else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd);
            }
        }

        if (mode == L"-Add") {
            // adjust option based on ProjectType
            switch (_Option->GetProjectType())
            {
            case VPGProjectType::CppDll:
            case VPGProjectType::VccDll:
                _Option->SetProjectNameExe(L"");
                break;
            case VPGProjectType::CppExe:
            case VPGProjectType::VccExe:
                _Option->SetProjectNameDll(L"");
                break;
            default:
                break;
            }

            // validation
            if (this->_Option->GetProjectType() == VPGProjectType::VccModule)
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Interface Type missing.");
        } else {
            std::wstring vccJsonFilePath = ConcatPaths({_Workspace, VPGGlobal::GetVccJsonFileName()});
            if (!IsFileExists(vccJsonFilePath))
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, vccJsonFilePath + L": File not found.");
            std::wstring fileContent = ReadFile(vccJsonFilePath);
            DECLARE_UPTR(JsonBuilder, jsonBuilder);
            DECLARE_SPTR(Json, json);
            jsonBuilder->Deserialize(fileContent, json);
            _Option->DeserializeJson(json);
            _Option->SetProjectType(VPGProjectType::VccComplex);
        }

        std::wstring localResponseDirectory = !_Option->GetTemplateWorkspace().empty() ? _Option->GetTemplateWorkspace() : VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType());
        std::wstring gitUrl = !_Option->GetTemplateGitUrl().empty() ? _Option->GetTemplateGitUrl() : VPGGlobal::GetProjecURL(_Option->GetProjectType());
        _Option->SetTemplateWorkspace(localResponseDirectory);
        _Option->SetTemplateGitUrl(gitUrl);

        if (mode == L"-Add")
            this->Add();
        else if (mode == L"-Update")
            this->Update();
        else if (mode == L"-Generate")
            this->Generate();
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknow Mode " + mode);
        
        LogService::LogInfo(this->_LogConfig.get(), L"VPGProcessManager", L"Process Complete Successfully!");
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}
