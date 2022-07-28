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

HyperGraph::HyperGraph(std::unordered_set<int>& vertex_set,
                       std::vector<IntSet>& edge_set)
    : edge_list_(new std::vector<IntSet>(std::move(edge_set))) {
    vertices_.reset(new BitSet());
    for (auto it = vertex_set.begin(); it != vertex_set.end(); ++it) {
        vertices_->Set(*it);
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

std::vector<std::vector<int>> HyperGraph::GetSubGraph(
    std::vector<std::shared_ptr<HyperGraph>>& sub_graphs) {
    std::vector<std::vector<int>> original_ei;
    std::vector<bool> vertex_flag(vertices_->Size(), false);
    std::vector<bool> edge_flag(EdgeNum(), false);
    for (int i = 0; i < static_cast<int>(vertex_flag.size()); ++i) {
        if (!vertices_->Get(i) || vertex_flag[i])
            continue;
        std::unordered_set<int> vertex_set;
        std::vector<IntSet> edge_set;
        std::vector<int> ei_list;
        GetSubGraphHelpFunc(i, vertex_set, edge_set, vertex_flag, edge_flag,
                            ei_list);
        sub_graphs.emplace_back(new HyperGraph(vertex_set, edge_set));
        original_ei.emplace_back(std::move(ei_list));
    }
    return original_ei;
}

void HyperGraph::GetSubGraphHelpFunc(int vertex,
                                     std::unordered_set<int>& vertex_set,
                                     std::vector<IntSet>& edge_set,
                                     std::vector<bool>& vertex_flag,
                                     std::vector<bool>& edge_flag,
                                     std::vector<int>& ei_list) {
    if (vertex_flag[vertex])
        return;
    vertex_set.insert(vertex);
    vertex_flag[vertex] = true;
    for (int e_i : GetVertexHitting(vertex).Get()) {
        if (edge_flag[e_i])
            continue;
        edge_set.emplace_back(GetEdge(e_i));
        edge_flag[e_i] = true;
        ei_list.push_back(e_i);
        for (int new_vertex : GetEdge(e_i).Get()) {
            GetSubGraphHelpFunc(new_vertex, vertex_set, edge_set, vertex_flag,
                                edge_flag, ei_list);
        }
    }
}

}  // namespace vcc
