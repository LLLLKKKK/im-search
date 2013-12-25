#include "highgui.h"
#include "features2d.hpp"
#include "cxcore.h"
#include "cv.h"
#include <iostream>
#include "nonfree/features2d.hpp"
#include <stdio.h>
#include<string>
using namespace std;
using namespace cv;
#define IMAGE_NUM 1000
class matDescToImgfile{
public:
    int img_index;
    char s_imge_filename[20] ;
};
Mat Hist1(1,256,CV_32F);
Mat Hist2(1,256,CV_32F);
Mat forSift(1,1280,CV_32F);
Mat forAll(1,1536,CV_32F);
int H[500][500],S[500][500], V[500][500],L[500][500];
//char *fileName[5]{"2.jpg","1.jpg","3.jpg","4.jpg","5.jpg"};
void checkHSV(double h, double s, double v,int i, int j,Mat& Hist);
void colorHist(Mat& matTotalDesc,vector<matDescToImgfile>& vec_Desc_Imgfile){
   
    int img_num=0;
    for(int k=0;k<IMAGE_NUM;k++){
        char fileName[20]="./food/";
        char num[5];
        sprintf(num,"%d",k);
        strcat(fileName,num);
         strcat(fileName,".jpg");
            cout<<fileName<<endl;

        IplImage* src=cvLoadImage(fileName);
       
        matDescToImgfile one_img_log ; 
        IplImage* hsv=cvCreateImage(cvGetSize(src), 8, 3);
        Mat Hist=Mat::zeros(1,256,CV_32F);
        cvCvtColor( src, hsv, CV_BGR2HSV );
       
       
        
        double h,s,v;
        float pix_sum=src->height*src->width;
       // int count=0;
        for(int i=0;i<src->height;i++){
            for(int j=0;j<src->width;j++){
                h=cvGet2D(hsv,i,j).val[0]*2;
                s=cvGet2D(hsv,i,j).val[1]/255; 
                v=cvGet2D(hsv,i,j).val[2]/255; 
                checkHSV(h,s,v,i,j,Hist);
            }

        }
        
        if(k==1)
            Hist1=Hist;
        if(k==2)
            Hist2=Hist;
        Hist=Hist/pix_sum;
        
  //  cout<<"The color feature: "<<Hist<<endl;
        matTotalDesc.push_back(Hist);   //拼接颜色向量到一个矩阵里
        
       // Hist.row(3) = Hist.row(3) + Hist.row(5)*3;
      //  Hist.col(3);
      // cout<< Hist.at<int>(3,0);
     /*   one_img_log.img_index=k;
        one_img_log.s_imge_filename = fileName[k];
        img_num++;
        vec_Desc_Imgfile.push_back(one_img_log) ;
        */
     
    }
        
}
void imageRetrival(cv::flann::Index& m_index, const string query_image_name ,
    vector<matDescToImgfile>& vec_Desc_Imgfile){

    Mat QueryHist;
    Mat QueryMatDesc;
    Mat indices;
    Mat dists;
   // vector<int> indices;
   // vector<int> dists;
   // cout<<forSift.rows<<"    "<<forSift.cols<<endl;
   // cout<<forSift<<endl;
    m_index.knnSearch(forAll, indices, dists, 2 );
    cout<<indices.at<int>(0,0)<<" index "<<indices.at<int>(0,1)<<endl;
    //cout<<indices[0]<<" for index  "<<indices[1]<<endl;
    //cout<<dists[0]<<"  for dist   "<<dists[1]<<endl;
    cout<<vec_Desc_Imgfile[indices.at<int>(0,0)].s_imge_filename<<endl;
    IplImage* query= cvLoadImage(vec_Desc_Imgfile[indices.at<int>(0,0)].s_imge_filename);
    cout<<vec_Desc_Imgfile[indices.at<int>(0,0)].s_imge_filename<<endl;

    IplImage* result= cvLoadImage(vec_Desc_Imgfile[indices.at<int>(0,1)].s_imge_filename);
    cout<<vec_Desc_Imgfile[indices.at<int>(0,1)].s_imge_filename<<endl;
    cvNamedWindow("query image");
    cvShowImage("query image", query);
    cvNamedWindow("match result");
    cvShowImage("match result", result);
    cvWaitKey();
   // string s_the_result_filename ;

   // s_the_result_filename = GetImageFromKNN(vec_Desc_Imgfile,indices , vec_Desc_Imgfile.size() ,MIN_Image_Similar_Threshold) ;    
    



}
void getSift(Mat& matTotalDesc,vector<matDescToImgfile>& vec_Desc_Imgfile){
    
    for(int i =0;i<IMAGE_NUM;i++){
        char fileName[20]="./food/";
        char num[5];
        sprintf(num,"%d",i);
        strcat(fileName,num);
        strcat(fileName,".jpg");
        cout<<fileName<<endl;
        
      
        Mat input = imread(fileName);
        matDescToImgfile one_img_log ; 
        SiftFeatureDetector detector;
        SiftDescriptorExtractor extractor;
        vector<KeyPoint> keypoint;
        detector.detect(input,keypoint);
        Mat descriptor;
        extractor.compute(input,keypoint,descriptor);
    
        int clusterNum=10;
       
        BOWKMeansTrainer bowK(clusterNum,cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1),3,KMEANS_PP_CENTERS);
        cout<<"rows: "<<descriptor.rows<<"   cols: "<<descriptor.cols<<endl;
        Mat center = bowK.cluster(descriptor);
       // cout<<"cluster num: "<<center.rows<<endl;
       // cout<<"cluster dimension: "<<center.cols<<endl;
        Mat tenSift=Mat::zeros(1,1280,CV_32F);
        float sift_sum;
        for(int j =0;j<10;j++){
            sift_sum=0;
            for(int k =0;k<128;k++){
                tenSift.at<float>(0,j*128+k)=center.at<float>(j,k);
                sift_sum+=center.at<float>(j,k);
            }
        }
        tenSift=tenSift/sift_sum;
        if(i==1)
            forSift=tenSift;
        matTotalDesc.push_back(tenSift);   //拼接每张图片sift到一个矩阵里
        one_img_log.img_index=i;
      //  one_img_log.s_imge_filename = fileName;s
        strcpy(one_img_log.s_imge_filename, fileName);
        vec_Desc_Imgfile.push_back(one_img_log) ;
        
        
      //  cout<<matTotalDesc.rows<<"   "<<matTotalDesc.cols<<endl;
    }
}
void buildAllMat(Mat& matTotalColor,Mat& matTotalSift,Mat& matTotalDesc,vector<matDescToImgfile>& vec_Desc_Imgfile){
   matTotalDesc=matTotalColor.t();
    Mat tempSift=matTotalSift.t();
    for(int i =0;i<1280;i++){
        matTotalDesc.push_back(tempSift.row(i));
    }
    matTotalDesc=matTotalDesc.t();
    forAll=matTotalDesc.row(1);
    cout<<matTotalDesc.rows<<"   "<<matTotalDesc.cols<<endl;
    FileStorage fs("./food/Test.xml", FileStorage::WRITE);
    fs<<"Test"<<matTotalDesc;
    fs.release();
}

