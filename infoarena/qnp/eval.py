#!/usr/bin/env python3
import os
import subprocess
import shutil
import random
import argparse
from common import get_sources, run_source, check_diff, compile_all, cpp_compile

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Get scores')
    parser.add_argument('people', type=str, nargs='*')
    parser.add_argument('--all', dest='all', action='store_true')
    args = parser.parse_args()
    if not args.all and args.people == []:
        parser.print_help()
        exit(0)
    SOURCES = get_sources()
    if not args.all:
        SOURCES = list(set(SOURCES) & set(args.people))
    for source in SOURCES:
        cpp_compile(source)

    TESTS = int(input("TESTS = "))
    MAXM = int(input("MAXM = "))
    MAXDIGIT = int(input("MAXDIGIT = "))
    MAXK = int(input("MAXK = "))
    P = float(input("P = "))
    try:
        shutil.rmtree('./tmp', ignore_errors=False)
    except:
        pass
    os.mkdir('./tmp')

    for test in range(1, TESTS + 1):
        print("Case #%d: " % test, end="")

        M = random.randint(1, MAXM)
        if test == TESTS:
            M = MAXM

        subprocess.check_call(["./generator", str(M), str(MAXDIGIT), str(MAXK), str(P), str(random.randint(0, 2**32 - 1))], stdout=open("./tmp/qnp.in", "w"))

        for source in SOURCES:
            if run_source(source) == False:
                print("Runtime error on %s" % source)
                exit(1)

        for source in SOURCES[1:]:
            if check_diff(SOURCES[0], source):
                print("Error %s and %s differ, check tmp folder" % (SOURCES[0], source))
                exit(1)
        print("OK")
