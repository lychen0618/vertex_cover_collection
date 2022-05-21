#pragma once

#include "hypergraph.h"

namespace vcc {
double f1(std::shared_ptr<HyperGraph> hyper_graph, int uncov) {
    return 1 - uncov * 1.0 / hyper_graph->EdgeNum();
}
}  // namespace vcc