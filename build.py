import sys
import subprocess
import os
import shutil

if "platform=windows" in sys.argv:
    subprocess.run("scons platform=windows bits=32 target=release", shell = True)
    subprocess.run("scons platform=windows bits=64 target=release", shell = True)

elif "platform=linux" in sys.argv:
    subprocess.run("scons platform=linux bits=32 target=release", shell = True)
    subprocess.run("scons platform=linux bits=64 target=release", shell = True)

elif "platform=osx" in sys.argv:
    subprocess.run("scons platform=osx bits=64 target=release", shell = True)

elif "platform=android" in sys.argv:
    subprocess.run("scons platform=android android_arch=armv7 target=release", shell = True)
    subprocess.run("scons platform=android android_arch=arm64v8 target=release", shell = True)
    subprocess.run("scons platform=android android_arch=x86 target=release", shell = True)
    subprocess.run("scons platform=android android_arch=x86_64 target=release", shell = True)

elif "platform=ios" in sys.argv:
    subprocess.run("scons platform=ios ios_arch=arm64 target=release", shell = True)
    subprocess.run("lipo -create pixelpart-plugin/Assets/Pixelpart/Plugins/iOS/libpixelpart.arm64.a -output pixelpart-plugin/Assets/Pixelpart/Plugins/iOS/libpixelpart.a", shell = True)

elif "platform=javascript" in sys.argv:
    def filter_files(dir, files):
        return [f for f in files if
            os.path.isfile(os.path.join(dir, f)) and not (
                f.endswith(".h") or
                f.endswith(".hpp") or
                f.endswith(".c") or
                f.endswith(".cpp") or
                f.endswith(".inl"))
            or
            os.path.isfile(os.path.join(dir, f)) and (
                "zlib" in dir and "gz" in f and f.endswith(".c")) or
                "glm" in dir and "glm.cpp" in f
            or
            os.path.isdir(os.path.join(dir, f)) and (
                "test" in f or
                "doc" in f or
                "util" in f or
                "contrib" in f or
                "examples" in f or
                "benchmarks" in f or
                "third_party" in f or
                "cmake" in f or
                "amiga" in f or
                "msdos" in f or
                "nintendods" in f or
                "old" in f or
                "os400" in f or
                "qnx" in f or
                "watcom" in f or
                "win32" in f)
            ]

    shutil.copytree("src", "pixelpart-plugin/Assets/Pixelpart/Plugins/WebGL", ignore=filter_files, dirs_exist_ok=True)
    shutil.copytree("pixelpart-runtime", "pixelpart-plugin/Assets/Pixelpart/Plugins/WebGL", ignore=filter_files, dirs_exist_ok=True)