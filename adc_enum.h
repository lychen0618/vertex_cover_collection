#pragma once

#include "common/approximate_functions.h"
#include "mmcs.h"

namespace vcc {
class ADCEnum : public MMCS {
public:
    ADCEnum(std::shared_ptr<HyperGraph> hyper_graph,
            std::shared_ptr<OutputQueue> output_queue,
            double theta = 0.1);
    ADCEnum() = default;
    void RunApproximate() override {
        CommonInit();
        std::shared_ptr<BitSet> cand(new BitSet(hyper_graph_->GetVertices()));
        std::shared_ptr<IntSetVector> crit(new IntSetVector());
        RunApproximate_(cand, crit);
    }

protected:
    void CommonInit();
    bool IsMinimal(std::shared_ptr<IntSetVector> crit);
    void UpdateCanCover(std::shared_ptr<BitSet>& cand_copy);
    bool WillCover(std::shared_ptr<BitSet>& cand_copy);
    const IntSet& GetGoodEdgeToCover(std::shared_ptr<BitSet>& cand_copy) const;

private:
    void RunApproximate_(std::shared_ptr<BitSet> cand,
                         std::shared_ptr<IntSetVector> crit);

    const double theta_;
    std::unique_ptr<BitSet> can_hit_;
};

}  // namespace vcc
