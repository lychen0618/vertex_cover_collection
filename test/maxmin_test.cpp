#include "common/maxmin.h"
#include <cmath>
#include <vector>
#include "gtest/gtest.h"

using vcc::BitSet;
using vcc::Maxmin;

namespace {
class MaxminTest : public ::testing::Test {
protected:
    MaxminTest() {}
    ~MaxminTest() override {}
};

TEST_F(MaxminTest, SimpleTest1) {
    std::vector<BitSet> vc_list;
    vc_list.push_back(BitSet({0, 1, 2}));
    vc_list.push_back(BitSet({1, 2, 3}));
    vc_list.push_back(BitSet({4, 5}));
    float val = Maxmin(vc_list, 3);
    EXPECT_EQ(val, 0.5);
}

TEST_F(MaxminTest, SimpleTest2) {
    std::vector<BitSet> vc_list;
    vc_list.push_back(BitSet({0, 1, 2}));
    vc_list.push_back(BitSet({1, 2, 3}));
    vc_list.push_back(BitSet({4, 5}));
    vc_list.push_back(BitSet(std::vector<int>({0})));
    float val = Maxmin(vc_list, 3);
    EXPECT_TRUE(std::fabs(val - 0.66) <= 0.01);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
}  // namespace
