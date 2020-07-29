#!/usr/bin/env python3
# ex)
# >>> import SphereDist as dist
# >>> sd = dist.dist(20)
# >>> sd.save("output.tsv")

import math

# in radians
class TSpherical_Coordinates:
    def __init__(self):
        self.col = 0.0 # colatitude
        self.lon = 0.0 # longitude

class Tvector2:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0

class Tvector3:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0
    def add(self, another):
        self.x += another.x
        self.y += another.y
        self.z += another.z

def sqr(x):
    return x * x

def Spherical2Cartesian(p):
    ret = Tvector3()
    ret.x = math.sin(p.col) * math.cos(p.lon)
    ret.y = math.sin(p.col) * math.sin(p.lon)
    ret.z = math.cos(p.col)
    return ret

def Cartesian2Spherical(p):
    ret = TSpherical_Coordinates()
    ret.col = math.acos(p.z / math.sqrt(sqr(p.x) + sqr(p.y) + sqr(p.z)))
    ret.lon = math.atan2(p.y, p.x)
    return ret

class dist:
    def __init__(self, N = 100, hemisphere = False, relocating = True):
        self.N = N
        self.hemisphere = hemisphere
        self.relocating = relocating
        self.generate()
        self.NP_on_hemisphere = N / 2

    def generate(self):
        if self.N < 6:
            raise "N must be >= 6"
        self.GSS = []
        for i in range(0, self.N + 2):
            self.GSS.append(TSpherical_Coordinates())
        self.GSS[1].lon = 0.0
        self.GSS[1].col = math.pi
        for k in range(2, self.N):
            hk = -1.0 + 2.0 * (k - 1) / (self.N - 1)
            self.GSS[k].col = math.acos(hk)
            self.GSS[k].lon = self.GSS[k - 1].lon + 3.6 / math.sqrt(self.N) / math.sqrt(1.0 - hk * hk)
        self.GSS[self.N].lon = self.GSS[self.N].col = 0
        if self.relocating:
            self.relocate()
        self.NP_on_hemisphere = 0
        if self.hemisphere:
            for ith_point in range(1, self.N + 1):
                if self.GSS[ith_point].col <= math.pi / 2.0:
                    break
                self.NP_on_hemisphere += 1

    def relocate(self):
        mean = Tvector3()
        five_points = [0xFFFF, 2, 3, 5, 6, 7]

        # relocate 1st point
        mean = Tvector3()
        for i in range(1, 6):
            mean.add(Spherical2Cartesian(self.GSS[five_points[i]]))
        self.GSS[1] = Cartesian2Spherical(mean)

        # relocate Nth point
        mean = Tvector3()
        for i in range(1, 6):
            mean.add(Spherical2Cartesian(self.GSS[self.N - five_points[i] + 1]))
        self.GSS[self.N] = Cartesian2Spherical(mean)

    def rows(self):
        vertexes = []
        last = self.NP_on_hemisphere if self.hemisphere else self.N
        for ith_point in range(1, last + 1):
            p = Spherical2Cartesian(self.GSS[ith_point])
            vertexes.append([p.x, p.y, -p.z])
        return vertexes

    def save(self, filename):
        import csv
        rows = self.rows()
        with open(filename, 'w') as file:
            for row in rows:
                file.write(f'{row[0]:g}\t{row[1]:g}\t{row[2]:g}\n')
