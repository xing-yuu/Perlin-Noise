#include "PerlinNoise4D.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include<iostream>






/////////////////////////////////////////////////////////////////////
// return a random float in [-1,1]

float PerlinNoise4D::randNoiseFloat() {
    return (float)((rand() % (NOISE_WRAP_INDEX + NOISE_WRAP_INDEX)) -
        NOISE_WRAP_INDEX) / NOISE_WRAP_INDEX;
};


void PerlinNoise4D::normalize4d(float* vector) {
    float length = 1 / sqrt((vector[0] * vector[0]) +
        (vector[1] * vector[1]) +
        (vector[2] * vector[2]) +
        (vector[3] * vector[3]));
    *vector++ *= length;
    *vector++ *= length;
    *vector++ *= length;
    *vector *= length;
}

/////////////////////////////////////////////////////////////////////
// initialize everything during constructor or reseed -- note
// that space was already allocated for the gradientTable
// during the constructor

void PerlinNoise4D::generateLookupTables() {
    unsigned i, j, temp;

    for (i = 0; i < NOISE_WRAP_INDEX; i++) {
        permutationTable[i] = i;
        //4DGradient for all Dimension (1d, 2d, 3d 4d)

        for (j = 0; j < 4; j++) {
            gradientTable4d[i][j] = randNoiseFloat(); //std::cout << gradientTable4d[i][j] << std::endl;
        }
        normalize4d(gradientTable4d[i]);
    }

    // Shuffle permutation table up to NOISE_WRAP_INDEX
    for (i = 0; i < NOISE_WRAP_INDEX; i++) {
        j = rand() & NOISE_MOD_MASK;
        temp = permutationTable[i];
        permutationTable[i] = permutationTable[j];
        permutationTable[j] = temp;
    }

    // Add the rest of the table entries in, duplicating 
    // indices and entries so that they can effectively be indexed
    // by unsigned chars.
    //
    for (i = 0; i < NOISE_WRAP_INDEX + 2; i++) {
        permutationTable[NOISE_WRAP_INDEX + i] = permutationTable[i];

        //4DGradient for all Dimension (1d, 2d, 3d 4d)
        for (j = 0; j < 4; j++)
            gradientTable4d[NOISE_WRAP_INDEX + i][j] = gradientTable4d[i][j];
    }

    // And we're done. Set initialized to true
    initialized = TRUE;
}

/////////////////////////////////////////////////////////////////////
// reinitialize with new, random values.

void PerlinNoise4D::reseed() {
    srand((unsigned int)(time(NULL) + rand()));
    generateLookupTables();
}

/////////////////////////////////////////////////////////////////////
// reinitialize using a user-specified random seed.

void PerlinNoise4D::reseedVal(unsigned int rSeed) {
    srand(rSeed);
    generateLookupTables();
}

