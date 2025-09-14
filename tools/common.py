import os
import subprocess

def using_clang(env):
    return "clang" in os.path.basename(env["CC"])

def using_vanilla_clang(env):
    if not using_clang(env):
        return False

    try:
        version = subprocess.check_output([env.subst(env["CXX"]), "--version"]).strip().decode("utf-8")
    except (subprocess.CalledProcessError, OSError):
        print("Could not parse CXX environment variable to infer compiler version.")
        return False

    return not version.startswith("Apple")

def exists(env):
    return True

def generate(env):
    # C++17
    if env.get("is_msvc", False):
        env.Append(CXXFLAGS=["/std:c++17"])
    else:
        env.Append(CXXFLAGS=["-std=c++17"])

    # Exceptions
    if env.get("is_msvc", False):
        env.Append(CXXFLAGS=["/EHsc"])

    # Optimization
    if env.get("is_msvc", False):
        env.Append(CCFLAGS=["/O2"])
        env.Append(LINKFLAGS=["/OPT:REF"])
    else:
        env.Append(CCFLAGS=["-O2"])
        if using_clang(env) and not using_vanilla_clang(env) and not env["use_mingw"]:
            env.Append(LINKFLAGS=["-Wl,-S", "-Wl,-x", "-Wl,-dead_strip"])
        else:
            env.Append(LINKFLAGS=["-s"])
