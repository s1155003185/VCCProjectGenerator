#include "vpg_git_log_property_accessor.hpp"

#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_git_log.hpp"
#include "vpg_git_log_property.hpp"

bool VPGGitLogPropertyAccessor::_readBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::IsHead:
            return obj->getIsHead();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGGitLogPropertyAccessor::_readBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGGitLogPropertyAccessor::_readBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGitLogPropertyAccessor::_writeBool(const int64_t &objectProperty, const bool &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::IsHead:
            obj->setIsHead(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_writeBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_writeBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_insertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGGitLogPropertyAccessor::_readLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AuthorDate:
            return obj->getAuthorDate();
        case VPGGitLogProperty::ColumnIndex:
            return obj->getColumnIndex();
        case VPGGitLogProperty::CommitDate:
            return obj->getCommitDate();
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGGitLogPropertyAccessor::_readLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGGitLogPropertyAccessor::_readLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGGitLogPropertyAccessor::_writeLong(const int64_t &objectProperty, const long &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AuthorDate:
            obj->setAuthorDate(value);
            break;
        case VPGGitLogProperty::ColumnIndex:
            obj->setColumnIndex(value);
            break;
        case VPGGitLogProperty::CommitDate:
            obj->setCommitDate(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_writeLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_writeLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_insertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGGitLogPropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedHashID:
            return obj->getAbbreviatedHashID();
        case VPGGitLogProperty::AbbreviatedTreeHashID:
            return obj->getAbbreviatedTreeHashID();
        case VPGGitLogProperty::Author:
            return obj->getAuthor();
        case VPGGitLogProperty::AuthorDateStr:
            return obj->getAuthorDateStr();
        case VPGGitLogProperty::AuthorEmail:
            return obj->getAuthorEmail();
        case VPGGitLogProperty::CommitDateStr:
            return obj->getCommitDateStr();
        case VPGGitLogProperty::Committer:
            return obj->getCommitter();
        case VPGGitLogProperty::CommitterEmail:
            return obj->getCommitterEmail();
        case VPGGitLogProperty::FullMessage:
            return obj->getFullMessage();
        case VPGGitLogProperty::HashID:
            return obj->getHashID();
        case VPGGitLogProperty::Message:
            return obj->getMessage();
        case VPGGitLogProperty::Title:
            return obj->getTitle();
        case VPGGitLogProperty::TreeHashID:
            return obj->getTreeHashID();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGitLogPropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            return obj->getAbbreviatedParentHashIDsAtIndex(index);
        case VPGGitLogProperty::Branches:
            return obj->getBranchesAtIndex(index);
        case VPGGitLogProperty::ParentHashIDs:
            return obj->getParentHashIDsAtIndex(index);
        case VPGGitLogProperty::Tags:
            return obj->getTagsAtIndex(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGitLogPropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGGitLogPropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedHashID:
            obj->setAbbreviatedHashID(value);
            break;
        case VPGGitLogProperty::AbbreviatedTreeHashID:
            obj->setAbbreviatedTreeHashID(value);
            break;
        case VPGGitLogProperty::Author:
            obj->setAuthor(value);
            break;
        case VPGGitLogProperty::AuthorDateStr:
            obj->setAuthorDateStr(value);
            break;
        case VPGGitLogProperty::AuthorEmail:
            obj->setAuthorEmail(value);
            break;
        case VPGGitLogProperty::CommitDateStr:
            obj->setCommitDateStr(value);
            break;
        case VPGGitLogProperty::Committer:
            obj->setCommitter(value);
            break;
        case VPGGitLogProperty::CommitterEmail:
            obj->setCommitterEmail(value);
            break;
        case VPGGitLogProperty::FullMessage:
            obj->setFullMessage(value);
            break;
        case VPGGitLogProperty::HashID:
            obj->setHashID(value);
            break;
        case VPGGitLogProperty::Message:
            obj->setMessage(value);
            break;
        case VPGGitLogProperty::Title:
            obj->setTitle(value);
            break;
        case VPGGitLogProperty::TreeHashID:
            obj->setTreeHashID(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            if (index > -1)
                obj->setAbbreviatedParentHashIDsAtIndex(index, value);
            else
                obj->insertAbbreviatedParentHashIDs(value);
            break;
        case VPGGitLogProperty::Branches:
            if (index > -1)
                obj->setBranchesAtIndex(index, value);
            else
                obj->insertBranches(value);
            break;
        case VPGGitLogProperty::ParentHashIDs:
            if (index > -1)
                obj->setParentHashIDsAtIndex(index, value);
            else
                obj->insertParentHashIDs(value);
            break;
        case VPGGitLogProperty::Tags:
            if (index > -1)
                obj->setTagsAtIndex(index, value);
            else
                obj->insertTags(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            if (index > -1)
                obj->insertAbbreviatedParentHashIDsAtIndex(index, value);
            else
                obj->insertAbbreviatedParentHashIDs(value);
            break;
        case VPGGitLogProperty::Branches:
            if (index > -1)
                obj->insertBranchesAtIndex(index, value);
            else
                obj->insertBranches(value);
            break;
        case VPGGitLogProperty::ParentHashIDs:
            if (index > -1)
                obj->insertParentHashIDsAtIndex(index, value);
            else
                obj->insertParentHashIDs(value);
            break;
        case VPGGitLogProperty::Tags:
            if (index > -1)
                obj->insertTagsAtIndex(index, value);
            else
                obj->insertTags(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

size_t VPGGitLogPropertyAccessor::_getCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ParentHashIDs:
            return obj->getParentHashIDs().size();
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            return obj->getAbbreviatedParentHashIDs().size();
        case VPGGitLogProperty::Branches:
            return obj->getBranches().size();
        case VPGGitLogProperty::Tags:
            return obj->getTags().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGGitLogPropertyAccessor::_getMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGGitLogPropertyAccessor::_isContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGitLogPropertyAccessor::_remove(const int64_t &objectProperty, const void *value)
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedParentHashIDs: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->removeAbbreviatedParentHashIDs(valuePtr);
            break;
        }
        case VPGGitLogProperty::Branches: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->removeBranches(valuePtr);
            break;
        }
        case VPGGitLogProperty::ParentHashIDs: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->removeParentHashIDs(valuePtr);
            break;
        }
        case VPGGitLogProperty::Tags: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->removeTags(valuePtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_removeObject(const int64_t &objectProperty, const vcc::IObject */*value*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_removeAtIndex(const int64_t &objectProperty, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            obj->removeAbbreviatedParentHashIDsAtIndex(index);
            break;
        case VPGGitLogProperty::Branches:
            obj->removeBranchesAtIndex(index);
            break;
        case VPGGitLogProperty::ParentHashIDs:
            obj->removeParentHashIDsAtIndex(index);
            break;
        case VPGGitLogProperty::Tags:
            obj->removeTagsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitLogPropertyAccessor::_removeAtKey(const int64_t &objectProperty, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitLogPropertyAccessor::_clear(const int64_t &objectProperty)
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitLog>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitLogProperty>(objectProperty))
        {
        case VPGGitLogProperty::ParentHashIDs:
            obj->clearParentHashIDs();
            break;
        case VPGGitLogProperty::AbbreviatedParentHashIDs:
            obj->clearAbbreviatedParentHashIDs();
            break;
        case VPGGitLogProperty::Branches:
            obj->clearBranches();
            break;
        case VPGGitLogProperty::Tags:
            obj->clearTags();
            break;
        default:
            assert(false);
        }
    CATCH
}
