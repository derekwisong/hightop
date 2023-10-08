#!/bin/bash
exec bazel run //tests:test_hightop --cache_test_results=no
