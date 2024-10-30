#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include "invertedIndex.cpp"      // Include the header instead of the source file.
#include "preprocessingDoc.cpp"   // Include the header instead of the source file.
#include "LRUcache.cpp"
#include <thread>
#include <chrono>
using namespace std::chrono;
using namespace boost::algorithm;
using namespace std;
vector<string> tokenizeQuery(string& query)
{
    vector<string> tokens;
    vector<string> tempTokens;
     boost::split(tempTokens,query, boost::is_any_of(" ,.!?;:\n\t\"'()[]{}"));
     for (string& token : tempTokens) {
            // Trim leading and trailing spaces
            boost::trim(token);

            // Add non-empty tokens to the list
            token=toLowercase(token);
            if (!token.empty()&&isStopWord(token,stopWords)) {
                tokens.push_back(token);
            }
        }
        return tokens;
}
 void print(unordered_map<int, int>&data,string& lowerToken)
 {
     for (const auto& x : data) {
                    cout << "The word \"" << lowerToken << "\" was found in document number " << x.first 
                        << " and its frequency is " << x.second << endl;
       }

 }
void satisfyQuery(string &query) {
    unordered_map<string, unordered_map<int, int>> data = index();
    while(true)
    {
        // cout << "Enter the senetence/word you want to search: ";
        // string query="english";
        // getline(cin, query);//--> dont use cin>>query as this will only read upto first white space.
        //to tokenize the inout query and stop word removal
        vector<string> allQueryTokens= tokenizeQuery(query);//al ready converted to lower case here
        for(auto lowerToken:allQueryTokens)
        {
                auto result=LRU_Query(lowerToken);//check that the result is present in LRU cache or not
            if(result.size()!=0)// yes presnt in LRU cache
            {
                print(result,lowerToken);
            }
            else{// not present in LRU cache
                    auto it = data.find(lowerToken); // Use find() to check if the word exists in the inverted index

                    if (it != data.end()) { // Check if the word was found
                        print(it->second,lowerToken);
                        insertLRUQuery(lowerToken,it->second);
                    } else {
                        cout << "The word \"" << lowerToken << "\" was not found in any document." << endl;
                    }
            }

        }
        // cout<<"Do you want to search another word? (y/n) ";
        // char decision;
        // cin>>decision;
        // if (decision == 'n' || decision == 'N') break;
        // cin.ignore();  // Ignore the leftover newline character from the decision cin>>
        break;
    }
}
