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
#include <thread>
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
unordered_map<int,string> docName;// to store the name of the document corresponding to the document number;
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
    cout << "The size of the document is: " << document.size() << endl;// represent the total number of text files
    auto start = high_resolution_clock::now();
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
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Could not open the file: " << filePath << endl;
        return "";
    }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

void processFiles(const vector<fs::path>& files, vector<string>& documents, unordered_map<int, string>& docName, int startIdx) {
    for (size_t i = 0; i < files.size(); ++i) {
        const auto& file = files[i];
        if (file.extension() == ".txt") {  // Only process .txt files
            string content = readFile(file.string());
            if (!content.empty()) {
                documents.push_back(content);
                docName[startIdx + i] = file.filename().string();
            }
        }
    }
}

vector<string> readDocuments(const string& folderPath, unordered_map<int, string>& docName) {
    vector<string> documents;
    vector<fs::path> files;

    // Collect all files in the folder
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        files.push_back(entry.path());
    }

    // Split the files into two halves
    size_t mid = files.size() / 2;
    vector<fs::path> firstHalf(files.begin(), files.begin() + mid);
    vector<fs::path> secondHalf(files.begin() + mid, files.end());

    // Create threads to process each half
    vector<string> documents1, documents2;
    unordered_map<int, string> docName1, docName2;
    auto start = high_resolution_clock::now();
    thread t1(processFiles, ref(firstHalf), ref(documents1), ref(docName1), 0);
    thread t2(processFiles, ref(secondHalf), ref(documents2), ref(docName2), mid);

    // Wait for threads to finish
    t1.join();
    t2.join();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << " microseconds to actually read all the files" << endl;
    // Combine results from both threads
    documents.insert(documents.end(), documents1.begin(), documents1.end());
    documents.insert(documents.end(), documents2.begin(), documents2.end());
    docName.insert(docName1.begin(), docName1.end());
    docName.insert(docName2.begin(), docName2.end());

    return documents;
}

void preProcessTheData()//to preprocess the document present in the document folder
{
    string folderPath = "Documents";
    vector<string> document=readDocuments(folderPath, docName);//this is where all the files are stored as a string
    processDocument(stopWords,document);
}
#endif