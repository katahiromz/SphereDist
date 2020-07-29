// SphereDist (equal distance distribution of vertexes on a sphere or a hemisphere).
// Thanks to Atsushi Yamaji (Kyoto Univ.).
// Copyright (C) 2020 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// License: MIT

#include "SphereDist.h"
#include <string>
#include <cstdlib>
#include <cstdio>

static void show_version(void)
{
    std::printf("SphereDist version 0.8.3 by katahiromz\n");
}

static void show_help(void)
{
    std::printf(
        "SphereDist --- Equal distance distribution of vertexes on a sphere or a hemisphere.\n"
        "Usage: SphereDist [options] [output.tsv]\n"
        "Options:\n"
        "  output.tsv     Specify the output file (default: output.tsv)\n"
        "  --number XXX   Specify the value of N (default: 100)\n"
        "  --hemisphere   Use a half of sphere\n"
        "  --no-relocate  Don't relocate\n"
        "  --help         Show this message\n"
        "  --version      Show version info\n"
        "\n"
        "E-mail: katayama.hirofumi.mz@gmail.com\n");
}

int main(int argc, char **argv)
{
    bool hemisphere = false, no_relocate = false;
    int N = 100;
    std::string filename;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--help")
        {
            show_help();
            return 0;
        }
        if (arg == "--version")
        {
            show_version();
            return 0;
        }
        if (arg == "--hemisphere")
        {
            hemisphere = true;
            continue;
        }
        if (arg == "--no-relocate")
        {
            no_relocate = true;
            continue;
        }
        if (arg == "--number")
        {
            ++i;
            N = std::atoi(argv[i]);
            continue;
        }
        if (arg[0] == '-')
        {
            std::fprintf(stderr, "SphereDist: Invalid argument '%s'.\n", arg.c_str());
            return -1;
        }
        if (filename.empty())
        {
            filename = arg;
        }
        else
        {
            std::fprintf(stderr, "SphereDist: Too many arguments.\n");
            return -2;
        }
    }

    if (filename.empty())
    {
        filename = "output.tsv";
    }

    if (SphereDist(filename.c_str(), N, hemisphere, !no_relocate))
        return 0;

    return -4;
}
