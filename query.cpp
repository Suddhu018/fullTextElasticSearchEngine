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
 void print(vector<int>& result)
 {
     for (const auto& x : result) {
                    cout << "The searched sentence was found in document number " << x << endl;
       }

 }
 vector<int> processIntersectionOfDocumentId(vector<vector<int>>& docNUM)
 {
        vector<int> ans;
        unordered_map<int,int> mp;
        for(auto it: docNUM)
        {
            for(auto x:it)mp[x]++;
        }
        for(auto it:mp)
        {
            if(it.second==docNUM.size())ans.push_back(it.first);
        }
        sort(ans.begin(),ans.end());
        return ans;
 }
void satisfyQuery(string &query) {
    unordered_map<string, unordered_map<int, int>> data = index();
    while(true)
    {
        vector<string> allQueryTokens= tokenizeQuery(query);//all ready converted to lower case here
        vector<vector<int>> docNUM;
        for(auto lowerToken:allQueryTokens)
        {
                auto result=LRU_Query(lowerToken);//check that the result is present in LRU cache or not
            if(result.size()!=0)// yes presnt in LRU cache
            {
                // print(result,lowerToken);
            }
            else{// not present in LRU cache
                    auto it = data.find(lowerToken); // Use find() to check if the word exists in the inverted index
                    vector<int> temp;
                    if (it != data.end()) { // Check if the word was found
                        for (const auto& x : it->second) {
                            temp.push_back(x.first);
                        }
                        docNUM.push_back(temp);
                        // print(it->second,lowerToken);
                        // insertLRUQuery(lowerToken,it->second);
                    } else {
                        cout << "The searched sentence was not found in any document." << endl;
                        break;//as we are searchung for a complete sentence so if one word is not found then the whole sentence is not found.
                    }
            }

        }
        vector<int> result=processIntersectionOfDocumentId(docNUM);
        print(result);
        break;
    }
}
