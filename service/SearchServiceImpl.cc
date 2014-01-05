

#include <iostream>
#include <opencv2/opencv.hpp>
#include "SearchServiceImpl.h"
#include "cv/CV.h"

using namespace std;
using namespace google::protobuf;
using namespace cv;

void SearchServiceImpl::Search(RpcController *controller, const SearchRequest *request, 
                               SearchResponse *response, Closure *done)
{
    printRequest(request);
    doSearch(request->query(), response);

    done->Run();
}

bool SearchServiceImpl::init() {
    FileStorage fs("FeatureMat.xml", FileStorage::READ);
    Mat matTotalDesc;
    fs["FeatureMat"] >> matTotalDesc;
    index_ = new cv::flann::Index(matTotalDesc, cv::flann::KDTreeIndexParams(4));
    return true;
}

void SearchServiceImpl::printRequest(const SearchRequest *request) {
    std::cout << "request: " << request->query() 
              << " page_num: " << request->page_number() << std::endl;
}

void SearchServiceImpl::doSearch(const string& imageName, SearchResponse *response) {
    std::cout << imageName << std::endl;
    Mat matColor;
    Mat matSift;
    colorHist(matColor, imageName);
    getSift1(matSift, imageName);
    Mat query = combineFeatures(matColor, matSift);

    Mat indices;
    Mat dists;
    clock_t start,end;
    start=clock();   
    index_->knnSearch(query, indices, dists, 10);
    end=clock();
    cout<< (double)(end - start) / CLOCKS_PER_SEC<<endl;
    cout<<indices.at<int>(0,0)<<" index "<<indices.at<int>(0,1)<<endl;
    cout<<"indice size: "<<indices.cols<<endl;
    for(int i = 0; i < indices.cols; i++){
        Result *result = response->add_result();
        stringstream ss;
        ss << indices.at<int>(0, i);
        result->set_url(ss.str());
    }
}
    
    
