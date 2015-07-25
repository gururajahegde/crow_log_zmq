#include <crow_all.h>

//based on original example_with_all.cpp

#include <sstream>

class ExampleLogHandler : public crow::ILogHandler {
    public:
        void log(std::string message, crow::LogLevel level) override {
//            cerr << "ExampleLogHandler -> " << message;
        }
};

int main()
{
    crow::SimpleApp app;

    app.route_dynamic("/").name("hello")
    ([]{
        return "Hello World!";
    });

    app.route_dynamic("/about")
    ([](){
        return "About Crow example.";
    });

    // simple json response
    app.route_dynamic("/json")
    ([]{
        crow::json::wvalue x;
        x["message"] = "Hello, World!";
        return x;
    });

    app.route_dynamic("/hello/<int>")
    ([](int count){
        if (count > 100)
            return crow::response(400);
        std::ostringstream os;
        os << count << " bottles of beer!";
        return crow::response(os.str());
    });

    app.route_dynamic("/add/<int>/<int>")
    ([](const crow::request& req, crow::response& res, int a, int b){
        std::ostringstream os;
        os << a+b;
        res.write(os.str());
        res.end();
    });

    // Compile error with message "Handler type is mismatched with URL paramters"
    //app.route_dynamic(/another/<int>")
    //([](int a, int b){
        //return crow::response(500);
    //});

    // more json example
    app.route_dynamic("/add_json")
    ([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);
        int sum = x["a"].i()+x["b"].i();
        std::ostringstream os;
        os << sum;
        return crow::response{os.str()};
    });

    app.route_dynamic("/params")
    ([](const crow::request& req){
        std::ostringstream os;
        os << "Params: " << req.url_params << "\n\n"; 
        os << "The key 'foo' was " << (req.url_params.get("foo") == nullptr ? "not " : "") << "found.\n";
        if(req.url_params.get("pew") != nullptr) {
            double countD = boost::lexical_cast<double>(req.url_params.get("pew"));
            os << "The value of 'pew' is " <<  countD << '\n';
        }
        auto count = req.url_params.get_list("count");
        os << "The key 'count' contains " << count.size() << " value(s).\n";
        for(const auto& countVal : count) {
            os << " - " << countVal << '\n';
        }
        return crow::response{os.str()};
    });    

    // ignore all log
    crow::logger::setLogLevel(crow::LogLevel::DEBUG);
    //crow::logger::setHandler(std::make_shared<ExampleLogHandler>());

    app.port(18080)
        .multithreaded()
        .run();
}