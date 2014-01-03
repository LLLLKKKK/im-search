
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Build.h"

using namespace std;
using namespace cv;

void checkHSV(double h, double s, double v, int i, int j, Mat& hist) {
    static int H[1000][1000] = { 0 };
    static int S[1000][1000] = { 0 };
    static int V[1000][1000] = { 0 };
    static int L[1000][1000] = { 0 };

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
    hist.at<float>(0,L[i][j])=hist.at<float>(0,L[i][j])+1;
}

Mat colorHist(const vector<string>& filenames) {
    Mat matTotalDesc;
    for(auto &filename : filenames) {
        IplImage* src=cvLoadImage(filename.c_str());
        assert(src);
        IplImage* hsv=cvCreateImage(cvGetSize(src), 8, 3);
        assert(hsv);
        cvCvtColor(src, hsv, CV_BGR2HSV);
        double h,s,v;
        float pix_sum=src->height*src->width;
        Mat hist = Mat::zeros(1,256,CV_32F);
        cvCvtColor( src, hsv, CV_BGR2HSV );
        for(int i=0;i<src->height;i++){
            for(int j=0;j<src->width;j++){
                h=cvGet2D(hsv,i,j).val[0]*2;
                s=cvGet2D(hsv,i,j).val[1]/255;
                v=cvGet2D(hsv,i,j).val[2]/255;
                checkHSV(h,s,v,i,j,hist);
            }
        }
        hist = hist / pix_sum;
        matTotalDesc.push_back(hist);
    }

    return matTotalDesc;
}

Mat getSift1(const vector<string> &filenames) {
    Mat matTotalSift;
    for(auto &filename : filenames) {
        Mat input = imread(filename);
        SiftFeatureDetector detector;
        SiftDescriptorExtractor extractor;
        vector<KeyPoint> keypoint;
        detector.detect(input, keypoint);
        Mat descriptor;
        extractor.compute(input, keypoint, descriptor);
        cout<<"rows: "<<descriptor.rows<<"   cols: "<<descriptor.cols<<endl;
        
        BOWKMeansTrainer bowK(CLUSTER_NUM_METHOD1,
                cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1),
                3, KMEANS_PP_CENTERS);
        Mat center = bowK.cluster(descriptor);
        Mat tenSift = Mat::zeros(1, 128 * CLUSTER_NUM_METHOD1, CV_32F);

        float sift_sum;
        for(int j =0;j < CLUSTER_NUM_METHOD1;j++){
            sift_sum=0;
            for(int k = 0; k < 128; k++){
                tenSift.at<float>(0,j*128+k)=center.at<float>(j,k);
                sift_sum+=center.at<float>(j,k);
            }
        }
        tenSift=tenSift/sift_sum;
        matTotalSift.push_back(tenSift);
    }
    return matTotalSift;
}

void build1(const Mat& matTotalColor, const Mat& matTotalSift){
    Mat matTotalDesc = matTotalColor.t();
    Mat tempSift = matTotalSift.t();
    for(int i =0 ;i < 128*CLUSTER_NUM_METHOD1; i++){
        matTotalDesc.push_back(tempSift.row(i));
    }
    matTotalDesc = matTotalDesc.t();
    cout<<matTotalDesc.rows<<"   "<<matTotalDesc.cols<< endl;
    FileStorage fs("Feature_Mat_1.xml", FileStorage::WRITE);
    fs << "Feature_Mat_1" << matTotalDesc;
    fs.release();
}
