#include "random_enum_auto_inc.h"

#include "gtest/gtest.h"

using vcc::HyperGraph;
using vcc::IntSet;
using vcc::MMCS;
using vcc::OutputQueue;
using vcc::RandomEnumAutoInc;

namespace {
class RandomEnumAutoIncTest : public ::testing::Test {
protected:
    RandomEnumAutoIncTest() {}
    ~RandomEnumAutoIncTest() override {}
};

TEST_F(RandomEnumAutoIncTest, CanRunTest){
    std::vector<std::vector<int>> raw_hypergraph = {{0, 2, 5}, {1, 4}, {2, 4}};
    std::vector<IntSet> intset_hypergraph;
    for (const auto& edge : raw_hypergraph) {
        intset_hypergraph.emplace_back(edge);
    }
    std::shared_ptr<HyperGraph> hyper_graph(new HyperGraph(intset_hypergraph));
    for (MMCS::Method m : {MMCS::Method::ORDER, MMCS::Method::RANDOM,
                           MMCS::Method::MIN, MMCS::Method::MAX}) {
        std::shared_ptr<OutputQueue> output_queue(new OutputQueue(100));
        RandomEnumAutoInc random_enum_auto_inc(hyper_graph, output_queue, 2);
        random_enum_auto_inc.SetMethod(m);
        random_enum_auto_inc.RunDiverse();
        EXPECT_EQ(output_queue->Size(), 2U);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace
