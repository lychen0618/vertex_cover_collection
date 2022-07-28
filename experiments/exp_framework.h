#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "common/hypergraph.h"

namespace exps {
const std::string data_base_dir =
    "/home/lishuai.0613/projects/build_mmcs/vertex_cover_collection/"
    "experiments/data";
using Func = std::function<void(std::shared_ptr<vcc::HyperGraph> hg)>;

void mmcs_executer(const std::vector<std::string>& datasets, Func func);

void simple_test();
}  // namespace exp
