#pragma once
#include <random>
#include <functional>
#include <iostream>
#include <cmath>
#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<cstdio>
#include<iostream>
#include<cmath>
#include<string>
using namespace std;
using namespace cv;
class NoiseVisualization
{
public:
	NoiseVisualization(vector<vector<int>>* img,string outputPath);

private:
	Mat *mat;
};

