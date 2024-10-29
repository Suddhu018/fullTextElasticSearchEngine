//using this to run the program otherwise its not using the boost library
/*
  g++ -std=c++17 -I/opt/homebrew/opt/boost/include -L/opt/homebrew/opt/boost/lib -lboost_system -lboost_filesystem driverCode.cpp -o driverCode
./driverCode

*/
#include <bits/stdc++.h>
#include "preprocessingDoc.cpp" //to preprocess the data saved in the documents folder
#include <boost/algorithm/string.hpp>// to use the features of the boost library
#include <thread>// to use the multithreadig functionality of c++
#include "query.cpp"
using namespace std;
using namespace boost::algorithm; // Use the correct namespace

int main()
{
    preProcessTheData();/* received the tokens which is stemmed by common words and which are not a stop words
                                                 and when the above function is called the inverted index is also already called and build*/
    satisfyQuery();// for the user to ask the query in the terminal

    return 0;
}