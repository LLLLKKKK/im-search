
from deltavsoft.rcfproto import *
import Search_pb2
import json

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

    results = []
    for result in response.result:
        results.append({ 'url' : result.url, 'title' : result.title})

    return results
