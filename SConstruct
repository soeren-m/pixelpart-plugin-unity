#!/usr/bin/env python

import os
import sys
import subprocess

if sys.version_info < (3,):
    def decode_utf8(x):
        return x
else:
    import codecs
    def decode_utf8(x):
        return codecs.utf_8_decode(x)[0]

# Workaround for MinGW. See:
# http://www.scons.org/wiki/LongCmdLinesOnWin32
if os.name == "nt":
    def mySubProcess(cmdline, env):
        startupinfo = subprocess.STARTUPINFO()
        startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        proc = subprocess.Popen(
            cmdline,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            startupinfo=startupinfo,
            shell=False,
            env=env,
        )
        data, err = proc.communicate()
        rv = proc.wait()
        if rv:
            print("=====")
            print(err.decode("utf-8"))
            print("=====")
        return rv

    def mySpawn(sh, escape, cmd, args, env):
        newargs = " ".join(args[1:])
        cmdline = cmd + " " + newargs
        rv = 0
        if len(cmdline) > 32000 and cmd.endswith("ar"):
            cmdline = cmd + " " + args[1] + " " + args[2] + " "
            for i in range(3, len(args)):
                rv = mySubProcess(cmdline + args[i], env)
                if rv:
                    break
        else:
            rv = mySubProcess(cmdline, env)

        return rv

def add_source(sources, file):
    sources.append(file)

def add_sources(sources, directory, extension):
    for file in os.listdir(directory):
        if file.endswith(extension):
            sources.append(directory + '/' + file)

if sys.platform.startswith("linux"):
    host_platform = "linux"
elif sys.platform.startswith("freebsd"):
    host_platform = "freebsd"
elif sys.platform == "darwin":
    host_platform = "macos"
elif sys.platform == "win32" or sys.platform == "msys":
    host_platform = "windows"
else:
    raise ValueError("Could not detect platform automatically, please specify with platform=<platform>")

env = Environment(ENV = os.environ)

is64 = sys.maxsize > 2**32
if (env["TARGET_ARCH"] == "amd64" or
    env["TARGET_ARCH"] == "emt64" or
    env["TARGET_ARCH"] == "x86_64" or
    env["TARGET_ARCH"] == "arm64-v8a"):
    is64 = True

opts = Variables([], ARGUMENTS)
opts.Add(EnumVariable("platform", "Target platform", host_platform,
    allowed_values=("linux", "freebsd", "macos", "windows", "android", "ios"),
    ignorecase=2
))
opts.Add(EnumVariable("bits", "Target platform bits", "64" if is64 else "32", allowed_values=("32", "64")))
opts.Add(BoolVariable("use_mingw", "Use the MinGW compiler instead of MSVC - only effective on Windows", False))
opts.Add(EnumVariable("target", "Compilation target", "debug", allowed_values=("debug", "release"), ignorecase=2))
opts.Add(EnumVariable("ios_arch", "Target iOS architecture", "arm64", ["armv7", "arm64", "x86_64"]))
opts.Add(BoolVariable("ios_simulator", "Target iOS Simulator", False))
opts.Add("IPHONEPATH", "Path to iPhone toolchain", "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain")
opts.Add(EnumVariable("android_arch", "Target Android architecture", "armv7", ["armv7","arm64v8","x86","x86_64"]))
opts.Add("android_api_level", "Target Android API level", "18" if ARGUMENTS.get("android_arch", "armv7") in ["armv7", "x86"] else "21")
opts.Add("ANDROID_NDK_ROOT", "Path to your Android NDK installation. By default, uses ANDROID_NDK_ROOT from your defined environment variables.", os.environ.get("ANDROID_NDK_ROOT", None))

opts.Update(env)
Help(opts.GenerateHelpText(env))

if host_platform == "windows" and env["platform"] != "android":
    if env["bits"] == "64":
        env = Environment(TARGET_ARCH="amd64")
    elif env["bits"] == "32":
        env = Environment(TARGET_ARCH="x86")
    opts.Update(env)

if env["target"] == "debug":
    env.Append(CPPDEFINES=["DEBUG_ENABLED", "DEBUG_METHODS_ENABLED"])

if env["platform"] == "linux" or env["platform"] == "freebsd":
    env.Append(CFLAGS=["-DHAVE_UNISTD_H"])
    env.Append(CXXFLAGS=["-std=c++17"])
    env.Append(CCFLAGS=["-fPIC", "-Wwrite-strings"])
    env.Append(LINKFLAGS=["-Wl,-R,'$$ORIGIN'", "-Wl,--no-undefined", "-pthread", "-static-libgcc", "-static-libstdc++"])

    if env["target"] == "debug":
        env.Append(CCFLAGS=["-Og", "-g"])
    elif env["target"] == "release":
        env.Append(CCFLAGS=["-O3"])

    if env["bits"] == "64":
        env.Append(CCFLAGS=["-m64"])
        env.Append(LINKFLAGS=["-m64"])
        target_path = "x86_64/"
    elif env["bits"] == "32":
        env.Append(CCFLAGS=["-m32"])
        env.Append(LINKFLAGS=["-m32"])
        target_path = "x86/"

    target_path += "libpixelpart.so"

