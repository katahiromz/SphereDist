// SphereDist (equal distance distribution of vertexes on a sphere or a hemisphere).
// Thanks to Atsushi Yamaji (Kyoto Univ.).
// Copyright (C) 2020 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// License: MIT

#include "SphereDist.h"
#include <cstdlib>
#include <cstdio>

#ifndef M_PI
    #define M_PI 3.141592653589793
#endif

inline void vector_add(Tvector3& a, const Tvector3& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}

static int Number_of_points;                    // N
static int NP_on_hemisphere;                    // N/2
static std::vector<TSpherical_Coordinates> GSS; // GSS

static bool GenerateGSS(int N)
{
    int k;
    double hk;
    const int Min_N = 6;
    if (N < Min_N)
    {
        std::fprintf(stderr, "SphereDist: N must be >= %d.\n", Min_N);
        return false;
    }

    Number_of_points = N;
    GSS.resize(N + 1);

    GSS[1].lon = 0;
    GSS[1].col = M_PI;

    for (k = 2; k <= N - 1; ++k)
    {
        hk = -1.0 + 2.0 * (k - 1) / (N - 1);
        GSS[k].col = acos(hk);
        GSS[k].lon = GSS[k - 1].lon + 3.6 / sqrt(N) / sqrt(1.0 - hk * hk);
    }

    GSS[N].lon = 0;
    GSS[N].col = 0;

    return true;
}

static void RelocatePoints(void)
{
    const std::vector<int> five_points = { 0xFFFF, 2, 3, 5, 6, 7 };
    Tvector3 mean;
    int i;

    // relocate 1st point
    mean.x = mean.y = mean.z = 0.0;
    for (i = 1; i <= 5; ++i)
    {
        vector_add(mean, Spherical2Cartesian(GSS[five_points[i]]));
    }
    GSS[1] = Cartesian2Spherical(mean);

    // relocate Nth point
    mean.x = mean.y = mean.z = 0.0;
    for (i = 1; i <= 5; ++i)
    {
        vector_add(mean, Spherical2Cartesian(GSS[Number_of_points - five_points[i] + 1]));
    }
    GSS[Number_of_points] = Cartesian2Spherical(mean);
}

static int GetNP_on_hemisphere(void)
{
    int ith_point;

    NP_on_hemisphere = 0;
    for (ith_point = 1; ith_point <= Number_of_points; ++ith_point)
    {
        if (GSS[ith_point].col <= M_PI / 2.0)
            break;
        ++NP_on_hemisphere;
    }

    return NP_on_hemisphere;
}

static bool
SaveToFile(const char *filename, bool hemisphere)
{
    Tvector3 p;
    int ith_point;
    int last = (hemisphere ? NP_on_hemisphere : Number_of_points);

    bool ret = false;
    if (FILE *fp = fopen(filename, "w"))
    {
        for (ith_point = 1; ith_point <= last; ++ith_point)
        {
            p = Spherical2Cartesian(GSS[ith_point]);
            fprintf(fp, "%g\t%g\t%g\n", p.x, p.y, -p.z);
        }
        fclose(fp);
        ret = true;
    }
    else
    {
        fprintf(stderr, "SphereDist: Cannot open file '%s'.\n", filename);
    }

    return ret;
}

bool SphereDist(std::vector<Tvector3>& vertexes, int N, bool hemisphere, bool relocate)
{
    if (!GenerateGSS(N))
        return false;

    if (relocate)
        RelocatePoints();

    GetNP_on_hemisphere();

    Tvector3 p;
    int ith_point;
    int last = (hemisphere ? NP_on_hemisphere : Number_of_points);
    for (ith_point = 1; ith_point <= last; ++ith_point)
    {
        p = Spherical2Cartesian(GSS[ith_point]);
        vertexes.push_back(p);
    }

    return true;
}

bool SphereDist(const char *filename, int N, bool hemisphere, bool relocate)
{
    if (!GenerateGSS(N))
        return false;

    if (relocate)
        RelocatePoints();

    GetNP_on_hemisphere();

    return SaveToFile(filename, hemisphere);
}
