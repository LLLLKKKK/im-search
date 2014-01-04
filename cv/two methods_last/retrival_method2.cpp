#include "header.h"
#include <iostream>
using namespace std;
using namespace cv;

int main(){
    Mat matTotalColor_m2;
    Mat matTotalSift_m2;
    Mat matTotalDesc_m2;
    Mat matSiftAllImages_m2;
    Mat queryMat;
    Mat center;
    vector<matDescToImgfile_method2> vec_Desc_Imgfile_m2 ;
    vector<int> siftOfimg;
  // colorHist(matTotalColor,vec_Desc_Imgfile);
    FileStorage fs("./food_2/Feature_Mat_2.xml", FileStorage::READ);
   
    fs["Feature_Mat_2"] >> matTotalDesc_m2;
    cv::flann::Index m_index(matTotalDesc_m2, cv::flann::KDTreeIndexParams(4));
    char* query_image_name="./food_2/1.jpg";
    if(!imread(query_image_name).data)
        cout<<"Error reading query image."<<endl;
    colorHist(matTotalColor_m2,0,query_image_name);
    matTotalSift_m2=getSift_method2(siftOfimg, matSiftAllImages_m2, vec_Desc_Imgfile_m2,0,query_image_name);
    buildAllMat_method2(matTotalColor_m2,matTotalSift_m2,matTotalDesc_m2,query_image_name);
    queryMat=matTotalDesc_m2;
    imageRetrival(m_index,queryMat,query_image_name);
      cout<<matTotalDesc_m2.rows<<"  "<<matTotalDesc_m2.cols<<endl;
    cout<<"opencv...hehe"<<endl;
    return 0;
}

