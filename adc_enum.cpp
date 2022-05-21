#include "adc_enum.h"

namespace vcc {
ADCEnum::ADCEnum(std::shared_ptr<HyperGraph> hyper_graph,
                 std::shared_ptr<OutputQueue> output_queue,
                 double theta)
    : MMCS(hyper_graph, output_queue), theta_(theta) {}

void ADCEnum::RunApproximate_(std::shared_ptr<BitSet> cand,
                              std::shared_ptr<IntSetVector> crit) {
    if (f1(hyper_graph_, uncov_->Count()) >= 1 - theta_) {
        if (IsMinimal(crit)) {
            LOG(INFO) << "Get one vc: " << cur_->ToString();
            output_queue_->Push(IntSet(*cur_));
        }
        return;
    }
    std::shared_ptr<BitSet> cand_copy = std::make_shared<BitSet>(*cand);
    const IntSet& next_cover_edge = GetGoodEdgeToCover(cand_copy);
    if (next_cover_edge.IsEmpty())
        return;
    std::shared_ptr<BitSet> c =
        std::make_shared<BitSet>(IntSet::And(*cand_copy, next_cover_edge));
    IntSet::AndNot(*cand_copy, *c);
    std::unique_ptr<BitSet> can_hit_copy(new BitSet(*can_hit_));
    UpdateCanCover(cand_copy);
    if (WillCover(cand_copy)) {
        RunApproximate_(cand_copy, crit);
    }
    can_hit_.reset(can_hit_copy.release());
    for (const int& v : c->Get()) {
        if (!VertexWouldViolate(crit, v)) {
            std::shared_ptr<IntSetVector> crit_clone =
                std::make_shared<IntSetVector>(*crit);
            UpdateCritAndUncov(crit_clone, hyper_graph_->GetVertexHitting(v));
            cur_->Flip(v);
            RunApproximate_(cand_copy, crit_clone);
            cur_->Flip(v);
            cand_copy->Set(v);
            RestoreCritAndUncov(crit_clone);
        }
    }
}

void ADCEnum::CommonInit() {
    MMCS::CommonInit();
    can_hit_.reset(new BitSet(hyper_graph_->EdgeNum()));
}

bool ADCEnum::IsMinimal(std::shared_ptr<IntSetVector> crit) {
    for (const auto& one_crit : *crit) {
        if (f1(hyper_graph_, uncov_->Count() + one_crit.Size()) >= 1 - theta_) {
            return false;
        }
    }
    return true;
}

const IntSet& ADCEnum::GetGoodEdgeToCover(
    std::shared_ptr<BitSet>& cand_copy) const {
    if (method_ == Method::ORDER) {
        for (auto e_i : uncov_->Get()) {
            if (can_hit_->Get(e_i))
                return hyper_graph_->GetEdge(e_i);
        }
        std::shared_ptr<IntSet> empty_intset(new IntSet());
        return *empty_intset;
    } else if (method_ == Method::RANDOM) {
        return hyper_graph_->GetEdge(uncov_->NextSetBit());
    } else {
        return hyper_graph_->GetEdge(uncov_->NextSetBit());
    }
}

void ADCEnum::UpdateCanCover(std::shared_ptr<BitSet>& cand_copy) {
    for (auto& e : uncov_->Get()) {
        const auto& edge = hyper_graph_->GetEdge(e);
        bool flag = false;
        for (size_t i = 0; i < edge.Size(); ++i) {
            if (cand_copy->Get(edge.Get(i))) {
                flag = true;
                break;
            }
        }
        if (!flag)
            can_hit_->Reset(e);
    }
}

bool ADCEnum::WillCover(std::shared_ptr<BitSet>& cand_copy) {
    std::shared_ptr<BitSet> uncov(new BitSet(*uncov_));
    for (auto& vertex : cand_copy->Get()) {
        for (auto& e_i : hyper_graph_->GetVertexHitting(vertex).Get()) {
            uncov->Reset(e_i);
        }
    }
    return f1(hyper_graph_, uncov->Count()) >= 1 - theta_;
}
}  // namespace vcc
