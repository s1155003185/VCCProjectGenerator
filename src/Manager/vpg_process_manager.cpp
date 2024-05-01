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
#include "vpg_base_generation_manager.hpp"
#include "vpg_cpp_generation_manager.hpp"
#include "vpg_project_type.hpp"
#include "vpg_vcc_generation_manager.hpp"

using namespace vcc;

void VPGProcessManager::InitLogProperty()
{
    if (this->_LogProperty != nullptr)
        this->_LogProperty->SetIsConsoleLog(true);
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

            GitService::Pull(this->GetLogProperty().get(), localResponseDirectory);
            if (_Option->GetProjectType() == VPGProjectType::VccComplex
                || _Option->GetProjectType() == VPGProjectType::VccDll
                || _Option->GetProjectType() == VPGProjectType::VccExe) {

                LogService::LogInfo(this->GetLogProperty().get(), L"", L"Check Version.");
                // check tag version
                // if same as current version of generator, no action
                // if not same, then check verison of genertor exists, if not exists, then master, else switch to correct branch
                DECLARE_SPTR(GitLog, currentLog);
                GitService::GetCurrentLog(this->GetLogProperty().get(), VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType()), currentLog);
                if (!Contains(currentLog->GetTags(), VPGGlobal::GetVersion())) {
                    try
                    {
                        GitService::SwitchTag(this->GetLogProperty().get(), VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType()), VPGGlobal::GetVersion());
                        LogService::LogInfo(this->GetLogProperty().get(), L"", L"Done.");
                    }
                    catch(const std::exception& e)
                    {
                        try {
                            LogService::LogError(this->GetLogProperty().get(), L"", L"VCC Project Generator version Not Exists. Switch to main.");
                            GitService::SwitchTag(this->GetLogProperty().get(), VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType()), L"main");
                            GitService::Pull(this->GetLogProperty().get(), VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType()));
                            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Done.");
                        } catch (const std::exception &e) {
                            LogService::LogWarning(this->GetLogProperty().get(), L"", str2wstr(e.what()));
                        }
                    }
                }
            }
        } else {
            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Not Exists.");
            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Clone from " + gitUrl);
            GitCloneOption cloneOption;
            cloneOption.SetIsQuiet(true);
            GitService::Clone(this->GetLogProperty().get(),  VPGGlobal::GetVccLocalResponseFolder(), gitUrl, &cloneOption);
            LogService::LogInfo(this->GetLogProperty().get(), L"", L"Done.");
            // switch to correct branch
            VerifyLocalResponse();
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
        if (mode == L"-Version") {
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
                    if (cmd == L"-workspace-destination") {
                        _Option->SetWorkspaceDestination(cmd2);
                    } else if (cmd == L"-interface") {
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
                    _Option->SetProjectNameGtest(L"");
                else if (cmd == L"--ExcludeExternalUnitTest")
                    _Option->SetIsExcludeVCCUnitTest(true);
                else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown argument " + cmd);
            }
        }

        if (mode == L"-Add") {
            // validation
            if (this->_Option->GetProjectType() == VPGProjectType::NA)
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Interface Type missing");
        } else {
            std::wstring fileContent = ReadFile(ConcatPaths({_Option->GetWorkspaceDestination(), VPGGlobal::GetVccJsonFileName()}));
            DECLARE_UPTR(JsonBuilder, jsonBuilder);
            DECLARE_SPTR(Json, json);
            jsonBuilder->Deserialize(fileContent, json);
            _Option->DeserializeJson(json);
            _Option->SetProjectType(VPGProjectType::VccComplex);
        }

        std::wstring localResponseDirectory = !_Option->GetWorkspaceSource().empty() ? _Option->GetWorkspaceSource() : VPGGlobal::GetVccProjectLocalResponseDirectory(_Option->GetProjectType());
        std::wstring gitUrl = !_Option->GetWorkspaceSourceGitUrl().empty() ? _Option->GetWorkspaceSourceGitUrl() : VPGGlobal::GetProjecURL(_Option->GetProjectType());
        _Option->SetWorkspaceSource(localResponseDirectory);
        _Option->SetWorkspaceSourceGitUrl(gitUrl);

        if (mode == L"-Add")
            this->Add();
        else if (mode == L"-Update")
            this->Update();
        else if (mode == L"-Generate")
            this->Generate();
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknow Mode " + mode);
        
        LogService::LogInfo(this->_LogProperty.get(), L"VPGProcessManager", L"Process Complete Successfully!");
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}
