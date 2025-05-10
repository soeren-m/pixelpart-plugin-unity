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

elif "platform=macos" in sys.argv:
    subprocess.run("scons platform=macos bits=64 target=release", shell = True)

elif "platform=android" in sys.argv:
    subprocess.run("scons platform=android android_arch=armv7 target=release", shell = True)
    subprocess.run("scons platform=android android_arch=arm64v8 target=release", shell = True)
    subprocess.run("scons platform=android android_arch=x86 target=release", shell = True)
    subprocess.run("scons platform=android android_arch=x86_64 target=release", shell = True)

elif "platform=ios" in sys.argv:
    subprocess.run("scons platform=ios ios_arch=arm64 target=release", shell = True)
    subprocess.run("lipo -create pixelpart/Runtime/Plugins/iOS/libpixelpart.arm64.a -output pixelpart/Runtime/Plugins/iOS/libpixelpart.a", shell = True)

elif "platform=web" in sys.argv:
    def filter_files(dir, files):
        return [f for f in files if
            os.path.isfile(os.path.join(dir, f)) and not (
                f.endswith(".h") or
                f.endswith(".hpp") or
                f.endswith(".c") or
                f.endswith(".cpp") or
                f.endswith(".inl"))
            or
            # zlib fix, glm fix
            os.path.isfile(os.path.join(dir, f)) and (
                "zlib" in dir and "gz" in f and f.endswith(".c") or
                "glm" in dir and "glm.cpp" in f)
        ]

    shutil.copytree("src", "pixelpart/Runtime/Plugins/WebGL", ignore=filter_files, dirs_exist_ok=True)
    shutil.copytree("pixelpart-runtime", "pixelpart/Runtime/Plugins/WebGL/pixelpart-runtime", ignore=filter_files, dirs_exist_ok=True)