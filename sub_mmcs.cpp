#include "sub_mmcs.h"

namespace vcc {
SubMMCS::SubMMCS(std::shared_ptr<HyperGraph> hyper_graph,
                 std::shared_ptr<OutputQueue> output_queue,
                 int number_of_vc,
                 const float& threshold)
    : MMCS(hyper_graph, output_queue),
      required_number_of_vc_(number_of_vc),
      threshold_(threshold),
      prev_(nullptr) {}

void SubMMCS::RunDiverse_(std::shared_ptr<BitSet> cand,
                          std::shared_ptr<IntSetVector> crit) {
    if (static_cast<int>(vc_list_.size()) == required_number_of_vc_)
        return;
    if (uncov_->IsEmpty()) {
        if (prev_ == nullptr ||
            BitSet::GetDistance(*prev_, *cur_) >= threshold_) {
            prev_ = std::make_shared<BitSet>(*cur_);
            vc_list_.emplace_back(*cur_);
        }
        if (to_log_)
            LOG(INFO) << "Get one vc: " << cur_->ToString();
        if (to_queue_)
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
            RunDiverse_(cand_copy, crit_clone);
            if (static_cast<int>(vc_list_.size()) == required_number_of_vc_)
                return;
            cur_->Flip(v);
            cand_copy->Set(v);
            RestoreCritAndUncov(crit_clone);
        }
    }
}
}  // namespace vcc
