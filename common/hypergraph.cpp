#include "hypergraph.h"

namespace vcc {
HyperGraph::HyperGraph(const std::vector<IntSet>& edge_list)
    : edge_list_(new std::vector<IntSet>(edge_list)) {
    vertices_.reset(new BitSet());
    for (const auto& edge : *edge_list_) {
        for (const auto& e : edge.Get()) {
            vertices_->Set(e);
        }
    }
}

const IntSet& HyperGraph::GetVertexHitting(int vertex) {
    if (vertex_hittings_ == nullptr) {
        vertex_hittings_.reset(new std::unordered_map<int, IntSet>());
        for (const int& e : vertices_->Get()) {
            (*vertex_hittings_)[e] = IntSet();
        }
        int edge_id = 0;
        for (const auto& edge : *edge_list_) {
            for (size_t i = 0; i < edge.Size(); ++i) {
                (*vertex_hittings_)[edge.Get(i)].Add(edge_id);
            }
            ++edge_id;
        }
    }
    return (*vertex_hittings_)[vertex];
}

}  // namespace vcc
