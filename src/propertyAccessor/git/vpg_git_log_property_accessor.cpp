#include "vpg_git_log_property_accessor.hpp"

#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_git_log.hpp"
#include "vpg_git_log_property.hpp"

using namespace vcc;

bool VPGGitLogPropertyAccessor::_ReadBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::IsHead:
            return obj->GetIsHead();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGGitLogPropertyAccessor::_ReadBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGGitLogPropertyAccessor::_ReadBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGitLogPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::IsHead:
            obj->SetIsHead(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_WriteBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_WriteBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_InsertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGGitLogPropertyAccessor::_ReadLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AuthorDate:
            return obj->GetAuthorDate();
        case VPGGitLogProperty::ColumnIndex:
            return obj->GetColumnIndex();
        case VPGGitLogProperty::CommitDate:
            return obj->GetCommitDate();
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGGitLogPropertyAccessor::_ReadLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGGitLogPropertyAccessor::_ReadLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGGitLogPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AuthorDate:
            obj->SetAuthorDate(value);
            break;
        case VPGGitLogProperty::ColumnIndex:
            obj->SetColumnIndex(value);
            break;
        case VPGGitLogProperty::CommitDate:
            obj->SetCommitDate(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_WriteLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_WriteLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_InsertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGGitLogPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedHashID:
            return obj->GetAbbreviatedHashID();
        case VPGGitLogProperty::AbbreviatedTreeHashID:
            return obj->GetAbbreviatedTreeHashID();
        case VPGGitLogProperty::Author:
            return obj->GetAuthor();
        case VPGGitLogProperty::AuthorDateStr:
            return obj->GetAuthorDateStr();
        case VPGGitLogProperty::AuthorEmail:
            return obj->GetAuthorEmail();
        case VPGGitLogProperty::CommitDateStr:
            return obj->GetCommitDateStr();
        case VPGGitLogProperty::Committer:
            return obj->GetCommitter();
        case VPGGitLogProperty::CommitterEmail:
            return obj->GetCommitterEmail();
        case VPGGitLogProperty::FullMessage:
            return obj->GetFullMessage();
        case VPGGitLogProperty::HashID:
            return obj->GetHashID();
        case VPGGitLogProperty::Message:
            return obj->GetMessage();
        case VPGGitLogProperty::Title:
            return obj->GetTitle();
        case VPGGitLogProperty::TreeHashID:
            return obj->GetTreeHashID();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGitLogPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            return obj->GetAbbreviatedParentHashIDsAtIndex(index);
        case VPGGitLogProperty::Branches:
            return obj->GetBranchesAtIndex(index);
        case VPGGitLogProperty::ParentHashIDs:
            return obj->GetParentHashIDsAtIndex(index);
        case VPGGitLogProperty::Tags:
            return obj->GetTagsAtIndex(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGitLogPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGGitLogPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedHashID:
            obj->SetAbbreviatedHashID(value);
            break;
        case VPGGitLogProperty::AbbreviatedTreeHashID:
            obj->SetAbbreviatedTreeHashID(value);
            break;
        case VPGGitLogProperty::Author:
            obj->SetAuthor(value);
            break;
        case VPGGitLogProperty::AuthorDateStr:
            obj->SetAuthorDateStr(value);
            break;
        case VPGGitLogProperty::AuthorEmail:
            obj->SetAuthorEmail(value);
            break;
        case VPGGitLogProperty::CommitDateStr:
            obj->SetCommitDateStr(value);
            break;
        case VPGGitLogProperty::Committer:
            obj->SetCommitter(value);
            break;
        case VPGGitLogProperty::CommitterEmail:
            obj->SetCommitterEmail(value);
            break;
        case VPGGitLogProperty::FullMessage:
            obj->SetFullMessage(value);
            break;
        case VPGGitLogProperty::HashID:
            obj->SetHashID(value);
            break;
        case VPGGitLogProperty::Message:
            obj->SetMessage(value);
            break;
        case VPGGitLogProperty::Title:
            obj->SetTitle(value);
            break;
        case VPGGitLogProperty::TreeHashID:
            obj->SetTreeHashID(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            if (index > -1)
                obj->SetAbbreviatedParentHashIDsAtIndex(index, value);
            else
                obj->InsertAbbreviatedParentHashIDs(value);
            break;
        case VPGGitLogProperty::Branches:
            if (index > -1)
                obj->SetBranchesAtIndex(index, value);
            else
                obj->InsertBranches(value);
            break;
        case VPGGitLogProperty::ParentHashIDs:
            if (index > -1)
                obj->SetParentHashIDsAtIndex(index, value);
            else
                obj->InsertParentHashIDs(value);
            break;
        case VPGGitLogProperty::Tags:
            if (index > -1)
                obj->SetTagsAtIndex(index, value);
            else
                obj->InsertTags(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            if (index > -1)
                obj->InsertAbbreviatedParentHashIDsAtIndex(index, value);
            else
                obj->InsertAbbreviatedParentHashIDs(value);
            break;
        case VPGGitLogProperty::Branches:
            if (index > -1)
                obj->InsertBranchesAtIndex(index, value);
            else
                obj->InsertBranches(value);
            break;
        case VPGGitLogProperty::ParentHashIDs:
            if (index > -1)
                obj->InsertParentHashIDsAtIndex(index, value);
            else
                obj->InsertParentHashIDs(value);
            break;
        case VPGGitLogProperty::Tags:
            if (index > -1)
                obj->InsertTagsAtIndex(index, value);
            else
                obj->InsertTags(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

size_t VPGGitLogPropertyAccessor::_GetCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ParentHashIDs:
            return obj->GetParentHashIDs().size();
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            return obj->GetAbbreviatedParentHashIDs().size();
        case VPGGitLogProperty::Branches:
            return obj->GetBranches().size();
        case VPGGitLogProperty::Tags:
            return obj->GetTags().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGGitLogPropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGGitLogPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGitLogPropertyAccessor::_Remove(const int64_t &objectProperty, const void *value) const
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ParentHashIDs: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->RemoveParentHashIDs(valuePtr);
            break;
        }
        case VPGGitLogProperty::AbbreviatedParentHashIDs: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->RemoveAbbreviatedParentHashIDs(valuePtr);
            break;
        }
        case VPGGitLogProperty::Branches: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->RemoveBranches(valuePtr);
            break;
        }
        case VPGGitLogProperty::Tags: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->RemoveTags(valuePtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_RemoveAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ParentHashIDs:
            obj->RemoveParentHashIDsAtIndex(index);
            break;
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            obj->RemoveAbbreviatedParentHashIDsAtIndex(index);
            break;
        case VPGGitLogProperty::Branches:
            obj->RemoveBranchesAtIndex(index);
            break;
        case VPGGitLogProperty::Tags:
            obj->RemoveTagsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_RemoveAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_Clear(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ParentHashIDs:
            obj->ClearParentHashIDs();
            break;
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            obj->ClearAbbreviatedParentHashIDs();
            break;
        case VPGGitLogProperty::Branches:
            obj->ClearBranches();
            break;
        case VPGGitLogProperty::Tags:
            obj->ClearTags();
            break;
        default:
            assert(false);
        }
    CATCH
}
