#include "vpg_vcc_generation_manager.hpp"

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "platform_type.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_global.hpp"

#define CLASS_ID L"VPGVccGenerationManager"

std::wstring VPGVccGenerationManager::adjustAppliationCpp(const std::wstring &fileContent) const
{
    std::wstring result = L"";
    TRY
        auto elements = std::make_shared<vcc::Xml>();
        VPGCodeReader reader(L"//");
        reader.deserialize(fileContent, elements);
        for (std::shared_ptr<vcc::Xml> element : elements->getChildren()) {
            if (element->getName() == L"vcc:vccconfig") {
                result += L"// <vcc:vccconfig sync=\"RESERVE\" gen=\"REPLACE\">\r\n";
                if (_Option->getBehavior()->getActionHistoryType() == VPGConfigActionHistoryType::Global)
                    result += INDENT + INDENT + L"_ActionManager = std::make_shared<ActionManager>(_LogConfig);\r\n";
                else
                    result += INDENT + INDENT + L"_ActionManager = nullptr;\r\n";
                result += INDENT + INDENT + L"// </vcc:vccconfig>";
            } else
                result += element->getFullText();
        }
    CATCH
    return result;
}

std::vector<std::wstring> VPGVccGenerationManager::getUpdateList() const
{
    std::vector<std::wstring> result;
    // VCC Core
    result.push_back(L"include/external/vcc/LICENSE");
    result.push_back(L"include/external/vcc/core/*");
    result.push_back(L"src/external/vcc/core/*");
    
    // Need to tmp create first, then override by Generate Mode
    // Otherwise, cannot be compiled
    // application
    
    // type
    result.push_back(vcc::concatPaths({_Option->getOutput() != nullptr ? _Option->getOutput()->getExceptionTypeDirectory() : L"", L"exception_type.hpp"}));
    result.push_back(vcc::concatPaths({_Option->getOutput() != nullptr ? _Option->getOutput()->getObjectTypeDirectory() : L"", L"object_type.hpp"}));
    
    // application
    if (_Option->getOutput() != nullptr && !vcc::isBlank(_Option->getOutput()->getApplicationDirectoryHpp()))
        result.push_back(vcc::concatPaths({_Option->getOutput()->getApplicationDirectoryHpp(), L"application.hpp"}));
    if (_Option->getOutput() != nullptr && !vcc::isBlank(_Option->getOutput()->getApplicationDirectoryCpp()))
        result.push_back(vcc::concatPaths({_Option->getOutput()->getApplicationDirectoryCpp(), L"application.cpp"}));

    // factory
    if (_Option->getOutput() != nullptr && !vcc::isBlank(_Option->getOutput()->getObjectFactoryDirectoryHpp()))
        result.push_back(vcc::concatPaths({_Option->getOutput()->getObjectFactoryDirectoryHpp(), L"object_factory.hpp"}));
    if (_Option->getOutput() != nullptr && !vcc::isBlank(_Option->getOutput()->getObjectFactoryDirectoryCpp()))
        result.push_back(vcc::concatPaths({_Option->getOutput()->getObjectFactoryDirectoryCpp(), L"object_factory.cpp"}));
    if (_Option->getOutput() != nullptr && !vcc::isBlank(_Option->getOutput()->getPropertyAccessorFactoryDirectoryHpp()))
        result.push_back(vcc::concatPaths({_Option->getOutput()->getPropertyAccessorFactoryDirectoryHpp(), L"property_accessor_factory.hpp"}));
    if (_Option->getOutput() != nullptr && !vcc::isBlank(_Option->getOutput()->getPropertyAccessorFactoryDirectoryCpp()))
        result.push_back(vcc::concatPaths({_Option->getOutput()->getPropertyAccessorFactoryDirectoryCpp(), L"property_accessor_factory.cpp"}));

    // plugins
    for (auto const &str : _Option->getPlugins()) {
        result.push_back(vcc::concatPaths({L"include/external/", str, L"*"}));
        result.push_back(vcc::concatPaths({L"src/external/", str, L"*"}));
    }
    return result;
}

std::vector<std::wstring> VPGVccGenerationManager::getUpdateUnitTestList() const
{
    std::vector<std::wstring> result;
    bool isExcludeUnittest = _Option->getTemplate() != nullptr && _Option->getTemplate()->getIsExcludeUnittest();
    bool isExcludeVCCUnittest = _Option->getTemplate() != nullptr && _Option->getTemplate()->getIsExcludeVCCUnitTest();
    if (!isExcludeVCCUnittest)
        result.push_back(L"external/vcc/core/*");

    for (auto const &str : _Option->getPlugins()) {
        if (_Option->getTemplate() == nullptr || !isExcludeUnittest) {
            if (!(vcc::isStartWith(str, L"vcc") && isExcludeVCCUnittest))
                result.push_back(vcc::concatPaths({L"external/", str, L"*"}));
        }
    }
    return result;    
}

