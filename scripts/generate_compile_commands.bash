#!/bin/bash
echo "Generating compile_commands.json for use by clangd."
exec bazel run @hedron_compile_commands//:refresh_all