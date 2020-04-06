load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "com_google_absl",
    remote = "https://github.com/abseil/abseil-cpp",
    commit = "d43b7997c0ca0f3312a51d1057fb73cfe934703b",
    shallow_since = "1586034530 -0400",
)

http_archive(
    name = "com_github_gflags_gflags",
    strip_prefix = "gflags-2.2.2",
    urls = [
        "https://mirror.bazel.build/github.com/gflags/gflags/archive/v2.2.2.tar.gz",
        "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
    ],
)

git_repository(
    name = "com_google_glog",
    remote = "https://github.com/google/glog/",
    commit = "3ba8976592274bc1f907c402ce22558011d6fc5e",
    shallow_since = "1585127859 +0100",
)