int main(){
    Mat matTotalColor;
    Mat matTotalSift;
    Mat matTotalDesc;
    vector<matDescToImgfile> vec_Desc_Imgfile ;
   colorHist(matTotalColor,vec_Desc_Imgfile);
    string query_image_name="1.jpg";
    getSift(matTotalSift,vec_Desc_Imgfile);
    buildAllMat(matTotalColor,matTotalSift,matTotalDesc,vec_Desc_Imgfile);
    cout<<"hang: "<<matTotalDesc.rows<<"   lie: "<<matTotalDesc.cols<<endl;
    //建立kd-tree
    //cout<<Hist1<<endl;
    cv::flann::Index m_index(matTotalDesc, cv::flann::KDTreeIndexParams(4));
    imageRetrival(m_index,query_image_name,vec_Desc_Imgfile);
    
    cout<<"opencv...hehe"<<endl;
    return 0;
}

void checkHSV(double h, double s, double v,int i, int j,Mat &Hist){
    if(h<=15||h>345)
        H[i][j]=0;
    else if(h<=25&&h>15)
        H[i][j]=1 ;
    else if(h<=45&&h>25)
        H[i][j]=2 ;
    else if(h<=55&&h>45)
        H[i][j]=3 ;
    else if(h<=80&&h>55)
        H[i][j]=4 ;
    else if(h<=108&&h>80)
        H[i][j]=5 ;
    else if(h<=140&&h>108)
        H[i][j]=6 ;
    else if(h<=165&&h>140)
        H[i][j]= 7;
    else if(h<=190&&h>165)
        H[i][j]= 8;
    else if(h<=220&&h>190)
        H[i][j]=9 ;
    else if(h<=255&&h>220)
        H[i][j]=10 ;
    else if(h<=275&&h>255)
        H[i][j]=11 ;
    else if(h<=290&&h>275)
        H[i][j]=12 ;
    else if(h<=316&&h>290)
        H[i][j]=13 ;
    else if(h<=330&&h>316)
        H[i][j]=14 ;
    else if(h<=345&&h>330)
        H[i][j]=15 ;

    if(s<=0.15&&s>0)
        S[i][j]=0;
    else if(s<=0.4&&s>0.15)
        S[i][j]=1;
    else if(s<=0.75&&s>0.4)
        S[i][j]=2;
    else if(s<=1&&s>0.75)
        S[i][j]=3;

    if(v<=0.15&&v>0)
        V[i][j]=0;
    else if(v<=0.4&&v>0.15)
        V[i][j]=1;
    else if(v<=0.75&&v>0.4)
        V[i][j]=2;
    else if(v<=1&&v>0.75)
        V[i][j]=3;
    L[i][j]=H[i][j]*16+S[i][j]*4+V[i][j];
    Hist.at<float>(0,L[i][j])=Hist.at<float>(0,L[i][j])+1;
   
}


