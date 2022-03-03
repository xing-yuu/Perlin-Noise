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
double Perlin::Noise(int x, int y)    // 整一个随机数，相当于伪随机，因为运算法则都是固定的
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double Perlin::SmoothedNoise(int x, int y)   //光滑噪声
{
    double corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
    double sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
    double center = Noise(x, y) / 4;
    return corners + sides + center;
}
double Perlin::Cosine_Interpolate(double a, double b, double x)  // 余弦插值
{
    double ft = x * 3.1415927;
    double f = (1 - cos(ft)) * 0.5;
    return a * (1 - f) + b * f;
}

double Perlin::InterpolatedNoise(float x, float y)   // 获取插值噪声
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

double Perlin::PerlinNoise(float x, float y)    // 最终调用：根据(x,y)获得其对应的PerlinNoise值
{
    double total = 0;
    double p = 0.5;
    int n =4;//决定叠加的频率和幅度
    for (int i = 0; i < n; i++)
    {
        double frequency = pow(2, i);
        double amplitude = pow(p, i);
        total = total + InterpolatedNoise(x * frequency, y * frequency) * amplitude;
    }
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
void Perlin::smoothEdge(vector<vector<int>>* oriNoise,int iteration) {
    int smoothSize = oriNoise->size() * 0.05;
    for (int iterationID = 0; iterationID < iteration; iterationID++) {
        vector<vector<int>> tempNoise;
        for (int i = 0; i < oriNoise->size(); i++) {
            vector<int> tempRow;
            for (int j = oriNoise->at(i).size() - smoothSize; j < oriNoise->at(i).size(); j++) {
                int rfficientPoint = 1;
                int nowGrew = oriNoise->at(i).at(j);
                if (i - 1 >= 0) {
                    if (j - 1 >= 0) {
                        nowGrew += oriNoise->at(i - 1).at(j - 1);
                        rfficientPoint++;
                    }
                    else {
                        nowGrew += oriNoise->at(i - 1).at(oriNoise->at(i - 1).size() - 1);
                        rfficientPoint++;
                    }
                    if (j + 1 < oriNoise->at(i).size()) {
                        nowGrew += oriNoise->at(i - 1).at(j + 1);
                        rfficientPoint++;
                    }
                    else {
                        nowGrew += oriNoise->at(i - 1).at(0);
                        rfficientPoint++;
                    }
                    nowGrew += oriNoise->at(i - 1).at(j);
                    rfficientPoint++;
                }
                if (i + 1 < oriNoise->size()) {
                    if (j - 1 >= 0) {
                        nowGrew += oriNoise->at(i + 1).at(j - 1);
                        rfficientPoint++;
                    }
                    else {
                        nowGrew += oriNoise->at(i + 1).at(oriNoise->at(i + 1).size() - 1);
                        rfficientPoint++;
                    }
                    if (j + 1 < oriNoise->at(i + 1).size()) {
                        nowGrew += oriNoise->at(i + 1).at(j + 1);
                        rfficientPoint++;
                    }
                    else {
                        nowGrew += oriNoise->at(i + 1).at(0);
                        rfficientPoint++;
                    }
                    nowGrew += oriNoise->at(i + 1).at(j);
                    rfficientPoint++;
                }
                if (j - 1 >= 0) {
                    nowGrew += oriNoise->at(i).at(j - 1);
                    rfficientPoint++;
                }
                else {
                    nowGrew += oriNoise->at(i).at(oriNoise->at(i).size() - 1);
                    rfficientPoint++;
                }
                if (j + 1 < oriNoise->at(i).size()) {
                    nowGrew += oriNoise->at(i).at(j + 1);
                    rfficientPoint++;
                }
                else {
                    nowGrew += oriNoise->at(i).at(0);
                    rfficientPoint++;
                }
                nowGrew /= rfficientPoint;
                tempRow.push_back(nowGrew);
            }
            for (int j = 0; j < smoothSize; j++) {
                int rfficientPoint = 1;
                int nowGrew = oriNoise->at(i).at(j);
                if (i - 1 >= 0) {
                    if (j - 1 >= 0) {
                        nowGrew += oriNoise->at(i - 1).at(j - 1);
                        rfficientPoint++;
                    }
                    else {
                        nowGrew += oriNoise->at(i - 1).at(oriNoise->at(i - 1).size() - 1);
                        rfficientPoint++;
                    }
                    if (j + 1 < oriNoise->at(i).size()) {
                        nowGrew += oriNoise->at(i - 1).at(j + 1);
                        rfficientPoint++;
                    }
                    else {
                        nowGrew += oriNoise->at(i - 1).at(0);
                        rfficientPoint++;
                    }
                    nowGrew += oriNoise->at(i - 1).at(j);
                    rfficientPoint++;
                }
                if (i + 1 < oriNoise->size()) {
                    if (j - 1 >= 0) {
                        nowGrew += oriNoise->at(i + 1).at(j - 1);
                        rfficientPoint++;
                    }
                    else {
                        nowGrew += oriNoise->at(i + 1).at(oriNoise->at(i + 1).size() - 1);
                        rfficientPoint++;
                    }
                    if (j + 1 < oriNoise->at(i + 1).size()) {
                        nowGrew += oriNoise->at(i + 1).at(j + 1);
                        rfficientPoint++;
                    }
                    else {
                        nowGrew += oriNoise->at(i + 1).at(0);
                        rfficientPoint++;
                    }
                    nowGrew += oriNoise->at(i + 1).at(j);
                    rfficientPoint++;
                }
                if (j - 1 >= 0) {
                    nowGrew += oriNoise->at(i).at(j - 1);
                    rfficientPoint++;
                }
                else {
                    nowGrew += oriNoise->at(i).at(oriNoise->at(i).size() - 1);
                    rfficientPoint++;
                }
                if (j + 1 < oriNoise->at(i).size()) {
                    nowGrew += oriNoise->at(i).at(j + 1);
                    rfficientPoint++;
                }
                else {
                    nowGrew += oriNoise->at(i).at(0);
                    rfficientPoint++;
                }
                nowGrew /= rfficientPoint;
                tempRow.push_back(nowGrew);
            }
            tempNoise.push_back(tempRow);
        }
        for (int i = 0; i < oriNoise->size(); i++) {
            for (int j = oriNoise->at(i).size() - smoothSize; j < oriNoise->size(); j++) {
                oriNoise->at(i).at(j) = tempNoise.at(i).at(j - (oriNoise->at(i).size() - smoothSize));
            }
            for (int j = 0; j < smoothSize; j++) {
                oriNoise->at(i).at(j) = tempNoise.at(i).at(j + smoothSize);
            }
        }
    }
}
vector<vector<int>>* Perlin::getPerlinNoise(int row, int columns, int type, bool edgeOptimization) {
    vector<int> seedColumns;
    int nowSeed = columns;
    for (int i = 0; i < columns / 30.0*3.0; i++) {
        seedColumns.push_back(nowSeed++);
    }
    for (int i = columns / 30.0 * 3.0; i < columns / 30.0 * 4.0; i++) {
        seedColumns.push_back(nowSeed--);
    }
    for (int i = columns / 30.0 * 4.0; i < columns / 30.0 * 7.0; i++) {
        seedColumns.push_back(nowSeed++);
    }
    for (int i = columns / 30.0 * 7.0; i < columns / 30.0 * 8.0; i++) {
        seedColumns.push_back(nowSeed--);
    }
    for (int i = columns / 30.0 * 8.0; i < columns / 30.0 * 11.0; i++) {
        seedColumns.push_back(nowSeed++);
    }
    for (int i = columns / 30.0 * 11.0; i < columns / 30.0 * 12.0; i++) {
        seedColumns.push_back(nowSeed--);
    }
    for (int i = columns / 30.0 * 12.0; i < columns / 30.0 * 14.0; i++) {
        seedColumns.push_back(nowSeed++);
    }
    for (int i = columns / 30.0 * 14.0; i < columns / 30.0 * 17.0; i++) {
        seedColumns.push_back(nowSeed--);
    }
    for (int i = columns / 30.0 * 17.0; i < columns / 30.0 * 18.0; i++) {
        seedColumns.push_back(nowSeed++);
    }
    for (int i = columns / 30.0 * 18.0; i < columns / 30.0 * 21.0; i++) {
        seedColumns.push_back(nowSeed--);
    }
    for (int i = columns / 30.0 * 21.0; i < columns / 30.0 * 22.0; i++) {
        seedColumns.push_back(nowSeed++);
    }
    for (int i = columns / 30.0 * 22.0; i < columns / 30.0 * 24.0; i++) {
        seedColumns.push_back(nowSeed--);
    }
    for (int i = columns / 30.0 * 24.0; i < columns / 30.0 * 25.0; i++) {
        seedColumns.push_back(nowSeed++);
    }
    for (int i = columns / 30.0 * 25.0; i < columns / 30.0 * 27.0; i++) {
        seedColumns.push_back(nowSeed--);
    }
    for (int i = columns / 30.0 * 27.0; i < columns / 30.0 * 28.0; i++) {
        seedColumns.push_back(nowSeed++);
    }
    int lastPoint = columns - seedColumns.size();
    float delta = (seedColumns.at(0) - seedColumns.at(seedColumns.size() - 1)) * 1.0 / lastPoint;
    for (int i = 0; i < lastPoint; i++) {
        seedColumns.push_back(seedColumns.at(seedColumns.size() - 1) + delta);
    }
    for (int i = 0; i < row; i++) {
        vector<int> everyRowPixel;
        for (int j = 0; j < columns; j++) {
            int nowi = i, nowj = j;
            if (edgeOptimization) {
                nowj = seedColumns.at((j + columns / 4) % columns);
            }
            float g = u1.eval(Vec20f(0.03 * (nowj), nowi * 0.03)) * 10;
            float d;
            if(type==0){//木纹
                d = g - int(g);
            }
            else if (type == 1) {//perlin noise 原图
                d = (PerlinNoise(0.05 * (nowj), nowi * 0.05)+1.0)/2.0;
            }
            else if (type == 2) {//大理石纹理
                float noiseValue = u1.fractalNoise(Vec20f(0.5 * (nowj), 0.5 * nowi));
                // we "displace" the value i used in the sin() expression by noiseValue * 100
                float d = (sin((nowj + noiseValue * 100) * 2  / 200.f) + 1) / 2.f;
            }
            everyRowPixel.push_back(int(d * 255));
        }
        result->push_back(everyRowPixel);
    }
    return result;
}