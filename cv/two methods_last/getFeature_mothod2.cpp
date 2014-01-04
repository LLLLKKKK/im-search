#include "header.h"
#include <iostream>
using namespace std;
using namespace cv;

//Get features of all images and store it into a xml file.
int main(){
    Mat matTotalColor_m2;
    Mat matTotalSift_m2;
    Mat matTotalDesc_m2;
    Mat matSiftAllImages_m2;
    vector<int> siftOfimg;
    vector<matDescToImgfile_method2> vec_Desc_Imgfile_m2 ;
    for(int k=0;k<IMAGE_NUM;k++)
        colorHist(matTotalColor_m2,k);
   // char* query_image_name="1.jpg";
    for(int k =0;k<IMAGE_NUM;k++)
       matTotalSift_m2=getSift_method2(siftOfimg, matSiftAllImages_m2, vec_Desc_Imgfile_m2,k);
  
    buildAllMat_method2(matTotalColor_m2,matTotalSift_m2,matTotalDesc_m2);
    cout<<"hang: "<<matTotalDesc_m2.rows<<"   lie: "<<matTotalDesc_m2.cols<<endl;    
    cout<<"opencv...hehe"<<endl;
    return 0;
}



