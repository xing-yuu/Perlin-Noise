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
	/*(i, j, k, l)
	* i:图像的高
	* j:图像的长
	* k:纹理类型：0，1,2
	* l:是否使用全局优化
	*/
	vector<vector<int>>* grayScale = perlinNoise->getPerlinNoise(360, 360, 1, false);
	//柏林噪声显示
	NoiseVisualization* noiseVisualization = new NoiseVisualization(grayScale,"./");

	return 0;
}