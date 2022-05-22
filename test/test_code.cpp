#include <unistd.h>

#include <bitset>
#include <iostream>

#include "base_algorithm.h"
#include "common/intset.h"
#include "common/thread_pool.h"
#include "glog/logging.h"

using vcc::BaseAlgorithm;
using vcc::OutputQueue;
using vcc::ThreadPool;

class A {
public:
    A() {
        thread_pool_.reset(new ThreadPool(2));
        thread_pool_->Start();
    }
    void func(int new_x) {
        std::ostringstream oss;
        oss << pthread_self() << " input x: " << new_x << std::endl;
        oss << "x: " << ++x << std::endl;
        std::cout << oss.str();
        sleep(1);
    }

    void start() {
        for (int i = 0; i < 4; ++i) {
            thread_pool_->AddTask(std::bind(&A::func, this, i));
        }
        sleep(20);
    }

private:
    std::shared_ptr<ThreadPool> thread_pool_;
    int x = 0;
};

int main(int argc, char** argv) {
    A a;
    a.start();
    return 0;
}