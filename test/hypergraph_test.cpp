#include "common/hypergraph.h"

#include "gtest/gtest.h"

using vcc::IntSet;
using vcc::BitSet;
using vcc::HyperGraph;

namespace {

class HyperGraphTest : public ::testing::Test {
protected:
    HyperGraphTest() {}
    ~HyperGraphTest() override {}
};

void Compare(const IntSet& a, const IntSet& b) {
    EXPECT_EQ(a.Size(), b.Size());
    for (size_t i = 0; i < a.Size(); ++i) {
        EXPECT_EQ(a.Get(i), b.Get(i));
    }
}

void Compare(const BitSet& a, const BitSet& b) {
    EXPECT_EQ(a.Size(), b.Size());
    EXPECT_EQ(a.Count(), b.Count());
    for (size_t i = 0; i < a.Size(); ++i) {
        EXPECT_EQ(a.Get(i), b.Get(i));
    }
}

TEST_F(HyperGraphTest, SimpleExampleTest) {
    std::vector<std::vector<int>> raw_hypergraph = {
        {0, 2, 5}, {1, 4, 6}, {2, 4}};
    std::vector<IntSet> intset_hypergraph;
    for (const auto& edge : raw_hypergraph) {
        intset_hypergraph.emplace_back(edge);
    }
    HyperGraph hg(intset_hypergraph);
    EXPECT_EQ(hg.VertexNum(), 6U);
    EXPECT_EQ(hg.EdgeNum(), 3U);
    Compare(hg.GetVertices(), BitSet({0, 1, 2, 4, 5, 6}));
    for (size_t i = 0; i < 3; ++i) {
        Compare(hg.GetEdge(i), intset_hypergraph[i]);
    }
    std::vector<std::vector<int>> vertex_hittings = {{0},    {1}, {0, 2}, {},
                                                     {1, 2}, {0}, {1}};
    for (const int& v : hg.GetVertices().Get()) {
        Compare(hg.GetVertexHitting(v), IntSet(vertex_hittings[v]));
    }
}

TEST_F(HyperGraphTest, GetSubGraphTest) {
    std::vector<std::vector<int>> raw_hypergraph = {{3, 5}, {1, 4, 6}, {2, 4}};
    std::vector<IntSet> intset_hypergraph;
    for (const auto& edge : raw_hypergraph) {
        intset_hypergraph.emplace_back(edge);
    }
    HyperGraph hg(intset_hypergraph);
    std::vector<std::shared_ptr<HyperGraph>> sub_graphs;
    std::vector<std::vector<int>> ei_list = hg.GetSubGraph(sub_graphs);
    EXPECT_EQ(sub_graphs.size(), 2U);
    EXPECT_EQ(sub_graphs[0]->VertexNum(), 4U);
    EXPECT_EQ(sub_graphs[0]->EdgeNum(), 2U);
    EXPECT_EQ(sub_graphs[1]->VertexNum(), 2U);
    EXPECT_EQ(sub_graphs[1]->EdgeNum(), 1U);
    EXPECT_EQ(ei_list.size(), 2U);
    EXPECT_EQ(ei_list[0], std::vector<int>({1, 2}));
    EXPECT_EQ(ei_list[1], std::vector<int>({0}));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace
