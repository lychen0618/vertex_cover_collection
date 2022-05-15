#include "common/concurrent_drop_head_queue.h"
#include "gtest/gtest.h"
using vcc::ConcurrentDropHeadQueue;

namespace {
class ConcurrentQueueTest : public ::testing::Test {
protected:
    ConcurrentQueueTest() {}
    ~ConcurrentQueueTest() override {}
};

TEST_F(ConcurrentQueueTest, SingleThreadTest) {
    ConcurrentDropHeadQueue<int> con_queue(3);
    con_queue.Push(3);
    con_queue.Push(2);
    con_queue.Push(1);
    EXPECT_EQ(con_queue.Pop(), 3);
    EXPECT_EQ(con_queue.Pop(), 2);
    EXPECT_EQ(con_queue.Pop(), 1);
    con_queue.Push(4);
    con_queue.Push(3);
    con_queue.Push(2);
    // drop head
    con_queue.Push(1);
    EXPECT_EQ(con_queue.Pop(), 3);
    EXPECT_EQ(con_queue.Pop(), 2);
    EXPECT_EQ(con_queue.Pop(), 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace
