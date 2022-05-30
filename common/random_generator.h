#pragma once
#include <random>
#include <limits.h>

namespace vcc
{
class RandomGenerator
{
public:
    RandomGenerator(size_t st = 0, size_t ed = INT_MAX, size_t seed = 1);
    ~RandomGenerator() = default;

    size_t RandomInt();
private:
    std::default_random_engine e;
    std::uniform_int_distribution<unsigned> u;
};
    
} // namespace vcc
