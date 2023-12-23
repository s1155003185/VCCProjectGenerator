#include <iostream>
#include <string>
#include <vector>

#include "process.hpp"

// copy from VCCModule string_helper.hpp
std::wstring str2wstr(const std::string& str)
{
	{
		if (str.empty())
			return L"";
		int len = str.size() + 1;
		std::unique_ptr<wchar_t[]> p(new wchar_t[len]);
		mbstowcs(p.get(), str.c_str(), len);
		std::wstring wstr(p.get());
		return wstr;
	}
}

int main(int argc, char **argv)
{
	try {
		if (argc < 2) {
			std::wcout << L"No Argument" << std::endl;
			return 0;
		}
		std::vector<std::wstring> argList;
		for (int i = 0; i < argc; i++) {
			std::wstring arg = str2wstr(argv[i]);
			//std::wcout << L"argv[" + std::to_wstring(i) + L"] " + arg  << std::endl;
			argList.push_back(arg);
		}

		Process process;
		process.Execute(argList);
	} catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return -1;
	}
	return 0;
}

