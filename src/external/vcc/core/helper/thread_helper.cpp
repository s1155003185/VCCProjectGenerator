#include "thread_helper.hpp"

#include <sstream>
#include <thread>

#include "exception_macro.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::wstring ToString(const std::thread::id &threadId)
    {
        TRY
            std::ostringstream oss;
            oss << threadId;
            std::string str = oss.str();
            return std::wstring(str.begin(), str.end());
        CATCH
        return L"";
    }
}
