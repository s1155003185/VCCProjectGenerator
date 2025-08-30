#include "vpg_file_sync_service.hpp"

#include <filesystem>
#include <memory>
#include <string>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "xml_builder.hpp"

#include "vpg_code_reader.hpp"
#include "vpg_file_sync_service.hpp"

const std::wstring VCC_NAMESPACE = L"vcc";
const std::wstring VCC_NAME = VCC_NAMESPACE + L":vccproj";

constexpr auto SYNC_TOKEN_SHORT = L"sync";
constexpr auto SYNC_TOKEN_LONG = L"synchronization";

constexpr auto GEN_TOKEN_SHORT = L"gen";
constexpr auto GEN_TOKEN_LONG = L"generation";

// vcc::Xml
constexpr auto FULL_MODE = L"FULL";
constexpr auto DEMAND_MODE = L"DEMAND";
constexpr auto SKIP_MODE = L"SKIP";
constexpr auto FORCE_MODE = L"FORCE";

// tag
constexpr auto REPLACE_TAG = L"REPLACE";
constexpr auto RESERVE_TAG = L"RESERVE";

bool VPGFileSyncService::isSyncTag(const VPGFileContentSyncTagMode &mode, const std::wstring &tag)
{
    switch (mode)
    {
    case VPGFileContentSyncTagMode::Synchronization:
        return tag == SYNC_TOKEN_LONG || tag == SYNC_TOKEN_SHORT;
    case VPGFileContentSyncTagMode::Generation:
        return tag == GEN_TOKEN_SHORT || tag == GEN_TOKEN_LONG;
    default:
        assert(false);
        break;
    }
    return false;
}

VPGFileContentSyncMode VPGFileSyncService::getSyncMode(const VPGFileContentSyncTagMode &mode, const vcc::Xml *codeElemet)
{
    TRY
        for (std::shared_ptr<vcc::Xml> child : codeElemet->getChildren()) {
            if (child->getName() == VCC_NAME) {
                for (std::shared_ptr<vcc::XmlAttribute> attr : child->getAttributes()) {
                    if (isSyncTag(mode, attr->getName())) {
                        std::wstring value = attr->getValue();
                        vcc::toUpper(value);
                        if (value == FORCE_MODE) {
                            return VPGFileContentSyncMode::Force;
                        } else if (value == FULL_MODE) {
                            return VPGFileContentSyncMode::Full;
                        } else if (value == DEMAND_MODE) {
                            return VPGFileContentSyncMode::Demand;
                        } else if (value == SKIP_MODE) {
                            return VPGFileContentSyncMode::Skip;
                        }
                    }
                }
                break;
            }
        }
    CATCH
    return VPGFileContentSyncMode::NA;
}

const vcc::Xml *VPGFileSyncService::getTagFromCode(const vcc::Xml *code, const std::wstring tagName)
{
    TRY
        for (size_t i = 0; i < code->getChildren().size(); i++) {
            if (code->getChildren().at(i)->getName() == tagName)
                return code->getChildren().at(i).get();
        }
    CATCH
    return nullptr;
}

bool VPGFileSyncService::IsTagReplace(const VPGFileContentSyncTagMode &mode, const vcc::Xml *child)
{
    TRY
        for (std::shared_ptr<vcc::XmlAttribute> attr : child->getAttributes()){
            if (isSyncTag(mode, attr->getName())) {
                std::wstring value = attr->getValue();
                vcc::toUpper(value);
                return value == REPLACE_TAG;
            }
        }
    CATCH
    return false;
}

bool VPGFileSyncService::IsTagReserve(const VPGFileContentSyncTagMode &mode, const vcc::Xml *child)
{
    TRY
        for (std::shared_ptr<vcc::XmlAttribute> attr : child->getAttributes()){
            if (isSyncTag(mode, attr->getName())) {
                std::wstring value = attr->getValue();
                vcc::toUpper(value);
                return value == RESERVE_TAG;
            }
        }
    CATCH
    return false;
}

void VPGFileSyncService::copyFile(const vcc::LogConfig *logConfig, const VPGFileContentSyncTagMode &mode, const std::wstring &sourcePath, const std::wstring &originalCodePath)
{
    TRY
        if (!vcc::isFilePresent(sourcePath))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, sourcePath + L": File not found.");
\
        if (vcc::isFilePresent(originalCodePath)) {
            std::wstring commandDelimiter = vcc::getFileName(sourcePath) == L"Makefile" ? L"#" : L"//";
            std::wstring fileContent = VPGFileSyncService::SyncFileContent(mode, vcc::readFile(sourcePath), vcc::readFile(originalCodePath), VPGFileContentSyncMode::Demand, commandDelimiter);
            vcc::writeFile(originalCodePath, fileContent, true);
            vcc::LogService::logInfo(logConfig, L"", L"Updated File: " + originalCodePath);
        } else {
            std::filesystem::copy_file(PATH(sourcePath), PATH(originalCodePath), std::filesystem::copy_options::overwrite_existing);
            vcc::LogService::logInfo(logConfig, L"", L"Added File: " + originalCodePath);
        }
    CATCH
}

std::wstring VPGFileSyncService::generateForceCode(const VPGFileContentSyncMode updatedCodeMode, const VPGFileContentSyncMode originalCodeMode, const vcc::Xml *updatedCode, const vcc::Xml *originalCode)
{
    std::wstring result = L"";
    TRY
        if (originalCodeMode == VPGFileContentSyncMode::NA) {
            for (std::shared_ptr<vcc::Xml> child : updatedCode->getChildren())
                result += child->getFullText();
        } else {
            for (std::shared_ptr<vcc::Xml> child : originalCode->getChildren()) {
                result += child->getFullText();
                if (child->getName() == VCC_NAME)
                    break;
            }
            bool shouldSkip = updatedCodeMode != VPGFileContentSyncMode::NA;
            for (std::shared_ptr<vcc::Xml> child : updatedCode->getChildren()) {
                if (!shouldSkip)
                    result += child->getFullText();
                if (child->getName() == VCC_NAME)
                    shouldSkip = false;
            }
        }
    CATCH
    return result;
}

