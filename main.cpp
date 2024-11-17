#include <iostream>
#include <string>
#include <vector>

#include "application.hpp"

#include "log_config.hpp"
#include "memory_macro.hpp"
#include "vpg_process_manager.hpp"
#include "string_helper.hpp"

using namespace vcc;

int main(int argc, char **argv)
{
	Application::Run();


	DECLARE_SPTR(LogConfig, logConfig);
	logConfig->Initialize(LogConfigInitialType::None);
	logConfig->SetIsConsoleLog(false);
	try {
		if (argc < 2) {
			std::wcout << L"No Argument" << std::endl;
			return 0;
		}
		std::vector<std::wstring> argList;
		for (int i = 0; i < argc; i++) {
			std::wstring arg = str2wstr(argv[i]);
			argList.push_back(arg);
		}

		// debug use
		// argList.clear();
		// argList.push_back(L"vpg");
		// argList.push_back(L"-Generate");

		VPGProcessManager process(logConfig);
		process.Execute(argList);
	} catch (std::exception &ex) {
		const IException *ie = dynamic_cast<const IException *>(&ex);
		if (ie != nullptr)
			LogService::LogError(logConfig.get(), L"", ie->GetErrorMessage());
		else
			LogService::LogError(logConfig.get(), L"", str2wstr(ex.what()));
		return -1;
	}
	return 0;
}

