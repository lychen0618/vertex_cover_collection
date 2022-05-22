#include "thread_pool.h"

namespace vcc {
ThreadPool::ThreadPool(int threads_limit)
    : thread_num_(threads_limit), mutex_(new std::mutex()), task_num_(0) {}

ThreadPool::~ThreadPool() {
    pool_.reset();
    mutex_.reset();
}

void ThreadPool::Start() {
    pool_.reset(new boost::asio::thread_pool(thread_num_));
}

void ThreadPool::Wait() { pool_->join(); }

void ThreadPool::AddTask(std::function<void()> task) {
    boost::asio::post(*pool_, task);
}

bool ThreadPool::IsFull() {
    std::unique_lock<std::mutex> lock(*mutex_);
    if (task_num_ < thread_num_) {
        task_num_++;
        return false;
    } else
        return true;
}

void ThreadPool::TaskEnd() {
    std::unique_lock<std::mutex> lock(*mutex_);
    task_num_--;
}
}  // namespace vcc