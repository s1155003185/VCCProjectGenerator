#pragma once

#include <string>

#include "log_property.hpp"
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
    
        static const Xml *GetTagFromCode(const Xml *code, const std::wstring &tagName);

        static bool IsGeneartionTag(const std::wstring &tag);
        static bool IsTagForce(const Xml *child);
        static bool IsTagSkip(const Xml *child);
        static VPGFileContentGenerationMode GetGenerationMode(const Xml *codeElemet);

        static std::wstring GenerateForceCode(const Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter);
        static std::wstring GenerateDemandCode(const Xml *src, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter);
        static std::wstring GenerateSkipCode(const std::wstring &src);

    public:
        static std::wstring GetIndent(const std::wstring &str);

        static std::wstring GenerateFileContent(const std::wstring &code, const std::wstring &tagName, const std::wstring &generatedContent, const std::wstring &commandDelimiter);
};