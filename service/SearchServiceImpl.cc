
class SearchServiceImpl : public SearchService
{
public:
    
    void Search(RpcController *controller, const SearchRequest *request, 
                SearchResponse *response, Closure *done)
    {
        printRequest(controller, request);

        // Fill in the response.
        SearchResponse_Result * result = response->add_result();
        result->set_title("First result");
        result->set_url("http://acme.org/");
        result->add_snippets("A snippet from acme.org.");

        result = response->add_result();
        result->set_title("Second result");
        result->set_url("http://acme.org/abc");
        result->add_snippets("Another snippet from acme.org.");

        done->Run();
    }
};
