import common

def exists(env):
    return True

def generate(env):
    env.Append(CCFLAGS=["-fPIC", "-Wwrite-strings"])
    env.Append(LINKFLAGS=["-Wl,-R,'$$ORIGIN'", "-Wl,--no-undefined"])

    # Architecture
    if env["arch"] == "x86_64":
        env.Append(CCFLAGS=["-m64", "-march=x86-64"])
        env.Append(LINKFLAGS=["-m64", "-march=x86-64"])
    elif env["arch"] == "x86_32":
        env.Append(CCFLAGS=["-m32", "-march=i686"])
        env.Append(LINKFLAGS=["-m32", "-march=i686"])
    elif env["arch"] == "arm64":
        env.Append(CCFLAGS=["-march=armv8-a"])
        env.Append(LINKFLAGS=["-march=armv8-a"])

    # Link libgcc and libstdc++ statically
    env.Append(LINKFLAGS=["-static-libgcc", "-static-libstdc++"])

    # zlib fix
    env.Append(CFLAGS=["-DHAVE_UNISTD_H"])

    # Options for pixelpart-runtime
    env.Append(CPPDEFINES=["PIXELPART_RUNTIME_MULTITHREADING"])

    common.generate(env)
