#!/usr/bin/env python3
import os
import subprocess
import shutil
import random
import argparse
from common import get_sources, run_source, score, compile_all, cpp_compile, problem_name


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Get scores')
    parser.add_argument('people', type=str, nargs='*')
    parser.add_argument('--all', dest='all', action='store_true')
    args = parser.parse_args()
    if not args.all and args.people == []:
        parser.print_help()
        exit(0)
    Sources = get_sources()
    if not args.all:
        Sources = list(set(Sources) & set(args.people))
    for source in Sources:
        cpp_compile(source)

    TESTS = int(input("TESTS = "))
    MAXN = int(input("MAXN = "))
    MAXK = int(input("MAXK = "))
    MAXX = int(input("MAXX = "))
    MAXY = int(input("MAXY = "))
    MAXPART = int(input("MAXPART = "))
    MAXV = int(input("MAXV = "))
    try:
        shutil.rmtree('./tmp', ignore_errors=False)
    except:
        pass
    os.mkdir('./tmp')

    for test in range(1, TESTS + 1):
        print("Case #%d: " % test, end="")

        N = random.randint(1, MAXN)
        K = random.randint(1, min(N, MAXK))
        X = random.randint(1, min(MAXX, N))
        Y = random.randint(1, min(MAXY, N))
        PART = random.randint(1, min(X, Y, MAXPART))
        subprocess.check_call(["./gen", str(N), str(K), str(X), str(Y), str(PART), str(MAXV), str(random.randint(0, 2**32 - 1))], stdout=open("./tmp/"+problem_name()+".in", "w"))

        for source in Sources:
            if run_source(source) == False:
                print("Runtime error on %s" % source)
                exit(1)

        subprocess.check_call(["cp", "./tmp/%s.out" % Sources[0], "./tmp/"+problem_name()+".ok"])

        for source in Sources:
            if score(source) != 10:
                print("Wrong answer: %s" % source)
                exit(1)
        print("OK")
