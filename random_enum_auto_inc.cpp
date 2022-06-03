#include "random_enum_auto_inc.h"

#include <iomanip>
#include <unordered_set>

namespace vcc {
float IncMaxmin(const std::vector<BitSet>& vc_list,
                std::vector<std::vector<float>>& set_distances,
                std::vector<int>& ans,
                std::vector<float>& distances,
                int cur_size) {
    int size = static_cast<int>(vc_list.size());
    if (cur_size == 0) {
        float max_dis = -1;
        int first = 0, second = 0;
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                if (set_distances[i][j] > max_dis) {
                    max_dis = set_distances[i][j];
                    first = i;
                    second = j;
                }
            }
        }
        ans[0] = first;
        ans[1] = second;
        distances[0] = distances[1] = max_dis;
        cur_size = 2;
    }
    // Inc compute
    std::unordered_set<int> res(ans.begin(), ans.begin() + cur_size);
    while (cur_size < size) {
        float max_dis = -1;
        int pos = 0;
        for (int i = 0; i < size; ++i) {
            if (res.count(i))
                continue;
            float min_dis = 2;
            for (int j : res) {
                min_dis = std::min(min_dis, set_distances[i][j]);
            }
            if (min_dis > max_dis) {
                max_dis = min_dis;
                pos = i;
            }
        }
        ans[cur_size] = pos;
        distances[cur_size] = max_dis;
        ++cur_size;
        res.insert(pos);
    }
    // Get general info
    float total_dis = 0, min_dis = 2, max_dis = -1;
    int num = 0;
    std::vector<int> res_list(res.begin(), res.end());
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            num++;
            float distance = set_distances[i][j];
            total_dis += distance;
            min_dis = std::min(min_dis, distance);
            max_dis = std::max(max_dis, distance);
        }
    }
    LOG(INFO) << "after inc_maxmin(max/min/avg): " << std::setprecision(3)
              << max_dis << "/" << min_dis << "/" << (total_dis / num);
    return min_dis;
}

AutoInc::AutoInc(int top_k)
    : top_k_(top_k), cur_size_(0), ans_(top_k), distances_(top_k) {
    set_distances_ =
        std::vector<std::vector<float>>(top_k, std::vector<float>(top_k, 0.0));
}

bool AutoInc::Judge(int new_cover_index, const std::vector<BitSet>& vc_list) {
    bool stop_flag = false;
    if (new_cover_index == top_k_) {
        for (int i = 0; i < top_k_; ++i) {
            for (int j = i + 1; j < top_k_; ++j) {
                float distance = BitSet::GetDistance(vc_list[i], vc_list[j]);
                set_distances_[i][j] = set_distances_[j][i] = distance;
            }
        }
        IncMaxmin(vc_list, set_distances_, ans_, distances_, cur_size_);
        cur_size_ = top_k_;
    } else if (new_cover_index > top_k_) {
        int failed_pos =
            Validate(vc_list, set_distances_, ans_, distances_, cur_size_);
        if (failed_pos == cur_size_) {
            // Validate success
            if (cur_size_ < top_k_) {
                IncMaxmin(vc_list, set_distances_, ans_, distances_, cur_size_);
                cur_size_ = top_k_;
                validate_success_time_ = 0;
            } else
                ++validate_success_time_;
        } else {
            // Validate failure
            validate_success_time_ = 0;
            if (failed_pos == 0) {
                IncMaxmin(vc_list, set_distances_, ans_, distances_,
                          failed_pos);
                cur_size_ = top_k_;
            } else
                cur_size_ = failed_pos;
        }
        if ((stop_flag = !JudgeCondition(static_cast<int>(vc_list.size())))) {
            // Complete ans
            if (cur_size_ < top_k_) {
                IncMaxmin(vc_list, set_distances_, ans_, distances_, cur_size_);
            }
        }
    }
    return stop_flag;
}

bool AutoInc::JudgeCondition(int number_of_vc) const {
    return (number_of_vc < max_num_of_vc_) &&
           (validate_success_time_ <= max_validate_success_time_);
}

int AutoInc::Validate(const std::vector<BitSet>& vc_list,
                      std::vector<std::vector<float>>& set_distances,
                      std::vector<int>& ans,
                      std::vector<float>& distances,
                      int cur_size) {
    // Compute distance
    int new_cover_id = static_cast<int>(vc_list.size() - 1);
    float max_dis = -1;
    set_distances_.push_back(std::vector<float>(vc_list.size(), 0));
    for (int i = 0; i < new_cover_id; ++i) {
        float distance = BitSet::GetDistance(vc_list[i], vc_list.back());
        max_dis = std::max(max_dis, distance);
        set_distances[i].push_back(distance);
        set_distances.back()[i] = distance;
    }
    // Judge
    int failed_pos = 0;
    if (max_dis <= distances[0]) {
        max_dis = std::min(set_distances[ans[0]][new_cover_id],
                           set_distances[ans[1]][new_cover_id]);
        failed_pos = 2;
        while (failed_pos < cur_size) {
            if (distances[failed_pos] < max_dis)
                break;
            max_dis =
                std::min(max_dis, set_distances[ans[failed_pos]][new_cover_id]);
            ++failed_pos;
        }
    }
    return failed_pos;
}

RandomEnumAutoInc::RandomEnumAutoInc(std::shared_ptr<HyperGraph> hyper_graph,
                                     std::shared_ptr<OutputQueue> output_queue,
                                     int number_of_vc)
    : RandomEnum(hyper_graph, output_queue, number_of_vc, true),
      auto_inc_(number_of_vc) {}

void RandomEnumAutoInc::RunDiverse() {
    std::vector<std::shared_ptr<HyperGraph>> sub_graphs;
    hyper_graph_->GetSubGraph(sub_graphs);
    cover_index_ = 0;
    while (true) {
        for (size_t g_i = 0; g_i < sub_graphs.size(); ++g_i) {
            hyper_graph_ = sub_graphs[g_i];
            CommonInit();
            cur_list_.clear();
            back_steps_ = 0;
            std::shared_ptr<BitSet> cand(
                new BitSet(hyper_graph_->GetVertices()));
            std::shared_ptr<IntSetVector> crit(new IntSetVector());
            RunDiverse_(cand, crit);
        }
        ++cover_index_;
        UpdateStopFlag();
        if (stop_flag_)
            break;
    }
    // fill output queue
    for (auto& vc_id : auto_inc_.GetTopKAns()) {
        LOG(INFO) << "Get one vc: " << vc_list_[vc_id].ToString();
        output_queue_->Push(IntSet(vc_list_[vc_id]));
    }
}

}  // namespace vcc
