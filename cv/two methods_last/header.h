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
#define IMAGE_NUM 11
#define CLUSTER_NUM_METHOD1 20
#define CLUSTER_NUM_METHOD2 20
class matDescToImgfile_method2{
public:
    int begin_index;
    int end_index;
    char s_imge_filename[20] ;
};
class matDescToImgfile_method1{
public:
    int img_index;
    char s_imge_filename[20] ;
};
//Mat queryMat;
//Mat Hist1(1,256,CV_32F);
//Mat Hist2(1,256,CV_32F);
//Mat forSift(1,1280,CV_32F);
//Mat forAll(1,1536,CV_32F);
//int H[1000][1000],S[1000][1000], V[1000][1000],L[1000][1000];
//char *fileName[5]{"2.jpg","1.jpg","3.jpg","4.jpg","5.jpg"};
void checkHSV( vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&,double h, double s, double v,int i, int j,Mat& Hist);
double Eudist(Mat oneSift, Mat oneCenter){
    double dist=0;
    for(int i =0;i<128;i++)
        dist+=(oneSift.at<float>(0,i)-oneCenter.at<float>(0,i))*(oneSift.at<float>(0,i)-oneCenter.at<float>(0,i));

    return dist;
}

void colorHist(Mat& matTotalDesc,int k,char *queryName="-1"){
    
    
   // for(int k=0;k<IMAGE_NUM;k++){
       // int H[1000][1000],S[1000][1000], V[1000][1000],L[1000][1000];
        char fileName[20]="./food_2/";
        char num[5];
        sprintf(num,"%d",k);
        strcat(fileName,num);
        strcat(fileName,".jpg");
        if(strcmp(queryName,"-1")!=0)
            strcpy(fileName,queryName);
        cout<<fileName<<endl;
        
        IplImage* src=cvLoadImage(fileName);
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
        // int count=0;
    
        for(int i=0;i<src->height;i++){
            
            for(int j=0;j<src->width;j++){
               
                h=cvGet2D(hsv,i,j).val[0]*2;
                s=cvGet2D(hsv,i,j).val[1]/255;
                v=cvGet2D(hsv,i,j).val[2]/255;
                checkHSV(H,S,V,L,h,s,v,i,j,Hist);
                
            }
        }
        Hist=Hist/pix_sum;
        
        matTotalDesc.push_back(Hist);   //拼接颜色向量到一个矩阵里
        
        
   // }
    
}
void imageRetrival(cv::flann::Index& m_index, Mat queryMat, char *queryName){
    clock_t start,end;
    start=clock();
    Mat QueryHist;
    Mat QueryMatDesc;
    Mat indices;
    Mat dists;
   
    m_index.knnSearch(queryMat, indices, dists, 3 );
    cout<<indices.at<int>(0,0)<<" index "<<indices.at<int>(0,1)<<endl;
    
    cout<<"In retrival query name: "<<queryName<<endl;
    IplImage* query= cvLoadImage(queryName);
    cvNamedWindow("query image");
    cvShowImage("query image", query);
    cout<<"indice size: "<<indices.cols<<endl;
    for(int i =0;i<indices.cols;i++){
        char resultName[20]="./food_2/";
        char num[5];
        sprintf(num,"%d",indices.at<int>(0,i));
        strcat(resultName,num);
        strcat(resultName,".jpg");
        cout<<"result: "<<resultName<<endl;
        
        IplImage* result= cvLoadImage(resultName);
        
        cvNamedWindow(strcat(resultName," match"));
        cvShowImage(resultName, result);
        // cvWaitKey(1000);
    }
    // sleep(1000);
    end=clock();
    cout<< (double)(end - start) / CLOCKS_PER_SEC<<endl;
    cvWaitKey();
    
    
}
void getSift_method1(Mat& matTotalDesc,vector<matDescToImgfile_method1>& vec_Desc_Imgfile,int i,char *queryName="-1"){
    
   // for(int i =0;i<IMAGE_NUM;i++){
        char fileName[20]="./food_2/";
        char num[5];
        sprintf(num,"%d",i);
        strcat(fileName,num);
        strcat(fileName,".jpg");
        if(strcmp(queryName,"-1")!=0)
            strcpy(fileName,queryName);
        cout<<fileName<<endl;
        
        
        Mat input = imread(fileName);
        matDescToImgfile_method1 one_img_log ;
        SiftFeatureDetector detector;
        SiftDescriptorExtractor extractor;
        vector<KeyPoint> keypoint;
        detector.detect(input,keypoint);
        Mat descriptor;
        extractor.compute(input,keypoint,descriptor);
        
        //int clusterNum=10;
        
        BOWKMeansTrainer bowK(CLUSTER_NUM_METHOD1,cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1),3,KMEANS_PP_CENTERS);
        cout<<"rows: "<<descriptor.rows<<"   cols: "<<descriptor.cols<<endl;
        Mat center = bowK.cluster(descriptor);
        // cout<<"cluster num: "<<center.rows<<endl;
        // cout<<"cluster dimension: "<<center.cols<<endl;
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
        matTotalDesc.push_back(tenSift);   //拼接每张图片sift到一个矩阵里
        one_img_log.img_index=i;
        //  one_img_log.s_imge_filename = fileName;s
        strcpy(one_img_log.s_imge_filename, fileName);
        vec_Desc_Imgfile.push_back(one_img_log) ;
        
        
        //  cout<<matTotalDesc.rows<<"   "<<matTotalDesc.cols<<endl;
    //}
}

