#pragma once

#include <glog/logging.h>

#include <condition_variable>
#include <mutex>
#include <queue>

namespace vcc {
template <typename RequestType>
class ConcurrentDropHeadQueue {
public:
    ConcurrentDropHeadQueue(size_t capacity) : capacity_(capacity) {}
    ~ConcurrentDropHeadQueue() = default;

    void Push(RequestType req) {
        std::unique_lock<std::mutex> lock(mutex_);
        size_t queue_size = queue_.size();
        if (queue_size >= capacity_) {
            // Queue full. Drop head, i.e., the oldest push request.
            // auto drop_req = std::move(queue_.front());
            queue_.pop();
            LOG(WARNING) << "Queue full. Drop head.";
        }
        queue_.push(std::move(req));
        cond_.notify_one();
    }

    RequestType Pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty()) {
            cond_.wait(lock);
        }
        auto req = std::move(queue_.front());
        queue_.pop();
        return req;
    }

private:
    std::queue<RequestType> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
    const size_t capacity_;
};
}  // namespace vcc