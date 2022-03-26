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
#include "SimplexNoise.h"
using namespace std;

#define PI 3.14159265
struct point2D {
    float x;
    float y;
};
class Perlin
{
public:
    Perlin();
    ~Perlin();
    double Noise(int x, int y, int z);
    double Noise(int x, int y);
    double SmoothedNoise(int x, int y);
    double SmoothedNoise(int x, int y, int z);
    double InterpolatedNoise(float x, float y, float z);
    double Cosine_Interpolate(double a, double b, double x);
    double InterpolatedNoise(float x, float y);
    double PerlinNoise(float x, float y);
    double PerlinNoiseT(float x, float y);
    double PerlinNoiseS(float x, float y);
    float TileablePerlin(float x, float y);


    float min(float v1, float v2) { return v1 < v2 ? v1 : v2; }
    float max(float v1, float v2) { return v1 > v2 ? v1 : v2; }

    float getLength(point2D vStart, point2D vEnd);
    void smoothEdge(vector<vector<int>>* oriNoise, int iteration);
    vector<vector<int>>* getPerlinNoise(int row, int columns, int edgeOptimization = 1);



private:
    vector<vector<int>>* result;
    int nowSeed;
};