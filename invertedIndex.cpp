#ifndef INVERTED_INDEX_H // Include guard
#define INVERTED_INDEX_H

#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include "trieAutoSuggestion.cpp"
#include <thread>
using namespace std;
using namespace boost::algorithm;

unordered_map<string,unordered_map<int, int>> invertedIndex; // to store word occurrences in documents
void buildTree(vector<string>&tokens,int docNumber)
{
    for (int i = 0; i < tokens.size(); i++) {
        string temp=tokens[i];
        to_lower(temp);
        invertedIndex[temp][docNumber]++;
    }
}
void buildInvertedIndex(vector<string> &tokens, int docNumber) {
    thread work1(buildTree,std::ref(tokens),docNumber);//used this thread to actually build the inverted index
    thread work2(insertIntoTrie,std::ref(tokens));//Now use this thred to actually insert into trie;
    work1.join();
    work2.join();
}

unordered_map<string, unordered_map<int, int>> index() {//this function return the formed invertedIndex wherever needed for eg:- in query
    return invertedIndex;
}

#endif // INVERTED_INDEX_H
