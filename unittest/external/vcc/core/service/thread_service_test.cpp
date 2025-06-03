#include <gtest/gtest.h>

#include <iostream>

#include "thread.hpp"
#include "thread_service.hpp"
#include "log_config.hpp"

TEST(ThreadServiceTest, Join)
{
    auto logConfig = std::make_shared<vcc::LogConfig>(vcc::LogConfigInitialType::None);
    auto thread = std::make_shared<vcc::Thread>(logConfig,
        [](const vcc::Thread* /*thread*/) {
            std::wcout << L"ThreadServiceTest::Join Execute!" << std::endl;
        }, [](const vcc::Thread* /*thread*/) {
            std::wcout << L"ThreadServiceTest::Join Complete!" << std::endl;
        });
    vcc::ThreadService::Join(thread);
}

TEST(ThreadServiceTest, Detach)
{
    auto logConfig = std::make_shared<vcc::LogConfig>(vcc::LogConfigInitialType::None);
    auto thread = std::make_shared<vcc::Thread>(logConfig,
        [](const vcc::Thread* /*thread*/) {
            std::wcout << L"ThreadServiceTest::Detach Execute!" << std::endl;
        }, [](const vcc::Thread* /*thread*/) {
            std::wcout << L"ThreadServiceTest::Detach Complete!" << std::endl;
        });
    vcc::ThreadService::Detach(thread);
}