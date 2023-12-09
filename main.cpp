#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
	try {
		if (argc < 2) {
			std::wcout << L"No Argument" << std::endl;
			return 0;
		}
	// 	std::vector<std::wstring> argList;
	// 	for (int i = 0; i < argc; i++) {
	// 		std::wstring arg = str2wstr(argv[i]);
	// 		//std::wcout << L"argv[" + std::to_wstring(i) + L"] " + arg  << std::endl;
	// 		argList.push_back(arg);
	// 	}

	// 	// Process process;
	// 	// process.Execute(argList);
	} catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return -1;
	}
	return 0;
}

