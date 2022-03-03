#include "Perlin.h"
using namespace std;


Perlin::Perlin()
{
    result = new vector<vector<int>>;
    for (int i = 0; i <= 256; i++) {
        for (int j = 0; j < 256; j++) {
            Gradient[i][j][0] = rand() % 256;
            Gradient[i][j][1] = rand() % 256;
        }
    }
}


Perlin::~Perlin()
{
    delete result;
}
double Perlin::Noise(int x, int y)    // ��һ����������൱��α�������Ϊ���㷨���ǹ̶���
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double Perlin::SmoothedNoise(int x, int y)   //�⻬����
{
    double corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
    double sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
    double center = Noise(x, y) / 4;
    return corners + sides + center;
}
double Perlin::Cosine_Interpolate(double a, double b, double x)  // ���Ҳ�ֵ
{
    double ft = x * 3.1415927;
    double f = (1 - cos(ft)) * 0.5;
    return a * (1 - f) + b * f;
}

double Perlin::InterpolatedNoise(float x, float y)   // ��ȡ��ֵ����
{
    int integer_X = int(x);
    float  fractional_X = x - integer_X;
    int integer_Y = int(y);
    float fractional_Y = y - integer_Y;
    double v1 = SmoothedNoise(integer_X, integer_Y);
    double v2 = SmoothedNoise(integer_X + 1, integer_Y);
    double v3 = SmoothedNoise(integer_X, integer_Y + 1);
    double v4 = SmoothedNoise(integer_X + 1, integer_Y + 1);
    double i1 = Cosine_Interpolate(v1, v2, fractional_X);
    double i2 = Cosine_Interpolate(v3, v4, fractional_X);
    return Cosine_Interpolate(i1, i2, fractional_Y);
}

double Perlin::PerlinNoise(float x, float y)    // ���յ��ã�����(x,y)������Ӧ��PerlinNoiseֵ
{
    double total = 0;
    double p = 0.5;
    int n = 2;//�������ӵ�Ƶ�ʺͷ���
    for (int i = 0; i < n; i++)
    {
        double frequency = pow(2, i);
        double amplitude = pow(p, i);
        total = total + InterpolatedNoise(x * frequency, y * frequency) * amplitude;
    }
    if (total < 0.15 && total>-0.15) return 0;
    return total;
}






float Perlin::lerp(float a0, float a1, float w)
{
    return (1.0f - w) * a0 + w * a1;
}

float Perlin::dotGridGradient(int ix, int iy, float x, float y)
{
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    // return dx;
    return (dx * Gradient[iy][ix][0] + dy * Gradient[iy][ix][1]);
}

float Perlin::perlin(float x, float y)
{
    // Determine grid cell coordinates
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (x > 255) x -= 255;

    while (y > 255) y -= 255;
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = lerp(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = lerp(n0, n1, sx);

    value = lerp(ix0, ix1, sy);
    int value2 = ceil(value);
    value -= value2;
    if (value < 0) return -value;
    return value;
}


float Perlin::getLength(point2D vStart, point2D vEnd)
{
    return sqrt((vEnd.x - vStart.x) * (vEnd.x - vStart.x) + (vEnd.y - vStart.y) * (vEnd.y - vStart.y));
}

vector<vector<int>>* Perlin::getPerlinNoise(int row, int columns, int type) {
    for (int i = 0; i < row; i++) {
        vector<int> everyRowPixel;
        for (int j = 0; j < columns; j++) {
            
            float g = u1.eval(Vec20f(0.05 * (j), i * 0.05)) * 10;
            float d;
            if(type==0){//ľ��
                d = g - int(g);
            }
            else if (type == 1) {//perlin noise ԭͼ
                d = fabs(PerlinNoise(0.15 * (j), i * 0.15));
            }
            else if (type == 2) {//����ʯ����
                float noiseValue = u1.fractalNoise(Vec20f(0.5 * (j), 0.5 * i));
                // we "displace" the value i used in the sin() expression by noiseValue * 100
                float d = (sin((0.15 * (j) + noiseValue * 100) * 2 / 200.f) + 1) / 2.f;
            }
            everyRowPixel.push_back(int(d * 255));
        }
        result->push_back(everyRowPixel);
    }
    return result;
}