void VPGVccGenerationManager::CreateVccJson(bool isNew) const
{
    TRY
        auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
        jsonBuilder->setIsBeautify(true);
        _Option->setVersion(VPGGlobal::getVersion());
        if (isNew) {
            if (_Option->getExports().empty())
                _Option->insertExports(std::make_shared<VPGConfigExport>());
        }
        vcc::appendFileOneLine(vcc::concatPaths({_Workspace, VPGGlobal::getVccJsonFileName()}), _Option->serializeJson(jsonBuilder.get()), true);
    CATCH
}

void VPGVccGenerationManager::ReadVccJson() const
{
    TRY
        std::wstring fileContent = vcc::readFile(vcc::concatPaths({_Workspace, VPGGlobal::getVccJsonFileName()}));
        auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
        auto json = std::make_shared<vcc::Json>();
        jsonBuilder->deserialize(fileContent, json);
        _Option->deserializeJson(json);
    CATCH
}

void VPGVccGenerationManager::add() const
{
    TRY
        assert(_Option->getTemplate() != nullptr);
        VPGBaseGenerationManager::CreateBasicProject();
        std::wstring src = VPGGlobal::getConvertedPath(_Option->getTemplate()->getWorkspace());
        std::wstring dest = _Workspace;
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"Copy Project to " + dest + L" ...");
        vcc::CopyDirectoryOption copyDirectoryOption;
        copyDirectoryOption.setIsForce(true);
        copyDirectoryOption.setIsRecursive(true);
        
        for (auto const &str : getUpdateList())
            copyDirectoryOption.insertIncludeFileFilters(str);
        vcc::copyDirectory(src, dest, &copyDirectoryOption);

        // handle unittest in next loop as unit test name can be changed
        if (_Option->getTemplate() == nullptr || !_Option->getTemplate()->getIsExcludeUnittest()) {
            copyDirectoryOption.clearIncludeFileFilters();
            for (auto const &str : getUpdateUnitTestList())
                copyDirectoryOption.insertIncludeFileFilters(str);
            if (!copyDirectoryOption.getIncludeFileFilters().empty())
                vcc::copyDirectory(vcc::concatPaths({src, unittestFolderName}), vcc::concatPaths({dest, unittestFolderName}), &copyDirectoryOption);
        }
        
        // Create Json file at the end to force override
        CreateVccJson(true);
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"Done");
    CATCH
}

void VPGVccGenerationManager::update() const
{
    TRY
        ReadVccJson();

        assert(_Option->getTemplate() != nullptr);
        std::wstring src = VPGGlobal::getConvertedPath(_Option->getTemplate()->getWorkspace());
        std::wstring dest = _Workspace;
        
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"Sync Project ...");
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"From " + src);
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"To " + dest);

        syncWorkspace(this->_LogConfig.get(), src, dest, getUpdateList(), {});
        
        if (_Option->getTemplate() == nullptr || !_Option->getTemplate()->getIsExcludeUnittest()) {
            auto list = getUpdateUnitTestList();
            if (!list.empty())
                syncWorkspace(this->_LogConfig.get(), vcc::concatPaths({src, unittestFolderName}), vcc::concatPaths({dest, unittestFolderName}), list, {});
        }

        // Update Makefile and unittest
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"Update Project according to vcc.json");
        if (!vcc::isFilePresent(vcc::concatPaths({dest, MakeFileName})))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Cannot find " + vcc::concatPaths({dest, MakeFileName}));
        
        // Makefile
        std::wstring makefilePath = vcc::concatPaths({dest, MakeFileName});
        std::wstring makefileContent = vcc::readFile(vcc::concatPaths({dest, MakeFileName}));
        vcc::writeFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        
        // Update application
        std::wstring applicationFilePath = vcc::concatPaths({dest, _Option->getOutput()->getApplicationDirectoryCpp(), L"application.cpp"});
        if (vcc::isFilePresent(applicationFilePath))
            vcc::writeFile(applicationFilePath, this->adjustAppliationCpp(vcc::readFile(applicationFilePath)), true);

        // Create Json file at the end to force override
        CreateVccJson(false);
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"Done");        
    CATCH
}

void VPGVccGenerationManager::generate() const
{
    TRY
        ReadVccJson();
        
        // Update Makefile
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"Update Project according to vcc.json");
        if (!vcc::isFilePresent(vcc::concatPaths({_Workspace, MakeFileName})))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Cannot find " + vcc::concatPaths({_Workspace, MakeFileName}));
        std::wstring makefilePath = vcc::concatPaths({_Workspace, MakeFileName});
        std::wstring makefileContent = vcc::readFile(vcc::concatPaths({_Workspace, MakeFileName}));
        vcc::writeFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        
        // Update application
        std::wstring applicationFilePath = vcc::concatPaths({_Workspace, _Option->getOutput()->getApplicationDirectoryCpp(), L"application.cpp"});
        if (vcc::isFilePresent(applicationFilePath))
            vcc::writeFile(applicationFilePath, this->adjustAppliationCpp(vcc::readFile(applicationFilePath)), true);

        auto manager = std::make_unique<VPGFileGenerationManager>(this->_LogConfig, _Workspace);
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"Generate Project ...");
        manager->generateProperty(_LogConfig.get(), _Option.get());
        vcc::LogService::logInfo(this->_LogConfig.get(), CLASS_ID, L"Done");
    CATCH
}