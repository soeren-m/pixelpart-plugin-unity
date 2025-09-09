import sys
import subprocess

if "platform=linux" in sys.argv:
    for arch in ["x86_64", "x86_32"]:
        subprocess.run("scons platform=linux arch=" + arch, shell=True, check=True)

elif "platform=windows" in sys.argv:
    for arch in ["x86_64", "x86_32"]:
        subprocess.run("scons platform=windows arch=" + arch, shell=True, check=True)

elif "platform=macos" in sys.argv:
    subprocess.run("scons platform=macos arch=universal", shell=True, check=True)

elif "platform=web" in sys.argv:
    subprocess.run("scons platform=web arch=wasm32", shell=True, check=True)
