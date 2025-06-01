#include <iostream>
#include <string>
#include <vector>

#include "application.hpp"

#include "log_config.hpp"
#include "vpg_process_manager.hpp"
#include "string_helper.hpp"

int main(int argc, char **argv)
{
	Application::Run();

	auto logConfig = std::make_shared<vcc::LogConfig>();
	logConfig->Initialize(vcc::LogConfigInitialType::None);
	logConfig->SetIsConsoleLog(false);
	try {
		if (argc < 2) {
			std::wcout << L"No Argument" << std::endl;
			return 0;
		}
		std::vector<std::wstring> argList;
		for (int i = 0; i < argc; i++)
			argList.push_back(vcc::str2wstr(argv[i]));

		// debug use
		// argList.clear();
		// argList.push_back(L"vpg");
		// argList.push_back(L"-Generate");

		VPGProcessManager process(logConfig);
		process.Execute(argList);
	} catch (std::exception &ex) {
		const vcc::IException *ie = dynamic_cast<const vcc::IException *>(&ex);
		if (ie != nullptr)
			vcc::LogService::LogError(logConfig.get(), L"", ie->GetErrorMessage());
		else
			vcc::LogService::LogError(logConfig.get(), L"", vcc::str2wstr(ex.what()));
		return -1;
	}
	return 0;
}

