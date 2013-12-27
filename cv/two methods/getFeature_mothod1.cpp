#include "header.h"
#include <iostream>
using namespace std;
using namespace cv;

//Get features of all images and store it into a xml file.
int main(){
    Mat matTotalColor;
    Mat matTotalSift;
    Mat matTotalDesc;
    vector<matDescToImgfile_method1> vec_Desc_Imgfile ;
    colorHist(matTotalColor);
    string query_image_name="1.jpg";
    getSift_method1(matTotalSift,vec_Desc_Imgfile);
    buildAllMat_method1(matTotalColor,matTotalSift,matTotalDesc);
    cout<<"hang: "<<matTotalDesc.rows<<"   lie: "<<matTotalDesc.cols<<endl;
    cout<<"opencv...hehe"<<endl;
    return 0;
}



