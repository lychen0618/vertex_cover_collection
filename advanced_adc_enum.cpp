#include "advanced_adc_enum.h"
#include <cmath>

namespace vcc {
AdvancedADCEnum::AdvancedADCEnum(std::shared_ptr<HyperGraph> hyper_graph,
                                 std::shared_ptr<OutputQueue> output_queue,
                                 double theta,
                                 int thread_num)
    : ADCEnum(hyper_graph, output_queue, theta) {
    thread_pool_.reset(new ThreadPool(thread_num));
    thread_pool_->Start();
}

void AdvancedADCEnum::RunApproximate_(std::shared_ptr<BitSet> cand,
                                      std::shared_ptr<IntSetVector> crit,
                                      std::shared_ptr<BitSet> cur,
                                      std::shared_ptr<BitSet> uncov,
                                      std::shared_ptr<BitSet> can_hit) {
    if (f1(hyper_graph_, uncov->Count()) >= 1 - theta_) {
        if (IsMinimal(crit, uncov)) {
            if (to_log_)
                LOG(INFO) << "Get one vc: " << cur->ToString();
            if (to_queue_)
                output_queue_->Push(IntSet(*cur));
        }
        return;
    }
    std::shared_ptr<BitSet> cand_copy = std::make_shared<BitSet>(*cand);
    const IntSet* next_cover_edge =
        GetGoodEdgeToCover(cand_copy, uncov, can_hit);
    if (!next_cover_edge)
        return;
    std::shared_ptr<BitSet> c =
        std::make_shared<BitSet>(IntSet::And(*cand_copy, *next_cover_edge));
    IntSet::AndNot(*cand_copy, *c);
    std::shared_ptr<BitSet> can_hit_copy(new BitSet(*can_hit));
    UpdateCanCover(cand_copy, uncov, can_hit_copy);
    if (WillCover(cand_copy, uncov)) {
        if (!thread_pool_->IsFull()) {
            thread_pool_->AddTask(
                std::bind(&AdvancedADCEnum::RunApproximateWrapper, this,
                          std::make_shared<BitSet>(*cand_copy),
                          std::make_shared<IntSetVector>(*crit),
                          std::make_shared<BitSet>(*cur),
                          std::make_shared<BitSet>(*uncov),
                          std::make_shared<BitSet>(*can_hit_copy)));
        } else
            RunApproximate_(cand_copy, crit, cur, uncov, can_hit_copy);
    }
    for (const int& v : c->Get()) {
        if (!VertexWouldViolate(crit, v)) {
            std::shared_ptr<IntSetVector> crit_clone =
                std::make_shared<IntSetVector>(*crit);
            UpdateCritAndUncov(crit_clone, hyper_graph_->GetVertexHitting(v),
                               uncov);
            cur->Flip(v);
            RunApproximate_(cand_copy, crit_clone, cur, uncov, can_hit);
            cur->Flip(v);
            cand_copy->Set(v);
            RestoreCritAndUncov(crit_clone, uncov);
        }
    }
}

bool AdvancedADCEnum::IsMinimal(std::shared_ptr<IntSetVector>& crit,
                                std::shared_ptr<BitSet>& uncov) {
    for (const auto& one_crit : *crit) {
        if (f1(hyper_graph_, uncov->Count() + one_crit.Size()) >= 1 - theta_) {
            return false;
        }
    }
    return true;
}

const IntSet* AdvancedADCEnum::GetGoodEdgeToCover(
    std::shared_ptr<BitSet>& cand_copy,
    std::shared_ptr<BitSet>& uncov,
    std::shared_ptr<BitSet>& can_hit) {
    if (method_ == Method::ORDER || method_ == Method::RANDOM) {
        for (auto e_i : uncov->Get()) {
            if (can_hit->Get(e_i)) {
                next_edge_ = e_i;
                return &(hyper_graph_->GetEdge(e_i));
            }
        }
        return nullptr;
    } else if (method_ == Method::MIN || method_ == Method::MAX) {
        size_t temp = ((method_ == Method::MIN) ? INT_MAX : 0);
        bool get = false;
        for (int w = uncov->NextSetBit(); w != -1; w = uncov->NextSetBit(w)) {
            if (!can_hit->Get(w))
                continue;
            size_t t =
                IntSet::And(*cand_copy, hyper_graph_->GetEdge(w)).Count();
            if (method_ == Method::MIN && t < temp) {
                next_edge_ = w;
                temp = t;
                get = true;
            }
            if (method_ == Method::MAX && t > temp) {
                next_edge_ = w;
                temp = t;
                get = true;
            }
        }
        return get ? &(hyper_graph_->GetEdge(next_edge_)) : nullptr;
    } else {
        if (edge_priority_.size() == 0) {
            edge_priority_ = std::vector<double>(hyper_graph_->EdgeNum(), 0);
            std::vector<std::shared_ptr<HyperGraph>> subgraph;
            std::vector<std::vector<int>> ei_list(
                hyper_graph_->GetSubGraph(subgraph));
            int g_i = 0;
            for (auto& graph : subgraph) {
                int edge_total_size = 0;
                for (size_t e_i = 0; e_i < graph->EdgeNum(); ++e_i) {
                    edge_total_size += graph->GetEdge(e_i).Size();
                }
                double avg = edge_total_size / graph->EdgeNum();
                double rho = graph->VertexNum() / edge_total_size;
                double p = pow(avg, rho * graph->EdgeNum());
                for (auto& e_i : ei_list[g_i]) {
                    edge_priority_[e_i] = p;
                }
                ++g_i;
            }
        }
        int edge_id = -1;
        for (int w = uncov->NextSetBit(); w != -1; w = uncov->NextSetBit(w)) {
            if (!can_hit->Get(w))
                continue;
            if (edge_id == -1 || edge_priority_[w] > edge_priority_[edge_id]) {
                edge_id = w;
            }
        }
        if (edge_id != -1)
            next_edge_ = edge_id;
        return (edge_id != -1) ? &(hyper_graph_->GetEdge(next_edge_)) : nullptr;
    }
}

void AdvancedADCEnum::UpdateCanCover(std::shared_ptr<BitSet>& cand_copy,
                                     std::shared_ptr<BitSet>& uncov,
                                     std::shared_ptr<BitSet>& can_hit) {
    for (auto& e : uncov->Get()) {
        const auto& edge = hyper_graph_->GetEdge(e);
        bool flag = false;
        for (size_t i = 0; i < edge.Size(); ++i) {
            if (cand_copy->Get(edge.Get(i))) {
                flag = true;
                break;
            }
        }
        if (!flag)
            can_hit->Reset(e);
    }
}

bool AdvancedADCEnum::WillCover(std::shared_ptr<BitSet>& cand_copy,
                                std::shared_ptr<BitSet>& uncov) {
    std::shared_ptr<BitSet> new_uncov(new BitSet(*uncov));
    for (auto& vertex : cand_copy->Get()) {
        for (auto& e_i : hyper_graph_->GetVertexHitting(vertex).Get()) {
            new_uncov->Reset(e_i);
        }
    }
    return f1(hyper_graph_, new_uncov->Count()) >= 1 - theta_;
}

void AdvancedADCEnum::UpdateCritAndUncov(std::shared_ptr<IntSetVector>& crit,
                                         const IntSet& vertex_hitting,
                                         std::shared_ptr<BitSet>& uncov) {
    for (auto& one_crit : *crit) {
        one_crit.AndNot(vertex_hitting);
    }
    crit->emplace_back(std::move(IntSet::And(*uncov, vertex_hitting)));
    IntSet::AndNot(*uncov, vertex_hitting);
}

void AdvancedADCEnum::RestoreCritAndUncov(std::shared_ptr<IntSetVector>& crit,
                                          std::shared_ptr<BitSet>& uncov) {
    IntSet::Or(*uncov, crit->back());
}
}  // namespace vcc