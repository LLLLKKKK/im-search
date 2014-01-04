#include "header.h"
#include <iostream>
using namespace std;
using namespace cv;

//Get features of all images and store it into a xml file.
int main(){
    Mat matTotalColor_m1;
    Mat matTotalSift_m1;
    Mat matTotalDesc_m1;
    vector<matDescToImgfile_method1> vec_Desc_Imgfile_m1 ;
    for(int k=0;k<IMAGE_NUM;k++)
        colorHist(matTotalColor_m1,k);
    string query_image_name="1.jpg";
    for(int i =0;i<IMAGE_NUM;i++)
        getSift_method1(matTotalSift_m1,vec_Desc_Imgfile_m1,i);
    buildAllMat_method1(matTotalColor_m1,matTotalSift_m1,matTotalDesc_m1);
    cout<<"hang: "<<matTotalDesc_m1.rows<<"   lie: "<<matTotalDesc_m1.cols<<endl;
    cout<<"opencv...hehe"<<endl;
    return 0;
}



