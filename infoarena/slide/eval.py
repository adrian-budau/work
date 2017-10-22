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
    MAXFILL = int(input("MAXFILL = "))
    MAXEMPTY = int(input("MAXEMPTY = "))
    MAXPARTS = int(input("MAXPARTS = "))
    MAXZERO = int(input("MAXZERO = "))

    try:
        shutil.rmtree('./tmp', ignore_errors=False)
    except:
        pass
    os.mkdir('./tmp')

    for test in range(1, TESTS + 1):
        print("Case #%d" % test, end="")

        FILL = random.randint(1, MAXFILL)
        EMPTY = random.randint(0, MAXEMPTY)
        PARTS = random.randint(1, min(MAXPARTS, FILL))
        ZERO = random.randint(0, min(PARTS - 1, MAXZERO))
        if test == TESTS:
            FILL, EMPTY, PARTS, ZERO = MAXFILL, MAXEMPTY, MAXPARTS, MAXZERO
        subprocess.check_call(["./gen", str(FILL), str(EMPTY), str(PARTS), str(ZERO)], stdout=open("./tmp/"+problem_name()+".in", "w"))
        print("(FILL = %d, EMPTY = %d, PARTS = %d, ZERO = %d): " % (FILL, EMPTY, PARTS, ZERO), end="")
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
