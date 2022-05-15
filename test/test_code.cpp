#include <iostream>
#include <bitset>
#include "common/intset.h"
#include "base_algorithm.h"

#include "glog/logging.h"

using vcc::BaseAlgorithm;
using vcc::OutputQueue;

int main(int argc, char** argv) {
    BaseAlgorithm base_alg(nullptr, std::make_shared<OutputQueue>(OutputQueue(10)));
    base_alg.RunAll();
    base_alg.RunApproximate();
    base_alg.RunDiverse();
    return 0;
}