#pragma once

#include <string>
#include <vector>

struct VPGEnumClassProperty
{
    std::wstring Name = L"";
    std::wstring Command = L"";
};

struct VPGEnumClass
{
    std::wstring Name = L"";
    std::wstring Command = L"";
    std::vector<VPGEnumClassProperty> Properties;
};

class VPGEnumClassReader
{
    private:
        std::wstring _GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg);

        std::wstring _GetName(const std::wstring &cppCode, size_t &pos);
        std::wstring _GetCommand(const std::wstring &cppCode, size_t &pos);

        void _ParseProperties(const std::wstring &cppCode, size_t &pos, VPGEnumClass &enumClass);
        void _ParseClass(const std::wstring &cppCode, size_t &pos, VPGEnumClass &enumClass);
    public:
        VPGEnumClassReader() = default;
        ~VPGEnumClassReader() {}
        void Parse(const std::wstring &cppCode, std::vector<VPGEnumClass> &results);
};