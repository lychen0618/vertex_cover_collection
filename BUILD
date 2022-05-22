cc_library(
    name="common",
    srcs=[
        "common/bitset.cpp",
        "common/intset.cpp",
        "common/hypergraph.cpp",
        "common/thread_pool.cpp"
    ],
    hdrs=[
        "common/bitset.h",
        "common/intset.h",
        "common/hypergraph.h",
        "common/concurrent_drop_head_queue.h",
        "common/approximate_functions.h",
        "common/thread_pool.h"
    ],
    deps=[
        "@com_github_google_glog//:glog",
        "@boost//:thread",
        "@boost//:asio"
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

cc_binary(
    name="test_code",
    srcs=["test/test_code.cpp"],
    deps=[
        ":common",
        ":base_alg",
        "@com_github_google_glog//:glog",
    ]
)

cc_test(
    name="common_test",
    srcs=[
        "test/intset_test.cpp",
        "test/hypergraph_test.cpp",
        "test/concurrent_drop_head_queue_test.cpp",
        "test/thread_pool_test.cpp"
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