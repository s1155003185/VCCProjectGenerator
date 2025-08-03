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

void VPGProcessManager::initLogConfig()
{
    if (this->_LogConfig != nullptr)
        this->_LogConfig->setIsConsoleLog(true);
}

void VPGProcessManager::verifyLocalResponse()
{
    TRY
        // 1. Check if source file exists, if not exist then clone
        // 2. Check if version != branch, then checkout
        // std::wstring localResponseDirectory = VPGGlobal::getConvertedPath(_Option->getWorkspace());
        // if (vcc::isBlank(localResponseDirectory))
        //     localResponseDirectory = VPGGlobal::getConvertedPath(VPGGlobal::getVccLocalResponseFolder());
        std::wstring localResponseDirectoryBase = VPGGlobal::getConvertedPath(VPGGlobal::getVccLocalResponseFolder());
        std::wstring localResponseDirectoryProject = VPGGlobal::getConvertedPath(VPGGlobal::getVccProjectLocalResponseDirectory(_Option->getProjectType()));
        std::wstring gitUrl = _Option->getTemplate() != nullptr ? _Option->getTemplate()->getUrl() : L"";

        vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Check VCC Local response existance: " + localResponseDirectoryProject);
        bool isNeedToCloneGitResponse = false;
        if (vcc::isDirectoryExists(localResponseDirectoryProject)) {
            if (vcc::GitService::IsGitResponse(this->getLogConfig().get(), localResponseDirectoryProject)) {
                vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Done.");

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

                    vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Check Version.");
                    // check tag version
                    // if same as current version of generator, no action
                    // if not same, then check verison of genertor exists, if not exists, then master, else switch to correct branch
                    auto currentLog = vcc::GitService::getCurrentLog(this->getLogConfig().get(), localResponseDirectoryProject);
                    if (!vcc::isContain(currentLog->getTags(), VPGGlobal::getVersion())) {
                        std::wstring currentBranchName = L"";
                        TRY
                            auto currentTag = vcc::GitService::getCurrentTag(this->getLogConfig().get(), localResponseDirectoryProject);
                            if (vcc::isBlank(currentTag->getTagName()))
                                currentBranchName = vcc::GitService::getCurrentBranchName(this->getLogConfig().get(), localResponseDirectoryProject);
                        CATCH_SLIENT
                        // If version is main and current tag version not exists, then no switch
                        std::wstring mainBranch = L"main";
                        std::vector<std::wstring> allTags = vcc::GitService::getTags(this->getLogConfig().get(), localResponseDirectoryProject);
                        if (currentBranchName == L"main" && !vcc::isContain(allTags, VPGGlobal::getVersion())) {
                            vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Currently in main branch and " + VPGGlobal::getVersion() + L" is not found. Keep in main branch.");
                        } else {
                            isNeedToCloneGitResponse = true;
                            vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Outdated.");
                        }
                    }
                }
            } else {
                isNeedToCloneGitResponse = true;
                vcc::LogService::logInfo(this->getLogConfig().get(), L"", localResponseDirectoryProject + L" already exists.");
            }            
        } else {
            isNeedToCloneGitResponse = true;
            vcc::LogService::logInfo(this->getLogConfig().get(), L"", localResponseDirectoryProject + L" not Exists.");
        }
        if (isNeedToCloneGitResponse) {
            // Try to drop and create
            // Note: Window version will hold folder for a while because of some threads of project have not been terminated. Cannot use drop create
            try
            {
                if (vcc::isDirectoryExists(localResponseDirectoryProject)) {
                    vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"remove current response.");
                    vcc::removeDirectory(localResponseDirectoryProject);
                    vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Done.");
                }
            }
            catch(const std::exception& e)
            {
                std::string msg(e.what());
                vcc::LogService::LogWarning(this->getLogConfig().get(), L"", vcc::str2wstr(msg));
            }
            try
            {            
                vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Clone from " + gitUrl);
                vcc::GitCloneOption cloneOption;
                cloneOption.setIsQuiet(true);
                vcc::GitService::cloneGitResponse(this->getLogConfig().get(), localResponseDirectoryBase, gitUrl, &cloneOption);
                vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Done.");
            }
            catch(const std::exception& e)
            {
                std::string msg(e.what());
                vcc::LogService::LogWarning(this->getLogConfig().get(), L"", vcc::str2wstr(msg));
            }
            
            // Switch to correct version
            vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Switch to current version " + VPGGlobal::getVersion());
            try
            {
                vcc::GitService::Switch(this->getLogConfig().get(), localResponseDirectoryProject, VPGGlobal::getVersion());
                vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Done.");
            }
            catch(const std::exception& e)
            {
                try {
                    vcc::LogService::LogError(this->getLogConfig().get(), L"", L"VCC Project Generator version Not Exists. Switch to main");
                    vcc::GitService::Switch(this->getLogConfig().get(), localResponseDirectoryProject, L"main");
                    vcc::GitService::Pull(this->getLogConfig().get(), localResponseDirectoryProject);
                    vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Done.");
                } catch (const std::exception &e) {
                    vcc::LogService::LogWarning(this->getLogConfig().get(), L"", vcc::str2wstr(e.what()));
                }
            }
        }
    CATCH
}

