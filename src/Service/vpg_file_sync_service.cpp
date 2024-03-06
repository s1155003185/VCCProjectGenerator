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
#include "xml_reader.hpp"

#include "vpg_code_reader.hpp"

using namespace vcc;

bool VPGFileSyncService::_IsSyncTag(const std::wstring &tag)
{
    return tag == SYNC_TOKEN_LONG || tag == SYNC_TOKEN_SHORT;
}

VPGFileContentSyncMode VPGFileSyncService::_GetSyncMode(const XMLElement *codeElemet)
{
    try
    {
        for (shared_ptr<XMLElement> child : codeElemet->GetChildren()) {
            if (child->GetNamespace() == VCC_NAMESPACE && child->GetName() == VCC_NAME) {
                for (shared_ptr<XMLAttribute> attr : child->GetAttributes()) {
                    if (_IsSyncTag(attr->GetName())) {
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
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return VPGFileContentSyncMode::NA;
}

VPGFileContentSyncTag VPGFileSyncService::_GetSyncTag(std::wstring tagValue)
{
    try
    {
        ToUpper(tagValue);
        if (tagValue == RESERVE_TAG)
            return VPGFileContentSyncTag::Reserve;
        if (tagValue == REPLACE_TAG)
            return VPGFileContentSyncTag::Replace;
        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknow Tag " + tagValue);
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return VPGFileContentSyncTag::NA;
}

const XMLElement *VPGFileSyncService::_GetTagFromCode(const XMLElement *code, const std::wstring tagName)
{
    try
    {
        for (size_t i = 0; i < code->GetChildren().size(); i++) {
            if (code->GetChildren().at(i)->GetNamespace() == VCC_NAMESPACE && code->GetChildren().at(i)->GetName() == tagName)
                return code->GetChildren().at(i).get();
        }
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return nullptr;
}

bool VPGFileSyncService::_IsTagReplace(const XMLElement *child)
{
    try
    {
        for (shared_ptr<XMLAttribute> attr : child->GetAttributes()){
            if (_IsSyncTag(attr->GetName())) {
                std::wstring value = attr->GetValue();
                ToUpper(value);
                return value == REPLACE_TAG;
            }
        }
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return false;
}

bool VPGFileSyncService::_IsTagReserve(const XMLElement *child)
{
    try
    {
        for (shared_ptr<XMLAttribute> attr : child->GetAttributes()){
            if (_IsSyncTag(attr->GetName())) {
                std::wstring value = attr->GetValue();
                ToUpper(value);
                return value == RESERVE_TAG;
            }
        }
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return false;
}

void VPGFileSyncService::CopyFile(const LogProperty *logProperty, const std::wstring &sourcePath, const std::wstring &destgetPath)
{
    try
    {
        if (!IsFileExists(sourcePath))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, sourcePath + L": File not found.");

        bool isModified = IsFileExists(destgetPath);
        if (isModified)
            std::filesystem::remove(PATH(destgetPath));
        std::filesystem::copy_file(PATH(sourcePath), PATH(destgetPath), std::filesystem::copy_options::overwrite_existing);

        if (isModified) {
           LogService::LogInfo(logProperty, L"", L"Updated File: " + destgetPath);
            // TODO handle tag
        } else
            LogService::LogInfo(logProperty, L"", L"Added File: " + destgetPath);
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
}

std::wstring VPGFileSyncService::_GenerateForceCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const XMLElement *src, const XMLElement *dest)
{
    std::wstring result = L"";
    if (destMode == VPGFileContentSyncMode::NA) {
        for (shared_ptr<XMLElement> child : src->GetChildren())
            result += child->GetFullText();
    } else {
        for (shared_ptr<XMLElement> child : dest->GetChildren()) {
            result += child->GetFullText();
            if (child->GetNamespace() == VCC_NAMESPACE && child->GetName() == VCC_NAME)
                break;
        }
        bool shouldSkip = srcMode != VPGFileContentSyncMode::NA;
        for (shared_ptr<XMLElement> child : src->GetChildren()) {
            if (!shouldSkip)
                result += child->GetFullText();
            if (child->GetNamespace() == VCC_NAMESPACE && child->GetName() == VCC_NAME)
                shouldSkip = false;
        }
    }
    return result;
}

std::wstring VPGFileSyncService::_GenerateFullCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const XMLElement *src, const XMLElement *dest)
{
    std::wstring result = L"";
    // if dest has header then add header, skip source header
    if (destMode != VPGFileContentSyncMode::NA) {
        for (shared_ptr<XMLElement> child : dest->GetChildren()) {
            result += child->GetFullText();
            if (child->GetNamespace() == VCC_NAMESPACE && child->GetName() == VCC_NAME)
                break;
        }
    }
    bool shouldSkip = srcMode != VPGFileContentSyncMode::NA;
    for (shared_ptr<XMLElement> child : src->GetChildren()) {
        if (!shouldSkip) {
            // if find tag then search tag in source, if reserve, then use source
            if (child->GetNamespace() == VCC_NAMESPACE) {
                const XMLElement *destTag = VPGFileSyncService::_GetTagFromCode(dest, child->GetName());
                if (destTag != nullptr && VPGFileSyncService::_IsTagReserve(destTag)) {
                    result += destTag->GetFullText();
                } else
                    result += child->GetFullText();
            } else
                result += child->GetFullText();
        }
        if (child->GetNamespace() == VCC_NAMESPACE && child->GetName() == VCC_NAME)
            shouldSkip = false;
    }
    return result;
}

std::wstring VPGFileSyncService::_GenerateDemandCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const XMLElement *src, const XMLElement *dest)
{
    std::wstring result = L"";
    // if dest has header then add header, skip source header
    if (destMode != VPGFileContentSyncMode::NA) {
        for (shared_ptr<XMLElement> child : dest->GetChildren()) {
            result += child->GetFullText();
            if (child->GetNamespace() == VCC_NAMESPACE && child->GetName() == VCC_NAME)
                break;
        }
    }
    bool shouldSkip = srcMode != VPGFileContentSyncMode::NA;
    for (shared_ptr<XMLElement> child : dest->GetChildren()) {
        if (!shouldSkip) {
            // if find tag then search tag in source, if reserve, then use source
            if (child->GetNamespace() == VCC_NAMESPACE) {
                const XMLElement *srcTag = VPGFileSyncService::_GetTagFromCode(src, child->GetName());
                if (srcTag != nullptr && VPGFileSyncService::_IsTagReplace(child.get())) {
                    result += srcTag->GetFullText();
                } else
                    result += child->GetFullText();
            } else
                result += child->GetFullText();
        }
        if (child->GetNamespace() == VCC_NAMESPACE && child->GetName() == VCC_NAME)
            shouldSkip = false;
    }
    return result;
}

std::wstring VPGFileSyncService::_GenerateSkipCode(const std::wstring &dest)
{
    return std::wstring(dest);
}

std::wstring VPGFileSyncService::SyncFileContent(const std::wstring &src, const std::wstring &dest, const VPGFileContentSyncMode defaultMode, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    try
    {
        std::unique_ptr<VPGCodeReader> reader = std::make_unique<VPGCodeReader>(commandDelimiter);
        DECLARE_SPTR(XMLElement, srcElement);
        DECLARE_SPTR(XMLElement, destElement);
        reader->Parse(src, srcElement);
        reader->Parse(dest, destElement);

        VPGFileContentSyncMode srcSyncMode = _GetSyncMode(srcElement.get());
        VPGFileContentSyncMode destSyncMode = _GetSyncMode(destElement.get());

        // read first line to determine sync mode. Dest first, then src. If not exists, then default
        VPGFileContentSyncMode mode = destSyncMode != VPGFileContentSyncMode::NA ? destSyncMode : (srcSyncMode != VPGFileContentSyncMode::NA ? srcSyncMode : defaultMode);
        switch (mode)
        {
        case VPGFileContentSyncMode::Force:
            return VPGFileSyncService::_GenerateForceCode(srcSyncMode, destSyncMode, srcElement.get(), destElement.get());
        case VPGFileContentSyncMode::Full:
            return VPGFileSyncService::_GenerateFullCode(srcSyncMode, destSyncMode, srcElement.get(), destElement.get());
        case VPGFileContentSyncMode::Demand:
            return VPGFileSyncService::_GenerateDemandCode(srcSyncMode, destSyncMode, srcElement.get(), destElement.get());
        case VPGFileContentSyncMode::Skip:
            return VPGFileSyncService::_GenerateSkipCode(dest);
        default:
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown File Content Sync Mode");
            break;
        }
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return result;
}