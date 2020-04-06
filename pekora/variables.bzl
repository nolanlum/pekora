PEKORA_DEFAULT_COPTS = [
    "/W3",
    "/DNOMINMAX",
    "/DWIN32_LEAN_AND_MEAN",
    "/D_CRT_SECURE_NO_WARNINGS",
    "/D_SCL_SECURE_NO_WARNINGS",
    "/D_ENABLE_EXTENDED_ALIGNED_STORAGE",
    "/DNTDDI_VERSION=0x06030000",
    "/D_WIN32_WINNT=0x0603",
    "/DWINVER=0x0603",
    "/bigobj",
    "/wd4005",
    "/wd4068",
    "/wd4180",
    "/wd4244",
    "/wd4267",
    "/wd4503",
    "/wd4800",
]

PEKORA_DEFUALT_LINKOPTS = [
    "-ignore:4221",
    "-DEFAULTLIB:ole32.lib",
]
