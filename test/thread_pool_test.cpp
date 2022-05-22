#include "common/thread_pool.h"

#include <unistd.h>

#include "gtest/gtest.h"
using vcc::ThreadPool;

namespace {
class ThreadPoolTest : public ::testing::Test {
protected:
    ThreadPoolTest() {}
    ~ThreadPoolTest() override {}
};

void* task(void* arg1, void* arg2) {
    int* param = (int*)arg1;
    printf("[%lu] --> Working on task %d\n", pthread_self(), *param);
    sleep(5);
    delete param;
    ThreadPool* pool = (ThreadPool*)arg2;
    pool->TaskEnd();
    return nullptr;
}

TEST_F(ThreadPoolTest, SimpleTest) {
    std::cout << "-------------" << std::endl;

    std::shared_ptr<ThreadPool> pool(new ThreadPool(4));
    pool->Start();
    for (int i = 0; i < 20; i++) {
        std::cout << i << std::endl;
        int* arg = new int(i);
        ThreadPool* pt = pool.get();
        if (!pool->IsFull())
            pool->AddTask([arg, pt] { task(arg, pt); });
        else {
            std::ostringstream oss;
            oss << "pool is full, skip task " << i << std::endl;
            std::cout << oss.str();
        }
        sleep(1);
    }

    pool->Wait();
    std::cout << "-------------" << std::endl;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace