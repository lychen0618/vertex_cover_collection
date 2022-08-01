#pragma once
#include <vector>
#include "bitset.h"

namespace vcc {
float IncMaxmin(const std::vector<BitSet>& vc_list,
                std::vector<std::vector<float>>& set_distances,
                std::vector<int>& ans,
                std::vector<float>& distances,
                int cur_size);

float Maxmin(const std::vector<BitSet>& vc_list);

}  // namespace vcc
