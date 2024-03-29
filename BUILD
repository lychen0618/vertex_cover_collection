cc_library(
    name="common",
    srcs=[
        "common/bitset.cpp",
        "common/intset.cpp",
        "common/hypergraph.cpp",
        "common/thread_pool.cpp",
        "common/random_generator.cpp",
        "common/maxmin.cpp"
    ],
    hdrs=[
        "common/bitset.h",
        "common/intset.h",
        "common/hypergraph.h",
        "common/concurrent_drop_head_queue.h",
        "common/approximate_functions.h",
        "common/thread_pool.h",
        "common/random_generator.h",
        "common/maxmin.h"
    ],
    deps=[
        "@com_github_google_glog//:glog",
        # "@boost//:thread",
        # "@boost//:asio"
    ]
)

cc_library(
    name="base_alg",
    srcs=["base_algorithm.cpp"],
    hdrs=["base_algorithm.h"],
    deps=[":common"]
)

cc_library(
    name="mmcs_set",
    srcs=["mmcs.cpp"],
    hdrs=["mmcs.h"],
    deps=[
        ":base_alg"
    ]
)

cc_library(
    name="adc_enum",
    srcs=["adc_enum.cpp"],
    hdrs=["adc_enum.h"],
    deps=[
        ":mmcs_set"
    ]
)

cc_library(
    name="advanced_adc_enum",
    srcs=["advanced_adc_enum.cpp"],
    hdrs=["advanced_adc_enum.h"],
    deps=[
        ":adc_enum"
    ]
)

cc_library(
    name="sub_mmcs",
    srcs=["sub_mmcs.cpp"],
    hdrs=["sub_mmcs.h"],
    deps=[
        ":mmcs_set"
    ]
)

cc_library(
    name="random_enum",
    srcs=["random_enum.cpp"],
    hdrs=["random_enum.h"],
    deps=[
        ":mmcs_set"
    ]
)

cc_library(
    name="random_enum_auto_inc",
    srcs=["random_enum_auto_inc.cpp"],
    hdrs=["random_enum_auto_inc.h"],
    deps=[
        ":random_enum"
    ]
)

cc_binary(
    name="test_code",
    srcs=["test/test_code.cpp"],
    deps=[
        ":common",
        ":base_alg",
        "@com_github_google_glog//:glog",
        ":exp"
    ]
)

cc_library(
    name="exp",
    srcs=["experiments/exp_framework.cpp"],
    hdrs=["experiments/exp_framework.h"],
    linkopts=[ 
        "-l/usr/local/lib/libboost_filesystem.so",
        "-l/usr/local/lib/libboost_system.so"
    ],
    deps=[
        ":common",
        # "@boost//:filesystem",
        # "@boost//:algorithm"
    ]
)

cc_binary(
    name="appro_exp",
    srcs=["experiments/approximation_exp.cpp"],
    deps=[
        ":exp",
        ":advanced_adc_enum"
    ]
)

cc_binary(
    name="diversity_exp",
    srcs=["experiments/diversity_exp.cpp"],
    deps=[
        ":exp",
        ":random_enum_auto_inc",
        ":sub_mmcs"
    ]
)

cc_test(
    name="common_test",
    srcs=[
        "test/intset_test.cpp",
        "test/hypergraph_test.cpp",
        "test/concurrent_drop_head_queue_test.cpp",
        "test/thread_pool_test.cpp",
        "test/maxmin_test.cpp"
    ],
    deps=[
        ":common",
        "@com_google_googletest//:gtest_main",
    ]
)

cc_test(
    name="mmcs_test",
    srcs=["test/mmcs_test.cpp"],
    deps=[
        ":mmcs_set",
        "@com_google_googletest//:gtest_main"
    ]
)

cc_test(
    name="adc_enum_test",
    srcs=["test/adc_enum_test.cpp"],
    deps=[
        ":adc_enum",
        "@com_google_googletest//:gtest_main"
    ]
)

cc_test(
    name="advanced_adc_enum_test",
    srcs=["test/advanced_adc_enum_test.cpp"],
    deps=[
        ":advanced_adc_enum",
        "@com_google_googletest//:gtest_main"
    ]
)

cc_test(
    name="random_enum_test",
    srcs=["test/random_enum_test.cpp"],
    deps=[
        ":random_enum",
        "@com_google_googletest//:gtest_main"
    ]
)

cc_test(
    name="random_enum_auto_inc_test",
    srcs=["test/random_enum_auto_inc_test.cpp"],
    deps=[
        ":random_enum_auto_inc",
        "@com_google_googletest//:gtest_main"
    ]
)

cc_binary(
    name = "vcc_client",
    srcs = ["rpc/vcc_client.cpp"],
    deps = [
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_gflags_gflags//:gflags",
        "//protos:vertex_cover",
    ],
)

cc_binary(
    name = "vcc_server",
    srcs = ["rpc/vcc_server.cpp"],
    deps = [
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "//protos:vertex_cover",
        "//:random_enum_auto_inc",
        "//:advanced_adc_enum",
    ],
)