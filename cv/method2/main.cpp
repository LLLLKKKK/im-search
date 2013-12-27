#include <highgui.h>
#include <features2d.hpp>
#include <cxcore.h>
#include <cv.h>
#include <iostream>
#include <nonfree/features2d.hpp>
#include <stdio.h>
#include <string>
using namespace std;
using namespace cv;
#define IMAGE_NUM 11
#define CLUSTER_NUM 20
class matDescToImgfile{
public:
    int begin_index;
    int end_index;
    char s_imge_filename[20] ;
};

Mat forAll;
Mat Hist1(1,256,CV_32F);
Mat Hist2(1,256,CV_32F);
//Mat forSift(1,1280,CV_32F);
//Mat forAll(1,1536,CV_32F);
int H[1000][1000],S[1000][1000], V[1000][1000],L[1000][1000];
//char *fileName[5]{"2.jpg","1.jpg","3.jpg","4.jpg","5.jpg"};
void checkHSV(double h, double s, double v,int i, int j,Mat& Hist);
double Eudist(Mat oneSift, Mat oneCenter){
    double dist=0;
    for(int i =0;i<128;i++)
        dist+=(oneSift.at<float>(0,i)-oneCenter.at<float>(0,i))*(oneSift.at<float>(0,i)-oneCenter.at<float>(0,i));

    return dist;
}

void colorHist(Mat& matTotalDesc,vector<matDescToImgfile>& vec_Desc_Imgfile){
    
    
    for(int k=0;k<IMAGE_NUM;k++){
        char fileName[20]="./food_2/";
        char num[5];
        sprintf(num,"%d",k);
        strcat(fileName,num);
        strcat(fileName,".jpg");
        cout<<fileName<<endl;
        
        IplImage* src=cvLoadImage(fileName);
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
    clock_t start,end;
    start=clock();
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
    char queryName[20]="./food_2/";
    char queryNum[5];
    sprintf(queryNum,"%d",indices.at<int>(0,0));
    strcat(queryName,queryNum);
    strcat(queryName,".jpg");
    cout<<queryName<<endl;
    IplImage* query= cvLoadImage(queryName);
    cvNamedWindow("query image");
    cvShowImage("query image", query);
    for(int i =1;i<=1;i++){
        char resultName[20]="./food_2/";
        char num[5];
        sprintf(num,"%d",indices.at<int>(0,i));
        strcat(resultName,num);
        strcat(resultName,".jpg");
        cout<<resultName<<endl;
        
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
Mat getSift(Mat& matTotalSift,vector<matDescToImgfile>& vec_Desc_Imgfile){
    matDescToImgfile one_img_log ;
    vector<int> siftOfimg;
    for(int i =0;i<IMAGE_NUM;i++){
        char fileName[20]="./food_2/";
        char num[5];
        sprintf(num,"%d",i);
        strcat(fileName,num);
        strcat(fileName,".jpg");
        cout<<fileName<<endl;

        Mat input = imread(fileName);
        
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
       
    }
        Mat SiftClassPerImage=Mat::zeros(IMAGE_NUM,CLUSTER_NUM,CV_32F);
        BOWKMeansTrainer bowK(CLUSTER_NUM,cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1),3,KMEANS_PP_CENTERS);
        cout<<"rows: "<<matTotalSift.rows<<"   cols: "<<matTotalSift.cols<<endl;
        Mat center = bowK.cluster(matTotalSift);
         cout<<"cluster num: "<<center.rows<<endl;
         cout<<"cluster dimension: "<<center.cols<<endl;
    double min_dist;int min_center;
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
    cout<<SiftClassPerImage<<endl;
    return SiftClassPerImage;
      /*  Mat tenSift=Mat::zeros(1,128*CLUSTER_NUM,CV_32F);
        float sift_sum;
        for(int j =0;j<CLUSTER_NUM;j++){
            sift_sum=0;
            for(int k =0;k<128;k++){
                tenSift.at<float>(0,j*128+k)=center.at<float>(j,k);
                sift_sum+=center.at<float>(j,k);
            }
        }
        tenSift=tenSift/sift_sum;
        if(i==1)
            forSift=tenSift;
        matTotalSift.push_back(tenSift);   //拼接每张图片sift到一个矩阵里
        one_img_log.img_index=i;
        //  one_img_log.s_imge_filename = fileName;s
        strcpy(one_img_log.s_imge_filename, fileName);
        vec_Desc_Imgfile.push_back(one_img_log) ;
        */
        
    
    
}


void buildAllMat(Mat& matTotalColor,Mat& matTotalSift,Mat& matTotalDesc,vector<matDescToImgfile>& vec_Desc_Imgfile){
    matTotalDesc=matTotalColor.t();
    Mat tempSift=matTotalSift.t();
    for(int i =0;i<CLUSTER_NUM;i++){
        matTotalDesc.push_back(tempSift.row(i));
    }
    matTotalDesc=matTotalDesc.t();
    forAll=matTotalDesc.row(1);
    cout<<matTotalDesc.rows<<"   "<<matTotalDesc.cols<<endl;
    FileStorage fs("./food_2/Feature_Mat_2.xml", FileStorage::WRITE);
    fs<<"Feature_Mat_2"<<matTotalDesc;
    fs.release();
}

int main(){
    Mat matTotalColor;
    Mat matTotalSift;
    Mat matTotalDesc;
    Mat matSiftAllImages;
    vector<matDescToImgfile> vec_Desc_Imgfile ;
    colorHist(matTotalColor,vec_Desc_Imgfile);
    string query_image_name="1.jpg";
   matTotalSift= getSift(matSiftAllImages,vec_Desc_Imgfile);
    buildAllMat(matTotalColor,matTotalSift,matTotalDesc,vec_Desc_Imgfile);
    cout<<"hang: "<<matTotalDesc.rows<<"   lie: "<<matTotalDesc.cols<<endl;
    //建立kd-tree
    cout<<Hist1<<endl;
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


