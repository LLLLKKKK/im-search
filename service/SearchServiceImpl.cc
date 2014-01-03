
#include <iostream>
#include <opencv2/opencv.hpp>
#include "SearchServiceImpl.h"

using namespace google::protobuf;

void SearchServiceImpl::Search(RpcController *controller, const SearchRequest *request, 
                               SearchResponse *response, Closure *done)
{
    printRequest(request);

    

    Result * result = response->add_result();
    result->set_url("aaa.jpg");
    result->set_title("bbb");

    done->Run();
}

bool SearchServiceImpl::init() {
    FileStorage fs("Feature_Mat_1.xml", FileStorage::READ);
    Mat matTotalDesc;
    fs["Feature_Mat_1"] >> matTotalDesc;
    index = new cv::flann::Index(matTotalDesc, cv::flann::KDTreeIndexParams(4));
}

void SearchServiceImpl::printRequest(const SearchRequest *request) {
    std::cout << "request: " << request->query() 
              << " page_num: " << request->page_number() << std::endl;
}

void SearchServiceImpl::doSearch(const string& imageName){
    clock_t start,end;
    start = clock();
    Mat QueryHist;
    Mat QueryMabtDesc;
    Mat indices;
    Mat dists;
   
    index->knnSearch(queryMat, indices, dists, 2 );
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
    }
    end=clock();
    cout<< (double)(end - start) / CLOCKS_PER_SEC<<endl;
    cvWaitKey();    
}