float PerlinNoise4D::noise4d(float* pos) {
    int gridPointL, gridPointR, gridPointD, gridPointU, gridPointB, gridPointF, gridPointV, gridPointW;
    float distFromL, distFromR, distFromD, distFromU, distFromB, distFromF, distFromV, distFromW;
    float sX, sY, sZ, sT, a, b, c, d, e, f, t, u, v;


    // find out neighboring grid points to pos and signed distances from pos to them.
    setupValues(pos, gridPointL, gridPointR, distFromL, distFromR);
    setupValues(pos + 1, gridPointD, gridPointU, distFromD, distFromU);
    setupValues(pos + 2, gridPointB, gridPointF, distFromB, distFromF);
    setupValues(pos + 3, gridPointV, gridPointW, distFromV, distFromW);

    {

        const int indexL = permutationTable[gridPointL];
        const int indexR = permutationTable[gridPointR];

        const int indexLD = permutationTable[indexL + gridPointD];
        const int indexRD = permutationTable[indexR + gridPointD];
        const int indexLU = permutationTable[indexL + gridPointU];
        const int indexRU = permutationTable[indexR + gridPointU];

        const int indexLDB = permutationTable[indexLD + gridPointB];
        const int indexRDB = permutationTable[indexRD + gridPointB];
        const int indexLUB = permutationTable[indexLU + gridPointB];
        const int indexRUB = permutationTable[indexRU + gridPointB];
        const int indexLDF = permutationTable[indexLD + gridPointF];
        const int indexRDF = permutationTable[indexRD + gridPointF];
        const int indexLUF = permutationTable[indexLU + gridPointF];
        const int indexRUF = permutationTable[indexRU + gridPointF];

        float* ptrV = (float*)&gradientTable4d[gridPointV];
        float* ptrW = (float*)&gradientTable4d[gridPointW];

        sX = easeCurve(distFromL);
        sY = easeCurve(distFromD);
        sZ = easeCurve(distFromB);
        sT = easeCurve(distFromV);


        u = dot4(distFromL, distFromD, distFromB, distFromV, ptrV + indexLDB);
        v = dot4(distFromR, distFromD, distFromB, distFromV, ptrV + indexRDB);
        a = linearInterp(sX, u, v);

        u = dot4(distFromL, distFromU, distFromB, distFromV, ptrV + indexLUB);
        v = dot4(distFromR, distFromU, distFromB, distFromV, ptrV + indexRUB);
        b = linearInterp(sX, u, v);
        c = linearInterp(sY, a, b);

        u = dot4(distFromL, distFromD, distFromF, distFromV, ptrV + indexLDF);
        v = dot4(distFromR, distFromD, distFromF, distFromV, ptrV + indexRDF);
        a = linearInterp(sX, u, v);

        u = dot4(distFromL, distFromU, distFromF, distFromV, ptrV + indexLUF);
        v = dot4(distFromR, distFromU, distFromF, distFromV, ptrV + indexRUF);
        b = linearInterp(sX, u, v);
        d = linearInterp(sY, a, b);
        e = linearInterp(sZ, c, d);


        u = dot4(distFromL, distFromD, distFromB, distFromW, ptrW + indexLDB);
        v = dot4(distFromR, distFromD, distFromB, distFromW, ptrW + indexRDB);
        a = linearInterp(sX, u, v);

        u = dot4(distFromL, distFromU, distFromB, distFromW, ptrW + indexLUB);
        v = dot4(distFromR, distFromU, distFromB, distFromW, ptrW + indexRUB);
        b = linearInterp(sX, u, v);
        c = linearInterp(sY, a, b);

        u = dot4(distFromL, distFromD, distFromF, distFromW, ptrW + indexLDF);
        v = dot4(distFromR, distFromD, distFromF, distFromW, ptrW + indexRDF);
        a = linearInterp(sX, u, v);

        u = dot4(distFromL, distFromU, distFromF, distFromW, ptrW + indexLUF);
        v = dot4(distFromR, distFromU, distFromF, distFromW, ptrW + indexRUF);
        b = linearInterp(sX, u, v);
        d = linearInterp(sY, a, b);
        f = linearInterp(sZ, c, d);
    }

    return linearInterp(sT, e, f);
}


float PerlinNoise4D::noise(float x, float y, float z, float w) {
    //static float tScale=1500.f;
    //static float xyzScale=15.f;
    float p[4] = { x * xyzScale, y * xyzScale, z * xyzScale, w / tScale };
    return noise4d(p);
}

float PerlinNoise4D::SeamlessNoise(float x, float y, float dx, float dy, float xyOffset) {
    float s = x;
    float t = y;
    float PI = 3.141593;
    float nx = xyOffset + cos(s * 2.0f * PI) * dx / (2.0f * PI);
    float ny = xyOffset + cos(t * 2.0f * PI) * dy / (2.0f * PI);
    float nz = xyOffset + sin(s * 2.0f * PI) * dx / (2.0f * PI);
    float nw = xyOffset + sin(t * 2.0f * PI) * dy / (2.0f * PI);

    return noise(nx, ny, nz, nw);
}
float PerlinNoise4D::SeamlessNoise(float dx, float dy, float xyOffset) {


    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    while (dx > 255) dx -= 255;

    while (dy > 255) dy -= 255;
    int x0 = floor(dx);
    int y0 = floor(dy);

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float x = dx - (float)x0;
    float y = dy - (float)y0;



    float s = x;
    float t = y;
    float PI = 3.141593;
    float nx = xyOffset + cos(s * 2.0f * PI) * dx / (2.0f * PI);
    float ny = xyOffset + cos(t * 2.0f * PI) * dy / (2.0f * PI);
    float nz = xyOffset + sin(s * 2.0f * PI) * dx / (2.0f * PI);
    float nw = xyOffset + sin(t * 2.0f * PI) * dy / (2.0f * PI);

    return noise(nx, ny, nz, nw);
}
