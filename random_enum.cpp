#include "random_enum.h"

#include <unordered_map>

namespace vcc {
RandomEnum::RandomEnum(std::shared_ptr<HyperGraph> hyper_graph,
                       std::shared_ptr<OutputQueue> output_queue,
                       int number_of_vc,
                       bool auto_inc)
    : MMCS(hyper_graph, output_queue),
      back_steps_(0),
      cover_index_(0),
      required_number_of_vc_(number_of_vc),
      stop_flag_(false),
      is_auto_inc_(auto_inc),
      vc_list_(number_of_vc),
      ran_gen_for_mess_(new RandomGenerator()),
      ran_gen_for_back_(new RandomGenerator()) {}

void RandomEnum::RunDiverse() {
    std::vector<std::shared_ptr<HyperGraph>> sub_graphs;
    hyper_graph_->GetSubGraph(sub_graphs);
    for (size_t g_i = 0; g_i < sub_graphs.size(); ++g_i) {
        hyper_graph_ = sub_graphs[g_i];
        cover_index_ = 0;
        while (cover_index_ < required_number_of_vc_) {
            CommonInit();
            cur_list_.clear();
            back_steps_ = 0;
            stop_flag_ = false;
            std::shared_ptr<BitSet> cand(
                new BitSet(hyper_graph_->GetVertices()));
            std::shared_ptr<IntSetVector> crit(new IntSetVector());
            RunDiverse_(cand, crit);
        }
    }
    // fill output queue
    for (auto& vc : vc_list_) {
        if (to_log_)
            LOG(INFO) << "Get one vc: " << vc.ToString();
        if (to_queue_)
            output_queue_->Push(IntSet(vc));
    }
}

void RandomEnum::RunDiverse_(std::shared_ptr<BitSet> cand,
                             std::shared_ptr<IntSetVector> crit) {
    if (stop_flag_)
        return;
    if (uncov_->IsEmpty()) {
        if (!is_auto_inc_) {
            vc_list_[cover_index_++].Or(*cur_);
            UpdateStopFlag();
        } else {
            if (cover_index_ == static_cast<int>(vc_list_.size())) {
                vc_list_.push_back(BitSet());
            }
            vc_list_[cover_index_].Or(*cur_);
            stop_flag_ = true;
        }
        // different choices
        if (back_method_.first)
            back_steps_ = cur_->Count();
        else {
            int max_step =
                std::max(1, static_cast<int>(back_method_.second * cur_->Count()));
            back_steps_ = ran_gen_for_back_->RandomInt() % max_step;
        }
        return;
    }
    std::shared_ptr<BitSet> cand_copy = std::make_shared<BitSet>(*cand);
    const IntSet& next_cover_edge = GetGoodEdgeToCover(cand_copy);
    std::shared_ptr<BitSet> c =
        std::make_shared<BitSet>(IntSet::And(*cand_copy, next_cover_edge));
    IntSet::AndNot(*cand_copy, *c);
    std::vector<int> messed_c;
    for (int w = c->NextSetBit(); w != -1; w = c->NextSetBit(w)) {
        messed_c.push_back(w);
        int id = ran_gen_for_mess_->RandomInt() % messed_c.size();
        std::swap(messed_c[id], messed_c.back());
    }

    bool flag = false;
    for (int v : messed_c) {
        if (back_steps_ != 0)
            return;
        if (!VertexWouldViolate(crit, v)) {
            flag = true;
            std::shared_ptr<IntSetVector> crit_clone =
                std::make_shared<IntSetVector>(*crit);
            UpdateCritAndUncov(crit_clone, hyper_graph_->GetVertexHitting(v));
            cur_->Flip(v);
            cur_list_.push_back(v);
            RunDiverse_(cand_copy, crit_clone);
            if (stop_flag_)
                return;
            cur_list_.pop_back();
            cur_->Flip(v);
            cand_copy->Set(v);
            RestoreCritAndUncov(crit_clone);
        }
    }

    if (!flag) {
        for (int v : messed_c) {
            if (back_steps_ != 0)
                break;
            std::unique_ptr<BitSet> old_cur(new BitSet(*cur_));
            std::vector<int> old_cur_list = cur_list_;
            std::unique_ptr<BitSet> old_uncov(new BitSet(*uncov_));
            std::shared_ptr<IntSetVector> crit_clone =
                std::make_shared<IntSetVector>(*crit);
            UpdateCritAndUncov(crit_clone, hyper_graph_->GetVertexHitting(v));
            cur_->Flip(v);
            cur_list_.push_back(v);
            std::unordered_map<int, int> crit_map;
            for (int i = 0; i < static_cast<int>(cur_list_.size()); ++i)
                crit_map[cur_list_[i]] = i;
            for (int i = 0; i < static_cast<int>(cur_list_.size()) - 1; ++i) {
                if ((*crit_clone)[i].Size() == 0) {
                    const IntSet& cur_hitting =
                        hyper_graph_->GetVertexHitting(cur_list_[i]);
                    for (int e_i : cur_hitting.Get()) {
                        IntSet common_vertice(std::move(
                            IntSet::And(*cur_, hyper_graph_->GetEdge(e_i))));
                        if (common_vertice.Size() == 2) {
                            (*crit_clone)[crit_map[common_vertice.Another(
                                              cur_list_[i])]]
                                .Add(e_i);
                        }
                    }
                    cur_->Flip(cur_list_[i]);
                }
            }
            std::vector<int> temp_cur_list;
            std::shared_ptr<IntSetVector> temp_crit_clone(new IntSetVector());
            for (int i = 0; i < static_cast<int>(cur_list_.size()); ++i) {
                if ((*crit_clone)[i].Size() != 0) {
                    temp_cur_list.push_back(cur_list_[i]);
                    temp_crit_clone->emplace_back(std::move((*crit_clone)[i]));
                }
            }
            cur_list_ = temp_cur_list;
            crit_clone = temp_crit_clone;
            RunDiverse_(cand_copy, crit_clone);
            if (stop_flag_)
                return;
            cur_.reset(old_cur.release());
            cur_list_ = old_cur_list;
            uncov_.reset(old_uncov.release());
            if (cur_->Count() != 0) {
                if (back_method_.first)
                    back_steps_ = cur_->Count() + 1;
                else {
                    int max_step = std::max(1, static_cast<int>(back_method_.second *
                                                    (cur_->Count() + 1)));
                    back_steps_ = ran_gen_for_back_->RandomInt() % max_step;
                }
            }
        }
    }

    if (back_steps_ != 0)
        back_steps_--;
}
}  // namespace vcc
