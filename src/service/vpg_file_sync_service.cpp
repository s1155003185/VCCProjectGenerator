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

using namespace vcc;

const std::wstring VCC_NAMESPACE = L"vcc";
const std::wstring VCC_NAME = VCC_NAMESPACE + L":vccproj";

constexpr auto SYNC_TOKEN_SHORT = L"sync";
constexpr auto SYNC_TOKEN_LONG = L"synchronization";

constexpr auto GEN_TOKEN_SHORT = L"gen";
constexpr auto GEN_TOKEN_LONG = L"generation";

// xml
constexpr auto FULL_MODE = L"FULL";
constexpr auto DEMAND_MODE = L"DEMAND";
constexpr auto SKIP_MODE = L"SKIP";
constexpr auto FORCE_MODE = L"FORCE";

// tag
constexpr auto REPLACE_TAG = L"REPLACE";
constexpr auto RESERVE_TAG = L"RESERVE";

bool VPGFileSyncService::IsSyncTag(const VPGFileContentSyncTagMode &mode, const std::wstring &tag)
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

VPGFileContentSyncMode VPGFileSyncService::GetSyncMode(const VPGFileContentSyncTagMode &mode, const Xml *codeElemet)
{
    TRY
        for (std::shared_ptr<Xml> child : codeElemet->GetChildren()) {
            if (child->GetName() == VCC_NAME) {
                for (std::shared_ptr<XmlAttribute> attr : child->GetAttributes()) {
                    if (IsSyncTag(mode, attr->GetName())) {
                        std::wstring value = attr->GetValue();
                        ToUpper(value);
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

const Xml *VPGFileSyncService::GetTagFromCode(const Xml *code, const std::wstring tagName)
{
    TRY
        for (size_t i = 0; i < code->GetChildren().size(); i++) {
            if (code->GetChildren().at(i)->GetName() == tagName)
                return code->GetChildren().at(i).get();
        }
    CATCH
    return nullptr;
}

bool VPGFileSyncService::IsTagReplace(const VPGFileContentSyncTagMode &mode, const Xml *child)
{
    TRY
        for (std::shared_ptr<XmlAttribute> attr : child->GetAttributes()){
            if (IsSyncTag(mode, attr->GetName())) {
                std::wstring value = attr->GetValue();
                ToUpper(value);
                return value == REPLACE_TAG;
            }
        }
    CATCH
    return false;
}

bool VPGFileSyncService::IsTagReserve(const VPGFileContentSyncTagMode &mode, const Xml *child)
{
    TRY
        for (std::shared_ptr<XmlAttribute> attr : child->GetAttributes()){
            if (IsSyncTag(mode, attr->GetName())) {
                std::wstring value = attr->GetValue();
                ToUpper(value);
                return value == RESERVE_TAG;
            }
        }
    CATCH
    return false;
}

void VPGFileSyncService::CopyFile(const LogConfig *logConfig, const VPGFileContentSyncTagMode &mode, const std::wstring &sourcePath, const std::wstring &originalCodePath)
{
    TRY
        if (!IsFilePresent(sourcePath))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, sourcePath + L": File not found.");
\
        if (IsFilePresent(originalCodePath)) {
            std::wstring commandDelimiter = GetFileName(sourcePath) == L"Makefile" ? L"#" : L"//";
            std::wstring fileContent = VPGFileSyncService::SyncFileContent(mode, ReadFile(sourcePath), ReadFile(originalCodePath), VPGFileContentSyncMode::Demand, commandDelimiter);
            WriteFile(originalCodePath, fileContent, true);
            LogService::LogInfo(logConfig, L"", L"Updated File: " + originalCodePath);
        } else {
            std::filesystem::copy_file(PATH(sourcePath), PATH(originalCodePath), std::filesystem::copy_options::overwrite_existing);
            LogService::LogInfo(logConfig, L"", L"Added File: " + originalCodePath);
        }
    CATCH
}

std::wstring VPGFileSyncService::GenerateForceCode(const VPGFileContentSyncMode updatedCodeMode, const VPGFileContentSyncMode originalCodeMode, const Xml *updatedCode, const Xml *originalCode)
{
    std::wstring result = L"";
    TRY
        if (originalCodeMode == VPGFileContentSyncMode::NA) {
            for (std::shared_ptr<Xml> child : updatedCode->GetChildren())
                result += child->GetFullText();
        } else {
            for (std::shared_ptr<Xml> child : originalCode->GetChildren()) {
                result += child->GetFullText();
                if (child->GetName() == VCC_NAME)
                    break;
            }
            bool shouldSkip = updatedCodeMode != VPGFileContentSyncMode::NA;
            for (std::shared_ptr<Xml> child : updatedCode->GetChildren()) {
                if (!shouldSkip)
                    result += child->GetFullText();
                if (child->GetName() == VCC_NAME)
                    shouldSkip = false;
            }
        }
    CATCH
    return result;
}

std::wstring VPGFileSyncService::GenerateFullCode(const VPGFileContentSyncTagMode &mode, const VPGFileContentSyncMode updatedCodeMode, const VPGFileContentSyncMode originalCodeMode, const Xml *updatedCode, const Xml *originalCode)
{
    std::wstring result = L"";
    TRY
        // if originalCode has header then add header, skip source header
        if (originalCodeMode != VPGFileContentSyncMode::NA) {
            for (std::shared_ptr<Xml> child : originalCode->GetChildren()) {
                result += child->GetFullText();
                if (child->GetName() == VCC_NAME)
                    break;
            }
        }
        bool shouldSkip = updatedCodeMode != VPGFileContentSyncMode::NA;
        for (std::shared_ptr<Xml> child : updatedCode->GetChildren()) {
            if (!shouldSkip) {
                // if find tag then search tag in source, if reserve, then use source
                if (IsStartWith(child->GetName(), L"vcc:")) {
                    const Xml *originalCodeTag = VPGFileSyncService::GetTagFromCode(originalCode, child->GetName());
                    if (originalCodeTag != nullptr && VPGFileSyncService::IsTagReserve(mode, originalCodeTag)) {
                        result += originalCodeTag->GetFullText();
                    } else
                        result += child->GetFullText();
                } else
                    result += child->GetFullText();
            }
            if (child->GetName() == VCC_NAME)
                shouldSkip = false;
        }
    CATCH
    return result;
}

std::wstring VPGFileSyncService::GenerateDemandCode(const VPGFileContentSyncTagMode &mode, const VPGFileContentSyncMode updatedCodeMode, const VPGFileContentSyncMode originalCodeMode, const Xml *updatedCode, const Xml *originalCode)
{
    std::wstring result = L"";
    TRY
        // if originalCode has header then add header, skip source header
        if (originalCodeMode != VPGFileContentSyncMode::NA) {
            for (std::shared_ptr<Xml> child : originalCode->GetChildren()) {
                result += child->GetFullText();
                if (child->GetName() == VCC_NAME)
                    break;
            }
        }
        bool shouldSkip = updatedCodeMode != VPGFileContentSyncMode::NA && originalCodeMode != VPGFileContentSyncMode::NA;
        for (std::shared_ptr<Xml> child : originalCode->GetChildren()) {
            if (!shouldSkip) {
                // if find tag then search tag in source, if reserve, then use source
                if (IsStartWith(child->GetName(), L"vcc:")) {
                    const Xml *updatedCodeTag = VPGFileSyncService::GetTagFromCode(updatedCode, child->GetName());
                    if (updatedCodeTag != nullptr && VPGFileSyncService::IsTagReplace(mode, child.get())) {
                        result += updatedCodeTag->GetFullText();
                    } else
                        result += child->GetFullText();
                } else
                    result += child->GetFullText();
            }
            if (child->GetName() == VCC_NAME)
                shouldSkip = false;
        }
    CATCH
    return result;
}

std::wstring VPGFileSyncService::GenerateSkipCode(const std::wstring &originalCode)
{
    return originalCode;
}

std::wstring VPGFileSyncService::SyncFileContent(const VPGFileContentSyncTagMode &mode, const std::wstring &updatedCode, const std::wstring &originalCode, const VPGFileContentSyncMode defaultMode, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    TRY
        std::unique_ptr<VPGCodeReader> reader = std::make_unique<VPGCodeReader>(commandDelimiter);
        auto updatedCodeElement = std::make_shared<Xml>();
        auto originalCodeElement = std::make_shared<Xml>();
        reader->Deserialize(updatedCode, updatedCodeElement);
        reader->Deserialize(originalCode, originalCodeElement);

        VPGFileContentSyncMode updatedCodeSyncMode = GetSyncMode(mode, updatedCodeElement.get());
        VPGFileContentSyncMode originalCodeSyncMode = GetSyncMode(mode, originalCodeElement.get());

        // read first line to determine sync mode. originalCode first, then updatedCode. If not exists, then default
        VPGFileContentSyncMode syncMode = originalCodeSyncMode != VPGFileContentSyncMode::NA ? originalCodeSyncMode : (updatedCodeSyncMode != VPGFileContentSyncMode::NA ? updatedCodeSyncMode : defaultMode);
        switch (syncMode)
        {
        case VPGFileContentSyncMode::Force:
            return VPGFileSyncService::GenerateForceCode(updatedCodeSyncMode, originalCodeSyncMode, updatedCodeElement.get(), originalCodeElement.get());
        case VPGFileContentSyncMode::Full:
            return VPGFileSyncService::GenerateFullCode(mode, updatedCodeSyncMode, originalCodeSyncMode, updatedCodeElement.get(), originalCodeElement.get());
        case VPGFileContentSyncMode::Demand:
            return VPGFileSyncService::GenerateDemandCode(mode, updatedCodeSyncMode, originalCodeSyncMode, updatedCodeElement.get(), originalCodeElement.get());
        case VPGFileContentSyncMode::Skip:
            return VPGFileSyncService::GenerateSkipCode(originalCode);
        default:
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown File Content Sync Mode");
            break;
        }
    CATCH
    return result;
}