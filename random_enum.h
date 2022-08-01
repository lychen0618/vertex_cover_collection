#pragma once

#include "mmcs.h"
#include "common/maxmin.h"

namespace vcc {
class RandomEnum : public MMCS {
public:
    RandomEnum(std::shared_ptr<HyperGraph> hyper_graph,
               std::shared_ptr<OutputQueue> output_queue,
               int number_of_vc,
               bool auto_inc = false);
    ~RandomEnum() = default;

    void RunDiverse() override;

protected:
    virtual void UpdateStopFlag() {
        stop_flag_ = (cover_index_ == required_number_of_vc_);
    }

    void RunDiverse_(std::shared_ptr<BitSet> cand,
                     std::shared_ptr<IntSetVector> crit);

    int back_steps_;
    int cover_index_;
    const int required_number_of_vc_;
    bool stop_flag_;
    bool is_auto_inc_;
    std::vector<int> cur_list_;
    std::vector<BitSet> vc_list_;

private:
    std::unique_ptr<RandomGenerator> ran_gen_for_mess_;
    std::unique_ptr<RandomGenerator> ran_gen_for_back_;
};
}  // namespace vcc
