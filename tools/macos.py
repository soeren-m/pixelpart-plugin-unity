import common

def exists(env):
    return True

def options(opts):
    opts.Add("macos_sdk_path", "macOS SDK path", "")

def generate(env):
    if env["arch"] != "universal":
        print("Only universal architecture supported on macOS. Exiting.")
        env.Exit(1)

    env["CC"] = "clang"
    env["CXX"] = "clang++"

    # Remove lib prefix
    env["SHLIBPREFIX"] = ""

    # Architecture
    env.Append(CCFLAGS=["-arch", "x86_64", "-arch", "arm64"])
    env.Append(LINKFLAGS=["-arch", "x86_64", "-arch", "arm64"])

    # Target version
    env.Append(CCFLAGS=["-mmacosx-version-min=10.10"])
    env.Append(LINKFLAGS=["-mmacosx-version-min=10.10"])

    env.Append(LINKFLAGS=["-framework", "Cocoa"])

    # SDK path
    if env["macos_sdk_path"]:
        env.Append(CCFLAGS=["-isysroot", env["macos_sdk_path"]])
        env.Append(LINKFLAGS=["-isysroot", env["macos_sdk_path"]])

    # zlib fix
    env.Append(CFLAGS=["-DHAVE_UNISTD_H"])

    # Options for pixelpart-runtime
    env.Append(CPPDEFINES=["PIXELPART_RUNTIME_MULTITHREADING"])

    common.generate(env)
