#include "random_generator.h"

namespace vcc {

RandomGenerator::RandomGenerator(size_t st, size_t ed, size_t seed)
    : e(seed), u(st, ed) {}

size_t RandomGenerator::RandomInt() { return u(e); }

}  // namespace vcc