elif env["platform"] == "macos":
    if env["bits"] == "32":
        raise ValueError("Only 64-bit builds are supported for the macOS target.")

    env["CC"] = "clang"
    env["CXX"] = "clang++"

    env.Append(CCFLAGS=[
        "-arch", "x86_64",
        "-arch", "arm64",
        "-mmacosx-version-min=10.10"])
    env.Append(CFLAGS=[
        "-DHAVE_UNISTD_H"])
    env.Append(CXXFLAGS=[
        "-std=c++17"])
    env.Append(LINKFLAGS=[
        "-arch", "x86_64",
        "-arch", "arm64",
        "-mmacosx-version-min=10.10",
        "-framework", "Cocoa",
        "-Wl,-undefined,dynamic_lookup"])

    if env["target"] == "debug":
        env.Append(CCFLAGS=["-Og", "-g"])
    elif env["target"] == "release":
        env.Append(CCFLAGS=["-O3"])

    target_path = "macOS/pixelpart.bundle"

elif env["platform"] == "ios":
    if env["ios_simulator"]:
        sdk_name = "iphonesimulator"
        env.Append(CCFLAGS=["-mios-simulator-version-min=12.0"])
        env["LIBSUFFIX"] = ".simulator" + env["LIBSUFFIX"]
    else:
        sdk_name = "iphoneos"
        env.Append(CCFLAGS=["-miphoneos-version-min=12.0"])

    try:
        sdk_path = decode_utf8(subprocess.check_output(["xcrun", "--sdk", sdk_name, "--show-sdk-path"]).strip())
    except (subprocess.CalledProcessError, OSError):
        raise ValueError("Failed to find SDK path while running xcrun --sdk {} --show-sdk-path.".format(sdk_name))

    compiler_path = env["IPHONEPATH"] + "/usr/bin/"
    env["ENV"]["PATH"] = env["IPHONEPATH"] + "/Developer/usr/bin/:" + env["ENV"]["PATH"]
    env["CC"] = compiler_path + "clang"
    env["CXX"] = compiler_path + "clang++"
    env["AR"] = compiler_path + "ar"
    env["RANLIB"] = compiler_path + "ranlib"

    env.Append(CFLAGS=["-DHAVE_UNISTD_H"])
    env.Append(CXXFLAGS=["-std=c++17"])
    env.Append(CCFLAGS=["-arch", env["ios_arch"], "-isysroot", sdk_path])
    env.Append(LINKFLAGS=["-arch", env["ios_arch"], "-framework", "Cocoa", "-Wl,-undefined,dynamic_lookup", "-isysroot", sdk_path, "-F" + sdk_path])

    if env["target"] == "debug":
        env.Append(CCFLAGS=["-Og", "-g"])
    elif env["target"] == "release":
        env.Append(CCFLAGS=["-O3"])

    target_path = "iOS/libpixelpart." + env["ios_arch"] + ".a"

elif env["platform"] == "windows":
    if host_platform == "windows" and not env["use_mingw"]:
        env.Append(CXXFLAGS=["/std:c++17"])
        env.Append(CPPDEFINES=["TYPED_METHOD_BIND"])
        env.Append(LINKFLAGS=["/WX"])
        if env["target"] == "debug":
            env.Append(CCFLAGS=["/Z7", "/Od", "/EHsc", "/D_DEBUG", "/MTd"])
        elif env["target"] == "release":
            env.Append(CCFLAGS=["/O2", "/EHsc", "/DNDEBUG", "/MT"])

    elif host_platform == "linux" or host_platform == "freebsd" or host_platform == "macos":
        if env["bits"] == "64":
            env["CXX"] = "x86_64-w64-mingw32-g++"
            env["AR"] = "x86_64-w64-mingw32-ar"
            env["RANLIB"] = "x86_64-w64-mingw32-ranlib"
            env["LINK"] = "x86_64-w64-mingw32-g++"
        elif env["bits"] == "32":
            env["CXX"] = "i686-w64-mingw32-g++"
            env["AR"] = "i686-w64-mingw32-ar"
            env["RANLIB"] = "i686-w64-mingw32-ranlib"
            env["LINK"] = "i686-w64-mingw32-g++"

    elif host_platform == "windows" and env["use_mingw"]:
        env = Environment(ENV=os.environ, tools=["mingw"])
        opts.Update(env)

        env.Append(CXXFLAGS=["-std=c++17"])
        env["IMPLIBPREFIX"] = ""
        env["SHLIBPREFIX"] = ""

        env["SPAWN"] = mySpawn
        env.Replace(ARFLAGS=["q"])

    if host_platform == "linux" or host_platform == "freebsd" or host_platform == "macos" or env["use_mingw"]:
        env.Append(CCFLAGS=["-O3", "-Wwrite-strings"])
        env.Append(LINKFLAGS=["--static", "-Wl,--no-undefined", "-static-libgcc", "-static-libstdc++"])

        if env["bits"] == "64":
            env.Append(CCFLAGS=["-m64"])
            env.Append(LINKFLAGS=["-m64"])
        elif env["bits"] == "32":
            env.Append(CCFLAGS=["-m32"])
            env.Append(LINKFLAGS=["-m32"])

    if env["bits"] == "64":
        target_path = "x86_64/"
    elif env["bits"] == "32":
        target_path = "x86/"

    target_path += "pixelpart.dll"

