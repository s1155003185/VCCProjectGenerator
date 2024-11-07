#include "vpg_file_generation_service.hpp"

#include <string>

#include "exception_macro.hpp"
#include "memory_macro.hpp"

#include "vpg_code_reader.hpp"

using namespace vcc;

const std::wstring VCC_NAMESPACE = L"vcc";
const std::wstring VCC_NAME = VCC_NAMESPACE + L":vccproj";

constexpr auto GEN_TOKEN_SHORT = L"gen";
constexpr auto GEN_TOKEN_LONG = L"generation";

// xml
constexpr auto DEMAND_MODE = L"DEMAND";
constexpr auto SKIP_MODE = L"SKIP";
constexpr auto FORCE_MODE = L"FORCE";

// tag
constexpr auto RESERVE_TAG = L"RESERVE";
constexpr auto REPLACE_TAG = L"REPLACE";


std::wstring VPGFileGenerationService::GetIndent(const std::wstring &str)
{
    std::wstring result = L"";
    TRY
        std::vector<std::wstring> lines = SplitStringByLine(str);
        if (lines.empty())
            return L"";

        for (size_t i = 0; i < lines.size(); i++) {
            std::wstring line = lines[lines.size() - i - 1];
            while (!line.empty() && line[line.length() - 1] == L'\r')
                line.pop_back();

            if (line.empty())
                continue;
            
            if (IsBlank(line))
                result = line;
            else {
                size_t pos = 0;
                GetNextCharPos(line, pos, true);
                if (pos > 0 && pos < line.length())
                    result = line.substr(0, pos);
            }
            break;
        }
    CATCH
    return result;
}

const Xml *VPGFileGenerationService::GetTagFromCode(const Xml *code, const std::wstring &tagName)
{
    TRY
        for (size_t i = 0; i < code->GetChildren().size(); i++) {
            if (code->GetChildren().at(i)->GetName() == tagName)
                return code->GetChildren().at(i).get();
        }
    CATCH
    return nullptr;
}

bool VPGFileGenerationService::IsGeneartionTag(const std::wstring &tag)
{
    return tag == GEN_TOKEN_SHORT || tag == GEN_TOKEN_LONG;
}

bool VPGFileGenerationService::IsTagForce(const Xml *child)
{
    TRY
        for (std::shared_ptr<XmlAttribute> attr : child->GetAttributes()){
            if (IsGeneartionTag(attr->GetName())) {
                std::wstring value = attr->GetValue();
                ToUpper(value);
                return value == REPLACE_TAG;
            }
        }
    CATCH
    return false;
}

bool VPGFileGenerationService::IsTagSkip(const Xml *child)
{
    TRY
        for (std::shared_ptr<XmlAttribute> attr : child->GetAttributes()){
            if (IsGeneartionTag(attr->GetName())) {
                std::wstring value = attr->GetValue();
                ToUpper(value);
                return value == RESERVE_TAG;
            }
        }
    CATCH
    return false;
}

VPGFileContentGenerationMode VPGFileGenerationService::GetGenerationMode(const Xml *codeElemet)
{
    TRY
        for (std::shared_ptr<Xml> child : codeElemet->GetChildren()) {
            if (child->GetName() == VCC_NAME) {
                for (std::shared_ptr<XmlAttribute> attr : child->GetAttributes()) {
                    if (IsGeneartionTag(attr->GetName())) {
                        std::wstring value = attr->GetValue();
                        ToUpper(value);
                        if (value == FORCE_MODE) {
                            return VPGFileContentGenerationMode::Force;
                        } else if (value == DEMAND_MODE) {
                            return VPGFileContentGenerationMode::Demand;
                        } else if (value == SKIP_MODE) {
                            return VPGFileContentGenerationMode::Skip;
                        }
                    }
                }
                break;
            }
        }
    CATCH
    return VPGFileContentGenerationMode::Demand;
}

std::wstring VPGFileGenerationService::GenerateForceCode(const Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    TRY
        // split replacement to lines
        std::vector<std::wstring> lines = SplitStringByLine(generatedContent);
        bool isFound = false;
        std::wstring indent = L"";
        for (const auto &child : src->GetChildren()) {
            if (IsStartWith(child->GetName(), L"vcc:") && child->GetName() == tagName) {
                result += commandDelimiter + L" " + child->GetOpeningTag() + L"\r\n";
                for (auto line : lines) {
                    Trim(line);
                    result += indent + line + L"\r\n";
                }
                result += indent + commandDelimiter + L" " + child->GetClosingTag();
                isFound = true;
            } else {
                result += child->GetFullText();
                indent = GetIndent(child->GetFullText());
            }
        }
        if (!isFound) {
            result += L"\r\n" + indent + commandDelimiter + L" <" + tagName + L" gen=\"REPLACE\">\r\n";
            for (auto line : lines) {
                Trim(line);
                result += indent + line + L"\r\n";
            }
            result += indent + commandDelimiter + L" </" + tagName + L">\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGFileGenerationService::GenerateDemandCode(const Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    TRY
        // split replacement to lines
        std::vector<std::wstring> lines = SplitStringByLine(generatedContent);
        std::wstring indent = L"";
        for (const auto &child : src->GetChildren()) {
            if (IsStartWith(child->GetName(), L"vcc:") && child->GetName() == tagName) {            
                const Xml *srcTag = VPGFileGenerationService::GetTagFromCode(src, child->GetName());
                if (srcTag != nullptr && VPGFileGenerationService::IsTagForce(child.get())) {
                    result += commandDelimiter + L" " + child->GetOpeningTag() + L"\r\n";
                    for (auto line : lines) {
                        Trim(line);
                        result += indent + line + L"\r\n";
                    }
                    result += indent + commandDelimiter + L" " + child->GetClosingTag();
                } else
                    result += child->GetFullText();
            } else {
                result += child->GetFullText();
                indent = GetIndent(child->GetFullText());
            }
        }
    CATCH
    return result;
}

std::wstring VPGFileGenerationService::GenerateSkipCode(const std::wstring &src)
{
    return src;
}

std::wstring VPGFileGenerationService::GenerateFileContent(const std::wstring &code, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter)
{
    TRY
        std::unique_ptr<VPGCodeReader> reader = std::make_unique<VPGCodeReader>(commandDelimiter);
        DECLARE_SPTR(Xml, codeElement);
        reader->Deserialize(code, codeElement);

        switch (GetGenerationMode(codeElement.get()))
        {
        case VPGFileContentGenerationMode::Force:
            return VPGFileGenerationService::GenerateForceCode(codeElement.get(), tagName, generatedContent, commandDelimiter);
        case VPGFileContentGenerationMode::Demand:
            return VPGFileGenerationService::GenerateDemandCode(codeElement.get(), tagName, generatedContent, commandDelimiter);
        case VPGFileContentGenerationMode::Skip:
            return VPGFileGenerationService::GenerateSkipCode(code);
        default:
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown File Content Generation Mode");
            break;
        }
    CATCH
    return code;
}
