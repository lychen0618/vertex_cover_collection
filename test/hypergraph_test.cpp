#include "common/hypergraph.h"

#include "gtest/gtest.h"

namespace vcc {
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

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace vcc
