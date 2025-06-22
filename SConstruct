#!/usr/bin/env python

import os
import platform
import sys
from SCons import __version__ as scons_raw_version
from SCons.Script import ARGUMENTS
from SCons.Tool import Tool
from SCons.Variables import EnumVariable

def add_sources(sources, directory, extension):
    for file in os.listdir(directory):
        if file.endswith(extension):
            sources.append(directory + '/' + file)

EnsureSConsVersion(4, 0)
EnsurePythonVersion(3, 8)

platforms = [
    "linux",
    "macos",
    "windows",
    "web"
]
architectures = [
    "",
    "universal",
    "x86_32",
    "x86_64",
    "arm32",
    "arm64",
    "wasm32"
]
architecture_aliases = {
    "x64": "x86_64",
    "amd64": "x86_64",
    "armv7": "arm32",
    "armv8": "arm64",
    "arm64v8": "arm64",
    "aarch64": "arm64"
}

env = Environment(tools=["default"], PLATFORM="")
env.PrependENVPath("PATH", os.getenv("PATH"))

# Try to detect the host platform automatically
if sys.platform.startswith("linux"):
    default_platform = "linux"
elif sys.platform == "darwin":
    default_platform = "macos"
elif sys.platform == "win32" or sys.platform == "msys":
    default_platform = "windows"
elif ARGUMENTS.get("platform", ""):
    default_platform = ARGUMENTS.get("platform")
else:
    raise ValueError("Could not detect platform automatically, please specify with platform=<platform>")

opts = Variables(args=ARGUMENTS)
opts.Add(EnumVariable(
    key="platform",
    help="Target platform",
    default=env.get("platform", default_platform),
    allowed_values=platforms,
    ignorecase=2))
opts.Add(EnumVariable(
    key="arch",
    help="CPU architecture",
    default=env.get("arch", ""),
    allowed_values=architectures,
    map=architecture_aliases))

#opts.Add(BoolVariable(key="threads", help="Enable threading support", default=env.get("threads", True)))

# Add platform options
for pl in sorted(set(platforms)):
    tl = Tool(pl, toolpath=["tools"])
    if hasattr(tl, "options"):
        tl.options(opts)

opts.Update(env)

Help(opts.GenerateHelpText(env))

unknown = opts.UnknownVariables()
if unknown:
    print("WARNING: Unknown SCons variables were passed and will be ignored:")
    for item in unknown.items():
        print("    " + item[0] + "=" + item[1])

scons_cache_path = os.environ.get("SCONS_CACHE")
if scons_cache_path is not None:
    CacheDir(scons_cache_path)
    Decider("MD5")

env.scons_version = env._get_major_minor_revision(scons_raw_version)

# CPU architecture
if env["arch"] == "":
    if env["platform"] in ["macos", "ios"]:
        env["arch"] = "universal"
    elif env["platform"] == "android":
        env["arch"] = "arm64"
    elif env["platform"] == "web":
        env["arch"] = "wasm32"
    else:
        host_machine = platform.machine().lower()
        if host_machine in architectures:
            env["arch"] = host_machine
        elif host_machine in architecture_aliases.keys():
            env["arch"] = architecture_aliases[host_machine]
        elif "86" in host_machine:
            env["arch"] = "x86_32"
        else:
            print("Unsupported CPU architecture: " + host_machine)
            env.Exit(1)

print("Building for " + env["platform"] + "/" + env["arch"])

tool = Tool(env["platform"], toolpath=["tools"])
if tool is None:
    raise ValueError("Required toolchain not found for platform " + env["platform"])

tool.generate(env)

# Sources
sources = []
add_sources(sources, "src", ".cpp")
add_sources(sources, "pixelpart-runtime/asset", ".cpp")
add_sources(sources, "pixelpart-runtime/common", ".cpp")
add_sources(sources, "pixelpart-runtime/computegraph", ".cpp")
add_sources(sources, "pixelpart-runtime/effect", ".cpp")
add_sources(sources, "pixelpart-runtime/engine", ".cpp")
add_sources(sources, "pixelpart-runtime/shadergraph", ".cpp")
add_sources(sources, "pixelpart-runtime/zlib", ".c")

# Includes
env.Append(CPPPATH=["."])

# Library path
library_path = "pixelpart/Runtime/Plugins/"

if env["platform"] == "linux":
    if env["arch"] == "x86_64":
        library_path += "x86_64/libpixelpart.so"
    elif env["arch"] == "x86_32":
        library_path += "x86/libpixelpart.so"
elif env["platform"] == "windows":
    if env["arch"] == "x86_64":
        library_path += "x86_64/pixelpart.dll"
    elif env["arch"] == "x86_32":
        library_path += "x86/pixelpart.dll"
elif env["platform"] == "macos":
    library_path += "macOS/pixelpart.bundle"
elif env["platform"] == "web":
    library_path += "WebGL/libpixelpart.a"

if env["platform"] == "macos":
    library = env.LoadableModule(library_path, source=sources)
elif env["platform"] == "web":
    library = env.StaticLibrary(library_path, source=sources)
else:
    library = env.SharedLibrary(library_path, source=sources)

env.NoCache(library)
Default(library)