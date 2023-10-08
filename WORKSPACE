workspace(name="hightop")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# https://github.com/ArthurSonzogni/FTXUI
http_archive(
    name = "ftxui",  # MIT
    build_file = "//third_party:ftxui.BUILD",
    sha256 = "a2991cb222c944aee14397965d9f6b050245da849d8c5da7c72d112de2786b5b",
    strip_prefix = "FTXUI-5.0.0",
    url = "https://github.com/ArthurSonzogni/FTXUI/archive/v5.0.0.tar.gz",
)
