cc_binary(
    name = "vcc_client",
    srcs = ["vcc_client.cpp"],
    deps = [
        "@com_github_grpc_grpc//:grpc++",
        "//protos:vertex_cover",
    ],
)

cc_binary(
    name = "vcc_server",
    srcs = ["vcc_server.cpp"],
    deps = [
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "//protos:vertex_cover",
    ],
)