std::wstring VPGFileSyncService::GenerateFullCode(const VPGFileContentSyncTagMode &mode, const VPGFileContentSyncMode updatedCodeMode, const VPGFileContentSyncMode originalCodeMode, const vcc::Xml *updatedCode, const vcc::Xml *originalCode)
{
    std::wstring result = L"";
    TRY
        // if originalCode has header then add header, skip source header
        if (originalCodeMode != VPGFileContentSyncMode::NA) {
            for (std::shared_ptr<vcc::Xml> child : originalCode->getChildren()) {
                result += child->getFullText();
                if (child->getName() == VCC_NAME)
                    break;
            }
        }
        bool shouldSkip = updatedCodeMode != VPGFileContentSyncMode::NA;
        for (std::shared_ptr<vcc::Xml> child : updatedCode->getChildren()) {
            if (!shouldSkip) {
                // if find tag then search tag in source, if reserve, then use source
                if (vcc::isStartWith(child->getName(), L"vcc:")) {
                    const vcc::Xml *originalCodeTag = VPGFileSyncService::getTagFromCode(originalCode, child->getName());
                    if (originalCodeTag != nullptr && VPGFileSyncService::IsTagReserve(mode, originalCodeTag)) {
                        result += originalCodeTag->getFullText();
                    } else
                        result += child->getFullText();
                } else
                    result += child->getFullText();
            }
            if (child->getName() == VCC_NAME)
                shouldSkip = false;
        }
    CATCH
    return result;
}

std::wstring VPGFileSyncService::generateDemandCode(const VPGFileContentSyncTagMode &mode, const VPGFileContentSyncMode updatedCodeMode, const VPGFileContentSyncMode originalCodeMode, const vcc::Xml *updatedCode, const vcc::Xml *originalCode)
{
    std::wstring result = L"";
    TRY
        // if originalCode has header then add header, skip source header
        if (originalCodeMode != VPGFileContentSyncMode::NA) {
            for (std::shared_ptr<vcc::Xml> child : originalCode->getChildren()) {
                result += child->getFullText();
                if (child->getName() == VCC_NAME)
                    break;
            }
        }
        bool shouldSkip = updatedCodeMode != VPGFileContentSyncMode::NA && originalCodeMode != VPGFileContentSyncMode::NA;
        for (std::shared_ptr<vcc::Xml> child : originalCode->getChildren()) {
            if (!shouldSkip) {
                // if find tag then search tag in source, if reserve, then use source
                if (vcc::isStartWith(child->getName(), L"vcc:")) {
                    const vcc::Xml *updatedCodeTag = VPGFileSyncService::getTagFromCode(updatedCode, child->getName());
                    if (updatedCodeTag != nullptr && VPGFileSyncService::IsTagReplace(mode, child.get())) {
                        result += updatedCodeTag->getFullText();
                    } else
                        result += child->getFullText();
                } else
                    result += child->getFullText();
            }
            if (child->getName() == VCC_NAME)
                shouldSkip = false;
        }
    CATCH
    return result;
}

std::wstring VPGFileSyncService::generateSkipCode(const std::wstring &originalCode)
{
    return originalCode;
}

std::wstring VPGFileSyncService::SyncFileContent(const VPGFileContentSyncTagMode &mode, const std::wstring &updatedCode, const std::wstring &originalCode, const VPGFileContentSyncMode defaultMode, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    TRY
        std::unique_ptr<VPGCodeReader> reader = std::make_unique<VPGCodeReader>(commandDelimiter);
        auto updatedCodeElement = std::make_shared<vcc::Xml>();
        auto originalCodeElement = std::make_shared<vcc::Xml>();
        reader->deserialize(updatedCode, updatedCodeElement);
        reader->deserialize(originalCode, originalCodeElement);

        VPGFileContentSyncMode updatedCodeSyncMode = getSyncMode(mode, updatedCodeElement.get());
        VPGFileContentSyncMode originalCodeSyncMode = getSyncMode(mode, originalCodeElement.get());

        // read first line to determine sync mode. originalCode first, then updatedCode. If not exists, then default
        VPGFileContentSyncMode syncMode = originalCodeSyncMode != VPGFileContentSyncMode::NA ? originalCodeSyncMode : (updatedCodeSyncMode != VPGFileContentSyncMode::NA ? updatedCodeSyncMode : defaultMode);
        switch (syncMode)
        {
        case VPGFileContentSyncMode::Force:
            return VPGFileSyncService::generateForceCode(updatedCodeSyncMode, originalCodeSyncMode, updatedCodeElement.get(), originalCodeElement.get());
        case VPGFileContentSyncMode::Full:
            return VPGFileSyncService::GenerateFullCode(mode, updatedCodeSyncMode, originalCodeSyncMode, updatedCodeElement.get(), originalCodeElement.get());
        case VPGFileContentSyncMode::Demand:
            return VPGFileSyncService::generateDemandCode(mode, updatedCodeSyncMode, originalCodeSyncMode, updatedCodeElement.get(), originalCodeElement.get());
        case VPGFileContentSyncMode::Skip:
            return VPGFileSyncService::generateSkipCode(originalCode);
        default:
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown File Content Sync Mode");
            break;
        }
    CATCH
    return result;
}