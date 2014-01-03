#include <iostream>
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#define IMAGE_NUM 9
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
Mat queryMat;

void checkHSV(double h, double s, double v,int i, int j,Mat& Hist);
double Eudist(Mat oneSift, Mat oneCenter){
    double dist=0;
    for(int i =0;i<128;i++)
        dist+=(oneSift.at<float>(0,i)-oneCenter.at<float>(0,i))*(oneSift.at<float>(0,i)-oneCenter.at<float>(0,i));

    return dist;
}

Mat colorHist(const std::vector<std::string>& filenames) {
    Mat matTotalDesc;
    for(auto &filename : filenames) {
        IplImage* src=cvLoadImage(filename.c_str());
        IplImage* hsv=cvCreateImage(cvGetSize(src), 8, 3);
        Mat Hist=Mat::zeros(1,256,CV_32F);
        cvCvtColor( src, hsv, CV_BGR2HSV );

        double h,s,v;
        float pix_sum=src->height*src->width;

        for(int i=0;i<src->height;i++){
            for(int j=0;j<src->width;j++){
                h=cvGet2D(hsv,i,j).val[0]*2;
                s=cvGet2D(hsv,i,j).val[1]/255;
                v=cvGet2D(hsv,i,j).val[2]/255;
                checkHSV(h,s,v,i,j,Hist);
            }

        }
        Hist=Hist/pix_sum;
        matTotalDesc.push_back(Hist);
    }

    return matTotalDesc;
}

void imageRetrival(cv::flann::Index& m_index, const string query_image_name){
    clock_t start,end;
    start=clock();
    Mat QueryHist;
    Mat QueryMabtDesc;
    Mat indices;
    Mat dists;
   
    m_index.knnSearch(queryMat, indices, dists, 2 );
    cout<<indices.at<int>(0,0)<<" index "<<indices.at<int>(0,1)<<endl;
    
    char queryName[20]="./food_2/";
    char queryNum[5];
    sprintf(queryNum,"%d",indices.at<int>(0,0));
    strcat(queryName,queryNum);
    strcat(queryName,".jpg");
    cout<<queryName<<endl;
    IplImage* query= cvLoadImage(queryName);
    cvNamedWindow("query image");
    cvShowImage("query image", query);
    for(int i =1;i<=5;i++){
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
void getSift_method1(Mat& matTotalDesc,vector<matDescToImgfile_method1>& vec_Desc_Imgfile){
    
    for(int i =0;i<IMAGE_NUM;i++){
        char fileName[20]="./food_2/";
        char num[5];
        sprintf(num,"%d",i);
        strcat(fileName,num);
        strcat(fileName,".jpg");
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
    }
}


Mat getSift_method2(Mat& matTotalSift,vector<matDescToImgfile_method2>& vec_Desc_Imgfile){
    matDescToImgfile_method2 one_img_log ;
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
        Mat SiftClassPerImage=Mat::zeros(IMAGE_NUM,CLUSTER_NUM_METHOD2,CV_32F);
        BOWKMeansTrainer bowK(CLUSTER_NUM_METHOD2,cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1),3,KMEANS_PP_CENTERS);
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


void buildAllMat_method2(Mat& matTotalColor,Mat& matTotalSift,Mat& matTotalDesc){
    matTotalDesc=matTotalColor.t();
    Mat tempSift=matTotalSift.t();
    for(int i =0;i<CLUSTER_NUM_METHOD2;i++){
        matTotalDesc.push_back(tempSift.row(i));
    }
    matTotalDesc=matTotalDesc.t();
    queryMat=matTotalDesc.row(1);
    cout<<matTotalDesc.rows<<"   "<<matTotalDesc.cols<<endl;
    FileStorage fs("./food_2/Feature_Mat_2.xml", FileStorage::WRITE);
    fs<<"Feature_Mat_2"<<matTotalDesc;
    fs.release();
}

void buildAllMat_method1(Mat& matTotalColor,Mat& matTotalSift,Mat& matTotalDesc){
    matTotalDesc=matTotalColor.t();
    Mat tempSift=matTotalSift.t();
    for(int i =0;i<128*CLUSTER_NUM_METHOD1;i++){
        matTotalDesc.push_back(tempSift.row(i));
    }
    matTotalDesc=matTotalDesc.t();
    queryMat=matTotalDesc.row(1);
    cout<<matTotalDesc.rows<<"   "<<matTotalDesc.cols<<endl;
    FileStorage fs("./food_2/Feature_Mat_1.xml", FileStorage::WRITE);
    fs<<"Feature_Mat_1"<<matTotalDesc;
    fs.release();
}



