#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <bitset>
#include <memory>
#include <string>
#include <vector>
#define MAXBIT 2048

namespace vcc {

class BitSet {
private:
    typedef std::bitset<MAXBIT> BigBitSet;

public:
    BitSet() : bitset_(new BigBitSet()) {}
    BitSet(size_t size);
    BitSet(const std::vector<int>& vec);
    BitSet(const BitSet& bit_set) {
        bitset_.reset(new BigBitSet(*bit_set.bitset_));
    }
    BitSet& operator=(const BitSet& bit_set) {
        bitset_.reset(new BigBitSet(*bit_set.bitset_));
        return *this;
    }
    BitSet(BitSet&& bit_set) : bitset_(bit_set.bitset_.release()) {}
    ~BitSet() = default;

    size_t Count() const { return bitset_->count(); }

    size_t Size() const { return bitset_->size(); }

    bool IsEmpty() const { return bitset_->none(); }

    int Get(size_t index) const { return (*bitset_)[index]; }

    std::vector<int> Get() const;

    void Flip(const int& element) { bitset_->flip(element); }

    void Set(const int& element) { bitset_->set(element); }

    void Reset(const int& element) { bitset_->reset(element); }

    int NextSetBit(int start = -1) const;

    std::string ToString() const;

private:
    std::unique_ptr<BigBitSet> bitset_;
};
}  // namespace vcc
