
#ifndef _SEARCHSERVICEIMPL_H_
#define _SEARCHSERVICEIMPL_H_

#include "Search.pb.h"

class SearchServiceImpl : public SearchService
{
public:
    void Search(google::protobuf::RpcController *controller,
                const  SearchRequest *request, SearchResponse *response,  
                google::protobuf::Closure *done);
private:
    void printRequest(const SearchRequest *request);
};

#endif