#include <iostream>
#include <bitset>
#include "common/intset.h"
#include "base_algorithm.h"

#include "glog/logging.h"

using vcc::BaseAlgorithm;
using vcc::OutputQueue;

int main(int argc, char** argv) {
    std::shared_ptr<OutputQueue> output_queue = std::make_shared<OutputQueue>(10);
    BaseAlgorithm base_alg(nullptr, output_queue);
    base_alg.RunAll();
    base_alg.RunApproximate();
    base_alg.RunDiverse();
    return 0;
}