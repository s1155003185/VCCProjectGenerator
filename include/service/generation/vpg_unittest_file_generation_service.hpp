#pragma once

#include <set>
#include <string>
#include <vector>

#include "log_config.hpp"
#include "vpg_enum_class.hpp"

class VPGUnittestFileGenerationService {
   private:
    VPGUnittestFileGenerationService() = delete;
    ~VPGUnittestFileGenerationService() {}

    static std::wstring generateUnittestClass(const std::wstring& className);
    static std::wstring generateUnittestTestCase(const std::wstring& testClassName,
                                                 const std::wstring& testName);

   public:
    static void generateCppAction(const vcc::LogConfig* logConfig,
                                  const std::wstring& projectPrefix,
                                  const std::wstring& originalPropertyFilePath,
                                  const std::wstring& folderPathCpp,
                                  const std::vector<std::shared_ptr<VPGEnumClass>>& enumClassList,
                                  const std::vector<std::wstring>& unittestNames,
                                  const bool& isSeperateFile);
};
