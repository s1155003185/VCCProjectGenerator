#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "memory_macro.hpp"
#include "platform_type.hpp"
#include "string_helper.hpp"
#include "vpg_include_path_reader.hpp"

using namespace vcc;

TEST(VPGIncludePathReaderTest, Normal)
{
    DECLARE_UPTR(VPGIncludePathReader, reader);
    std::wstring code = L""
        L"#parma once\r\n"
        L"#include <sth>\r\n"
        L"\r\n"
        L"using namespace qqq;\r\n"
        // command
        L"/*\r\n"
        L"namespace ddd{"
        L"\r\n"
        L"template<some template>\r\n"
        L"class type\r\n"
        L"{\r\n"
        L"public:\r\n"
        L"typedef aaa bbb;\r\n"
        L"};*/\r\n"
        L"\r\n"
        // namspace
        L"namespace /*ddd*/ vcc\r\n"
        L"{\r\n"
        // tmp class
        L"class A;"
        // normal class
        L"template<some template>\r\n"
        L"class type\r\n"
        L"{\r\n"
        L"public:\r\n"
        L"typedef aaa bbb;\r\n"
        L"};\r\n"
        // normal class without ;
        L"template<some template>\r\n"
        L"class type2\r\n"
        L"{\r\n"
        L"public:\r\n"
        L"typedef aaa bbb;\r\n"
        L"}\r\n"
        // class inherit
        L"class Json : public BaseDocument<Json>\r\n"
        L"{};\r\n"
        // typedef
        L"typedef aaa bbb;\r\n"
        // using
        L"using usingType = basic_string<char16_t>;\r\n"
        // command
        L"//typedef aaa ccc;\r\n"
        // multi namespace
        L"namespace multinamespace {\r\n"
        L"class multinamspaceclass\r\n"
        L"{\r\n"
        L"public:\r\n"
        L"typedef aaa bbb;\r\n"
        L"}\r\n"
        L"}\r\n"
        // function
        L"int abc (...);\r\n"
        L"}\r\n";
    std::set<std::wstring> classList;
    reader->Parse(PlatformType::NA, code, classList);

    std::set<std::wstring> resultList;
    resultList.insert(L"vcc::type");
    resultList.insert(L"vcc::type2");
    resultList.insert(L"vcc::Json");
    resultList.insert(L"vcc::bbb");
    resultList.insert(L"vcc::usingType");
    resultList.insert(L"vcc::multinamespace::multinamspaceclass");
    EXPECT_EQ(classList, resultList);
}

// TEST(VPGIncludePathReaderTest, Mac)
// {
//     DECLARE_UPTR(VPGIncludePathReader, reader);
//     // copy and simplify from <string>
//     std::wstring code = L""
//         L"// some command\r\n"
//         L"// some command\r\n}"
//         L"#ifndef _LIBCPP_STRING\r\n"
//         L"#define _LIBCPP_STRING\r\n"
//         L"\r\n"
//         L"/*\r\n"
//         L"    string synopsis\r\n"
//         L"#include <sth>\r\n"
//         L"#include <sth2>\r\n"
//         L"\r\n"
//         L"namespace std\r\n"
//         L"{\r\n"
//         L"template<some template>\r\n"
//         L"class basic_string\r\n"
//         L"{\r\n"
//         L"public:\r\n"
//         L"typedef aaa bbb;\r\n"
//         L"};\r\n"
//         L"typedef basic_string<char>    string;\r\n"
//         L"typedef basic_string<wchar_t> wstring;\r\n"
//         L"typedef basic_string<char8_t> u8string; // C++20\r\n"
//         L"typedef basic_string<char16_t> u16string;\r\n"
//         L"typedef basic_string<char32_t> u32string;\r\n"
//         L"\r\n"
//         L"int abc (...);\r\n"
//         L"} // std\r\n"
//         L"\r\n"
//         L"*/\r\n"
//         L"#endif\r\n";
//     std::set<std::wstring> classList;
//     reader->Parse(PlatformType::MacOs, code, classList);

//     std::set<std::wstring> resultList;
//     resultList.insert(L"std::basic_string");
//     resultList.insert(L"std::string");
//     resultList.insert(L"std::wstring");
//     resultList.insert(L"std::u8string");
//     resultList.insert(L"std::u16string");
//     resultList.insert(L"std::u32string");
//     EXPECT_EQ(classList, resultList);
// }

// TEST(VPGIncludePathReaderTest, Window)
// {
//     DECLARE_UPTR(VPGIncludePathReader, reader);
//     // Visual Studio standard
//     //std::wstring code = L""
//         // L"// xstring internal header (from <string>)\r\n"
//         // L"_STD_BEGIN\r\n"
//         // L"template<aaa,\r\n"
//         // L"bbb>\r\n"
//         // L"class basic_string\r\n"
//         // L": public _String_alloc<_String_base_types<_Elem, _Alloc> >\r\n"
//         // L"{ }\r\n"
//         // L"\r\n"
//         // L"typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >\r\n"
//         // L"wstring;\r\n"
//         // L"typedef basic_string<char, char_traits<char>, allocator<char> >\r\n"
//         // L"string;\r\n"
//         // L"_STD_END\r\n";
    
//     std::set<std::wstring> classList;
//     reader->Parse(PlatformType::Window, code, classList);

//     std::set<std::wstring> resultList;
//     resultList.insert(L"std::basic_string");
//     resultList.insert(L"std::wstring");
//     resultList.insert(L"std::string");
//     EXPECT_EQ(classList, resultList);
// }

// TEST(VPGIncludePathReaderTest, Linux)
// {
//     //assert(false);
//     DECLARE_UPTR(VPGIncludePathReader, reader); 
//     std::wstring code = L"";
//     std::set<std::wstring> classList;
//     reader->Parse(PlatformType::Linux, code, classList);

//     std::set<std::wstring> resultList;
//     EXPECT_EQ(classList, resultList);
// }
