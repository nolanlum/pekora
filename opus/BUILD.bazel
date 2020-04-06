package(default_visibility = ['//visibility:public'])

COPTS = [
    '-DHAVE_CONFIG_H',
    '-Iopus/opus',
    '-Iopus/opus/celt',
    '-Iopus/opus/celt/x86',
    '-Iopus/opus/include',
    '-Iopus/opus/silk',
    '-Iopus/opus/silk/float',
    '-Iopus/opus/silk/x86',
    '-I$(GENDIR)/opus/opus',
]


cc_binary(
    name = 'opus_demo',
    deps = ['opus'],
    srcs = [
        'opus/config.h',
        'opus/src/opus_demo.c',
    ],
    copts = COPTS,
    linkopts = ['-lm'],
)

cc_binary(
    name = 'repacketizer_demo',
    deps = ['opus'],
    srcs = [
        'opus/config.h',
        'opus/src/repacketizer_demo.c',
    ],
    copts = COPTS,
    linkopts = ['-lm'],
)

filegroup(
    name = "celt",
    srcs = glob(
        [
            "opus/celt/*.c",
            "opus/celt/x86/*.c",
        ],
        exclude = ["opus/celt/opus_custom_demo.c"],
    ),
)

filegroup(
    name = "celt_headers",
    srcs = glob([
        "opus/celt/*.h",
        "opus/celt/x86/*.h",
    ]),
)

filegroup(
    name = "silk",
    srcs = glob([
        "opus/silk/*.c",
        "opus/silk/float/*.c",
        "opus/silk/x86/*.c",
    ]),
)

filegroup(
    name = "silk_headers",
    srcs = glob([
        "opus/silk/*.h",
        "opus/silk/float/*.h",
        "opus/silk/x86/*.h",
    ]),
)

cc_library(
    name = 'opus',
    srcs = [
        ':celt',
        ':silk',
        ":celt_headers",
        ":silk_headers",
        'opus/src/analysis.c',
        'opus/src/mlp.c',
        'opus/src/mlp_data.c',
        'opus/src/opus.c',
        'opus/src/opus_decoder.c',
        'opus/src/opus_encoder.c',
        'opus/src/opus_multistream.c',
        'opus/src/opus_multistream_decoder.c',
        'opus/src/opus_multistream_encoder.c',
        'opus/src/repacketizer.c',
        "opus/src/analysis.h",
        "opus/src/mapping_matrix.h",
        "opus/src/mlp.h",
        "opus/src/opus_private.h",
        "opus/src/tansig_table.h",
    ],
    hdrs = [
        "opus/config.h",
        "opus/version.h",
    ] + glob([
        "opus/include/*.h",
    ]),
    copts = COPTS,
)

genrule(
    name = "genconfig",
    srcs = ["opus/win32/config.h"],
    outs = ["opus/config.h"],
    cmd_bat = "cp $< $@ > NUL",
)

genrule(
    name = "genversion_script",
    srcs = ["opus/win32/genversion.bat"],
    outs = ["opus/genversion.bat"],
    cmd_bat = "cp $< $@ > NUL",
)

genrule(
    name = "genversion",
    outs = ["opus/version.h"],
    cmd_bat = "$(location opus/win32/genversion.bat) $@ PACKAGE_VERSION",
    tools = [
        "opus/win32/genversion.bat",
        "opus/package_version",
    ],
)