bool VPGProcessManager::isUpdateAvaliable()
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

std::shared_ptr<IVPGGenerationManager> VPGProcessManager::getGenerationManager()
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

void VPGProcessManager::add()
{
    TRY
        this->verifyLocalResponse();
        getGenerationManager()->add();
    CATCH
}

void VPGProcessManager::update()
{
    TRY
        if (!isUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        this->verifyLocalResponse();
        getGenerationManager()->update();
    CATCH
}

void VPGProcessManager::generate()
{
    TRY
        if (!isUpdateAvaliable())
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Only VCC Module can be updated.");
        // No need to varify response for generate
        // this->verifyLocalResponse();
        getGenerationManager()->generate();
    CATCH
}

void VPGProcessManager::execute(const std::vector<std::wstring> &cmds)
{
    if (cmds.size() < 2)
        return;

    this->initLogConfig();

    _Workspace = vcc::getCurrentFolderPath();
    try {
        std::wstring mode = cmds[1];
        if (mode == L"-Version") {
            std::wcout << VPGGlobal::getVersion() << std::endl;
            return;        
        }

        // ensure no nullptr
        if (_Option->getTemplate() == nullptr)
            _Option->setTemplate(std::make_shared<VPGConfigTemplate>());
        
        for (size_t i = 2; i < cmds.size(); i++) {
            std::wstring cmd = cmds[i];
            if (!vcc::isStartWith(cmd, L"-")) {
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Argument missing - for " + cmd);
            }
            // single tag
            if (!vcc::isStartWith(cmd, L"--")) {
                // must have second argument
                if (i + 1 < cmds.size()) {
                    i++;
                    std::wstring cmd2 = cmds[i];
                    if (cmd == L"-workspace-destination") {
                        _Workspace = cmd2;
                    } else if (cmd == L"-interface") {
                        vcc::toUpper(cmd2);
                        if (vcc::isStartWith(cmd2, L"CPP")) {
                            if (vcc::isEndWith(cmd2, L"DLL"))
                                _Option->setProjectType(VPGProjectType::CppDll);
                            else if (vcc::isEndWith(cmd2, L"EXE"))
                                _Option->setProjectType(VPGProjectType::CppExe);
                            else if (vcc::isEndWith(cmd2, L"COMPLEX"))
                                _Option->setProjectType(VPGProjectType::CppComplex);
                            else
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd2);
                        } else if (vcc::isStartWith(cmd2, L"VCC")) {
                            if (vcc::isEndWith(cmd2, L"DLL"))
                                _Option->setProjectType(VPGProjectType::VccDll);
                            else if (vcc::isEndWith(cmd2, L"EXE"))
                                _Option->setProjectType(VPGProjectType::VccExe);
                            else if (vcc::isEndWith(cmd2, L"COMPLEX"))
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
            std::wstring vccJsonFilePath = vcc::concatPaths({_Workspace, VPGGlobal::getVccJsonFileName()});
            if (!vcc::isFilePresent(vccJsonFilePath))
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, vccJsonFilePath + L": File not found.");
            std::wstring fileContent = vcc::readFile(vccJsonFilePath);
            auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
            auto json = std::make_shared<vcc::Json>();
            jsonBuilder->deserialize(fileContent, json);
            _Option->deserializeJson(json);
            _Option->setProjectType(VPGProjectType::VccComplex);
        }

        std::wstring localResponseDirectory = _Option->getTemplate() != nullptr && !vcc::isBlank(_Option->getTemplate()->getWorkspace()) ? _Option->getTemplate()->getWorkspace() : VPGGlobal::getVccProjectLocalResponseDirectory(_Option->getProjectType());
        std::wstring gitUrl = _Option->getTemplate() != nullptr && !vcc::isBlank(_Option->getTemplate()->getUrl()) ? _Option->getTemplate()->getUrl() : VPGGlobal::getProjecURL(_Option->getProjectType());
        _Option->getTemplate()->setWorkspace(localResponseDirectory);
        _Option->getTemplate()->setUrl(gitUrl);

        if (mode == L"-Add")
            this->add();
        else if (mode == L"-Update")
            this->update();
        else if (mode == L"-Generate")
            this->generate();
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknow Mode " + mode);
        
        vcc::LogService::logInfo(this->_LogConfig.get(), L"VPGProcessManager", L"Process Complete Successfully!");
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}
