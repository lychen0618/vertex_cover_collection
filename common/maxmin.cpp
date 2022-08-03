#include "maxmin.h"
#include <iomanip>
#include <unordered_set>
#include "glog/logging.h"

namespace vcc {
float IncMaxmin(const std::vector<BitSet>& vc_list,
                std::vector<std::vector<float>>& set_distances,
                std::vector<int>& ans,
                std::vector<float>& distances,
                int cur_size) {
    int size = static_cast<int>(vc_list.size());
    int top_k = static_cast<int>(ans.size());
    if (cur_size == 0) {
        float max_dis = -1;
        int first = 0, second = 0;
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                if (set_distances[i][j] > max_dis) {
                    max_dis = set_distances[i][j];
                    first = i;
                    second = j;
                }
            }
        }
        ans[0] = first;
        ans[1] = second;
        distances[0] = distances[1] = max_dis;
        cur_size = 2;
    }
    // Inc compute
    std::unordered_set<int> res(ans.begin(), ans.begin() + cur_size);
    while (cur_size < top_k) {
        float max_dis = -1;
        int pos = 0;
        for (int i = 0; i < size; ++i) {
            if (res.count(i))
                continue;
            float min_dis = 2;
            for (int j : res) {
                min_dis = std::min(min_dis, set_distances[i][j]);
            }
            if (min_dis > max_dis) {
                max_dis = min_dis;
                pos = i;
            }
        }
        ans[cur_size] = pos;
        distances[cur_size] = max_dis;
        ++cur_size;
        res.insert(pos);
    }
    // Get general info
    float total_dis = 0, min_dis = 2, max_dis = -1;
    int num = 0;
    std::vector<int> res_list(res.begin(), res.end());
    for (int i = 0; i < top_k; ++i) {
        for (int j = i + 1; j < top_k; ++j) {
            num++;
            float distance = set_distances[res_list[i]][res_list[j]];
            total_dis += distance;
            min_dis = std::min(min_dis, distance);
            max_dis = std::max(max_dis, distance);
        }
    }
    LOG(INFO) << "maxmin info(max/min/avg): " << std::setprecision(3) << max_dis
              << "/" << min_dis << "/" << (total_dis / num);
    return min_dis;
}

float Maxmin(const std::vector<BitSet>& vc_list, int top_k) {
    int n = static_cast<int>(vc_list.size());
    std::vector<std::vector<float>> set_distances(n,
                                                  std::vector<float>(n, 0.0));
    // get distances
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            float distance = BitSet::GetDistance(vc_list[i], vc_list[j]);
            set_distances[i][j] = set_distances[j][i] = distance;
        }
    }
    // call incmaxmin
    std::vector<int> ans(top_k);
    std::vector<float> distances(top_k);
    return IncMaxmin(vc_list, set_distances, ans, distances, 0);
}
}  // namespace vcc
