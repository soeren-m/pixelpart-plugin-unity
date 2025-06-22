import os
import sys
import common
import my_spawn
from SCons.Tool import mingw, msvc
from SCons.Variables import BoolVariable

def exists(env):
    return True

def options(opts):
    mingw = os.getenv("MINGW_PREFIX", "")

    opts.Add(BoolVariable("use_mingw", "Use the MinGW compiler instead of MSVC - only effective on Windows", False))
    opts.Add("mingw_prefix", "MinGW prefix", mingw)

def generate(env):
    # Options for pixelpart-runtime
    env.Append(CPPDEFINES=["PIXELPART_RUNTIME_MULTITHREADING"])

    if not env["use_mingw"] and msvc.exists(env):
        # Architecture
        if env["arch"] == "x86_64":
            env["TARGET_ARCH"] = "amd64"
        elif env["arch"] == "arm64":
            env["TARGET_ARCH"] = "arm64"
        elif env["arch"] == "arm32":
            env["TARGET_ARCH"] = "arm"
        elif env["arch"] == "x86_32":
            env["TARGET_ARCH"] = "x86"

        env["MSVC_SETUP_RUN"] = False
        env["MSVS_VERSION"] = None
        env["MSVC_VERSION"] = None

        env["is_msvc"] = True

        msvc.generate(env)
        env.Tool("msvc")
        env.Tool("mslib")
        env.Tool("mslink")

        env.Append(CCFLAGS=["/utf-8"])
        env.Append(LINKFLAGS=["/WX"])

        # Link C++ runtime statically
        env.AppendUnique(CCFLAGS=["/MT"])

    elif (sys.platform == "win32" or sys.platform == "msys") and not env["mingw_prefix"]:
        env["use_mingw"] = True

        mingw.generate(env)
        env["IMPLIBPREFIX"] = ""
        env["SHLIBPREFIX"] = ""
        env["SHLIBSUFFIX"] = ".dll"

        env.Append(CCFLAGS=["-Wwrite-strings"])
        env.Append(LINKFLAGS=["-Wl,--no-undefined"])

        # Link libgcc and libstdc++ statically
        env.Append(LINKFLAGS=["-static", "-static-libgcc", "-static-libstdc++"])

        # Long line hack
        my_spawn.configure(env)

    else:
        # Cross-compilation using MinGW
        env["use_mingw"] = True

        prefix = ""
        if env["mingw_prefix"]:
            prefix = env["mingw_prefix"] + "/bin/"

        # Architecture
        if env["arch"] == "x86_64":
            prefix += "x86_64"
        elif env["arch"] == "arm64":
            prefix += "aarch64"
        elif env["arch"] == "arm32":
            prefix += "armv7"
        elif env["arch"] == "x86_32":
            prefix += "i686"

        env["CXX"] = prefix + "-w64-mingw32-g++"
        env["CC"] = prefix + "-w64-mingw32-gcc"
        env["AR"] = prefix + "-w64-mingw32-gcc-ar"
        env["RANLIB"] = prefix + "-w64-mingw32-ranlib"
        env["LINK"] = prefix + "-w64-mingw32-g++"
        env["SHLIBSUFFIX"] = ".dll"

        env.Append(CCFLAGS=["-Wwrite-strings"])
        env.Append(LINKFLAGS=["-Wl,--no-undefined"])

        # Link libgcc and libstdc++ statically
        env.Append(LINKFLAGS=["-static", "-static-libgcc", "-static-libstdc++"])

        if sys.platform == "win32" or sys.platform == "msys":
            my_spawn.configure(env)

    common.generate(env)