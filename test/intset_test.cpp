#include "common/intset.h"

#include "gtest/gtest.h"
using vcc::IntSet;
using vcc::BitSet;

namespace {
class IntSetTest : public ::testing::Test {
protected:
    IntSetTest() {}
    ~IntSetTest() override {}
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

TEST_F(IntSetTest, BitSetConstructorTest) {
    BitSet bit_set1;
    EXPECT_EQ(bit_set1.Size(), MAXBIT);
    EXPECT_EQ(bit_set1.Count(), 0U);
    bit_set1 = BitSet(3);
    EXPECT_EQ(bit_set1.Count(), 3);
    for (size_t i = 0; i < bit_set1.Count(); ++i) {
        EXPECT_EQ(bit_set1.Get(i), 1);
    }
    BitSet bit_set2(bit_set1);
    Compare(bit_set1, bit_set2);
    bit_set2.Set(5);
    EXPECT_TRUE(bit_set2.Count() != bit_set1.Count());
    BitSet bit_set3 = std::move(bit_set1);
    bit_set3.Flip(5);
    Compare(bit_set2, bit_set3);
    BitSet bit_set4({0, 1, 2, 5});
    Compare(bit_set3, bit_set4);
}

TEST_F(IntSetTest, BitSetMethodTest) {
    BitSet bit_set({1, 3, 4, 6, 7});
    EXPECT_EQ(bit_set.Count(), 5U);
    EXPECT_FALSE(bit_set.IsEmpty());
    EXPECT_EQ(bit_set.Get(6), 1);
    std::vector<int> vec({1, 3, 4, 6, 7});
    size_t id = 0;
    for (const int& e : bit_set.Get()) {
        EXPECT_EQ(vec[id], e);
        ++id;
    }
    bit_set.Flip(2);
    EXPECT_EQ(bit_set.Get(2), 1);
    bit_set.Flip(6);
    EXPECT_EQ(bit_set.Get(6), 0);
    bit_set.Set(6);
    EXPECT_EQ(bit_set.Get(6), 1);
    bit_set.Reset(6);
    EXPECT_EQ(bit_set.Get(6), 0);
    EXPECT_EQ(bit_set.NextSetBit(6), 7);
    EXPECT_EQ(bit_set.NextSetBit(7), -1);
    EXPECT_EQ(bit_set.NextSetBit(), 1);
    bit_set.Flip(2);
    bit_set.Set(6);
    EXPECT_EQ(bit_set.ToString(), "1 3 4 6 7");
}

TEST_F(IntSetTest, IntSetConstructorTest) {
    IntSet int_set1;
    EXPECT_TRUE(int_set1.IsEmpty());
    // = operator called.
    int_set1 = IntSet(3);
    EXPECT_EQ(int_set1.Size(), 3U);
    for (size_t i = 0; i < int_set1.Size(); ++i) {
        EXPECT_EQ(int_set1.Get(i), int(i));
    }
    // copy constructor called.
    IntSet int_set2(int_set1);
    Compare(int_set1, int_set2);
    int_set2.Add(5);
    EXPECT_TRUE(int_set2.Size() != int_set1.Size());
    // move constructor called
    IntSet int_set3 = std::move(int_set1);
    int_set3.Add(5);
    Compare(int_set2, int_set3);
    // vec constructor called
    IntSet int_set4({0, 1, 2, 5});
    Compare(int_set3, int_set4);
    // from bitset
    IntSet int_set5(BitSet({0, 1, 2, 5}));
    Compare(int_set4, int_set5);
}

void InitializeAB(IntSet& a, IntSet& b) {
    a.Set({1, 2, 3, 5, 8});
    b.Set({2, 3, 4, 8});
}

TEST_F(IntSetTest, IntSetMethodTest) {
    // basic method
    IntSet int_set1(3);
    EXPECT_FALSE(int_set1.IsEmpty());
    EXPECT_EQ(int_set1.Size(), 3U);
    int_set1.Add(3);
    int_set1.Add(4);
    for (int i = 0; i < static_cast<int>(int_set1.Size()); ++i) {
        EXPECT_EQ(int_set1.Get(i), i);
    }
    std::vector<int> vec(10, 1);
    int_set1.Set(vec);
    for (const int& e : int_set1.Get()) {
        EXPECT_EQ(e, 1);
    }
    EXPECT_EQ(int_set1.Size(), 10U);
    // logic method
    IntSet a, b, c;
    InitializeAB(a, b);
    c.Set({2, 3, 8});
    Compare(IntSet::And(a, b), c);
    Compare(a, IntSet({1, 2, 3, 5, 8}));
    Compare(b, IntSet({2, 3, 4, 8}));
    BitSet bit_set(a.Get());
    Compare(IntSet(IntSet::And(bit_set, b)), c);
    Compare(a, IntSet({1, 2, 3, 5, 8}));
    Compare(b, IntSet({2, 3, 4, 8}));
    a.And(b);
    Compare(a, c);
    Compare(b, IntSet({2, 3, 4, 8}));

    InitializeAB(a, b);
    c.Set({1, 2, 3, 4, 5, 8});
    Compare(IntSet::Or(a, b), c);
    Compare(a, IntSet({1, 2, 3, 5, 8}));
    Compare(b, IntSet({2, 3, 4, 8}));
    bit_set = BitSet(a.Get());
    IntSet::Or(bit_set, b);
    Compare(IntSet(bit_set), c);
    Compare(a, IntSet({1, 2, 3, 5, 8}));
    Compare(b, IntSet({2, 3, 4, 8}));
    a.Or(b);
    Compare(a, c);
    Compare(b, IntSet({2, 3, 4, 8}));

    InitializeAB(a, b);
    c.Set({1, 5});
    Compare(IntSet::AndNot(a, b), c);
    Compare(a, IntSet({1, 2, 3, 5, 8}));
    Compare(b, IntSet({2, 3, 4, 8}));
    bit_set = BitSet(a.Get());
    IntSet::AndNot(bit_set, b);
    Compare(IntSet(bit_set), c);
    Compare(a, IntSet({1, 2, 3, 5, 8}));
    Compare(b, IntSet({2, 3, 4, 8}));
    bit_set = BitSet(a.Get());
    IntSet::AndNot(bit_set, BitSet(b.Get()));
    Compare(IntSet(bit_set), c);
    Compare(a, IntSet({1, 2, 3, 5, 8}));
    Compare(b, IntSet({2, 3, 4, 8}));
    a.AndNot(b);
    Compare(a, c);
    Compare(b, IntSet({2, 3, 4, 8}));

    EXPECT_EQ(b.ToString(), "2 3 4 8");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace
