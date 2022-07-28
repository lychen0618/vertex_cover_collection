#include "exp_framework.h"
#include <glog/logging.h>
#include <fstream>
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
using boost::filesystem::path;
using boost::filesystem::directory_iterator;
using boost::algorithm::split;
using boost::algorithm::is_space;
namespace exps {
void mmcs_executer(const std::vector<std::string>& datasets, Func func) {
    for (auto& dataset : datasets) {
        LOG(INFO) << "#### Process Dataset - " << dataset << " ####";
        path dir{data_base_dir + "/" + dataset};
        directory_iterator dir_iter{dir};
        while (dir_iter != directory_iterator{}) {
            const path& file_path = dir_iter->path();
            LOG(INFO) << "## Process File - " << file_path.filename() << " ##";
            std::ifstream fstrm(file_path.string(), std::ifstream::in);
            std::string line;
            std::vector<vcc::IntSet> input;
            while (getline(fstrm, line)) {
                if (line == "")
                    break;
                std::vector<std::string> out_strs;
                split(out_strs, line, is_space());
                input.emplace_back(vcc::IntSet());
                for (auto& str : out_strs)
                    input.back().Add(std::stoi(str));
            }
            std::shared_ptr<vcc::HyperGraph> hg(new vcc::HyperGraph(input));
            LOG(INFO) << "vertex: " << hg->VertexNum()
                      << " edge: " << hg->EdgeNum();
            func(hg);
            dir_iter++;
        }
    }
}

void simple_test() {
    auto func = [](std::shared_ptr<vcc::HyperGraph> hg) {
        std::cout << "func called\n";
    };
    mmcs_executer({"hospital_conflict"}, func);
}
}  // namespace exp
