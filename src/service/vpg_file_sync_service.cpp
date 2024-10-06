#include "vpg_file_sync_service.hpp"

#include <filesystem>
#include <memory>
#include <string>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "memory_macro.hpp"
#include "xml_builder.hpp"

#include "vpg_code_reader.hpp"
#include "vpg_file_sync_service.hpp"

using namespace vcc;

const std::wstring VCC_NAMESPACE = L"vcc";
const std::wstring VCC_NAME = VCC_NAMESPACE + L":vccproj";

constexpr auto SYNC_TOKEN_SHORT = L"sync";
constexpr auto SYNC_TOKEN_LONG = L"synchronization";

// xml
constexpr auto FULL_MODE = L"FULL";
constexpr auto DEMAND_MODE = L"DEMAND";
constexpr auto SKIP_MODE = L"SKIP";
constexpr auto FORCE_MODE = L"FORCE";

// tag
constexpr auto REPLACE_TAG = L"REPLACE";
constexpr auto RESERVE_TAG = L"RESERVE";

bool VPGFileSyncService::IsSyncTag(const std::wstring &tag)
{
    return tag == SYNC_TOKEN_LONG || tag == SYNC_TOKEN_SHORT;
}

VPGFileContentSyncMode VPGFileSyncService::GetSyncMode(const Xml *codeElemet)
{
    TRY
        for (std::shared_ptr<Xml> child : codeElemet->GetChildren()) {
            if (child->GetName() == VCC_NAME) {
                for (std::shared_ptr<XmlAttribute> attr : child->GetAttributes()) {
                    if (IsSyncTag(attr->GetName())) {
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

bool VPGFileSyncService::IsTagReplace(const Xml *child)
{
    TRY
        for (std::shared_ptr<XmlAttribute> attr : child->GetAttributes()){
            if (IsSyncTag(attr->GetName())) {
                std::wstring value = attr->GetValue();
                ToUpper(value);
                return value == REPLACE_TAG;
            }
        }
    CATCH
    return false;
}

bool VPGFileSyncService::IsTagReserve(const Xml *child)
{
    TRY
        for (std::shared_ptr<XmlAttribute> attr : child->GetAttributes()){
            if (IsSyncTag(attr->GetName())) {
                std::wstring value = attr->GetValue();
                ToUpper(value);
                return value == RESERVE_TAG;
            }
        }
    CATCH
    return false;
}

void VPGFileSyncService::CopyFile(const LogProperty *logProperty, const std::wstring &sourcePath, const std::wstring &destPath)
{
    TRY
        if (!IsFileExists(sourcePath))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, sourcePath + L": File not found.");
\
        if (IsFileExists(destPath)) {
            std::wstring commandDelimiter = GetFileName(sourcePath) == L"Makefile" ? L"#" : L"//";
            std::wstring fileContent = VPGFileSyncService::SyncFileContent(ReadFile(sourcePath), ReadFile(destPath), VPGFileContentSyncMode::Demand, commandDelimiter);
            WriteFile(destPath, fileContent, true);
            LogService::LogInfo(logProperty, L"", L"Updated File: " + destPath);
        } else {
            std::filesystem::copy_file(PATH(sourcePath), PATH(destPath), std::filesystem::copy_options::overwrite_existing);
            LogService::LogInfo(logProperty, L"", L"Added File: " + destPath);
        }
    CATCH
}

std::wstring VPGFileSyncService::GenerateForceCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const Xml *src, const Xml *dest)
{
    std::wstring result = L"";
    TRY
        if (destMode == VPGFileContentSyncMode::NA) {
            for (std::shared_ptr<Xml> child : src->GetChildren())
                result += child->GetFullText();
        } else {
            for (std::shared_ptr<Xml> child : dest->GetChildren()) {
                result += child->GetFullText();
                if (child->GetName() == VCC_NAME)
                    break;
            }
            bool shouldSkip = srcMode != VPGFileContentSyncMode::NA;
            for (std::shared_ptr<Xml> child : src->GetChildren()) {
                if (!shouldSkip)
                    result += child->GetFullText();
                if (child->GetName() == VCC_NAME)
                    shouldSkip = false;
            }
        }
    CATCH
    return result;
}

std::wstring VPGFileSyncService::GenerateFullCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const Xml *src, const Xml *dest)
{
    std::wstring result = L"";
    TRY
        // if dest has header then add header, skip source header
        if (destMode != VPGFileContentSyncMode::NA) {
            for (std::shared_ptr<Xml> child : dest->GetChildren()) {
                result += child->GetFullText();
                if (child->GetName() == VCC_NAME)
                    break;
            }
        }
        bool shouldSkip = srcMode != VPGFileContentSyncMode::NA;
        for (std::shared_ptr<Xml> child : src->GetChildren()) {
            if (!shouldSkip) {
                // if find tag then search tag in source, if reserve, then use source
                if (IsStartWith(child->GetName(), L"vcc:")) {
                    const Xml *destTag = VPGFileSyncService::GetTagFromCode(dest, child->GetName());
                    if (destTag != nullptr && VPGFileSyncService::IsTagReserve(destTag)) {
                        result += destTag->GetFullText();
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

std::wstring VPGFileSyncService::GenerateDemandCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const Xml *src, const Xml *dest)
{
    std::wstring result = L"";
    TRY
        // if dest has header then add header, skip source header
        if (destMode != VPGFileContentSyncMode::NA) {
            for (std::shared_ptr<Xml> child : dest->GetChildren()) {
                result += child->GetFullText();
                if (child->GetName() == VCC_NAME)
                    break;
            }
        }
        bool shouldSkip = srcMode != VPGFileContentSyncMode::NA;
        for (std::shared_ptr<Xml> child : dest->GetChildren()) {
            if (!shouldSkip) {
                // if find tag then search tag in source, if reserve, then use source
                if (IsStartWith(child->GetName(), L"vcc:")) {
                    const Xml *srcTag = VPGFileSyncService::GetTagFromCode(src, child->GetName());
                    if (srcTag != nullptr && VPGFileSyncService::IsTagReplace(child.get())) {
                        result += srcTag->GetFullText();
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

std::wstring VPGFileSyncService::GenerateSkipCode(const std::wstring &dest)
{
    return dest;
}

std::wstring VPGFileSyncService::SyncFileContent(const std::wstring &src, const std::wstring &dest, const VPGFileContentSyncMode defaultMode, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    TRY
        std::unique_ptr<VPGCodeReader> reader = std::make_unique<VPGCodeReader>(commandDelimiter);
        DECLARE_SPTR(Xml, srcElement);
        DECLARE_SPTR(Xml, destElement);
        reader->Deserialize(src, srcElement);
        reader->Deserialize(dest, destElement);

        VPGFileContentSyncMode srcSyncMode = GetSyncMode(srcElement.get());
        VPGFileContentSyncMode destSyncMode = GetSyncMode(destElement.get());

        // read first line to determine sync mode. Dest first, then src. If not exists, then default
        VPGFileContentSyncMode mode = destSyncMode != VPGFileContentSyncMode::NA ? destSyncMode : (srcSyncMode != VPGFileContentSyncMode::NA ? srcSyncMode : defaultMode);
        switch (mode)
        {
        case VPGFileContentSyncMode::Force:
            return VPGFileSyncService::GenerateForceCode(srcSyncMode, destSyncMode, srcElement.get(), destElement.get());
        case VPGFileContentSyncMode::Full:
            return VPGFileSyncService::GenerateFullCode(srcSyncMode, destSyncMode, srcElement.get(), destElement.get());
        case VPGFileContentSyncMode::Demand:
            return VPGFileSyncService::GenerateDemandCode(srcSyncMode, destSyncMode, srcElement.get(), destElement.get());
        case VPGFileContentSyncMode::Skip:
            return VPGFileSyncService::GenerateSkipCode(dest);
        default:
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown File Content Sync Mode");
            break;
        }
    CATCH
    return result;
}