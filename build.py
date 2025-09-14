import sys
import subprocess

if "platform=windows" in sys.argv:
    for arch in ["x86_64", "x86_32"]:
        subprocess.run("scons platform=windows arch=" + arch, shell=True, check=True)

elif "platform=linux" in sys.argv:
    for arch in ["x86_64", "x86_32"]:
        subprocess.run("scons platform=linux arch=" + arch, shell=True, check=True)

elif "platform=macos" in sys.argv:
    subprocess.run("scons platform=macos arch=universal", shell=True, check=True)

elif "platform=ios" in sys.argv:
    subprocess.run("scons platform=ios arch=x86_64 ios_simulator=yes", shell=True, check=True)
    subprocess.run("scons platform=ios arch=arm64 ios_simulator=no", shell=True, check=True)
    subprocess.run("lipo -create " +
        "net.pixelpart.core/Runtime/Plugins/iOS/libpixelpart-phone.a " +
        "net.pixelpart.core/Runtime/Plugins/iOS/libpixelpart-simulator.a " +
        "-output net.pixelpart.core/Runtime/Plugins/iOS/libpixelpart.a", shell=True, check=True)

elif "platform=android" in sys.argv:
    for arch in ["arm64", "arm32", "x86_64", "x86_32"]:
        subprocess.run("scons platform=android arch=" + arch, shell=True, check=True)

elif "platform=web" in sys.argv:
    subprocess.run("scons platform=web arch=wasm32", shell=True, check=True)
