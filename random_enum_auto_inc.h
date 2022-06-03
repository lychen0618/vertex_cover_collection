#pragma once

#include "random_enum.h"

namespace vcc {
float IncMaxmin(const std::vector<BitSet>& vc_list,
                std::vector<std::vector<float>>& set_distances,
                std::vector<int>& ans,
                std::vector<float>& distances,
                int cur_size);

class AutoInc {
public:
    AutoInc(int top_k);
    ~AutoInc() = default;
    bool Judge(int new_cover_index, const std::vector<BitSet>& vc_list);

    const std::vector<int>& GetTopKAns() const { return ans_; }

private:
    bool JudgeCondition(int number_of_vc) const;

    int Validate(const std::vector<BitSet>& vc_list,
                 std::vector<std::vector<float>>& set_distances,
                 std::vector<int>& ans,
                 std::vector<float>& distances,
                 int cur_size);

    const int top_k_;
    std::vector<std::vector<float>> set_distances_;
    int cur_size_;
    std::vector<int> ans_;
    std::vector<float> distances_;
    int validate_success_time_ = 0;
    const int max_validate_success_time_ = 5;
    const int max_num_of_vc_ = 2000;
};

class RandomEnumAutoInc : public RandomEnum {
public:
    RandomEnumAutoInc(std::shared_ptr<HyperGraph> hyper_graph,
                      std::shared_ptr<OutputQueue> output_queue,
                      int number_of_vc);
    void RunDiverse() override;

protected:
    void UpdateStopFlag() override {
        stop_flag_ = auto_inc_.Judge(cover_index_, vc_list_);
    }

    AutoInc auto_inc_;
};
}  // namespace vcc
