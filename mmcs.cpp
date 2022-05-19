#include "mmcs.h"

#include <random>

namespace vcc {
MMCS::MMCS(std::shared_ptr<HyperGraph> hyper_graph, std::shared_ptr<OutputQueue> output_queue)
    : BaseAlgorithm(hyper_graph, output_queue) {}

void MMCS::CommonInit() {
    cur_.reset(new BitSet());
    uncov_.reset(new BitSet(hyper_graph_->EdgeNum()));
}

void MMCS::RunAll_(std::shared_ptr<BitSet> cand,
                   std::shared_ptr<IntSetVector> crit) {
    if (uncov_->IsEmpty()) {
        LOG(INFO) << "Get one vc: " << cur_->ToString();
        output_queue_->Push(IntSet(*cur_));
        return;
    }
    std::shared_ptr<BitSet> cand_copy = std::make_shared<BitSet>(*cand);
    const IntSet& next_cover_edge = GetGoodEdgeToCover(cand_copy);
    std::shared_ptr<BitSet> c =
        std::make_shared<BitSet>(IntSet::And(*cand_copy, next_cover_edge));
    IntSet::AndNot(*cand_copy, *c);
    for (const int& v : c->Get()) {
        if (!VertexWouldViolate(crit, v)) {
            std::shared_ptr<IntSetVector> crit_clone =
                std::make_shared<IntSetVector>(*crit);
            UpdateCritAndUncov(crit_clone, hyper_graph_->GetVertexHitting(v));
            cur_->Flip(v);
            RunAll_(cand_copy, crit_clone);
            cur_->Flip(v);
            cand_copy->Set(v);
            RestoreCritAndUncov(crit_clone);
        }
    }
}

const IntSet& MMCS::GetGoodEdgeToCover(
    std::shared_ptr<BitSet>& cand_copy) const {
    if (method_ == Method::ORDER) {
        return hyper_graph_->GetEdge(uncov_->NextSetBit());
    } else if (method_ == Method::RANDOM) {
        return hyper_graph_->GetEdge(uncov_->NextSetBit());
    } else {
        return hyper_graph_->GetEdge(uncov_->NextSetBit());
    }
}

bool MMCS::VertexWouldViolate(std::shared_ptr<IntSetVector> crit,
                              const int& vertex) const {
    const IntSet& vertex_hitting = hyper_graph_->GetVertexHitting(vertex);
    for (const IntSet& one_crit : *crit) {
        IntSet tmp = std::move(IntSet::AndNot(one_crit, vertex_hitting));
        if (tmp.IsEmpty())
            return true;
    }
    return false;
}

void MMCS::UpdateCritAndUncov(std::shared_ptr<IntSetVector> crit,
                              const IntSet& vertex_hitting) {
    for (auto& one_crit : *crit) {
        one_crit.AndNot(vertex_hitting);
    }
    crit->emplace_back(std::move(IntSet::And(*uncov_, vertex_hitting)));
    IntSet::AndNot(*uncov_, vertex_hitting);
}

void MMCS::RestoreCritAndUncov(std::shared_ptr<IntSetVector> crit) {
    IntSet::Or(*uncov_, crit->back());
}

}  // namespace vcc
