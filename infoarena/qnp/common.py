import os
import subprocess
import shutil

def get_sources():
    return [entry.name[:-4] for entry in os.scandir('./sources') if entry.is_file() and entry.name[-4:] == '.cpp']

def cpp_compile(source):
    print("Compiling %s ... " % source, end="")
    try:
        subprocess.check_output(["g++", "-O2", "-Wall", "-std=c++11", "./sources/" + source + ".cpp", "-o", "./sources/" + source, "-lm"], stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        print("Compile error:")
        print(e.output)
    print("Done")

def compile_all():
     for source in get_sources():
        cpp_compile(source)

def run_source(source):
    try:
        shutil.rmtree('./tmp/current')
    except:
        pass
    os.mkdir("./tmp/current")
    subprocess.check_call(["cp", "./sources/" + source, "./tmp/current/" + source])
    subprocess.check_call(["cp", "./tmp/qnp.in", "./tmp/current/qnp.in"])
    os.chdir("./tmp/current")
    try:
        subprocess.check_call(["./" + source], timeout=2)
    except:
        os.chdir("../../")
        return False
    os.chdir("../../")
    subprocess.check_call(["cp", "./tmp/current/qnp.out", "./tmp/" + source + ".out"])
    #shutil.rmtree("./tmp/current")
    return True

def check_diff(source1, source2):
    return subprocess.call(["diff", "-w", "./tmp/" + source1 + ".out", "./tmp/" + source2 + ".out"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

