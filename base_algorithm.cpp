#include "base_algorithm.h"

namespace vcc {

BaseAlgorithm::BaseAlgorithm(HyperGraph* hyper_graph,
                             std::shared_ptr<OutputQueue> output_queue)
    : output_queue_(std::move(output_queue)) {
    hyper_graph_.reset(hyper_graph);
}
}  // namespace vcc
