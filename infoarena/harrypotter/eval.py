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
    MAXK1 = int(input("MAXK1 = "))
    MAXK2 = int(input("MAXK2 = "))

    try:
        shutil.rmtree('./tmp', ignore_errors=False)
    except:
        pass
    os.mkdir('./tmp')

    for test in range(1, TESTS + 1):
        print("Case #%d" % test, end="")

        N = random.randint(1, MAXN)
        K1 = random.randint(1, min(MAXK1, N))
        K2 = random.randint(1, min(MAXK2, N))
        if test == TESTS:
            N, K1, K2 = MAXN, MAXK1, MAXK2
        subprocess.check_call(["./gen", str(N), str(K1), str(K2)], stdout=open("./tmp/"+problem_name()+".in", "w"))
        print("(N = %d, K1 = %d, K2 = %d): " % (N, K1, K2), end="")
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
