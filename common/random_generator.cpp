#include "random_generator.h"
#include <ctime>

namespace vcc {

RandomGenerator::RandomGenerator(size_t st, size_t ed)
    : e(time(0)), u(st, ed) {}

size_t RandomGenerator::RandomInt() { return u(e); }

}  // namespace vcc
