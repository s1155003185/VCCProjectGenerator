#include <iostream>
#include <string>
#include <vector>

#include "log_property.hpp"
#include "memory_macro.hpp"
#include "vpg_process_manager.hpp"
#include "string_helper.hpp"

using namespace vcc;

int main(int argc, char **argv)
{
	DECLARE_SPTR(LogProperty, logProperty);
	logProperty->Init(LogPropertyType::None);
	logProperty->SetIsConsoleLog(true);
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
		VPGProcessManager process(logProperty);
		process.Execute(argList);
	} catch (std::exception &ex) {
		const IException *ie = dynamic_cast<const IException *>(&ex);
		if (ie != nullptr)
			LogService::LogError(logProperty.get(), L"", ie->GetErrorMessage());
		else
			LogService::LogError(logProperty.get(), L"", str2wstr(ex.what()));
		return -1;
	}
	return 0;
}