Mat getSift_method2(vector<int> & siftOfimg,Mat& matTotalSift,vector<matDescToImgfile_method2>& vec_Desc_Imgfile,int i,
                     char *queryName="-1")
{
    matDescToImgfile_method2 one_img_log ;
       // vector<int> siftOfimg;
    // for(int i =0;i<IMAGE_NUM;i++){
        char fileName[20]="./food_2/";
        char num[5];
        sprintf(num,"%d",i);
        strcat(fileName,num);
        strcat(fileName,".jpg");
        if(strcmp(queryName,"-1")!=0)
            strcpy(fileName,queryName);
        cout<<fileName<<endl;
        Mat center;
        Mat input = imread(fileName);
        Mat SiftClassPerImage=Mat::zeros(IMAGE_NUM,CLUSTER_NUM_METHOD2,CV_32F);
        SiftFeatureDetector detector;
        SiftDescriptorExtractor extractor;
        vector<KeyPoint> keypoint;
        detector.detect(input,keypoint);
        Mat descriptor;
        extractor.compute(input,keypoint,descriptor);
        for(int j=0;j<descriptor.rows;j++){
            siftOfimg.push_back(i);
        }
        
        matTotalSift.push_back(descriptor);   //拼接每张图片sift到一个矩阵里
        if(i==0){
            one_img_log.begin_index=0;
            one_img_log.end_index=descriptor.rows-1;
            strcpy(one_img_log.s_imge_filename, fileName);
            vec_Desc_Imgfile.push_back(one_img_log) ;
        }
        else{
            one_img_log.begin_index=matTotalSift.rows;
            one_img_log.end_index=one_img_log.begin_index+descriptor.rows-1;
            strcpy(one_img_log.s_imge_filename, fileName);
            vec_Desc_Imgfile.push_back(one_img_log) ;
        }
  //  }
    
    if(i==IMAGE_NUM-1||strcmp(queryName,"-1")!=0){       //全部图片提取完了，弄每个图片的sift表示了
        if(strcmp(queryName,"-1")==0){
            BOWKMeansTrainer bowK(CLUSTER_NUM_METHOD2,cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1),3,KMEANS_PP_CENTERS);
            cout<<"rows: "<<matTotalSift.rows<<"   cols: "<<matTotalSift.cols<<endl;
            center = bowK.cluster(matTotalSift);
            cout<<"cluster num: "<<center.rows<<endl;
            cout<<"cluster dimension: "<<center.cols<<endl;
            FileStorage fs("./food_2/Feature_Mat_2_center.xml", FileStorage::WRITE);
            fs<<"Feature_Mat_2_center"<<center;
            fs.release();
        }
        else {
            FileStorage fss("./food_2/Feature_Mat_2_center.xml", FileStorage::READ);
            fss["Feature_Mat_2_center"] >> center;
            Mat SiftClassQuery=Mat::zeros(1,CLUSTER_NUM_METHOD2,CV_32F);
            cout<<"center:: "<<center.rows<<"   "<<center.cols<<endl;
            double min_dist;int min_center;
            cout<<matTotalSift.rows<<" mattotoal si "<<matTotalSift.cols<<endl;
            for(long j=0;j<matTotalSift.rows;j++){      //找每个sift和哪个中心比较近
                for(int k =0;k<center.rows;k++){
                    double distance = Eudist(matTotalSift.row(j),center.row(k));
                    if(k==0){
                        min_dist=distance; min_center=0;
                    }
                    if(distance<min_dist){
                        min_dist=distance;min_center=k;
                        
                    }
                }
                //cout<<"min_center"<<min_center<<endl;
                SiftClassQuery.at<float>(0,min_center)=SiftClassQuery.at<float>(0,min_center)+1;
                
            }
           // cout<<"before guiyihua "<<SiftClassQuery<<endl;
            float sift_sum;
            for(int j=0;j<  SiftClassQuery.rows;j++){     //归一化
                sift_sum=0;
                for(int k =0;k< SiftClassQuery.cols;k++){
                    sift_sum+= SiftClassQuery.at<float>(j,k);
                }
               SiftClassQuery= SiftClassQuery/sift_sum;
                
            }
            cout<<"sift this query"<< SiftClassQuery<<endl;
            return  SiftClassQuery;

        }
       
    double min_dist;int min_center;
        cout<<matTotalSift.rows<<" mattotoal si "<<matTotalSift.cols<<endl;
    for(long j=0;j<matTotalSift.rows;j++){      //找每个sift和哪个中心比较近
        for(int k =0;k<center.rows;k++){
            double distance = Eudist(matTotalSift.row(j),center.row(k));
            if(k==0){
                min_dist=distance; min_center=0;
            }
            if(distance<min_dist){
                min_dist=distance;min_center=k;
                
            }
        }
        SiftClassPerImage.at<float>(siftOfimg[j],min_center)=SiftClassPerImage.at<float>(siftOfimg[j],min_center)+1;
    
    }
        
    float sift_sum;
    for(int j=0;j< SiftClassPerImage.rows;j++){     //归一化
        sift_sum=0;
        for(int k =0;k<SiftClassPerImage.cols;k++){
            sift_sum+=SiftClassPerImage.at<float>(j,k);
        }
        SiftClassPerImage=SiftClassPerImage/sift_sum;
        
    }
    cout<<"sift per image"<<SiftClassPerImage<<endl;
    return SiftClassPerImage;

    }
    
    else
        return Mat::zeros(IMAGE_NUM,CLUSTER_NUM_METHOD2,CV_32F);
        
}


