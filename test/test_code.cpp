#include <iostream>
#include <bitset>
#include "common/intset.h"

#include "glog/logging.h"
typedef std::bitset<2> BitSet;

using vcc::IntSet;

int main(int argc, char** argv) {
    BitSet a(2);
    BitSet b(a);
    b.flip(1);
    LOG(INFO) << a[0] << " " << a[1];
    LOG(INFO) << b[0] << " " << b[1];
    std::unique_ptr<int> ptr;
    if (ptr == nullptr) printf("null\n");
    return 0;
}