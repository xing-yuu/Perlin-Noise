#pragma once
//#include "stdafx.h"
//#include "globalvariable.h"
#include <math.h>
//#include "basictexture.h"
#include <random>
#include <functional>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

#define PI 3.14159265



struct point2D {
    float x;
    float y;
};

template <typename T>
class Vec20 {
public:
    Vec20() : x(T(0)), y(T(0)) {}
    Vec20(T xx, T yy) : x(xx), y(yy) {}
    Vec20 operator * (const T& r) const { return Vec20(x * r, y * r); }
    T x, y;
};

typedef Vec20<float> Vec20f;
class ValueNoise {
public:
    ValueNoise(unsigned seed = 2016) {
        mt19937 gen(seed);
        uniform_real_distribution<float> distrFloat;
        auto randFloat = bind(distrFloat, gen);

        for (unsigned k = 0; k < kMaxTableSize; k++) {
            r[k] = randFloat();
            pTable[k] = k;
        }

        // shuffle values of the permutation pTable
        uniform_int_distribution<int> distUInt;
        auto randUInt = bind(distUInt, gen);
        for (size_t k = 0; k < kMaxTableSize; k++) {
            unsigned i = randUInt() & kMaxTableSizeMask;
            std::swap(pTable[k], pTable[i]);
            pTable[k + kMaxTableSize] = pTable[k];
        }
    }
    float lerpp(float lo, float hi, float t)
    {
        return lo * (1 - t) + hi * t;
    }
    float smoothstepp(float t) { return t * t * (3 - 2 * t); }
    float eval(Vec20f p)
    {
        int xi = std::floor(p.x);
        int yi = std::floor(p.y);

        float tx = p.x - xi;
        float ty = p.y - yi;

        int rx0 = xi & kMaxTableSizeMask;
        int rx1 = (rx0 + 1) & kMaxTableSizeMask;
        int ry0 = yi & kMaxTableSizeMask;
        int ry1 = (ry0 + 1) & kMaxTableSizeMask;


        // four corner (rx0, ry0), (rx1, ry0), (rx0, ry1), (rx1, ry1)
        float c00 = r[pTable[pTable[rx0]] + ry0];
        float c10 = r[pTable[pTable[rx1]] + ry0];
        float c01 = r[pTable[pTable[rx0]] + ry1];
        float c11 = r[pTable[pTable[rx1]] + ry1];

        float sx = smoothstepp(tx);
        float sy = smoothstepp(ty);

        //
        float nx0 = lerpp(c00, c10, sx);
        float nx1 = lerpp(c01, c11, sx);

        return lerpp(nx0, nx1, sy);
    }

    static const unsigned kMaxTableSize = 256;
    static const unsigned kMaxTableSizeMask = kMaxTableSize - 1;
    float r[kMaxTableSize];
    unsigned pTable[kMaxTableSize * 2];
    //	float value[2000 * 2000];
    float fractalNoise(Vec20f x) {


        float noiseSum = 0;
        static const unsigned numLayers = 5;

        float frequencyMult = 1.8;
        float amplitudeMult = 0.35;
        float frequency = 1.0;
        float amplitude = 1.0;

        for (unsigned i = 0; i < numLayers; i++) {
            noiseSum += eval(x * frequency) * amplitude;
            frequency *= frequencyMult;
            amplitude *= amplitudeMult;
        }

        return noiseSum;
    }
};




class Perlin
{
public:
    Perlin();
    ~Perlin();

    double Noise(int x, int y);
    double SmoothedNoise(int x, int y);
    double Cosine_Interpolate(double a, double b, double x);
    double InterpolatedNoise(float x, float y);
    double PerlinNoise(float x, float y);

   

    float lerp(float a0, float a1, float w);
    // Computes the dot product of the distance and gradient vectors.
    float dotGridGradient(int ix, int iy, float x, float y);
    // Compute Perlin noise at coordinates x, y
    float perlin(float x, float y);

    float min(float v1, float v2) { return v1 < v2 ? v1 : v2; }
    float max(float v1, float v2) { return v1 > v2 ? v1 : v2; }

    float getLength(point2D vStart, point2D vEnd);

    vector<vector<int>>* getPerlinNoise(int row, int columns, int type = 0);



private:
    float Gradient[1000][1000][2];
    vector<vector<int>>* result;
    ValueNoise u1;
};