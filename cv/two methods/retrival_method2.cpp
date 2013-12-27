#include "header.h"
#include <iostream>
using namespace std;
using namespace cv;

int main(){
    Mat matTotalColor;
    Mat matTotalSift;
    Mat matTotalDesc;
    vector<matDescToImgfile_method2> vec_Desc_Imgfile ;
  // colorHist(matTotalColor,vec_Desc_Imgfile);
    FileStorage fs("./food_2/Feature_Mat_2.xml", FileStorage::READ);
   
    fs["Feature_Mat_2"] >> matTotalDesc;
    cv::flann::Index m_index(matTotalDesc, cv::flann::KDTreeIndexParams(4));
    string query_image_name="1.jpg";
    for(;;){
        int queNum;
        cin>>queNum;
        if(queNum==-1)
            break;
        queryMat=matTotalDesc.row(queNum);
        imageRetrival(m_index,query_image_name);
    }
      cout<<matTotalDesc.rows<<"  "<<matTotalDesc.cols<<endl;
    cout<<"opencv...hehe"<<endl;
    return 0;
}

