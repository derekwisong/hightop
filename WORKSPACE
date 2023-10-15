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

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/ed994039a951b736091776d677f324b3903ef939.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-ed994039a951b736091776d677f324b3903ef939",
    sha256 = "085bde6c5212c8c1603595341ffe7133108034808d8c819f8978b2b303afc9e7",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()

# Add Google's GTest/googletest framework
http_archive(
    name = "com_google_googletest",
    urls = ["https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"],
    strip_prefix = "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
    sha256 = "755f9a39bc7205f5a0c428e920ddad092c33c8a1b46997def3f1d4a82aded6e1",
)

# ftui grid container component https://github.com/mingsheng13/grid-container-ftxui
http_archive(
    name = "grid-container-ftxui",
    url = "https://github.com/mingsheng13/grid-container-ftxui/archive/5deab11f7b60ff9d75c5718b63637e7a1dbed86a.zip",
    strip_prefix = "grid-container-ftxui-5deab11f7b60ff9d75c5718b63637e7a1dbed86a",
    build_file = "//third_party:grid-container-ftxui.BUILD",
    sha256 = "673f4ebfe3efa18cf02d545070bc4fe9e96b111728f193c9691107179b192cf1"
)
