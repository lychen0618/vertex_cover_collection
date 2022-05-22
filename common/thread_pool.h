#pragma once

#include <memory>
#include <vector>
#include <iostream>

#include "boost/asio.hpp"
#include "boost/thread.hpp"

namespace vcc{
class ThreadPool : public std::enable_shared_from_this<ThreadPool> {
  public:
    explicit ThreadPool(int threads_limit = 4);
    ~ThreadPool();

    void AddTask(std::function<void()> task);

    void Close();
    void Start();

  private:
    int thread_num_;
    boost::asio::io_service service_;
    std::unique_ptr<boost::asio::io_service::work> service_worker_;
    boost::thread_group group_;
    std::atomic<bool> closed_;
};

} // namespace vcc