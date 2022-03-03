#include<cstdio>
#include<iostream>
#include<vector>
#include<string>
#include"Perlin.h"
#include"NoiseVisualization.h"
using namespace std;
int main() {
	Perlin* perlinNoise = new Perlin;
	//柏林噪声生成
	vector<vector<int>>* grayScale = perlinNoise->getPerlinNoise(500, 500);
	//柏林噪声显示
	NoiseVisualization* noiseVisualization = new NoiseVisualization(grayScale,"./");

	return 0;
}