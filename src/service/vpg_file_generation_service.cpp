#include "vpg_file_generation_service.hpp"

#include <string>

#include "exception_macro.hpp"

#include "vpg_code_reader.hpp"

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


size_t VPGFileGenerationService::getLeadingSpace(const std::wstring &line)
{
    TRY
        for (size_t i = 0; i < line.size(); i++) {
            if (!std::iswspace(line.at(i)))
                return i;
        }
    CATCH
    return 0;
}

size_t VPGFileGenerationService::getMinimumLeadingSpace(const std::vector<std::wstring> &lines)
{
    size_t result = 0;
    TRY
        bool isFirstLine = true;
        for (auto const &line : lines) {
            if (!vcc::isBlank(line)) {
                size_t noOfSpace = getLeadingSpace(line);
                if (isFirstLine) {
                    result = noOfSpace;
                    isFirstLine = false;
                } else {
                    result = std::min(result, noOfSpace);
                }
            }
        }
    CATCH
    return result;
}

std::wstring VPGFileGenerationService::getIndent(const std::wstring &str)
{
    std::wstring result = L"";
    TRY
        std::vector<std::wstring> lines = vcc::splitStringByLine(str);
        if (lines.empty())
            return L"";

        for (size_t i = 0; i < lines.size(); i++) {
            std::wstring line = lines[lines.size() - i - 1];
            while (!line.empty() && line[line.length() - 1] == L'\r')
                line.pop_back();

            if (line.empty())
                continue;
            
            if (vcc::isBlank(line))
                result = line;
            else {
                size_t pos = 0;
                vcc::getNextCharPos(line, pos, true);
                if (pos > 0 && pos < line.length())
                    result = line.substr(0, pos);
            }
            break;
        }
    CATCH
    return result;
}

const vcc::Xml *VPGFileGenerationService::getTagFromCode(const vcc::Xml *code, const std::wstring &tagName)
{
    TRY
        for (size_t i = 0; i < code->getChildren().size(); i++) {
            if (code->getChildren().at(i)->getName() == tagName)
                return code->getChildren().at(i).get();
        }
    CATCH
    return nullptr;
}

bool VPGFileGenerationService::isGeneartionTag(const std::wstring &tag)
{
    return tag == GEN_TOKEN_SHORT || tag == GEN_TOKEN_LONG;
}

bool VPGFileGenerationService::isTagForce(const vcc::Xml *child)
{
    TRY
        for (std::shared_ptr<vcc::XmlAttribute> attr : child->getAttributes()){
            if (isGeneartionTag(attr->getName())) {
                std::wstring value = attr->getValue();
                vcc::toUpper(value);
                return value == REPLACE_TAG;
            }
        }
    CATCH
    return false;
}

bool VPGFileGenerationService::isTagSkip(const vcc::Xml *child)
{
    TRY
        for (std::shared_ptr<vcc::XmlAttribute> attr : child->getAttributes()){
            if (isGeneartionTag(attr->getName())) {
                std::wstring value = attr->getValue();
                vcc::toUpper(value);
                return value == RESERVE_TAG;
            }
        }
    CATCH
    return false;
}

VPGFileContentGenerationMode VPGFileGenerationService::getGenerationMode(const vcc::Xml *codeElemet)
{
    TRY
        for (std::shared_ptr<vcc::Xml> child : codeElemet->getChildren()) {
            if (child->getName() == VCC_NAME) {
                for (std::shared_ptr<vcc::XmlAttribute> attr : child->getAttributes()) {
                    if (isGeneartionTag(attr->getName())) {
                        std::wstring value = attr->getValue();
                        vcc::toUpper(value);
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

std::wstring VPGFileGenerationService::generateForceCode(const vcc::Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    TRY
        // split replacement to lines
        std::vector<std::wstring> lines = vcc::splitStringByLine(generatedContent);
        bool isFound = false;
        std::wstring indent = L"";
        for (const auto &child : src->getChildren()) {
            if (vcc::isStartWith(child->getName(), L"vcc:") && child->getName() == tagName) {
                result += commandDelimiter + L" " + child->getOpeningTag() + L"\r\n";
                size_t noOfSpace = getMinimumLeadingSpace(lines);
                for (auto line : lines) {
                    vcc::rTrim(line);
                    result += indent + line.substr(noOfSpace) + L"\r\n";
                }
                result += indent + commandDelimiter + L" " + child->getClosingTag();
                isFound = true;
            } else {
                result += child->getFullText();
                indent = getIndent(child->getFullText());
            }
        }
        if (!isFound) {
            result += L"\r\n" + indent + commandDelimiter + L" <" + tagName + L" gen=\"REPLACE\">\r\n";
            for (auto line : lines) {
                vcc::trim(line);
                result += indent + line + L"\r\n";
            }
            result += indent + commandDelimiter + L" </" + tagName + L">\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGFileGenerationService::generateDemandCode(const vcc::Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    TRY
        // split replacement to lines
        std::vector<std::wstring> lines = vcc::splitStringByLine(generatedContent);
        std::wstring indent = L"";
        for (const auto &child : src->getChildren()) {
            if (vcc::isStartWith(child->getName(), L"vcc:") && child->getName() == tagName) {            
                const vcc::Xml *srcTag = VPGFileGenerationService::getTagFromCode(src, child->getName());
                if (srcTag != nullptr && VPGFileGenerationService::isTagForce(child.get())) {
                    result += commandDelimiter + L" " + child->getOpeningTag() + L"\r\n";
                    size_t noOfSpace = getMinimumLeadingSpace(lines);
                    for (auto line : lines) {
                        if (!vcc::isBlank(line)) {
                            vcc::rTrim(line);
                            result += indent + line.substr(noOfSpace) + L"\r\n";
                        } else
                            result += L"\r\n";
                    }
                    result += indent + commandDelimiter + L" " + child->getClosingTag();
                } else
                    result += child->getFullText();
            } else {
                result += child->getFullText();
                indent = getIndent(child->getFullText());
            }
        }
    CATCH
    return result;
}

std::wstring VPGFileGenerationService::generateSkipCode(const std::wstring &src)
{
    return src;
}

std::wstring VPGFileGenerationService::generateFileContent(const std::wstring &code, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter)
{
    TRY
        std::unique_ptr<VPGCodeReader> reader = std::make_unique<VPGCodeReader>(commandDelimiter);
        auto codeElement = std::make_shared<vcc::Xml>();
        reader->deserialize(code, codeElement);

        switch (getGenerationMode(codeElement.get()))
        {
        case VPGFileContentGenerationMode::Force:
            return VPGFileGenerationService::generateForceCode(codeElement.get(), tagName, generatedContent, commandDelimiter);
        case VPGFileContentGenerationMode::Demand:
            return VPGFileGenerationService::generateDemandCode(codeElement.get(), tagName, generatedContent, commandDelimiter);
        case VPGFileContentGenerationMode::Skip:
            return VPGFileGenerationService::generateSkipCode(code);
        default:
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown File Content Generation Mode");
            break;
        }
    CATCH
    return code;
}
