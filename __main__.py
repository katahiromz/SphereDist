#!/usr/bin/env python3

import __init__ as SphereDist

def show_version():
    print("SphereDist version 0.8.3 by katahiromz")

def show_help():
    print(
        "SphereDist --- Equal distance distribution of vertexes on a sphere or a hemisphere.\n" +
        "Usage: SphereDist [options] [output.tsv]\n" +
        "Options:\n" +
        "  output.tsv     Specify the output file (default: output.tsv)\n" +
        "  --number XXX   Specify the value of N (default: 100)\n" +
        "  --hemisphere   Use a half of sphere\n" +
        "  --no-relocate  Don't relocate\n" +
        "  --help         Show this message\n" +
        "  --version      Show version info\n" +
        "\n" +
        "E-mail: katayama.hirofumi.mz@gmail.com\n")

def main():
    import sys

    hemisphere = False
    no_relocate = False
    N = 100
    filename = ""
    skip = False
    for i in range(1, len(sys.argv)):
        if skip:
            skip = False
            continue
        arg = sys.argv[i]
        if arg == "--help":
            show_help()
            return 0
        if arg == "--version":
            show_version()
            return 0
        if arg == "--hemisphere":
            hemisphere = True
            continue
        if arg == "--no-relocate":
            no_relocate = True
            continue
        if arg == "--number":
            N = int(sys.argv[i + 1])
            skip = True
            continue
        if arg[0] == "-":
            print("SphereDist: Invalid argument '", arg, "'.")
            return -1
        if len(filename) == 0:
            filename = arg
        else:
            printf("SphereDist: Too many arguments.")
            return -2
    if len(filename) == 0:
        filename = "output.tsv"
    sd = SphereDist.dist(N, hemisphere, not no_relocate)
    sd.save(filename)
    return 0

main()
