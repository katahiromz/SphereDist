// SphereDist (equal distance distribution of vertexes on a sphere or a hemisphere).
// Thanks to Atsushi Yamaji (Kyoto Univ.).
// Copyright (C) 2020 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// License: MIT

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

struct TSpherical_Coordinates {
    double col; // colatitude
    double lon; // longitude
}; // in radians

struct Tvector2 {
    double x, y;
};

struct Tvector3 {
    double x, y, z;
};

inline double sqr(double x)
{
    return x * x;
}

inline Tvector3
Spherical2Cartesian(const TSpherical_Coordinates & p)
{
    Tvector3 ret;
    ret.x = sin(p.col) * cos(p.lon);
    ret.y = sin(p.col) * sin(p.lon);
    ret.z = cos(p.col);
    return ret;
}

inline TSpherical_Coordinates
Cartesian2Spherical(const Tvector3& p)
{
    TSpherical_Coordinates ret;
    ret.col = acos(p.z / sqrt(sqr(p.x) + sqr(p.y) + sqr(p.z)));
    ret.lon = atan2(p.y, p.x);
    return ret;
}

bool SphereDist(std::vector<Tvector3>& vertexes, int N = 100,
                bool hemisphere = false, bool relocate = true);
bool SphereDist(const char *filename, int N = 100,
                bool hemisphere = false, bool relocate = true);
