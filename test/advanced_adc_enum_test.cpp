#include "advanced_adc_enum.h"

#include "gtest/gtest.h"
using vcc::AdvancedADCEnum;
using vcc::HyperGraph;
using vcc::IntSet;
using vcc::MMCS;
using vcc::OutputQueue;

namespace {
class AdvancedADCEnumTest : public ::testing::Test {
protected:
    AdvancedADCEnumTest() {}
    ~AdvancedADCEnumTest() override {}
};

TEST_F(AdvancedADCEnumTest, ZeroThetaSimpleExampleTest) {
    std::vector<std::vector<int>> raw_hypergraph = {{0, 2, 5}, {1, 4}, {2, 4}};
    std::vector<IntSet> intset_hypergraph;
    for (const auto& edge : raw_hypergraph) {
        intset_hypergraph.emplace_back(edge);
    }
    std::shared_ptr<HyperGraph> hyper_graph(new HyperGraph(intset_hypergraph));
    for (MMCS::Method m : {MMCS::Method::ORDER, MMCS::Method::RANDOM,
                           MMCS::Method::MIN, MMCS::Method::MAX}) {
        std::shared_ptr<OutputQueue> output_queue(new OutputQueue(100));
        AdvancedADCEnum adc_enum(hyper_graph, output_queue, 0);
        adc_enum.SetMethod(m);
        adc_enum.RunApproximate();
        // 1 2; 0 4; 2 4; 5 4
        EXPECT_EQ(output_queue->Size(), 4U);
        EXPECT_EQ(adc_enum.DebugFunc(), 0);
    }
}

TEST_F(AdvancedADCEnumTest, NonZeroThetaSimpleExampleTest) {
    std::vector<std::vector<int>> raw_hypergraph = {{0, 2, 5}, {1, 4}, {2, 4}};
    std::vector<IntSet> intset_hypergraph;
    for (const auto& edge : raw_hypergraph) {
        intset_hypergraph.emplace_back(edge);
    }
    std::shared_ptr<HyperGraph> hyper_graph(new HyperGraph(intset_hypergraph));
    for (MMCS::Method m :
         {MMCS::Method::ORDER, MMCS::Method::RANDOM, MMCS::Method::MIN,
          MMCS::Method::MAX, MMCS::Method::PRIORITY}) {
        std::shared_ptr<OutputQueue> output_queue(new OutputQueue(100));
        AdvancedADCEnum adc_enum(hyper_graph, output_queue, 0.34);
        adc_enum.SetMethod(m);
        adc_enum.RunApproximate();
        // 4; 0 1; 2; 1 5
        EXPECT_EQ(output_queue->Size(), 4U);
        EXPECT_EQ(adc_enum.DebugFunc(), 0);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace
