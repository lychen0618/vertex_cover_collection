#include "bitset.h"

namespace vcc {
BitSet::BitSet(size_t size) : bitset_(new BigBitSet()) {
    for (int i = 0; i < static_cast<int>(size); ++i) {
        bitset_->set(i);
    }
}

BitSet::BitSet(const std::vector<int>& vec) : bitset_(new BigBitSet()) {
    for (const auto& e : vec) {
        bitset_->set(e);
    }
}

std::vector<int> BitSet::Get() const {
    std::vector<int> tmp;
    for (int i = 0; i < static_cast<int>(bitset_->size()); ++i) {
        if ((*bitset_)[i] == 1)
            tmp.emplace_back(i);
    }
    return std::move(tmp);
}

int BitSet::NextSetBit(int start) const {
    for (int i = start + 1; i < static_cast<int>(bitset_->size()); ++i) {
        if ((*bitset_)[i] == 1)
            return i;
    }
    return -1;
}

void BitSet::Or(const BitSet& b) {
    for (int v : b.Get()) {
        bitset_->set(v);
    }
}

float BitSet::GetDistance(const BitSet& a, const BitSet& b) {
    int cnt = 0;
    for (int w = a.NextSetBit(); w != -1; w = a.NextSetBit(w)) {
        if (b.Get(w))
            cnt++;
    }
    return 1 - float(cnt) / (a.Count() + b.Count() - cnt);
}

std::string BitSet::ToString() const {
    std::string output;
    if (!IsEmpty()) {
        std::vector<int> vec(Get());
        output += std::to_string(vec[0]);
        for (size_t i = 1; i < vec.size(); ++i)
            output += " " + std::to_string(vec[i]);
    }
    return std::move(output);
}

}  // namespace vcc
