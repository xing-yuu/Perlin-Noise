#include "Perlin.h"
using namespace std;


Perlin::Perlin()
{
    srand((unsigned)time(NULL));
    nowSeed = rand();
    result = new vector<vector<int>>;
   
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
double Perlin::Noise(int x, int y,int z)    // 整一个随机数，相当于伪随机，因为运算法则都是固定的
{
    int n = x + y * 57 + z * nowSeed;
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
double Perlin::SmoothedNoise(int x, int y, int z)   //光滑噪声
{
    double corners = (Noise(x - 1, y - 1, z - 1) + Noise(x - 1, y - 1, z + 1) + Noise(x - 1, y + 1, z - 1) + Noise(x - 1, y + 1, z + 1) + Noise(x + 1, y - 1, z - 1) + Noise(x + 1, y - 1, z + 1) + Noise(x + 1, y + 1, z - 1) + Noise(x + 1, y + 1, z + 1)) / 32;
    double sides = (Noise(x - 1, y, z) + Noise(x + 1, y, z) + Noise(x, y - 1, z) + Noise(x, y + 1, z) + Noise(x, y, z + 1) + Noise(x, y, z - 1)) / 16;
    double center = Noise(x, y, z) / 8;
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

double Perlin::InterpolatedNoise(float x, float y, float z)   // 获取三维插值噪声
{
    int integer_X = floor(x);
    float  fractional_X = x - integer_X;
    int integer_Y = floor(y);
    float fractional_Y = y - integer_Y;
    int integer_Z = floor(z);
    float  fractional_Z = z - integer_Z;



    double c000 = SmoothedNoise(integer_X, integer_Y, integer_Z);
    double c001 = SmoothedNoise(integer_X, integer_Y, integer_Z + 1);
    double c010 = SmoothedNoise(integer_X, integer_Y + 1, integer_Z);
    double c011 = SmoothedNoise(integer_X, integer_Y + 1, integer_Z + 1);
    double c100 = SmoothedNoise(integer_X + 1, integer_Y, integer_Z);
    double c101 = SmoothedNoise(integer_X + 1, integer_Y, integer_Z + 1);
    double c110 = SmoothedNoise(integer_X + 1, integer_Y + 1, integer_Z);
    double c111 = SmoothedNoise(integer_X + 1, integer_Y + 1, integer_Z + 1);

    double nx00 = Cosine_Interpolate(c000, c001, fractional_Z);
    double nx01 = Cosine_Interpolate(c010, c011, fractional_Z);
    double nx10 = Cosine_Interpolate(c100, c101, fractional_Z);
    double nx11 = Cosine_Interpolate(c110, c111, fractional_Z);

    double nx0 = Cosine_Interpolate(nx00, nx01, fractional_Y);
    double nx1 = Cosine_Interpolate(nx10, nx11, fractional_Y);

    return Cosine_Interpolate(nx0, nx1, fractional_X);

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

double Perlin::PerlinNoiseT(float x, float y)    // 最终调用：根据(x,y)获得其对应的PerlinNoise值
{
    double c = 4, a = 2; // torus parameters (controlling size)
    double xt = (c + a * cos(2 * PI * y)) * cos(2 * PI * x);// +0.5;
    double yt = (c + a * cos(2 * PI * y)) * sin(2 * PI * x);// +1;// +101.5;
    double zt = a * sin(2 * PI * y);// +101.5;




    double total = 0;
    double p = 0.5;
    int n = 4;//决定叠加的频率和幅度
    for (int i = 0; i < n; i++)
    {
        double frequency = pow(2, i);
        double amplitude = pow(p, i);
        total = total + InterpolatedNoise(xt * frequency, yt * frequency, zt * frequency) * amplitude;
    }
    return total;
}


double Perlin::PerlinNoiseS(float x, float y)    // 最终调用：根据(x,y)获得其对应的PerlinNoise值
{
    double c = 4, a = 2; // torus parameters (controlling size)
    double xt = c + a * cos(2 * PI * x);// +0.5;
    double yt = c + a * sin(2 * PI * x);// +1;// +101.5;
    double zt = a * 2 * PI * y;// +101.5;




    double total = 0;
    double p = 0.5;
    int n = 4;//决定叠加的频率和幅度
    for (int i = 0; i < n; i++)
    {
        double frequency = pow(2, i);
        double amplitude = pow(p, i);
        total = total + InterpolatedNoise(xt * frequency, yt * frequency, zt * frequency) * amplitude;
    }
    return total;
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
vector<vector<int>>* Perlin::getPerlinNoise(int row, int columns, int edgeOptimization) {



    for (int i = 0; i < row; i++) {
        vector<int> everyRowPixel;
        for (int j = 0; j < columns; j++) {
            int nowi = i, nowj = j;
            float d, g;

            if (edgeOptimization==2) {
                d = (PerlinNoiseT(1.0 * (nowj) / columns, nowi * 1.0 / row) + 1.0) / 2.0;
            }
            else if (edgeOptimization == 1) {
                d = (PerlinNoiseS(1.0 * (nowj) / columns, nowi * 1.0 / row) + 1.0) / 2.0;
            }
            else {
                d = (PerlinNoise(0.05 * (nowj), nowi * 0.05) + 1.0) / 2.0;
            }
            /*



            else 
            else if (type == 3) {//周期纹理
                //float noiseValue = u1.fractalNoise(Vec20f(0.5 * (nowj), 0.5 * nowi));
                // we "displace" the value i used in the sin() expression by noiseValue * 100
                //float d = (sin((nowj + noiseValue * 100) * 2  / 200.f) + 1) / 2.f;
               
                d = (d + 1) / 2;
            }*/
        //    d = perlinNoise4D.SeamlessNoise(0.05 * nowj, nowi * 0.05, 0) + 1;
            //d /= 2;
 //           cout << d << endl;
            everyRowPixel.push_back(int(d * 255));
        }
        result->push_back(everyRowPixel);
    }
    return result;
}


