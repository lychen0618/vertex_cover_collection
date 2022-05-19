#include "base_algorithm.h"

namespace vcc {

BaseAlgorithm::BaseAlgorithm(std::shared_ptr<HyperGraph> hyper_graph,
                             std::shared_ptr<OutputQueue> output_queue)
    : hyper_graph_(std::move(hyper_graph)),
      output_queue_(std::move(output_queue)) {}
}  // namespace vcc
