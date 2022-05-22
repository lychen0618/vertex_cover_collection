#include <unistd.h>

#include "common/thread_pool.h"
#include "gtest/gtest.h"
using vcc::ThreadPool;

namespace {
class ThreadPoolTest : public ::testing::Test {
protected:
    ThreadPoolTest() {}
    ~ThreadPoolTest() override {}
};

void* task(void* arg) {
    int* param = (int*)arg;
    printf("[%lu] --> Working on task %d\n", pthread_self(), *param);
    sleep(2);
    delete param;
    return nullptr;
}

TEST_F(ThreadPoolTest, SimpleTest) {
    std::cout << "-------------" << std::endl;

    std::shared_ptr<ThreadPool> pool(new ThreadPool(4));
    pool->Start();
    for (int i = 0; i < 10; i++) {
        std::cout << i << std::endl;
        int* arg = new int(i);
        pool->AddTask([arg] { task(arg); });
    }

    sleep(40);
    std::cout << "-------------" << std::endl;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace