#pragma once

#include <string>

#include "log_config.hpp"
#include "vpg_code_reader.hpp"

enum class VPGFileContentGenerationMode
{
    NA,
    Force,
    Demand,
    Skip
};

class VPGFileGenerationService
{
    private:
        VPGFileGenerationService() = default;
        virtual ~VPGFileGenerationService() {}
    
        static size_t getLeadingSpace(const std::wstring &line);
        static size_t getMinimumLeadingSpace(const std::vector<std::wstring> &lines);
        static const vcc::Xml *getTagFromCode(const vcc::Xml *code, const std::wstring &tagName);

        static bool isGeneartionTag(const std::wstring &tag);
        static bool isTagForce(const vcc::Xml *child);
        static bool isTagSkip(const vcc::Xml *child);
        static VPGFileContentGenerationMode getGenerationMode(const vcc::Xml *codeElemet);

        static std::wstring generateForceCode(const vcc::Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter);
        static std::wstring generateDemandCode(const vcc::Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter);
        static std::wstring generateSkipCode(const std::wstring &src);

    public:
        static std::wstring getIndent(const std::wstring &str);

        static std::wstring generateFileContent(const std::wstring &code, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter);
};