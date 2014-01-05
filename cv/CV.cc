
#include <iostream>
#include <opencv2/opencv.hpp>
#include "CV.h"

double Eudist(const Mat &oneSift, const Mat &oneCenter){
    double dist=0;
    for(int i =0;i<128;i++) {
        dist+=(oneSift.at<float>(0,i)-oneCenter.at<float>(0,i))*(oneSift.at<float>(0,i)-oneCenter.at<float>(0,i));
    }
    return dist;
}

void checkHSV(vector<vector<int>>& H, vector<vector<int>>& S, vector<vector<int>>& V, vector<vector<int>>& L,double h, double s, double v,int i, int j,Mat& Hist){
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

void colorHist(Mat &matTotalDesc, const string &filename) {
    IplImage* src=cvLoadImage(filename.c_str());
    IplImage* hsv=cvCreateImage(cvGetSize(src), 8, 3);
    Mat Hist=Mat::zeros(1,256,CV_32F);
    cvCvtColor( src, hsv, CV_BGR2HSV );
    
    vector<vector<int>> H(src->height);
    vector<vector<int>> S(src->height);
    vector<vector<int>> V(src->height);
    vector<vector<int>> L(src->height);
    for(int kk=0;kk<src->height;kk++){
        H[kk].resize(src->width);
        S[kk].resize(src->width);
        V[kk].resize(src->width);
        L[kk].resize(src->width);
    }
        
    double h,s,v;
    float pix_sum=src->height*src->width;
    
    for(int i=0;i<src->height;i++){
            
        for(int j=0;j<src->width;j++){
               
            h=cvGet2D(hsv,i,j).val[0]*2;
            s=cvGet2D(hsv,i,j).val[1]/255;
            v=cvGet2D(hsv,i,j).val[2]/255;
            checkHSV(H,S,V,L,h,s,v,i,j,Hist);
                
        }
    }
    Hist=Hist/pix_sum;
        
    matTotalDesc.push_back(Hist);
}

void getSift1(Mat& matTotalDesc, const string &filename) {
    
    Mat input = imread(filename.c_str());
    SiftFeatureDetector detector;
    SiftDescriptorExtractor extractor;
    vector<KeyPoint> keypoint;
    detector.detect(input,keypoint);
    Mat descriptor;
    extractor.compute(input,keypoint,descriptor);
    
    BOWKMeansTrainer bowK(CLUSTER_NUM_METHOD1,cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1),3,KMEANS_PP_CENTERS);
    cout<<"rows: "<<descriptor.rows<<"   cols: "<<descriptor.cols<<endl;
    Mat center = bowK.cluster(descriptor);
    
    Mat tenSift=Mat::zeros(1,128*CLUSTER_NUM_METHOD1,CV_32F);
    float sift_sum;
    for(int j =0;j<CLUSTER_NUM_METHOD1;j++){
        sift_sum=0;
        for(int k =0;k<128;k++){
            tenSift.at<float>(0,j*128+k)=center.at<float>(j,k);
            sift_sum+=center.at<float>(j,k);
        }
    }
    tenSift=tenSift/sift_sum;
    matTotalDesc.push_back(tenSift);
}

Mat combineFeatures(const Mat &matColor, const Mat &matSift) {
    Mat matTotalDesc = matColor.t();
    Mat tempSift = matSift.t();
    for(int i =0;i<128*CLUSTER_NUM_METHOD1;i++){
        matTotalDesc.push_back(tempSift.row(i));
    }
    matTotalDesc=matTotalDesc.t();
    return matTotalDesc;
}
