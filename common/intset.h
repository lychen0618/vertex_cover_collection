#pragma once

#include "bitset.h"

namespace vcc {
class IntSet {
public:
    IntSet() : elements_(new std::vector<int>()){};
    IntSet(size_t num) : elements_(new std::vector<int>()) {
        for (size_t i = 0; i < num; ++i)
            elements_->push_back(i);
    }
    IntSet(const std::vector<int>& elements) { Set(elements); }
    IntSet(const BitSet& bit_set) { Set(bit_set.Get()); }
    IntSet(const IntSet& int_set) {
        elements_.reset(new std::vector<int>(*int_set.elements_));
    }
    IntSet& operator=(const IntSet& int_set) {
        elements_.reset(new std::vector<int>(*int_set.elements_));
        return *this;
    }
    IntSet(IntSet&& int_set) : elements_(int_set.elements_.release()) {}
    ~IntSet() = default;

    bool IsEmpty() const { return elements_ == nullptr || elements_->empty(); }

    void Add(int element) { elements_->push_back(element); }
    void Set(const std::vector<int>& elements) {
        elements_.reset(new std::vector<int>(elements));
    }
    size_t Size() const { return elements_->size(); }
    int Get(size_t index) const { return (*elements_)[index]; }

    const std::vector<int>& Get() const { return *elements_; }

    static IntSet And(const IntSet& a, const IntSet& b);

    static BitSet And(const BitSet& a, const IntSet& b);

    void And(const IntSet& b) {
        elements_.reset(And(*this, b).elements_.release());
    }

    static IntSet Or(const IntSet& a, const IntSet& b);

    static void Or(BitSet& a, const IntSet& b);

    void Or(const IntSet& b) {
        elements_.reset(Or(*this, b).elements_.release());
    }

    static IntSet AndNot(const IntSet& a, const IntSet& b);

    static void AndNot(BitSet& a, const BitSet& b);

    static void AndNot(BitSet& a, const IntSet& b);

    void AndNot(const IntSet& b) {
        elements_.reset(AndNot(*this, b).elements_.release());
    }

    std::string ToString() const;

private:
    std::unique_ptr<std::vector<int>> elements_;
};
}  // namespace vcc
