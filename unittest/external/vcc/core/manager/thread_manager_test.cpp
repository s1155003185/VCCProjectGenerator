#include <gtest/gtest.h>

#include "thread.hpp"
#include "thread_manager.hpp"

class ThreadManagerTest : public testing::Test 
{
    MANAGER_SPTR_NULL(vcc::ThreadManager, Manager);
    GETSET_SPTR_NULL(vcc::LogConfig, LogConfig)

    public:
        void SetUp() override
        {
            _Manager = std::make_shared<vcc::ThreadManager>(nullptr);
            _Manager->setThreadManagementMode(vcc::ThreadManagementMode::Join);
            _LogConfig = std::make_shared<vcc::LogConfig>(vcc::LogConfigInitialType::None);
        }

        void TearDown() override
        {
        }
};

TEST_F(ThreadManagerTest, SuspendAndQueue)
{
    size_t threadCnt = 0;
    size_t callbackCnt = 0;
    auto thread1 = std::make_shared<vcc::Thread>(getLogConfig(), [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/){
        EXPECT_EQ(threadCnt, (size_t)0);
        EXPECT_EQ(callbackCnt, (size_t)0);
        threadCnt++;
    }, [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/) {
        EXPECT_EQ(threadCnt, (size_t)1);
        EXPECT_EQ(callbackCnt, (size_t)0);
        callbackCnt++;
    });
    auto thread2 = std::make_shared<vcc::Thread>(getLogConfig(), [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/){
        EXPECT_EQ(threadCnt, (size_t)1);
        EXPECT_EQ(callbackCnt, (size_t)1);
        threadCnt++;
    }, [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/) {
        EXPECT_EQ(threadCnt, (size_t)2);
        EXPECT_EQ(callbackCnt, (size_t)1);
        callbackCnt++;
    });
    auto thread3 = std::make_shared<vcc::Thread>(getLogConfig(), [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/){
        EXPECT_EQ(threadCnt, (size_t)2);
        EXPECT_EQ(callbackCnt, (size_t)2);
        threadCnt++;
    }, [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/) {
        EXPECT_EQ(threadCnt, (size_t)3);
        EXPECT_EQ(callbackCnt, (size_t)2);
        callbackCnt++;
    });

    getManager()->suspend();
    getManager()->queue(thread1);
    getManager()->queue(thread2);
    getManager()->queue(thread3);
    getManager()->resume();

    EXPECT_EQ(threadCnt, (size_t)3);
    EXPECT_EQ(callbackCnt, (size_t)3);
}

TEST_F(ThreadManagerTest, SuspendAndUrgent) 
{
    size_t threadCnt = 0;
    auto thread1 = std::make_shared<vcc::Thread>(getLogConfig(),
        [&threadCnt](const vcc::Thread * /*thread*/){
            EXPECT_EQ(threadCnt, (size_t)1);
            threadCnt++;
        }, [](const vcc::Thread * /*thread*/) {});
    auto thread2 = std::make_shared<vcc::Thread>(getLogConfig(),
        [&threadCnt](const vcc::Thread * /*thread*/){
            EXPECT_EQ(threadCnt, (size_t)0);
            threadCnt++;
        }, [](const vcc::Thread * /*thread*/) {});
    getManager()->suspend();
    getManager()->queue(thread1);
    getManager()->urgent(thread2);
    getManager()->resume();

    EXPECT_EQ(threadCnt, (size_t)2);
}

TEST_F(ThreadManagerTest, Stop) 
{
    auto thread1 = std::make_shared<vcc::Thread>(getLogConfig(), [](const vcc::Thread * /*thread*/){}, [](const vcc::Thread * /*thread*/) {});
    getManager()->suspend();
    getManager()->queue(thread1);
    getManager()->stop();
    EXPECT_TRUE(getManager()->getThreads().empty());
    EXPECT_TRUE(getManager()->getActiveThreads().empty());
}

TEST_F(ThreadManagerTest, ClearWaitingThread) 
{
    auto thread1 = std::make_shared<vcc::Thread>(getLogConfig(), [](const vcc::Thread * /*thread*/){}, [](const vcc::Thread * /*thread*/) {});
    getManager()->suspend();
    getManager()->queue(thread1);
    getManager()->clearWaitingThread();
    EXPECT_TRUE(getManager()->getThreads().empty());
    EXPECT_TRUE(getManager()->getActiveThreads().empty());
}
