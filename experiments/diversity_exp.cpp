#include <utility>
#include "exp_framework.h"
#include "random_enum_auto_inc.h"
#include "sub_mmcs.h"

namespace exps {

float threshold = 0.5;
std::pair<bool, double> back_steps = {true, 1};
std::pair<bool, int> top_k = {false, 2};

int k = 25;

void RunSubMMCS(std::shared_ptr<vcc::HyperGraph> hg) {
    std::shared_ptr<vcc::OutputQueue> output_queue(new vcc::OutputQueue(100));
    vcc::SubMMCS sub_mmcs(hg, output_queue, k, threshold);
    sub_mmcs.SetMethod(vcc::MMCS::Method::MIN);
    sub_mmcs.SetToLog(false);
    sub_mmcs.SetToQueue(false);
    sub_mmcs.RunDiverse();
    vcc::Maxmin(sub_mmcs.GetResult(),
                static_cast<int>(sub_mmcs.GetResult().size()));
}

void RunRandomEnum(std::shared_ptr<vcc::HyperGraph> hg) {
    std::shared_ptr<vcc::OutputQueue> output_queue(new vcc::OutputQueue(100));
    vcc::RandomEnum random_enum(hg, output_queue, k);
    random_enum.SetMethod(vcc::MMCS::Method::MIN);
    random_enum.SetBackMethod(back_steps.first, back_steps.second);
    random_enum.SetToLog(false);
    random_enum.SetToQueue(false);
    random_enum.RunDiverse();
    int tk = top_k.first ? top_k.second
                         : static_cast<int>(random_enum.GetResult().size());
    vcc::Maxmin(random_enum.GetResult(), tk);
}

// Exp1
void compare_sub_with_random() {
    // {"hospital_evidence"}, {"tax_evidence"}
    // {"match_base"}, {"random_base"}
    // {"hospital_conflict"}, {"tax_conflict"}
    // {"hospital_evidence"},{"tax_evidence"},{"match_base"},{"random_base"}
    std::vector<std::vector<std::string>> datasets_list{{"hospital_conflict"},
                                                        {"tax_conflict"},
                                                        {"match_base"},
                                                        {"random_base"}};
    int round = 1;
    for (auto& datasets : datasets_list) {
        LOG(INFO) << "Round - " << round++;
        // LOG(INFO) << "Run SubMMCS";
        // for (float thd : {0.1, 0.5, 0.75}) {
        //     LOG(INFO) << "Threshold - " << thd;
        //     threshold = thd;
        //     mmcs_executer(datasets, RunSubMMCS);
        // }
        LOG(INFO) << "Run RandomEnum";
        top_k = {false, 2};

        for (auto& p : std::vector<std::pair<bool, double>>(
                 {{false, 0.25}, {false, 0.5}, {true, 1}})) {
            LOG(INFO) << "Backsteps - " << p.second;
            back_steps = p;
            mmcs_executer(datasets, RunRandomEnum);
        }
    }
}

void RunBestOfTimes(std::shared_ptr<vcc::HyperGraph> hg) {
    float max_diversity = 0;
    for (int time = 0; time < 5; ++time) {
        std::shared_ptr<vcc::OutputQueue> output_queue(
            new vcc::OutputQueue(100));
        vcc::RandomEnum random_enum(hg, output_queue, k);
        random_enum.SetMethod(vcc::MMCS::Method::MIN);
        random_enum.SetBackMethod(back_steps.first, back_steps.second);
        random_enum.SetToLog(false);
        random_enum.SetToQueue(false);
        random_enum.RunDiverse();
        float temp =
            vcc::Maxmin(random_enum.GetResult(),
                        static_cast<int>(random_enum.GetResult().size()));
        max_diversity = std::max(max_diversity, temp);
    }
    LOG(INFO) << "Diversity Value of BestOfTimes: " << max_diversity;
}

void RunMaxmin(std::shared_ptr<vcc::HyperGraph> hg) {
    std::shared_ptr<vcc::OutputQueue> output_queue(new vcc::OutputQueue(100));
    vcc::RandomEnum random_enum(hg, output_queue, k * 5);
    random_enum.SetMethod(vcc::MMCS::Method::MIN);
    random_enum.SetBackMethod(back_steps.first, back_steps.second);
    random_enum.SetToLog(false);
    random_enum.SetToQueue(false);
    random_enum.RunDiverse();
    LOG(INFO) << "Diversity Value of Maxmin: "
              << vcc::Maxmin(random_enum.GetResult(), k);
}

void RunIncMaxmin(std::shared_ptr<vcc::HyperGraph> hg) {
    std::shared_ptr<vcc::OutputQueue> output_queue(new vcc::OutputQueue(100));
    vcc::RandomEnumAutoInc random_enum(hg, output_queue, k);
    random_enum.SetMethod(vcc::MMCS::Method::MIN);
    random_enum.SetBackMethod(back_steps.first, back_steps.second);
    random_enum.SetToLog(false);
    random_enum.SetToQueue(false);
    random_enum.RunDiverse();
    LOG(INFO) << "Diversity Value of IncMaxmin: "
              << vcc::Maxmin(random_enum.GetResult(), k);
}

// Exp2
void random_with_different_strategy() {
    std::vector<std::vector<std::string>> datasets_list{{"hospital_conflict"},
                                                        {"tax_conflict"},
                                                        {"match_base"},
                                                        {"random_base"}};
    int round = 1;
    back_steps = {true, 1};
    for (auto& datasets : datasets_list) {
        LOG(INFO) << "Round - " << round++;
        back_steps = {true, 1};
        LOG(INFO) << "Strategy - BestOfTimes";
        mmcs_executer(datasets, RunBestOfTimes);
        // LOG(INFO) << "Strategy - Maxmin";
        // mmcs_executer(datasets, RunMaxmin);
        // LOG(INFO) << "Strategy - IncMaxmin";
        // mmcs_executer(datasets, RunIncMaxmin);
    }
}

// Exp3
void compare_gmm_with_inc_gmm(std::shared_ptr<vcc::HyperGraph> hg) {}

}  // namespace exps

int main(int argc, char** argv) {
    FLAGS_log_dir = "experiments/log";
    google::InitGoogleLogging(argv[0]);
    // exps::compare_sub_with_random();
    exps::random_with_different_strategy();
    // exps::compare_gmm_with_inc_gmm();
    return 0;
}