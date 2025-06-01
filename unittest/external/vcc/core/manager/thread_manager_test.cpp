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
            _Manager->SetThreadManagementMode(vcc::ThreadManagementMode::Join);
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
    auto thread1 = std::make_shared<vcc::Thread>(GetLogConfig(), [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/){
        EXPECT_EQ(threadCnt, (size_t)0);
        EXPECT_EQ(callbackCnt, (size_t)0);
        threadCnt++;
    }, [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/) {
        EXPECT_EQ(threadCnt, (size_t)1);
        EXPECT_EQ(callbackCnt, (size_t)0);
        callbackCnt++;
    });
    auto thread2 = std::make_shared<vcc::Thread>(GetLogConfig(), [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/){
        EXPECT_EQ(threadCnt, (size_t)1);
        EXPECT_EQ(callbackCnt, (size_t)1);
        threadCnt++;
    }, [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/) {
        EXPECT_EQ(threadCnt, (size_t)2);
        EXPECT_EQ(callbackCnt, (size_t)1);
        callbackCnt++;
    });
    auto thread3 = std::make_shared<vcc::Thread>(GetLogConfig(), [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/){
        EXPECT_EQ(threadCnt, (size_t)2);
        EXPECT_EQ(callbackCnt, (size_t)2);
        threadCnt++;
    }, [&threadCnt, &callbackCnt](const vcc::Thread * /*thread*/) {
        EXPECT_EQ(threadCnt, (size_t)3);
        EXPECT_EQ(callbackCnt, (size_t)2);
        callbackCnt++;
    });

    GetManager()->Suspend();
    GetManager()->Queue(thread1);
    GetManager()->Queue(thread2);
    GetManager()->Queue(thread3);
    GetManager()->Resume();

    EXPECT_EQ(threadCnt, (size_t)3);
    EXPECT_EQ(callbackCnt, (size_t)3);
}

TEST_F(ThreadManagerTest, SuspendAndUrgent) 
{
    size_t threadCnt = 0;
    auto thread1 = std::make_shared<vcc::Thread>(GetLogConfig(),
        [&threadCnt](const vcc::Thread * /*thread*/){
            EXPECT_EQ(threadCnt, (size_t)1);
            threadCnt++;
        }, [](const vcc::Thread * /*thread*/) {});
    auto thread2 = std::make_shared<vcc::Thread>(GetLogConfig(),
        [&threadCnt](const vcc::Thread * /*thread*/){
            EXPECT_EQ(threadCnt, (size_t)0);
            threadCnt++;
        }, [](const vcc::Thread * /*thread*/) {});
    GetManager()->Suspend();
    GetManager()->Queue(thread1);
    GetManager()->Urgent(thread2);
    GetManager()->Resume();

    EXPECT_EQ(threadCnt, (size_t)2);
}

TEST_F(ThreadManagerTest, Stop) 
{
    auto thread1 = std::make_shared<vcc::Thread>(GetLogConfig(), [](const vcc::Thread * /*thread*/){}, [](const vcc::Thread * /*thread*/) {});
    GetManager()->Suspend();
    GetManager()->Queue(thread1);
    GetManager()->Stop();
    EXPECT_TRUE(GetManager()->GetThreads().empty());
    EXPECT_TRUE(GetManager()->GetActiveThreads().empty());
}

TEST_F(ThreadManagerTest, ClearWaitingThread) 
{
    auto thread1 = std::make_shared<vcc::Thread>(GetLogConfig(), [](const vcc::Thread * /*thread*/){}, [](const vcc::Thread * /*thread*/) {});
    GetManager()->Suspend();
    GetManager()->Queue(thread1);
    GetManager()->ClearWaitingThread();
    EXPECT_TRUE(GetManager()->GetThreads().empty());
    EXPECT_TRUE(GetManager()->GetActiveThreads().empty());
}
