import common

def exists(env):
    return True

def generate(env):
    if env["arch"] not in ("wasm32"):
        print("Only wasm32 supported on web. Exiting.")
        env.Exit(1)

    # Emscripten toolchain
    env["CC"] = "emcc"
    env["CXX"] = "em++"
    env["AR"] = "emar"
    env["RANLIB"] = "emranlib"

    # Use TempFileMunge since some AR invocations are too long for cmd.exe.
    # Use POSIX-style paths, required with TempFileMunge.
    env["ARCOM_POSIX"] = env["ARCOM"].replace("$TARGET", "$TARGET.posix").replace("$SOURCES", "$SOURCES.posix")
    env["ARCOM"] = "${TEMPFILE(ARCOM_POSIX)}"

    env["OBJSUFFIX"] = ".o"
    env["SHOBJSUFFIX"] = ".o"
    env["LIBPREFIX"] = "lib"
    env["LIBSUFFIX"] = ".a"
    env["SHLIBSUFFIX"] = ".wasm"

    common.generate(env)
