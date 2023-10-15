load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "grid-container-ftxui",
    srcs = glob(
        include = ["src/**"],
        exclude = [
            "example/**",
        ],
    ),
    hdrs = glob(["include/**/*.hpp"]),
    copts = ["-Iexternal/grid-container-ftxui/src"] + select({
        "@platforms//os:linux": [],
        "@platforms//os:windows": ["-utf-8"],
    }),
    includes = ["include/"],
    local_defines = select({
        "@platforms//os:linux": [],
        "@platforms//os:windows": [
            "_UNICODE",
            "UNICODE",
        ],
    }),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = ["@ftxui"]
)