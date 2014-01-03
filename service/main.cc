
#include <RCFProto.hpp>
#include "SearchServiceImpl.h"

using namespace RCF;

int main(int argc, char** argv) {

    RCF::init();

    RcfProtoServer server;    
    SearchServiceImpl searchServiceImpl;

    if (!searchServiceImpl.init()) {
        return -1;
    }

    server.bindService(searchServiceImpl);    
    server.addEndpoint(RCF::TcpEndpoint(50001));
    
    RCF::ThreadPoolPtr threadPoolPtr( new RCF::ThreadPool(1, 10) );
    threadPoolPtr->setThreadName("Image Search Server");
    server.setThreadPool(threadPoolPtr);
    
    server.start();
    
    while (true) {
        RCF::sleepMs(1000);
    }
        
    std::cout << "Server shutting down." << std::endl;
    
    return 0;
}
