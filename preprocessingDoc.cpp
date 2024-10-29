#ifndef PREPROCESSING_DATA // Include guard
#define PREPROCESSING_DATA
#include <bits/stdc++.h>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include "invertedIndex.cpp"
#include "portersStemming.cpp"
#include <fstream>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std;
using namespace boost::algorithm;
//global variable decleration
unordered_set<string> stopWords={"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", 
    "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its",
     "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these",
      "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", 
      "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with",
       "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down",
        "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", 
        "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", 
        "s", "t", "can", "will", "just", "don", "should", "now"};
// to convert the tokens to the lower case
string toLowercase(const string& token) {
    string lowerToken = token;
    to_lower(lowerToken);
    return lowerToken;
}

// to check that the tokens is a stop word or not
bool isStopWord(const string& token, const unordered_set<string>& stopWords) {
    return stopWords.find(token)==stopWords.end()?true:false;
}
void threadDocumentProcessing(string& data,unordered_set<string>& stopWords,int docNumber)
{
        vector<string> tempTokens;
        vector<string> tokens;
        // Tokenize the document string by splitting using common delimiters
        boost::split(tempTokens, data, boost::is_any_of(" ,.!?;:\n\t\"'()[]{}"));

        // Process each token: trim spaces and add to the token list
        for (string& token : tempTokens) {
            // Trim leading and trailing spaces
            boost::trim(token);

            // Add non-empty tokens to the list
            token=toLowercase(token);
            if (!token.empty()&&isStopWord(token,stopWords)) {
                tokens.push_back(token);
            }
        }

        // Build the inverted index for this document
        buildInvertedIndex(tokens, docNumber);
}
//to split or make tokens of the document words
void tokenize(vector<string>& document, unordered_set<string>& stopWords) {
    vector<string> tokens;
    cout << "The size of the document is: " << document.size() << endl;
    auto start = high_resolution_clock::now();
    // for(int i=0;i<document.size();i++)
    // {
    //     threadDocumentProcessing(document[i],stopWords,i+1);
    // }
    for (int i = 0; i < document.size(); i += 2) {
        // Create threads for each document
        thread processDoc1(threadDocumentProcessing, std::ref(document[i]), std::ref(stopWords), i + 1);

        // Check if there's a second document to process
        thread processDoc2;
        if (i + 1 < document.size()) {
            processDoc2 = thread(threadDocumentProcessing, std::ref(document[i + 1]), std::ref(stopWords), i + 2);
        }

        // Wait for the first thread to finish
        if (processDoc1.joinable()) {
            processDoc1.join();
        }

        // Wait for the second thread if it was created
        if (processDoc2.joinable()) {
            processDoc2.join();
        }
    }

    // If the number of documents is odd, process the last document
    if (document.size() % 2 != 0) {
        thread processLastDoc(threadDocumentProcessing, std::ref(document.back()), std::ref(stopWords), document.size());
        if (processLastDoc.joinable()) {
            processLastDoc.join();
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << " microseconds" << endl;
}


// //simple version of Porter's Stemming algorithms for stemming(removing ing, ed, ation etc)
// //logic Written in porterStemming.cpp
// string stem(string& word)
// {
//     return stemmingWord(word);// function present in porterStemming.cpp file
// }

void processDocument(unordered_set<string>& stopWords, vector<string>& document)
{   
    tokenize(document,stopWords);
}

string readFile(const string& filePath) {
    ifstream file(filePath, ios::in | ios::binary);
    if (file.is_open()) {
        stringstream buffer;
        buffer << file.rdbuf();  // Read entire file content into a buffer
        return buffer.str();     // Return the content as a string
        } else {
            cerr << "Could not open file: " << filePath << endl;
            return "";
        }
    }
vector<string> readDocuments(const string& folderPath) {
    vector<string> documents;
    // Iterate through all files in the folder
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.path().extension() == ".txt") {  // Only process .txt files
            string content = readFile(entry.path().string());
            if (!content.empty()) {
                documents.push_back(content);
            }
        }
    }
    return documents;
}
void preProcessTheData()//to preprocess the document present in the document folder
{
    string folderPath = "Documents";
    vector<string> document=readDocuments(folderPath);//this is where all the tokens are which is present in document and will be processed.
    processDocument(stopWords,document);
}
#endif