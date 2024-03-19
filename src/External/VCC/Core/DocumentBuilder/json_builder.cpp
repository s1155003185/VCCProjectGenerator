#include "json_builder.hpp"

#include <string>

#include "json.hpp"

namespace vcc
{
    void JsonBuilder::Serialize(const std::wstring &str, std::shared_ptr<Json> doc)
    {

    }

    std::wstring JsonBuilder::Deserialize(const Json *doc)
    {
        return L"";
    }
}