elif env["platform"] == "android":
    if host_platform == "windows":
        raise ValueError("Android build not supported by Windows host platform.")

    if not "ANDROID_NDK_ROOT" in env:
        raise ValueError("To build for Android, ANDROID_NDK_ROOT must be defined. Please set ANDROID_NDK_ROOT to the root folder of your Android NDK installation.")

    api_level = int(env["android_api_level"])
    if env["android_arch"] in ["x86_64", "arm64v8"] and api_level < 21:
        print("WARN: 64-bit Android architectures require an API level of at least 21; setting android_api_level=21")
        env["android_api_level"] = "21"
        api_level = 21

    toolchain = env["ANDROID_NDK_ROOT"] + "/toolchains/llvm/prebuilt/"
    if host_platform == "linux":
        toolchain += "linux-x86_64"
    elif host_platform == "macos":
        toolchain += "darwin-x86_64"
    env.PrependENVPath("PATH", toolchain + "/bin")

    arch_info_table = {
        "armv7": {
            "march": "armv7-a",
            "target": "armv7a-linux-androideabi",
            "tool_path": "arm-linux-androideabi",
            "compiler_path": "armv7a-linux-androideabi",
            "ccflags": ["-mfpu=neon"],
        },
        "arm64v8": {
            "march": "armv8-a",
            "target": "aarch64-linux-android",
            "tool_path": "aarch64-linux-android",
            "compiler_path": "aarch64-linux-android",
            "ccflags": [],
        },
        "x86": {
            "march": "i686",
            "target": "i686-linux-android",
            "tool_path": "i686-linux-android",
            "compiler_path": "i686-linux-android",
            "ccflags": ["-mstackrealign"],
        },
        "x86_64": {
            "march": "x86-64",
            "target": "x86_64-linux-android",
            "tool_path": "x86_64-linux-android",
            "compiler_path": "x86_64-linux-android",
            "ccflags": [],
        },
    }
    arch_info = arch_info_table[env["android_arch"]]

    env["CC"] = toolchain + "/bin/clang"
    env["CXX"] = toolchain + "/bin/clang++"
    env["AR"] = toolchain + "/bin/" + arch_info["tool_path"] + "-ar"

    env.Append(CFLAGS=["-DHAVE_UNISTD_H"])
    env.Append(CXXFLAGS=["-std=c++17"])
    env.Append(CCFLAGS=["--target=" + arch_info["target"] + env["android_api_level"], "-march=" + arch_info["march"], "-fPIC"])
    env.Append(CCFLAGS=arch_info["ccflags"])
    env.Append(LINKFLAGS=["--target=" + arch_info["target"] + env["android_api_level"], "-march=" + arch_info["march"]])

    if env["target"] == "debug":
        env.Append(CCFLAGS=["-Og", "-g"])
    elif env["target"] == "release":
        env.Append(CCFLAGS=["-O3"])

    target_path = "Android/" + env["android_arch"] + "/libpixelpart.so"

sources = []

add_sources(sources, "src", ".cpp")
add_sources(sources, "pixelpart-runtime/", ".cpp")
add_sources(sources, "pixelpart-runtime/common", ".cpp")
add_sources(sources, "pixelpart-runtime/computegraph", ".cpp")
add_sources(sources, "pixelpart-runtime/effect", ".cpp")
add_sources(sources, "pixelpart-runtime/engine", ".cpp")
add_sources(sources, "pixelpart-runtime/project", ".cpp")
add_sources(sources, "pixelpart-runtime/shadergraph", ".cpp")
add_sources(sources, "pixelpart-runtime/zlib", ".c")

env.Append(CPPPATH=[
    ".",
    "pixelpart-runtime/"
])

base_path = "pixelpart/Runtime/Plugins/"

if env["platform"] == "macos":
    library = env.LoadableModule(base_path + target_path, source=sources)
elif env["platform"] == "ios":
    library = env.StaticLibrary(base_path + target_path, source=sources)
else:
    library = env.SharedLibrary(base_path + target_path, source=sources)

Default(library)