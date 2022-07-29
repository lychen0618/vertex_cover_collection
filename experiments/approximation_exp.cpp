#include "advanced_adc_enum.h"
#include "exp_framework.h"

namespace exps {
// vars
int thread_num = 4;

// ADCEnum use MAX method.
void baseline(std::shared_ptr<vcc::HyperGraph> hg) {
    std::shared_ptr<vcc::OutputQueue> output_queue(new vcc::OutputQueue(100));
    vcc::ADCEnum adc_enum(hg, output_queue, 0.1);
    adc_enum.SetMethod(vcc::MMCS::Method::MAX);
    adc_enum.SetToQueue(false);
    adc_enum.SetToLog(false);
    adc_enum.RunApproximate();
    LOG(INFO) << "vertex cover num: " << adc_enum.GetTotalNumMVC();
}

// AdvanceADCEnum use MAX method. Different sizes of ThreadPool.
void max_and_different_thread_num(std::shared_ptr<vcc::HyperGraph> hg) {
    std::shared_ptr<vcc::OutputQueue> output_queue(new vcc::OutputQueue(100));
    vcc::AdvancedADCEnum adc_enum(hg, output_queue, 0.1, thread_num);
    adc_enum.SetMethod(vcc::MMCS::Method::MAX);
    adc_enum.SetToQueue(false);
    adc_enum.SetToLog(false);
    adc_enum.RunApproximate();
}

// The size of ThreadPool is zero. Use Priority Method.
void priority_without_thread_pool(std::shared_ptr<vcc::HyperGraph> hg) {}

// Priority + ThreadPool
void priority_and_thread_pool(std::shared_ptr<vcc::HyperGraph> hg) {}

}  // namespace exps

int main(int argc, char** argv) {
    FLAGS_log_dir = "experiments/log";
    google::InitGoogleLogging(argv[0]);
    // {"hospital_evidence"}, {"tax_evidence"}, {"match_base"}, {"random_base"}
    std::vector<std::vector<std::string>> datasets_list{{"hospital_evidence"},
                                                        {"tax_evidence"},
                                                        {"match_base"},
                                                        {"random_base"}};
    int round = 1;
    for (auto& datasets : datasets_list) {
        LOG(INFO) << "Round - " << round++;
        exps::mmcs_executer(datasets, exps::baseline);
        for (int tu : {1, 2, 4, 8, 16}) {
            thread_num = tu;
            LOG(INFO) << "ThreadNum - " << thread_num;
            exps::mmcs_executer(datasets, exps::max_and_different_thread_num);
        }
    }
    return 0;
}