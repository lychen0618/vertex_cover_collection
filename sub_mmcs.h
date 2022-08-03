#pragma once

#include "mmcs.h"

namespace vcc {
class SubMMCS : public MMCS {
public:
    SubMMCS(std::shared_ptr<HyperGraph> hyper_graph,
            std::shared_ptr<OutputQueue> output_queue,
            int number_of_vc,
            const float& threshold);
    ~SubMMCS() = default;

    void RunDiverse() override {
        CommonInit();
        std::shared_ptr<BitSet> cand(new BitSet(hyper_graph_->GetVertices()));
        std::shared_ptr<IntSetVector> crit(new IntSetVector());
        RunDiverse_(cand, crit);
    }

    std::vector<BitSet>& GetResult() { return vc_list_; }

private:
    void RunDiverse_(std::shared_ptr<BitSet> cand,
                     std::shared_ptr<IntSetVector> crit);

    std::vector<BitSet> vc_list_;
    const int required_number_of_vc_;
    const float threshold_;
    std::shared_ptr<BitSet> prev_;
};
}  // namespace vcc
