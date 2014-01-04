#include "header.h"
#include <iostream>
using namespace std;
using namespace cv;

int main(){
    Mat matTotalColor_m1;
    Mat matTotalSift_m1;
    Mat matTotalDesc_m1;
    Mat queryMat;
    vector<matDescToImgfile_method1> vec_Desc_Imgfile_m1 ;
  // colorHist(matTotalColor,vec_Desc_Imgfile);
    FileStorage fs("./food_2/Feature_Mat_1.xml", FileStorage::READ);
    fs["Feature_Mat_1"] >> matTotalDesc_m1;
    //cout<<matTotalDesc<<endl;
    cv::flann::Index m_index(matTotalDesc_m1, cv::flann::KDTreeIndexParams(4));
    char* query_image_name="./food_2/10.jpg";
    if(!imread(query_image_name).data)
        cout<<"Error reading query image."<<endl;
    colorHist(matTotalColor_m1,0,query_image_name);
    cout<<matTotalColor_m1<<endl;
    getSift_method1(matTotalSift_m1,vec_Desc_Imgfile_m1,0,query_image_name);
    buildAllMat_method1(matTotalColor_m1,matTotalSift_m1,matTotalDesc_m1,query_image_name);
    queryMat=matTotalDesc_m1;
    imageRetrival(m_index,queryMat,query_image_name);
    
      cout<<matTotalDesc_m1.rows<<"  "<<matTotalDesc_m1.cols<<endl;
    cout<<"opencv...hehe"<<endl;
    return 0;
}

