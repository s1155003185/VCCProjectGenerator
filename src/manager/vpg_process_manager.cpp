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
        this->_LogConfig->setIsConsoleLog(true);
}

void VPGProcessManager::VerifyLocalResponse()
{
    TRY
        // 1. Check if source file exists, if not exist then clone
        // 2. Check if version != branch, then checkout
        // std::wstring localResponseDirectory = VPGGlobal::GetConvertedPath(_Option->getWorkspace());
        // if (vcc::IsBlank(localResponseDirectory))
        //     localResponseDirectory = VPGGlobal::GetConvertedPath(VPGGlobal::GetVccLocalResponseFolder());
        std::wstring localResponseDirectoryBase = VPGGlobal::GetConvertedPath(VPGGlobal::GetVccLocalResponseFolder());
        std::wstring localResponseDirectoryProject = VPGGlobal::GetConvertedPath(VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->getProjectType()));
        std::wstring gitUrl = _Option->getTemplate() != nullptr ? _Option->getTemplate()->getUrl() : L"";

        vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Check VCC Local response existance: " + localResponseDirectoryProject);
        bool isNeedToCloneGitResponse = false;
        if (vcc::isDirectoryExists(localResponseDirectoryProject)) {
            if (vcc::GitService::IsGitResponse(this->getLogConfig().get(), localResponseDirectoryProject)) {
                vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Done.");

                // Cannot push for tag
                try
                {            
                    vcc::GitService::Pull(this->getLogConfig().get(), localResponseDirectoryProject);
                }
                catch(...)
                {
                }
                if (_Option->getProjectType() == VPGProjectType::VccComplex
                    || _Option->getProjectType() == VPGProjectType::VccDll
                    || _Option->getProjectType() == VPGProjectType::VccExe) {

                    vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Check Version.");
                    // check tag version
                    // if same as current version of generator, no action
                    // if not same, then check verison of genertor exists, if not exists, then master, else switch to correct branch
                    auto currentLog = vcc::GitService::GetCurrentLog(this->getLogConfig().get(), localResponseDirectoryProject);
                    if (!vcc::isContain(currentLog->getTags(), VPGGlobal::GetVersion())) {
                        std::wstring currentBranchName = L"";
                        TRY
                            auto currentTag = vcc::GitService::GetCurrentTag(this->getLogConfig().get(), localResponseDirectoryProject);
                            if (vcc::IsBlank(currentTag->getTagName()))
                                currentBranchName = vcc::GitService::GetCurrentBranchName(this->getLogConfig().get(), localResponseDirectoryProject);
                        CATCH_SLIENT
                        // If version is main and current tag version not exists, then no switch
                        std::wstring mainBranch = L"main";
                        std::vector<std::wstring> allTags = vcc::GitService::GetTags(this->getLogConfig().get(), localResponseDirectoryProject);
                        if (currentBranchName == L"main" && !vcc::isContain(allTags, VPGGlobal::GetVersion())) {
                            vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Currently in main branch and " + VPGGlobal::GetVersion() + L" is not found. Keep in main branch.");
                        } else {
                            isNeedToCloneGitResponse = true;
                            vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Outdated.");
                        }
                    }
                }
            } else {
                isNeedToCloneGitResponse = true;
                vcc::LogService::LogInfo(this->getLogConfig().get(), L"", localResponseDirectoryProject + L" already exists.");
            }            
        } else {
            isNeedToCloneGitResponse = true;
            vcc::LogService::LogInfo(this->getLogConfig().get(), L"", localResponseDirectoryProject + L" not Exists.");
        }
        if (isNeedToCloneGitResponse) {
            // Try to drop and create
            // Note: Window version will hold folder for a while because of some threads of project have not been terminated. Cannot use drop create
            try
            {
                if (vcc::isDirectoryExists(localResponseDirectoryProject)) {
                    vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"remove current response.");
                    vcc::removeDirectory(localResponseDirectoryProject);
                    vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Done.");
                }
            }
            catch(const std::exception& e)
            {
                std::string msg(e.what());
                vcc::LogService::LogWarning(this->getLogConfig().get(), L"", vcc::str2wstr(msg));
            }
            try
            {            
                vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Clone from " + gitUrl);
                vcc::GitCloneOption cloneOption;
                cloneOption.SetIsQuiet(true);
                vcc::GitService::cloneGitResponse(this->getLogConfig().get(), localResponseDirectoryBase, gitUrl, &cloneOption);
                vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Done.");
            }
            catch(const std::exception& e)
            {
                std::string msg(e.what());
                vcc::LogService::LogWarning(this->getLogConfig().get(), L"", vcc::str2wstr(msg));
            }
            
            // Switch to correct version
            vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Switch to current version " + VPGGlobal::GetVersion());
            try
            {
                vcc::GitService::Switch(this->getLogConfig().get(), localResponseDirectoryProject, VPGGlobal::GetVersion());
                vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Done.");
            }
            catch(const std::exception& e)
            {
                try {
                    vcc::LogService::LogError(this->getLogConfig().get(), L"", L"VCC Project Generator version Not Exists. Switch to main");
                    vcc::GitService::Switch(this->getLogConfig().get(), localResponseDirectoryProject, L"main");
                    vcc::GitService::Pull(this->getLogConfig().get(), localResponseDirectoryProject);
                    vcc::LogService::LogInfo(this->getLogConfig().get(), L"", L"Done.");
                } catch (const std::exception &e) {
                    vcc::LogService::LogWarning(this->getLogConfig().get(), L"", vcc::str2wstr(e.what()));
                }
            }
        }
    CATCH
}

