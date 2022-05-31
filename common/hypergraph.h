#pragma once

#include <unordered_map>
#include <unordered_set>

#include "common/intset.h"

namespace vcc {

class HyperGraph {
public:
    HyperGraph(const std::vector<IntSet>& edge_list);
    HyperGraph(std::unordered_set<int>& vertex_set, std::vector<IntSet>& edge_set);
    ~HyperGraph() = default;
    size_t VertexNum() const { return vertices_->Count(); }
    size_t EdgeNum() const { return edge_list_->size(); }

    const BitSet& GetVertices() const { return *vertices_; }

    const IntSet& GetEdge(size_t index) const { return (*edge_list_)[index]; }

    const IntSet& GetVertexHitting(int vertex);

    void GetSubGraph(std::vector<std::shared_ptr<HyperGraph>>& sub_graphs);

private:
    void GetSubGraphHelpFunc(int vertex,
                             std::unordered_set<int>& vertex_set,
                             std::vector<IntSet>& edge_set,
                             std::vector<bool>& vertex_flag,
                             std::vector<bool>& edge_flag);

    std::unique_ptr<BitSet> vertices_;
    std::unique_ptr<std::vector<IntSet>> edge_list_;
    std::unique_ptr<std::unordered_map<int, IntSet>> vertex_hittings_;
};

}  // namespace vcc