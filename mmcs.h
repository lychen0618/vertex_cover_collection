#pragma once

#include "base_algorithm.h"
#include "common/random_generator.h"

namespace vcc {
class MMCS : public BaseAlgorithm {
public:
    enum Method { ORDER, RANDOM, MIN, MAX, PRIORITY };
    typedef std::vector<IntSet> IntSetVector;

    MMCS(std::shared_ptr<HyperGraph> hyper_graph,
         std::shared_ptr<OutputQueue> output_queue);

    ~MMCS() = default;

    void RunAll() override {
        CommonInit();
        std::shared_ptr<BitSet> cand(new BitSet(hyper_graph_->GetVertices()));
        std::shared_ptr<IntSetVector> crit(new IntSetVector());
        RunAll_(cand, crit);
    }

    void SetMethod(Method m) { method_ = m; }

    void SetToQueue(const bool& to_queue) { to_queue_ = to_queue; }

    void SetToLog(const bool& to_log) { to_log_ = to_log; }

protected:
    void CommonInit();
    const IntSet& GetGoodEdgeToCover(std::shared_ptr<BitSet>& cand_copy);

    bool VertexWouldViolate(std::shared_ptr<IntSetVector>& crit,
                            const int& vertex) const;

    void UpdateCritAndUncov(std::shared_ptr<IntSetVector>& crit,
                            const IntSet& vertex_hitting);

    void RestoreCritAndUncov(std::shared_ptr<IntSetVector>& crit);

    bool to_queue_ = true;
    bool to_log_ = true;
    std::unique_ptr<BitSet> cur_;
    std::unique_ptr<BitSet> uncov_;
    Method method_;
    std::unique_ptr<RandomGenerator> random_gen_;
    int next_edge_;

private:
    void RunAll_(std::shared_ptr<BitSet> cand,
                 std::shared_ptr<IntSetVector> crit);
};
}  // namespace vcc
