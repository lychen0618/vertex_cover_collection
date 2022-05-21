#include "adc_enum.h"

#include "gtest/gtest.h"

namespace vcc {
class ADCEnumTest : public ::testing::Test {
protected:
    ADCEnumTest() {}
    ~ADCEnumTest() override {}
};

TEST_F(ADCEnumTest, ZeroThetaSimpleExampleTest) {
    std::vector<std::vector<int>> raw_hypergraph = {{0, 2, 5}, {1, 4}, {2, 4}};
    std::vector<IntSet> intset_hypergraph;
    for (const auto& edge : raw_hypergraph) {
        intset_hypergraph.emplace_back(edge);
    }
    std::shared_ptr<OutputQueue> output_queue(new OutputQueue(100));
    std::shared_ptr<HyperGraph> hyper_graph(new HyperGraph(intset_hypergraph));
    ADCEnum adc_enum(hyper_graph, output_queue, 0);
    adc_enum.RunApproximate();
    // 1 2; 0 4; 2 4; 5 4
    EXPECT_EQ(output_queue->Size(), 4U);
}

TEST_F(ADCEnumTest, NonZeroThetaSimpleExampleTest) {
    std::vector<std::vector<int>> raw_hypergraph = {{0, 2, 5}, {1, 4}, {2, 4}};
    std::vector<IntSet> intset_hypergraph;
    for (const auto& edge : raw_hypergraph) {
        intset_hypergraph.emplace_back(edge);
    }
    std::shared_ptr<OutputQueue> output_queue(new OutputQueue(100));
    std::shared_ptr<HyperGraph> hyper_graph(new HyperGraph(intset_hypergraph));
    ADCEnum adc_enum(hyper_graph, output_queue, 0.34);
    adc_enum.RunApproximate();
    // 4; 0 1; 2; 1 5
    EXPECT_EQ(output_queue->Size(), 4U);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace vcc