bool VPGProcessManager::IsUpdateAvaliable()
{
    switch (_Option->getProjectType())
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
    switch (_Option->getProjectType())
    {
    case VPGProjectType::VccComplex:
    case VPGProjectType::VccDll:
    case VPGProjectType::VccExe:
        return std::make_shared<VPGVccGenerationManager>(this->getLogConfig(), _Workspace, _Option);
    case VPGProjectType::CppComplex:
    case VPGProjectType::CppDll:
    case VPGProjectType::CppExe:
        return std::make_shared<VPGCppGenerationManager>(this->getLogConfig(), _Workspace, _Option);
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
        getGenerationManager()->Add();
    CATCH
}

void VPGProcessManager::Update()
{
    TRY
        if (!IsUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        this->VerifyLocalResponse();
        getGenerationManager()->Update();
    CATCH
}

void VPGProcessManager::Generate()
{
    TRY
        if (!IsUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        // No need to varify response for generate
        // this->VerifyLocalResponse();
        getGenerationManager()->generate();
    CATCH
}

void VPGProcessManager::execute(const std::vector<std::wstring> &cmds)
{
    if (cmds.size() < 2)
        return;

    this->initLogConfig();

    _Workspace = vcc::GetCurrentFolderPath();
    try {
        std::wstring mode = cmds[1];
        if (mode == L"-Version") {
            std::wcout << VPGGlobal::GetVersion() << std::endl;
            return;        
        }

        // ensure no nullptr
        if (_Option->getTemplate() == nullptr)
            _Option->setTemplate(std::make_shared<VPGConfigTemplate>());
        
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
                                _Option->setProjectType(VPGProjectType::CppDll);
                            else if (vcc::IsEndWith(cmd2, L"EXE"))
                                _Option->setProjectType(VPGProjectType::CppExe);
                            else if (vcc::IsEndWith(cmd2, L"COMPLEX"))
                                _Option->setProjectType(VPGProjectType::CppComplex);
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        } else if (vcc::IsStartWith(cmd2, L"VCC")) {
                            if (vcc::IsEndWith(cmd2, L"DLL"))
                                _Option->setProjectType(VPGProjectType::VccDll);
                            else if (vcc::IsEndWith(cmd2, L"EXE"))
                                _Option->setProjectType(VPGProjectType::VccExe);
                            else if (vcc::IsEndWith(cmd2, L"COMPLEX"))
                                _Option->setProjectType(VPGProjectType::VccComplex);
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        }
                    } else if (cmd == L"-version") 
                        _Option->setVersion(cmd2);
                    else if (cmd == L"-project-prefix")
                        _Option->setProjectPrefix(cmd2);
                    else if (cmd == L"-project-name")
                        _Option->setProjectName(cmd2);
                    else if (cmd == L"-exe-name")
                        _Option->setProjectNameExe(cmd2);
                    else if (cmd == L"-dll-name")
                        _Option->setProjectNameDll(cmd2);
                    else if (cmd == L"-plugins")
                        _Option->insertPlugins(cmd2);
                    else
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd);
                } else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Argument missing for " + cmd);
            } else {
                // double tag, no second argument
                if (cmd == L"--ExcludeUnitTest")
                    _Option->getTemplate()->setIsExcludeUnittest(true);
                else if (cmd == L"--ExcludeExternalUnitTest")
                    _Option->getTemplate()->setIsExcludeVCCUnitTest(true);
                else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd);
            }
        }

        if (mode == L"-Add") {
            // adjust option based on ProjectType
            switch (_Option->getProjectType())
            {
            case VPGProjectType::CppDll:
            case VPGProjectType::VccDll:
                _Option->setProjectNameExe(L"");
                break;
            case VPGProjectType::CppExe:
            case VPGProjectType::VccExe:
                _Option->setProjectNameDll(L"");
                break;
            default:
                break;
            }

            // validation
            if (this->_Option->getProjectType() == VPGProjectType::VccModule)
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Interface Type missing.");
        } else {
            std::wstring vccJsonFilePath = vcc::concatPaths({_Workspace, VPGGlobal::GetVccJsonFileName()});
            if (!vcc::isFilePresent(vccJsonFilePath))
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, vccJsonFilePath + L": File not found.");
            std::wstring fileContent = vcc::readFile(vccJsonFilePath);
            auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
            auto json = std::make_shared<vcc::Json>();
            jsonBuilder->Deserialize(fileContent, json);
            _Option->DeserializeJson(json);
            _Option->setProjectType(VPGProjectType::VccComplex);
        }

        std::wstring localResponseDirectory = _Option->getTemplate() != nullptr && !vcc::IsBlank(_Option->getTemplate()->getWorkspace()) ? _Option->getTemplate()->getWorkspace() : VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->getProjectType());
        std::wstring gitUrl = _Option->getTemplate() != nullptr && !vcc::IsBlank(_Option->getTemplate()->getUrl()) ? _Option->getTemplate()->getUrl() : VPGGlobal::GetProjecURL(_Option->getProjectType());
        _Option->getTemplate()->setWorkspace(localResponseDirectory);
        _Option->getTemplate()->setUrl(gitUrl);

        if (mode == L"-Add")
            this->Add();
        else if (mode == L"-Update")
            this->Update();
        else if (mode == L"-Generate")
            this->generate();
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknow Mode " + mode);
        
        vcc::LogService::LogInfo(this->_LogConfig.get(), L"VPGProcessManager", L"Process Complete Successfully!");
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}
