#pragma once

#include <math.h>
//#include "basictexture.h"
#include <random>
#include <functional>
#include <iostream>
#include <cmath>
#include <vector>
#include "SimplexNoise.h"
#define PI 3.14159265
using namespace std;

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
    int nowseed;
    vector<vector<int>>* result;
    ValueNoise(unsigned seed = 2016) {
        mt19937 gen(seed);
        result = new vector<vector<int>>;
        uniform_real_distribution<float> distrFloat;
        auto randFloat = bind(distrFloat, gen);
        srand((unsigned)time(NULL));
        nowseed = rand();
        for (unsigned k = 0; k < kMaxTableSize; k++) {
            // r[k] = randFloat();
            r[k] = (rand() % 1000) / 1000.0;
            pTable[k] = k;
        }
        uniform_int_distribution<int> distUInt;
        auto randUInt = bind(distUInt, gen);
        for (size_t k = 0; k < kMaxTableSize; k++) {
            unsigned i = randUInt() & kMaxTableSizeMask;
            std::swap(pTable[k], pTable[i]);
            pTable[k + kMaxTableSize] = pTable[k];
        }
    }
    float lolrand(unsigned int x, unsigned int y, unsigned int seed)
    {
        unsigned int r1, r2, r;
        r1 = x * 0x1B873593;
        r2 = y * 0x19088711;

        r = seed + r1 + r2;
        r ^= r >> 5;
        r *= 0xCC9E2D51;
        r %= 10000;
        float rf = r * 1.0 / 10000;
        //   cout << rf << endl;
        return fabs(rf);
    }
    float lolrand(unsigned int x, unsigned int y, unsigned int z, unsigned int seed)
    {
        unsigned int r1, r2, r3, r;
        r1 = x * 0x1B873593;
        r2 = y * 0x19088711;
        r3 = z * 0xB2D05E13;

        r = seed + r1 + r2 + r3;
        r ^= r >> 5;
        r *= 0xCC9E2D51;
        r %= 10000;
        float rf = r * 1.0 / 10000;
        //   cout << rf << endl;
        return fabs(rf);
    }
    float lolrand(unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int seed)
    {
        unsigned int r1, r2, r3, r4, r;
        r1 = x * 0x1B873593;
        r2 = y * 0x19088711;
        r3 = z * 0xB2D05E13;
        r4 = z * 0x63D02313;

        r = seed + r1 + r2 + r3 + r4;
        r ^= r >> 5;
        r *= 0xCC9E2D51;
        r %= 10000;
        float rf = r * 1.0 / 10000;
        //   cout << rf << endl;
        return rf;
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
        float c00 = lolrand(rx0, ry0, nowseed);
        float c01 = lolrand(rx0, ry1, nowseed);
        float c10 = lolrand(rx1, ry0, nowseed);
        float c11 = lolrand(rx1, ry1, nowseed);
        /*
        float c00 = r[pTable[pTable[rx0]] + ry0];
        float c10 = r[pTable[pTable[rx1]] + ry0];
        float c01 = r[pTable[pTable[rx0]] + ry1];
        float c11 = r[pTable[pTable[rx1]] + ry1];
        */
        float sx = smoothstepp(tx);
        float sy = smoothstepp(ty);

        //
        float nx0 = lerpp(c00, c10, sx);
        float nx1 = lerpp(c01, c11, sx);

        return lerpp(nx0, nx1, sy);
    }


    float eval(float x, float y)
    {
        //       cout << "(" << x << "," << y << ")" << endl;
        double c = 1, a = 0.5; // torus parameters (controlling size)
        double xt = (c + a * cos(2 * PI * y)) * cos(2 * PI * x);// +0.5;
        double yt = (c + a * cos(2 * PI * y)) * sin(2 * PI * x);// +1;// +101.5;
        double zt = a * sin(2 * PI * y);// +101.5;

//        cout << "(" << xt << "," << yt << "," << zt << ")" << endl;
        int xi = std::floor(xt);
        int yi = std::floor(yt);
        int zi = std::floor(zt);


        float tx = xt - xi;
        float ty = yt - yi;
        float tz = zt - zi;


        int rx0 = xi;// &kMaxTableSizeMask;
        int rx1 = (rx0 + 1);// &kMaxTableSizeMask;
        int ry0 = yi;// &kMaxTableSizeMask;
        int ry1 = (ry0 + 1);// &kMaxTableSizeMask;
        int rz0 = zi;// &kMaxTableSizeMask;
        int rz1 = (rz0 + 1);// &kMaxTableSizeMask;





        //   cout << nowseed << endl;

        float c000 = lolrand(rx0, ry0, rz0, nowseed);
        float c001 = lolrand(rx0, ry0, rz1, nowseed);
        float c010 = lolrand(rx0, ry1, rz0, nowseed);
        float c011 = lolrand(rx0, ry1, rz1, nowseed);
        float c100 = lolrand(rx1, ry0, rz0, nowseed);
        float c101 = lolrand(rx1, ry0, rz1, nowseed);
        float c110 = lolrand(rx1, ry1, rz0, nowseed);
        float c111 = lolrand(rx1, ry1, rz1, nowseed);

        float sx = smoothstepp(tx);
        float sy = smoothstepp(ty);
        float sz = smoothstepp(tz);
        //




        float nx00 = lerpp(c000, c001, sz);
        float nx01 = lerpp(c010, c011, sz);
        float nx10 = lerpp(c100, c101, sz);
        float nx11 = lerpp(c110, c111, sz);

        float nx0 = lerpp(nx00, nx01, sy);
        float nx1 = lerpp(nx10, nx11, sy);

        return lerpp(nx0, nx1, sx);
    }

    float evalS(float x, float y)
    {
        //       cout << "(" << x << "," << y << ")" << endl;
        double c = 1, a = 0.5; // torus parameters (controlling size)
        double xt = c + a * cos(2 * PI * x);// +0.5;
        double yt = c + a * sin(2 * PI * x);// +1;// +101.5;
        //ÖÜ³¤ÊÇ2PI
        double zt = c + a * 2 * PI * y; // +101.5;

//        cout << "(" << xt << "," << yt << "," << zt << ")" << endl;
        int xi = std::floor(xt);
        int yi = std::floor(yt);
        int zi = std::floor(zt);


        float tx = xt - xi;
        float ty = yt - yi;
        float tz = zt - zi;


        int rx0 = xi;// &kMaxTableSizeMask;
        int rx1 = (rx0 + 1);// &kMaxTableSizeMask;
        int ry0 = yi;// &kMaxTableSizeMask;
        int ry1 = (ry0 + 1);// &kMaxTableSizeMask;
        int rz0 = zi;// &kMaxTableSizeMask;
        int rz1 = (rz0 + 1);// &kMaxTableSizeMask;





        //   cout << nowseed << endl;

        float c000 = lolrand(rx0, ry0, rz0, nowseed);
        float c001 = lolrand(rx0, ry0, rz1, nowseed);
        float c010 = lolrand(rx0, ry1, rz0, nowseed);
        float c011 = lolrand(rx0, ry1, rz1, nowseed);
        float c100 = lolrand(rx1, ry0, rz0, nowseed);
        float c101 = lolrand(rx1, ry0, rz1, nowseed);
        float c110 = lolrand(rx1, ry1, rz0, nowseed);
        float c111 = lolrand(rx1, ry1, rz1, nowseed);

        float sx = smoothstepp(tx);
        float sy = smoothstepp(ty);
        float sz = smoothstepp(tz);
        //




        float nx00 = lerpp(c000, c001, sz);
        float nx01 = lerpp(c010, c011, sz);
        float nx10 = lerpp(c100, c101, sz);
        float nx11 = lerpp(c110, c111, sz);

        float nx0 = lerpp(nx00, nx01, sy);
        float nx1 = lerpp(nx10, nx11, sy);

        return lerpp(nx0, nx1, sx);
    }


    static const unsigned kMaxTableSize = 512;
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


    vector<vector<int>>* getNoise(int row, int columns, int edgeOptimization) {
        for (int i = 0; i < row; i++) {
            vector<int> everyRowPixel;
            for (int j = 0; j < columns; j++) {
                int nowi = i, nowj = j;
                float d, g;
                if (edgeOptimization == 2) {
                    g = eval(nowj * 1.0 / columns, nowi * 1.0 / row) * 10;
                }
                else if (edgeOptimization == 1) {
                    g = evalS(nowj * 1.0 / columns, nowi * 1.0 / row) * 10;
                }
                else {
                    g = eval(Vec20f(0.01 * (nowj), nowi * 0.01)) * 10;
                }
                d = g - int(g);
                everyRowPixel.push_back(int(d * 255));
            }
            result->push_back(everyRowPixel);
        }
        return result;
    }
};


