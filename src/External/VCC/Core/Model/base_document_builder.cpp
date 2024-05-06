#include "base_document_builder.hpp"

#include <string>

namespace vcc
{
    std::wstring BaseDocumentBuilder::GetErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const
    {
        size_t row = 0, column = 0;
        GetCharacterRowAndColumn(str, pos, row, column);
        
        size_t lengthOfPos = std::min(pos, _NumberOfCharactersBeforePosForErrorMessage);
        size_t lengthOfSub = std::min(str.length() - pos, _NumberOfCharactersAfterPosForErrorMessage);

        std::wstring preString = str.substr(pos - lengthOfPos, lengthOfPos);
        std::wstring subString = str.substr(pos + 1, lengthOfSub);
        return L"Error at position " + std::to_wstring(pos + 1) 
            + L", row " + std::to_wstring(row) 
            + L", column " + std::to_wstring(column) + L": " + preString + L"<<" + std::wstring(1, str[pos]) + L">>" + subString + L"\r\n "
            + L" Error message: " + msg;
    }
}
