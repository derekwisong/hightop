# hightop

Hightop provides a dynamic real-time view of a running system, similar to top.


## Compile and Run Using Bazel

This project is built using [Bazel][1]. I prefer to use the [Bazelisk][2] launcher.

```bash
bazel run //hightop
```

## Dependencies

- [FTXUI][3] (see WORKSPACE)
- GoogleTest (to run test suite, see WORKSPACE)


[1]: https://bazel.build "The Bazel build system"
[2]: https://github.com/bazelbuild/bazelisk "Bazelisk: A user-friendly launcher for Bazel."
[3]: https://github.com/ArthurSonzogni/FTXUI "A simple cross-platform C++ library for terminal based user interfaces!"