void buildAllMat_method2(Mat& matTotalColor,Mat& matTotalSift,Mat& matTotalDesc,char *query_image_name="-1"){
    matTotalDesc=matTotalColor.t();
    Mat tempSift=matTotalSift.t();
    for(int i =0;i<CLUSTER_NUM_METHOD2;i++){
        matTotalDesc.push_back(tempSift.row(i));
    }
    matTotalDesc=matTotalDesc.t();
   // queryMat=matTotalDesc.row(1);
    cout<<matTotalDesc.rows<<"   "<<matTotalDesc.cols<<endl;
    cout<<matTotalDesc<<endl;
    if(strcmp(query_image_name,"-1")==0){
        FileStorage fs("./food_2/Feature_Mat_2.xml", FileStorage::WRITE);
        fs<<"Feature_Mat_2"<<matTotalDesc;
        fs.release();
    }
    
}

void buildAllMat_method1(Mat& matTotalColor,Mat& matTotalSift,Mat& matTotalDesc,char *query_image_name="-1"){
    matTotalDesc=matTotalColor.t();
    Mat tempSift=matTotalSift.t();
    for(int i =0;i<128*CLUSTER_NUM_METHOD1;i++){
        matTotalDesc.push_back(tempSift.row(i));
    }
    matTotalDesc=matTotalDesc.t();
   // queryMat=matTotalDesc.row(1);
    cout<<matTotalDesc.rows<<"   "<<matTotalDesc.cols<<endl;
    if(strcmp(query_image_name,"-1")==0){
        FileStorage fs("./food_2/Feature_Mat_1.xml", FileStorage::WRITE);
        fs<<"Feature_Mat_1"<<matTotalDesc;
        fs.release();
    }
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


