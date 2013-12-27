#include "header.h"
#include <iostream>
using namespace std;
using namespace cv;

//Get features of all images and store it into a xml file.
int main(){
    Mat matTotalColor;
    Mat matTotalSift;
    Mat matTotalDesc;
    Mat matSiftAllImages;
    vector<matDescToImgfile_method2> vec_Desc_Imgfile ;
    colorHist(matTotalColor);
    string query_image_name="1.jpg";
    matTotalSift= getSift_method2(matSiftAllImages,vec_Desc_Imgfile);
    buildAllMat_method2(matTotalColor,matTotalSift,matTotalDesc);
    cout<<"hang: "<<matTotalDesc.rows<<"   lie: "<<matTotalDesc.cols<<endl;    
    cout<<"opencv...hehe"<<endl;
    return 0;
}



