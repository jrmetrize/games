# Build the dependencies
import platform
import os
import subprocess

root_dir = os.getcwd()

def get_source_dir(dep_name):
    return root_dir + "/deps/" + dep_name

def get_build_dir(dep_name = None):
    if dep_name != None:
        return root_dir + "/deps/build/" + dep_name
    else:
        return root_dir + "/deps/build"

def make_build_dir(dep_name):
    if not os.path.exists(root_dir + "/deps/build"):
        os.mkdir(root_dir + "/deps/build")
    if not os.path.exists(get_build_dir(dep_name)):
        os.mkdir(get_build_dir(dep_name))

def build_cmake(dep_name):
    make_build_dir(dep_name)

    cmd_result = subprocess.run(['cmake',
        '-S', get_source_dir(dep_name),
        '-B', get_build_dir(dep_name)],
        capture_output=True)
    if cmd_result.returncode != 0:
        print("CMake generation failed.")
        print(cmd_result.stderr)
        return -1

    cmd_result = subprocess.run(['cmake',
        '--build', get_build_dir(dep_name)],
        capture_output=True)
    if cmd_result.returncode != 0:
        print("Build failed.")
        print(cmd_result.stderr)
        return -1

    cmd_result = subprocess.run(['cmake',
        '--install', get_build_dir(dep_name),
        '--prefix', get_build_dir()],
        capture_output=True)
    if cmd_result.returncode != 0:
        print("Install failed.")
        print(cmd_result.stderr)
        return -1

    return 0

def build_freetype():
    print("Building dependency 'freetype'")
    return build_cmake("freetype")

def build_glfw():
    print("Building dependency 'glfw'")
    return build_cmake("glfw")

build_cmds = [
    build_freetype,
    build_glfw
]
for cmd in build_cmds:
    cmd_result = cmd()
    if cmd_result != 0:
        print("Stopping.")
        exit()
