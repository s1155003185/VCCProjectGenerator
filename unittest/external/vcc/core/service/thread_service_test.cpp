#include <gtest/gtest.h>

#include <iostream>

#include "memory_macro.hpp"
#include "thread.hpp"
#include "thread_service.hpp"
#include "log_config.hpp"

using namespace vcc;

TEST(ThreadServiceTest, Join)
{
    DECLARE_SPTR(LogConfig, logProperty, LogConfigInitialType::None);
    DECLARE_SPTR(Thread, thread, logProperty, [](const Thread* /*thread*/) {
        std::wcout << L"ThreadServiceTest::Join Execute!" << std::endl;
    }, [](const Thread* /*thread*/) {
        std::wcout << L"ThreadServiceTest::Join Complete!" << std::endl;
    });
    ThreadService::Join(thread);
}

TEST(ThreadServiceTest, Detach)
{
    DECLARE_SPTR(LogConfig, logProperty, LogConfigInitialType::None);
    DECLARE_SPTR(Thread, thread, logProperty, [](const Thread* /*thread*/) {
        std::wcout << L"ThreadServiceTest::Detach Execute!" << std::endl;
    }, [](const Thread* /*thread*/) {
        std::wcout << L"ThreadServiceTest::Detach Complete!" << std::endl;
    });
    ThreadService::Detach(thread);
}