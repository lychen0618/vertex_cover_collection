#pragma once

#include <memory>
#include <mutex>

#include "boost/asio.hpp"
#include "boost/thread.hpp"


namespace vcc{
class ThreadPool : public std::enable_shared_from_this<ThreadPool> {
  public:
    explicit ThreadPool(int threads_limit = 4);

    ~ThreadPool();

    void Start();

    void Wait();

    void AddTask(std::function<void()> task);

    bool IsFull();

    void TaskEnd();

  private:
    int thread_num_;
    std::shared_ptr<std::mutex> mutex_;
    int task_num_;
    std::unique_ptr<boost::asio::thread_pool> pool_;
};

} // namespace vcc