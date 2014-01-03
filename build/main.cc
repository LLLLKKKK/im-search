
#include <sstream>
#include <iostream>
#include <vector>
#include "Build.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    vector<string> filenames;
    for (int i = 0; i < 6; i++) {
        stringstream ss;
        ss << i << ".jpg";
        filenames.push_back(ss.str());
    }
    Mat matTotalColor = colorHist(filenames);
    Mat matTotalSift = getSift1(filenames);
    build1(matTotalColor ,matTotalSift);
    
    return 0;
}
