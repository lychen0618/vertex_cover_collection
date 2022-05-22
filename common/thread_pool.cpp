#include "thread_pool.h"

namespace vcc {
ThreadPool::ThreadPool(int threads_limit)
    : thread_num_(threads_limit),
      service_{},
      service_worker_(new boost::asio::io_service::work(service_)),
      closed_(false) {}

ThreadPool::~ThreadPool() { Close(); }

void ThreadPool::Start() {
    auto thisPtr = shared_from_this();
    auto worker = [thisPtr] {
        std::ostringstream oss;
        oss << "current thread: " << std::this_thread::get_id();
        std::cout << oss.str() << std::endl;

        if (!thisPtr->closed_) {
            std::cout << oss.str() << std::endl;
            return thisPtr->service_.run();
        }
        return size_t(0);
    };

    for (int i = 0; i < thread_num_; ++i) {
        group_.add_thread(new boost::thread(worker));
    }
}

void ThreadPool::Close() {
    closed_ = true;
    service_worker_.reset();
    group_.join_all();
    service_.stop();
}

void ThreadPool::AddTask(std::function<void()> task) {
    service_.dispatch(task);
}
}  // namespace vcc