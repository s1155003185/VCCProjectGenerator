#include "vpg_git_log_property_accessor.hpp"

#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_git_log.hpp"
#include "vpg_git_log_property.hpp"

using namespace vcc;

bool VPGGitLogPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
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

bool VPGGitLogPropertyAccessor::_ReadBool(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGitLogPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
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

void VPGGitLogPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_InsertBool(const int64_t &objectProperty, const bool & /*value*/, const int64_t & /*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGGitLogPropertyAccessor::_ReadLong(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ColumnIndex:
            return obj->GetColumnIndex();
        case VPGGitLogProperty::AuthorDate:
            return obj->GetAuthorDate();
        case VPGGitLogProperty::CommitDate:
            return obj->GetCommitDate();
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGGitLogPropertyAccessor::_ReadLong(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGGitLogPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ColumnIndex:
            obj->SetColumnIndex(value);
            break;
        case VPGGitLogProperty::AuthorDate:
            obj->SetAuthorDate(value);
            break;
        case VPGGitLogProperty::CommitDate:
            obj->SetCommitDate(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long & /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_InsertLong(const int64_t &objectProperty, const long & /*value*/, const int64_t & /*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGGitLogPropertyAccessor::_ReadString(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::HashID:
            return obj->GetHashID();
        case VPGGitLogProperty::AbbreviatedHashID:
            return obj->GetAbbreviatedHashID();
        case VPGGitLogProperty::TreeHashID:
            return obj->GetTreeHashID();
        case VPGGitLogProperty::AbbreviatedTreeHashID:
            return obj->GetAbbreviatedTreeHashID();
        case VPGGitLogProperty::ParentHashIDs:
            return obj->GetParentHashIDs(index);
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            return obj->GetAbbreviatedParentHashIDs(index);
        case VPGGitLogProperty::Branches:
            return obj->GetBranches(index);
        case VPGGitLogProperty::Tags:
            return obj->GetTags(index);
        case VPGGitLogProperty::Author:
            return obj->GetAuthor();
        case VPGGitLogProperty::AuthorEmail:
            return obj->GetAuthorEmail();
        case VPGGitLogProperty::AuthorDateStr:
            return obj->GetAuthorDateStr();
        case VPGGitLogProperty::Committer:
            return obj->GetCommitter();
        case VPGGitLogProperty::CommitterEmail:
            return obj->GetCommitterEmail();
        case VPGGitLogProperty::CommitDateStr:
            return obj->GetCommitDateStr();
        case VPGGitLogProperty::Title:
            return obj->GetTitle();
        case VPGGitLogProperty::Message:
            return obj->GetMessage();
        case VPGGitLogProperty::FullMessage:
            return obj->GetFullMessage();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGitLogPropertyAccessor::_ReadString(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGGitLogPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::HashID:
            obj->SetHashID(value);
            break;
        case VPGGitLogProperty::AbbreviatedHashID:
            obj->SetAbbreviatedHashID(value);
            break;
        case VPGGitLogProperty::TreeHashID:
            obj->SetTreeHashID(value);
            break;
        case VPGGitLogProperty::AbbreviatedTreeHashID:
            obj->SetAbbreviatedTreeHashID(value);
            break;
        case VPGGitLogProperty::ParentHashIDs:
            if (index > -1)
                obj->SetParentHashIDs(index, value);
            else
                obj->InsertParentHashIDs(value);
            break;
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            if (index > -1)
                obj->SetAbbreviatedParentHashIDs(index, value);
            else
                obj->InsertAbbreviatedParentHashIDs(value);
            break;
        case VPGGitLogProperty::Branches:
            if (index > -1)
                obj->SetBranches(index, value);
            else
                obj->InsertBranches(value);
            break;
        case VPGGitLogProperty::Tags:
            if (index > -1)
                obj->SetTags(index, value);
            else
                obj->InsertTags(value);
            break;
        case VPGGitLogProperty::Author:
            obj->SetAuthor(value);
            break;
        case VPGGitLogProperty::AuthorEmail:
            obj->SetAuthorEmail(value);
            break;
        case VPGGitLogProperty::AuthorDateStr:
            obj->SetAuthorDateStr(value);
            break;
        case VPGGitLogProperty::Committer:
            obj->SetCommitter(value);
            break;
        case VPGGitLogProperty::CommitterEmail:
            obj->SetCommitterEmail(value);
            break;
        case VPGGitLogProperty::CommitDateStr:
            obj->SetCommitDateStr(value);
            break;
        case VPGGitLogProperty::Title:
            obj->SetTitle(value);
            break;
        case VPGGitLogProperty::Message:
            obj->SetMessage(value);
            break;
        case VPGGitLogProperty::FullMessage:
            obj->SetFullMessage(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring & /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_InsertString(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ParentHashIDs:
            if (index > -1)
                obj->InsertParentHashIDs(index, value);
            else
                obj->InsertParentHashIDs(value);
            break;
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            if (index > -1)
                obj->InsertAbbreviatedParentHashIDs(index, value);
            else
                obj->InsertAbbreviatedParentHashIDs(value);
            break;
        case VPGGitLogProperty::Branches:
            if (index > -1)
                obj->InsertBranches(index, value);
            else
                obj->InsertBranches(value);
            break;
        case VPGGitLogProperty::Tags:
            if (index > -1)
                obj->InsertTags(index, value);
            else
                obj->InsertTags(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

size_t VPGGitLogPropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const
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

bool VPGGitLogPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGitLogPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ParentHashIDs:
            obj->RemoveParentHashIDs(index);
            break;
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            obj->RemoveAbbreviatedParentHashIDs(index);
            break;
        case VPGGitLogProperty::Branches:
            obj->RemoveBranches(index);
            break;
        case VPGGitLogProperty::Tags:
            obj->RemoveTags(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_ClearContainer(const int64_t &objectProperty) const
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
