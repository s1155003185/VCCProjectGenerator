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
    
        static size_t GetLeadingSpace(const std::wstring &line);
        static size_t GetMinimumLeadingSpace(const std::vector<std::wstring> &lines);
        static const vcc::Xml *GetTagFromCode(const vcc::Xml *code, const std::wstring &tagName);

        static bool IsGeneartionTag(const std::wstring &tag);
        static bool IsTagForce(const vcc::Xml *child);
        static bool IsTagSkip(const vcc::Xml *child);
        static VPGFileContentGenerationMode GetGenerationMode(const vcc::Xml *codeElemet);

        static std::wstring GenerateForceCode(const vcc::Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter);
        static std::wstring GenerateDemandCode(const vcc::Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter);
        static std::wstring GenerateSkipCode(const std::wstring &src);

    public:
        static std::wstring GetIndent(const std::wstring &str);

        static std::wstring GenerateFileContent(const std::wstring &code, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter);
};