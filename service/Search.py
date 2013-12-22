
from deltavsoft.rcfproto import *
import Search_pb2

def search(query, page_number):
    init()
    tcpEndpoint     = TcpEndpoint("127.0.0.1", 50001)
    channel         = RcfProtoChannel(tcpEndpoint)
    searchService   = Search_pb2.SearchService_Stub(channel)
    request         = Search_pb2.SearchRequest()
    request.query = 'enen.jpg'
    request.page_number = 100
    searchService.Search(None, request, None)
    response = channel.GetResponse()
    for result in response.result:
        print result.url, result.title

search('sdfsdf', 12)
