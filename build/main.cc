
#include <sstream>
#include <iostream>
#include <vector>
#include "cv/CV.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat matTotalColor;
    Mat matTotalSift;
    vector<string> filenames;

    for (int i = 0; i < 6; i++) {
        stringstream ss;
        ss << i << ".jpg";
        filenames.push_back(ss.str());
    }
    for (auto it = filenames.begin(); it != filenames.end(); it++) {
        colorHist(matTotalColor, *it);
        getSift1(matTotalSift, *it);
    }

    Mat featureMat = combineFeatures(matTotalColor,matTotalSift);

    FileStorage fs("FeatureMat.xml", FileStorage::WRITE);
    fs << "FeatureMat" << featureMat;
    fs.release();
    return 0;
}
