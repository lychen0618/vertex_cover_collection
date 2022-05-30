#pragma once
#include "adc_enum.h"
#include "common/thread_pool.h"

namespace vcc {
class AdvancedADCEnum : public ADCEnum {
public:
    AdvancedADCEnum(std::shared_ptr<HyperGraph> hyper_graph,
                    std::shared_ptr<OutputQueue> output_queue,
                    double theta = 0.1,
                    int thread_num = 4);
    ~AdvancedADCEnum() = default;

    void RunApproximate() {
        std::shared_ptr<BitSet> cand(new BitSet(hyper_graph_->GetVertices()));
        std::shared_ptr<IntSetVector> crit(new IntSetVector());
        std::shared_ptr<BitSet> cur(new BitSet());
        std::shared_ptr<BitSet> uncov(new BitSet(hyper_graph_->EdgeNum()));
        std::shared_ptr<BitSet> can_hit(new BitSet(hyper_graph_->EdgeNum()));
        RunApproximate_(cand, crit, cur, uncov, can_hit);
        thread_pool_->Wait();
    }

    int DebugFunc() { return thread_pool_->DebugFunc(); }

private:
    void RunApproximateWrapper(std::shared_ptr<BitSet> cand,
                               std::shared_ptr<IntSetVector> crit,
                               std::shared_ptr<BitSet> cur,
                               std::shared_ptr<BitSet> uncov,
                               std::shared_ptr<BitSet> can_hit) {
        RunApproximate_(cand, crit, cur, uncov, can_hit);
        thread_pool_->TaskEnd();
    }

    void RunApproximate_(std::shared_ptr<BitSet> cand,
                         std::shared_ptr<IntSetVector> crit,
                         std::shared_ptr<BitSet> cur,
                         std::shared_ptr<BitSet> uncov,
                         std::shared_ptr<BitSet> can_hit);

    bool IsMinimal(std::shared_ptr<IntSetVector>& crit,
                   std::shared_ptr<BitSet>& uncov);

    const IntSet* GetGoodEdgeToCover(std::shared_ptr<BitSet>& cand_copy,
                                     std::shared_ptr<BitSet>& uncov,
                                     std::shared_ptr<BitSet>& can_hit);

    void UpdateCanCover(std::shared_ptr<BitSet>& cand_copy,
                        std::shared_ptr<BitSet>& uncov,
                        std::shared_ptr<BitSet>& can_hit);

    bool WillCover(std::shared_ptr<BitSet>& cand_copy,
                   std::shared_ptr<BitSet>& uncov);

    void UpdateCritAndUncov(std::shared_ptr<IntSetVector>& crit,
                            const IntSet& vertex_hitting,
                            std::shared_ptr<BitSet>& uncov);

    void RestoreCritAndUncov(std::shared_ptr<IntSetVector>& crit,
                             std::shared_ptr<BitSet>& uncov);

    std::unique_ptr<ThreadPool> thread_pool_;
};
}  // namespace vcc
