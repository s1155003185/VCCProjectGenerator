#include "json_builder.hpp"

#include <string>

#include "json.hpp"

namespace vcc
{
    std::wstring JsonBuilder::Serialize(const Json *doc)
    {
        return L"";
    }

    void JsonBuilder::Deserialize(const std::wstring &str, std::shared_ptr<Json> doc)
    {

    }
}
