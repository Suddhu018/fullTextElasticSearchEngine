//using this to run the program otherwise its not using the boost library
/*
  g++ -std=c++17 -I/opt/homebrew/opt/boost/include -L/opt/homebrew/opt/boost/lib -lboost_system -lboost_filesystem driverCode.cpp -o driverCode
./driverCode

*/
#include <bits/stdc++.h>
#include "crow_all.h"
#include "preprocessingDoc.cpp" //to preprocess the data saved in the documents folder
#include <boost/algorithm/string.hpp>// to use the features of the boost library
#include "query.cpp"
#include <sstream>

using namespace std;
using namespace boost::algorithm; // Use the correct namespace

int main() {
    // Preprocess the data once
    preProcessTheData();

    crow::SimpleApp app;

    CROW_ROUTE(app, "/search")
    ([](const crow::request& req) {
        auto query = req.url_params.get("query");
        if (!query) {
            return crow::response(400, "Query parameter is missing");
        }

        string inputString = query;
        vector<int> finalResult=satisfyQuery(inputString);
        print(finalResult);
        return crow::response(200, "everything worked perfectly");
    });

    app.port(8080).multithreaded().run();

    return 0;
}
