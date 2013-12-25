
#include <iostream>
#include "SearchServiceImpl.h"

using namespace google::protobuf;

void SearchServiceImpl::Search(RpcController *controller, const SearchRequest *request, 
                               SearchResponse *response, Closure *done)
{
    printRequest(request);

    // search ....

    Result * result = response->add_result();
    result->set_url("aaa.jpg");
    result->set_title("bbb");

    done->Run();
}

void SearchServiceImpl::printRequest(const SearchRequest *request) {
    std::cout << "request: " << request->query() 
              << " page_num: " << request->page_number() << std::endl;
}
