#include "NoiseVisualization.h"
NoiseVisualization::NoiseVisualization(vector<vector<int>>* img , string outputPath) {
    Mat mat (img->at(0).size(), img->size(), CV_8UC1);
//	mat = new cv::Mat(img->at(0).size(), img->size(), CV_8UC1);
    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {   
            mat.at<uchar>(i, j) = img->at(i).at(j);
        }
    }
    imshow("pic", mat);
    imwrite("result.png", mat);
    waitKey(0);
}