import os

def using_clang(env):
    return "clang" in os.path.basename(env["CC"])

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
        if using_clang(env):
            env.Append(LINKFLAGS=["-Wl,-S", "-Wl,-x", "-Wl,-dead_strip"])
        else:
            env.Append(LINKFLAGS=["-s"])
