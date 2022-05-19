#pragma once

#include "common/concurrent_drop_head_queue.h"
#include "common/hypergraph.h"

namespace vcc {

typedef ConcurrentDropHeadQueue<IntSet> OutputQueue;

class BaseAlgorithm {
public:
    BaseAlgorithm(std::shared_ptr<HyperGraph> hyper_graph,
                  std::shared_ptr<OutputQueue> output_queue);

    virtual void RunAll() { LOG(WARNING) << "Generate all vertex covers"; }
    virtual void RunDiverse() {
        LOG(WARNING) << "Generate a set of diverse vertex covers";
    }
    virtual void RunApproximate() {
        LOG(WARNING) << "Generate all approximate vertex covers";
    }

protected:
    std::shared_ptr<HyperGraph> hyper_graph_;
    std::shared_ptr<OutputQueue> output_queue_;
};
}  // namespace vcc