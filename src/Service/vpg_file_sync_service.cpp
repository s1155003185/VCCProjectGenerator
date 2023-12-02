#include "vpg_file_sync_service.hpp"

#include <filesystem>
#include <memory>
#include <string>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "xml_reader.hpp"

#include "vpg_code_reader.hpp"

using namespace vcc;

VPGFileContentSyncMode VPGFileSyncService::_GetSyncMode(const XMLElement &codeElemet)
{
    try
    {
        for (XMLElement child : codeElemet.Children) {
            if (child.Namespace == VCC_NAMESPACE && child.Name == VCC_NAME) {
                for (XMLAttribute attr : child.Attributes) {
                    if (attr.Name == SYNC_TOKEN) {
                        std::wstring value = attr.Value;
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
        THROW_EXCEPTION_M(ExceptionType::CUSTOM_ERROR, L"Unknow Tag " + tagValue);
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return VPGFileContentSyncTag::NA;
}

const XMLElement *VPGFileSyncService::_GetTagFromCode(const XMLElement &code, const std::wstring tagName)
{
    try
    {
        for (size_t i = 0; i < code.Children.size(); i++) {
            if (code.Children.at(i).Namespace == VCC_NAMESPACE && code.Children.at(i).Name == tagName)
                return &(code.Children.at(i));
        }
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return nullptr;
}

bool VPGFileSyncService::_IsTagReplace(const XMLElement &child)
{
    try
    {
        for (XMLAttribute attr : child.Attributes){
            if (attr.Name == SYNC_TOKEN) {
                std::wstring value = attr.Value;
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

bool VPGFileSyncService::_IsTagReserve(const XMLElement &child)
{
    try
    {
        for (XMLAttribute attr : child.Attributes){
            if (attr.Name == SYNC_TOKEN) {
                std::wstring value = attr.Value;
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

void VPGFileSyncService::CopyFile(LogProperty &logProperty, const std::wstring &sourcePath, const std::wstring &destgetPath)
{
    try
    {
        if (!IsFileExists(sourcePath))
            THROW_EXCEPTION_M(ExceptionType::FILE_NOT_FOUND, sourcePath + L": File not found.");

        bool isAdd = IsFileExists(destgetPath);
        std::filesystem::copy_file(PATH(sourcePath), PATH(destgetPath), std::filesystem::copy_options::overwrite_existing);

        if (isAdd)
            LogService::LogInfo(logProperty, L"", L"Added File: " + destgetPath);
        else
            LogService::LogInfo(logProperty, L"", L"Updated File: " + destgetPath);
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
}

std::wstring VPGFileSyncService::_GenerateForceCode(const VPGFileContentSyncMode &srcMode, const VPGFileContentSyncMode &destMode, const XMLElement &src, const XMLElement &dest)
{
    std::wstring result = L"";
    if (destMode == VPGFileContentSyncMode::NA) {
        for (XMLElement child : src.Children)
            result += child.FullText;
    } else {
        for (XMLElement child : dest.Children) {
            result += child.FullText;
            if (child.Namespace == VCC_NAMESPACE && child.Name == VCC_NAME)
                break;
        }
        bool shouldSkip = srcMode != VPGFileContentSyncMode::NA;
        for (XMLElement child : src.Children) {
            if (!shouldSkip)
                result += child.FullText;
            if (child.Namespace == VCC_NAMESPACE && child.Name == VCC_NAME)
                shouldSkip = false;
        }
    }
    return result;
}

std::wstring VPGFileSyncService::_GenerateFullCode(const VPGFileContentSyncMode &srcMode, const VPGFileContentSyncMode &destMode, const XMLElement &src, const XMLElement &dest)
{
    std::wstring result = L"";
    // if dest has header then add header, skip source header
    if (destMode != VPGFileContentSyncMode::NA) {
        for (XMLElement child : dest.Children) {
            result += child.FullText;
            if (child.Namespace == VCC_NAMESPACE && child.Name == VCC_NAME)
                break;
        }
    }
    bool shouldSkip = srcMode != VPGFileContentSyncMode::NA;
    for (XMLElement child : src.Children) {
        if (!shouldSkip) {
            // if find tag then search tag in source, if reserve, then use source
            if (child.Namespace == VCC_NAMESPACE) {
                const XMLElement *destTag = VPGFileSyncService::_GetTagFromCode(dest, child.Name);
                if (destTag != nullptr && VPGFileSyncService::_IsTagReserve(*destTag)) {
                    result += destTag->FullText;
                } else
                    result += child.FullText;
            } else
                result += child.FullText;
        }
        if (child.Namespace == VCC_NAMESPACE && child.Name == VCC_NAME)
            shouldSkip = false;
    }
    return result;
}

std::wstring VPGFileSyncService::_GenerateDemandCode(const VPGFileContentSyncMode &srcMode, const VPGFileContentSyncMode &destMode, const XMLElement &src, const XMLElement &dest)
{
    std::wstring result = L"";
    // if dest has header then add header, skip source header
    if (destMode != VPGFileContentSyncMode::NA) {
        for (XMLElement child : dest.Children) {
            result += child.FullText;
            if (child.Namespace == VCC_NAMESPACE && child.Name == VCC_NAME)
                break;
        }
    }
    bool shouldSkip = srcMode != VPGFileContentSyncMode::NA;
    for (XMLElement child : dest.Children) {
        if (!shouldSkip) {
            // if find tag then search tag in source, if reserve, then use source
            if (child.Namespace == VCC_NAMESPACE) {
                const XMLElement *srcTag = VPGFileSyncService::_GetTagFromCode(src, child.Name);
                if (srcTag != nullptr && VPGFileSyncService::_IsTagReplace(child)) {
                    result += srcTag->FullText;
                } else
                    result += child.FullText;
            } else
                result += child.FullText;
        }
        if (child.Namespace == VCC_NAMESPACE && child.Name == VCC_NAME)
            shouldSkip = false;
    }
    return result;
}

std::wstring VPGFileSyncService::_GenerateSkipCode(const std::wstring &dest)
{
    return std::wstring(dest);
}

std::wstring VPGFileSyncService::SyncFileContent(const std::wstring &src, const std::wstring &dest, const VPGFileContentSyncMode &defaultMode, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    try
    {
        std::unique_ptr<VPGCodeReader> reader = std::make_unique<VPGCodeReader>(commandDelimiter);
        XMLElement srcElement = reader->Parse(src);
        XMLElement destElement = reader->Parse(dest);

        VPGFileContentSyncMode srcSyncMode = _GetSyncMode(srcElement);
        VPGFileContentSyncMode destSyncMode = _GetSyncMode(destElement);

        // read first line to determine sync mode. Dest first, then src. If not exists, then default
        VPGFileContentSyncMode mode = destSyncMode != VPGFileContentSyncMode::NA ? destSyncMode : (srcSyncMode != VPGFileContentSyncMode::NA ? srcSyncMode : defaultMode);
        switch (mode)
        {
        case VPGFileContentSyncMode::Force:
            return VPGFileSyncService::_GenerateForceCode(srcSyncMode, destSyncMode, srcElement, destElement);
        case VPGFileContentSyncMode::Full:
            return VPGFileSyncService::_GenerateFullCode(srcSyncMode, destSyncMode, srcElement, destElement);
        case VPGFileContentSyncMode::Demand:
            return VPGFileSyncService::_GenerateDemandCode(srcSyncMode, destSyncMode, srcElement, destElement);
        case VPGFileContentSyncMode::Skip:
            return VPGFileSyncService::_GenerateSkipCode(dest);
        default:
            THROW_EXCEPTION_M(ExceptionType::CUSTOM_ERROR, L"Unknown File Content Sync Mode");
            break;
        }
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return result;
}