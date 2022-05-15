#include "intset.h"

#include <algorithm>
#include <unordered_set>

namespace vcc {

IntSet IntSet::And(const IntSet& a, const IntSet& b) {
    IntSet new_set;
    std::unordered_set<int> b_set(b.Get().begin(), b.Get().end());
    for (size_t i = 0; i < a.Size(); ++i) {
        const int& tmp = a.Get(i);
        if (b_set.count(tmp))
            new_set.Add(tmp);
    }
    return std::move(new_set);
}

BitSet IntSet::And(const BitSet& a, const IntSet& b) {
    BitSet new_set;
    std::unordered_set<int> b_set(b.Get().begin(), b.Get().end());
    for (const int& e : a.Get()) {
        if (b_set.count(e)) {
            new_set.Flip(e);
        }
    }
    return std::move(new_set);
}

IntSet IntSet::Or(const IntSet& a, const IntSet& b) {
    IntSet new_set;
    for (size_t ai = 0, bi = 0; ai < a.Size() || bi < b.Size();) {
        int t1 = (ai < a.Size() ? a.Get(ai) : INT32_MAX);
        int t2 = (bi < b.Size() ? b.Get(bi) : INT32_MAX);
        int mv = std::min(t1, t2);
        new_set.Add(mv);
        while (ai < a.Size() && a.Get(ai) == mv)
            ++ai;
        while (bi < b.Size() && b.Get(bi) == mv)
            ++bi;
    }
    return std::move(new_set);
}

void IntSet::Or(BitSet& a, const IntSet& b){
    for(const int& e : b.Get()){
        a.Set(e);
    }
}

IntSet IntSet::AndNot(const IntSet& a, const IntSet& b) {
    IntSet new_set;
    std::unordered_set<int> b_set(b.Get().begin(), b.Get().end());
    for (const int& e : a.Get()) {
        if (b_set.count(e) == 0) {
            new_set.Add(e);
        }
    }
    return std::move(new_set);
}

void IntSet::AndNot(BitSet& a, const BitSet& b) {
    for (const int& e : b.Get()) {
        a.Reset(e);
    }
}

void IntSet::AndNot(BitSet& a, const IntSet& b) {
    for (const int& e : b.Get()) {
        a.Reset(e);
    }
}

std::string IntSet::ToString() const {
    std::string output;
    if (!IsEmpty()) {
        output += std::to_string((*elements_)[0]);
        for (size_t i = 1; i < elements_->size(); ++i)
            output += " " + std::to_string((*elements_)[i]);
    }
    return std::move(output);
}

}  // namespace vcc
