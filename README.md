# pekora
usagi aidoru pekora

# build

This only builds on Win10, probably. Tested with MSVC and clang-cl.

```bash
bazel run //pekora
```

To use clang-cl:

```bash
bazel run //pekora --compiler=clang-cl
```