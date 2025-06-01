#include "vpg_process_manager.hpp"

#include <iostream>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "git_service.hpp"
#include "i_vpg_generation_manager.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"
#include "vpg_global.hpp"
#include "vpg_cpp_generation_manager.hpp"
#include "vpg_config.hpp"
#include "vpg_project_type.hpp"
#include "vpg_vcc_generation_manager.hpp"

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
        // std::wstring localResponseDirectory = VPGGlobal::GetConvertedPath(_Option->GetWorkspace());
        // if (vcc::IsBlank(localResponseDirectory))
        //     localResponseDirectory = VPGGlobal::GetConvertedPath(VPGGlobal::GetVccLocalResponseFolder());
        std::wstring localResponseDirectoryBase = VPGGlobal::GetConvertedPath(VPGGlobal::GetVccLocalResponseFolder());
        std::wstring localResponseDirectoryProject = VPGGlobal::GetConvertedPath(VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType()));
        std::wstring gitUrl = _Option->GetTemplate() != nullptr ? _Option->GetTemplate()->GetUrl() : L"";

        vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Check VCC Local response existance: " + localResponseDirectoryProject);
        bool isNeedToCloneGitResponse = false;
        if (vcc::IsDirectoryExists(localResponseDirectoryProject)) {
            if (vcc::GitService::IsGitResponse(this->GetLogConfig().get(), localResponseDirectoryProject)) {
                vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");

                // Cannot push for tag
                try
                {            
                    vcc::GitService::Pull(this->GetLogConfig().get(), localResponseDirectoryProject);
                }
                catch(...)
                {
                }
                if (_Option->GetProjectType() == VPGProjectType::VccComplex
                    || _Option->GetProjectType() == VPGProjectType::VccDll
                    || _Option->GetProjectType() == VPGProjectType::VccExe) {

                    vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Check Version.");
                    // check tag version
                    // if same as current version of generator, no action
                    // if not same, then check verison of genertor exists, if not exists, then master, else switch to correct branch
                    auto currentLog = vcc::GitService::GetCurrentLog(this->GetLogConfig().get(), localResponseDirectoryProject);
                    if (!vcc::IsContain(currentLog->GetTags(), VPGGlobal::GetVersion())) {
                        std::wstring currentBranchName = L"";
                        TRY
                            auto currentTag = vcc::GitService::GetCurrentTag(this->GetLogConfig().get(), localResponseDirectoryProject);
                            if (vcc::IsBlank(currentTag->GetTagName()))
                                currentBranchName = vcc::GitService::GetCurrentBranchName(this->GetLogConfig().get(), localResponseDirectoryProject);
                        CATCH_SLIENT
                        // If version is main and current tag version not exists, then no switch
                        std::wstring mainBranch = L"main";
                        std::vector<std::wstring> allTags = vcc::GitService::GetTags(this->GetLogConfig().get(), localResponseDirectoryProject);
                        if (currentBranchName == L"main" && !vcc::IsContain(allTags, VPGGlobal::GetVersion())) {
                            vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Currently in main branch and " + VPGGlobal::GetVersion() + L" is not found. Keep in main branch.");
                        } else {
                            isNeedToCloneGitResponse = true;
                            vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Outdated.");
                        }
                    }
                }
            } else {
                isNeedToCloneGitResponse = true;
                vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", localResponseDirectoryProject + L" already exists.");
            }            
        } else {
            isNeedToCloneGitResponse = true;
            vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", localResponseDirectoryProject + L" not Exists.");
        }
        if (isNeedToCloneGitResponse) {
            // Try to drop and create
            // Note: Window version will hold folder for a while because of some threads of project have not been terminated. Cannot use drop create
            try
            {
                if (vcc::IsDirectoryExists(localResponseDirectoryProject)) {
                    vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Remove current response.");
                    vcc::RemoveDirectory(localResponseDirectoryProject);
                    vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");
                }
            }
            catch(const std::exception& e)
            {
                std::string msg(e.what());
                vcc::LogService::LogWarning(this->GetLogConfig().get(), L"", vcc::str2wstr(msg));
            }
            try
            {            
                vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Clone from " + gitUrl);
                vcc::GitCloneOption cloneOption;
                cloneOption.SetIsQuiet(true);
                vcc::GitService::CloneGitResponse(this->GetLogConfig().get(), localResponseDirectoryBase, gitUrl, &cloneOption);
                vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");
            }
            catch(const std::exception& e)
            {
                std::string msg(e.what());
                vcc::LogService::LogWarning(this->GetLogConfig().get(), L"", vcc::str2wstr(msg));
            }
            
            // Switch to correct version
            vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Switch to current version " + VPGGlobal::GetVersion());
            try
            {
                vcc::GitService::Switch(this->GetLogConfig().get(), localResponseDirectoryProject, VPGGlobal::GetVersion());
                vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");
            }
            catch(const std::exception& e)
            {
                try {
                    vcc::LogService::LogError(this->GetLogConfig().get(), L"", L"VCC Project Generator version Not Exists. Switch to main");
                    vcc::GitService::Switch(this->GetLogConfig().get(), localResponseDirectoryProject, L"main");
                    vcc::GitService::Pull(this->GetLogConfig().get(), localResponseDirectoryProject);
                    vcc::LogService::LogInfo(this->GetLogConfig().get(), L"", L"Done.");
                } catch (const std::exception &e) {
                    vcc::LogService::LogWarning(this->GetLogConfig().get(), L"", vcc::str2wstr(e.what()));
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

    _Workspace = vcc::GetCurrentFolderPath();
    try {
        std::wstring mode = cmds[1];
        if (mode == L"-Version") {
            std::wcout << VPGGlobal::GetVersion() << std::endl;
            return;        
        }

        // ensure no nullptr
        if (_Option->GetTemplate() == nullptr)
            _Option->SetTemplate(std::make_shared<VPGConfigTemplate>());
        
        for (size_t i = 2; i < cmds.size(); i++) {
            std::wstring cmd = cmds[i];
            if (!vcc::IsStartWith(cmd, L"-")) {
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Argument missing - for " + cmd);
            }
            // single tag
            if (!vcc::IsStartWith(cmd, L"--")) {
                // must have second argument
                if (i + 1 < cmds.size()) {
                    i++;
                    std::wstring cmd2 = cmds[i];
                    if (cmd == L"-workspace-destination") {
                        _Workspace = cmd2;
                    } else if (cmd == L"-interface") {
                        vcc::ToUpper(cmd2);
                        if (vcc::IsStartWith(cmd2, L"CPP")) {
                            if (vcc::IsEndWith(cmd2, L"DLL"))
                                _Option->SetProjectType(VPGProjectType::CppDll);
                            else if (vcc::IsEndWith(cmd2, L"EXE"))
                                _Option->SetProjectType(VPGProjectType::CppExe);
                            else if (vcc::IsEndWith(cmd2, L"COMPLEX"))
                                _Option->SetProjectType(VPGProjectType::CppComplex);
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        } else if (vcc::IsStartWith(cmd2, L"VCC")) {
                            if (vcc::IsEndWith(cmd2, L"DLL"))
                                _Option->SetProjectType(VPGProjectType::VccDll);
                            else if (vcc::IsEndWith(cmd2, L"EXE"))
                                _Option->SetProjectType(VPGProjectType::VccExe);
                            else if (vcc::IsEndWith(cmd2, L"COMPLEX"))
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
                    _Option->GetTemplate()->SetIsExcludeUnittest(true);
                else if (cmd == L"--ExcludeExternalUnitTest")
                    _Option->GetTemplate()->SetIsExcludeVCCUnitTest(true);
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
            std::wstring vccJsonFilePath = vcc::ConcatPaths({_Workspace, VPGGlobal::GetVccJsonFileName()});
            if (!vcc::IsFilePresent(vccJsonFilePath))
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, vccJsonFilePath + L": File not found.");
            std::wstring fileContent = vcc::ReadFile(vccJsonFilePath);
            auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
            auto json = std::make_shared<vcc::Json>();
            jsonBuilder->Deserialize(fileContent, json);
            _Option->DeserializeJson(json);
            _Option->SetProjectType(VPGProjectType::VccComplex);
        }

        std::wstring localResponseDirectory = _Option->GetTemplate() != nullptr && !vcc::IsBlank(_Option->GetTemplate()->GetWorkspace()) ? _Option->GetTemplate()->GetWorkspace() : VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType());
        std::wstring gitUrl = _Option->GetTemplate() != nullptr && !vcc::IsBlank(_Option->GetTemplate()->GetUrl()) ? _Option->GetTemplate()->GetUrl() : VPGGlobal::GetProjecURL(_Option->GetProjectType());
        _Option->GetTemplate()->SetWorkspace(localResponseDirectory);
        _Option->GetTemplate()->SetUrl(gitUrl);

        if (mode == L"-Add")
            this->Add();
        else if (mode == L"-Update")
            this->Update();
        else if (mode == L"-Generate")
            this->Generate();
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknow Mode " + mode);
        
        vcc::LogService::LogInfo(this->_LogConfig.get(), L"VPGProcessManager", L"Process Complete Successfully!");
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}
