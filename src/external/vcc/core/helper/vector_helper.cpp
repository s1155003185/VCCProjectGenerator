#include "vector_helper.hpp"

#include <string>
#include <vector>

#include "exception_macro.hpp"

namespace vcc
{
    std::wstring concat(const std::vector<std::wstring> &v, const std::wstring &delimitor)
    {
        TRY
            if (isEmpty(v))
                return L"";
            
            std::wstring result = L"";
            for (std::wstring str : v)
                result += str + delimitor;
            return result.substr(0, result.size() - delimitor.size());
        CATCH
        return L"";
    }
}