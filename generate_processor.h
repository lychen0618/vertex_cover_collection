#pragma once
#include "common/ConcurrentDropHeadQueue.h"
#include "protos/vertex_cover.grpc.pb.h"

namespace vcc {
typedef ConcurrentDropHeadQueue<GenResponse> GenResponseQueue;
class GenerateProcessor {
public:
    GenerateProcessor(std::shared_ptr<GenResponseQueue> req_queue);
    ~GenerateProcessor() = default;

    void Process(const GenRequest* const req);

private:
    std::shared_ptr<GenResponseQueue> req_queue_;
};
}  // namespace vcc