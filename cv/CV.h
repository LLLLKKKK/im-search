
#ifndef BUILD_H
#define BUILD_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#define CLUSTER_NUM_METHOD1 20

using namespace std;
using namespace cv;

void checkHSV(vector<vector<int>>& H, vector<vector<int>>& S, vector<vector<int>>& V, vector<vector<int>>& L,double h, double s, double v,int i, int j,Mat& Hist);
void colorHist(Mat &matTotalDesc, const string &filename);
void getSift1(Mat& matTotalDesc, const string &filename);
Mat combineFeatures(const Mat &matColor, const Mat &matSift);

#endif
