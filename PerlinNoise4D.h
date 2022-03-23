#pragma once

#define TEXT3D

#define BITS 6  // Dimensions 6 -> 64
#define PRJ_DIM (1<<BITS)

#define NOISE_WRAP_INDEX	128
#define NOISE_MOD_MASK		127

// A large power of 2, we'll go for 4096, to add to negative numbers
// in order to make them positive

#define NOISE_LARGE_PWR2	4096

#define easeCurve(t)		( t * t * (3.0 - (t + t)) )
#define linearInterp(t, a, b)	( a + t * (b - a) )
#define dot2(rx, ry, q)		( rx * q[0] + ry * q[1] )
#define dot3(rx, ry, rz, q)     ( rx * *(q) + ry * *(q+1) + rz * *(q+2) )
#define dot4(rx, ry, rz, rt, q)     ( rx * *(q) + ry * *(q+1) + rz * *(q+2) + rt * *(q+3))

#define setupValues(pos, g0, g1, d0, d1) { \
const float t = *(pos) + NOISE_LARGE_PWR2; \
            g0 = ((int)t) & NOISE_MOD_MASK; \
            g1 = (g0 + 1) & NOISE_MOD_MASK; \
            d0 = ((float)t) - (int)t; \
            d1 = d0 - 1.0; \
} \

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
class PerlinNoise4D
{
public:
    float randNoiseFloat();
    void normalize4d(float* vector);
    void generateLookupTables();
    void reseed();
    void reseedVal(unsigned int rSeed);
    float noise4d(float* pos);
    float noise(float x, float y, float z, float w);
    float SeamlessNoise(float x, float y, float dx, float dy, float xyOffset);
    float SeamlessNoise(float dx, float dy, float xyOffset);
private:
    float gradientTable4d[NOISE_WRAP_INDEX * 2 + 2][4] = { 0 };
    unsigned permutationTable[NOISE_WRAP_INDEX * 2 + 2];	// permutation table
    int initialized = FALSE;
    float tScale = 12000.f;
    float xyzScale = 1.5f;

};


