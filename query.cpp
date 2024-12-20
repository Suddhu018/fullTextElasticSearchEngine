#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include "invertedIndex.cpp"      // Include the header instead of the source file.
#include "preprocessingDoc.cpp"   // Include the header instead of the source file.
#include "LRUcache.cpp"
#include "typoCorrection.cpp"
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
                    cout << "The searched sentence was found in the document : "<<docName[x]<< endl;
       }
       return;

 }
 vector<int> processIntersectionOfDocumentId(vector<vector<int>>& docNUM)// to find the intersection of n number of arrays (Leetcode Question)
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
vector<int> satisfyQuery(string &query) {
     auto result=LRU_Query(query);//check that the result is present in LRU cache or not
    //before processing everything search that is it present in the LRU cache or not
        if(result.size()!=0)// yes presnt in LRU cache
        {
                return result;
        }
    unordered_map<string, unordered_map<int, int>> data = index();
    cout<<"Your query is: "<<query<<endl;
        vector<string> allQueryTokens= tokenizeQuery(query);//all ready converted to lower case here
        vector<vector<int>> docNUM;// to note down the documents where these words are found so that it can be to find the intersection later
        for(auto lowerToken:allQueryTokens)
        {
            // not present in LRU cache
                    auto it = data.find(lowerToken); // Use find() to check if the word exists in the inverted index
                    vector<int> temp;
                    if (it != data.end()) { // Check if the word was found
                        for (const auto& x : it->second) {
                            temp.push_back(x.first);
                        }
                        docNUM.push_back(temp);
                    } else {
                        cout << "The searched sentence was not found in any document because this word "<<lowerToken <<" was not found anywhere"<< endl;
                        cout<<"Did you mean to search "<<findClosestMatch(lowerToken,data)<<" ?"<<endl;
                       return {};//as we are searchung for a complete sentence so if one word is not found then the whole sentence is not found.
                    }

        }
        vector<int> finalRes=processIntersectionOfDocumentId(docNUM);
        insertLRUQuery(query,finalRes);
        return finalRes;
}
