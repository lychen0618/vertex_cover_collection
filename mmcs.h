#pragma once

#include "base_algorithm.h"

namespace vcc {
class MMCS : public BaseAlgorithm {
public:
    enum Method { ORDER, RANDOM, MIN, MAX };
    typedef std::vector<IntSet> IntSetVector;

    MMCS(std::shared_ptr<HyperGraph> hyper_graph, std::shared_ptr<OutputQueue> output_queue);

    ~MMCS() = default;

    void RunAll() override {
        CommonInit();
        std::shared_ptr<BitSet> cand(new BitSet(hyper_graph_->GetVertices()));
        std::shared_ptr<IntSetVector> crit(new IntSetVector());
        RunAll_(cand, crit);
    }

    void SetMethod(Method m) { method_ = m; }

protected:
    void CommonInit();
    const IntSet& GetGoodEdgeToCover(std::shared_ptr<BitSet>& cand_copy) const;

    bool VertexWouldViolate(std::shared_ptr<IntSetVector> crit,
                            const int& vertex) const;

    void UpdateCritAndUncov(std::shared_ptr<IntSetVector> crit,
                            const IntSet& vertex_hitting);

    void RestoreCritAndUncov(std::shared_ptr<IntSetVector> crit);

    std::unique_ptr<BitSet> cur_;
    std::unique_ptr<BitSet> uncov_;
    Method method_;
private:
    void RunAll_(std::shared_ptr<BitSet> cand,
                 std::shared_ptr<IntSetVector> crit);
};
}  // namespace vcc
