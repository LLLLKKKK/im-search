
#ifndef _SEARCHSERVICEIMPL_H_
#define _SEARCHSERVICEIMPL_H_

#include "Search.pb.h"

namespace cv {
namespace flann {
class Index;
};
};

class SearchServiceImpl : public SearchService {
public:
    bool init();

public:
    void Search(google::protobuf::RpcController *controller,
                const  SearchRequest *request, SearchResponse *response,  
                google::protobuf::Closure *done);

private:
    void doSearch(const std::string& imageName, SearchResponse *response);
    void printRequest(const SearchRequest *request);
private:
    cv::flann::Index *index_;
};

#endif
