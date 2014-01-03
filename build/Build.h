
#ifndef BUILD_H
#define BUILD_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#define CLUSTER_NUM_METHOD1 20

void checkHSV(double h, double s, double v, int i, int j, cv::Mat& hist);
cv::Mat colorHist(const std::vector<std::string>& filenames);
cv::Mat getSift1(const std::vector<std::string> &filenames);
void build1(const cv::Mat& matTotalColor, const cv::Mat& matTotalSift);

#endif
