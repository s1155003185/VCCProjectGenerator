#include <gtest/gtest.h>

#include <iostream>

#include "thread.hpp"
#include "thread_service.hpp"
#include "log_config.hpp"

using namespace vcc;

TEST(ThreadServiceTest, Join)
{
    auto logConfig = std::make_shared<LogConfig>(LogConfigInitialType::None);
    auto thread = std::make_shared<Thread>(logConfig,
        [](const Thread* /*thread*/) {
            std::wcout << L"ThreadServiceTest::Join Execute!" << std::endl;
        }, [](const Thread* /*thread*/) {
            std::wcout << L"ThreadServiceTest::Join Complete!" << std::endl;
        });
    ThreadService::Join(thread);
}

TEST(ThreadServiceTest, Detach)
{
    auto logConfig = std::make_shared<LogConfig>(LogConfigInitialType::None);
    auto thread = std::make_shared<Thread>(logConfig,
        [](const Thread* /*thread*/) {
            std::wcout << L"ThreadServiceTest::Detach Execute!" << std::endl;
        }, [](const Thread* /*thread*/) {
            std::wcout << L"ThreadServiceTest::Detach Complete!" << std::endl;
        });
    ThreadService::Detach(thread);
}