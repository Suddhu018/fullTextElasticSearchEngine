//This is the implementtion of TF_IDF algorithm for the document ranking
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <set>
#include "invertedIndex.cpp"

using namespace std;

void TF_IDF()
{
    unordered_map<string, unordered_map<int, int>> tokenANDInvertedIndex = index();
    set<string> allCollectedTokens;
    for (auto it : tokenANDInvertedIndex) allCollectedTokens.insert(it.first); // create a vocabulary that includes all unique words from all documents.

    int N = tokenANDInvertedIndex.size(); // Total number of documents

    // Step 1: Calculate Term Frequency (TF)
    unordered_map<int, unordered_map<string, double>> termFrequency;
    for (auto& [term, docMap] : tokenANDInvertedIndex) {
        for (auto& [docID, count] : docMap) {
            termFrequency[docID][term] = (double)count / docMap.size();
        }
    }

    // Step 2: Compute Inverse Document Frequency (IDF)
    unordered_map<string, double> inverseDocumentFrequency;
    for (auto& term : allCollectedTokens) {
        int df = tokenANDInvertedIndex[term].size();
        inverseDocumentFrequency[term] = log((double)N / (1 + df));
    }

    // Step 3: Compute TF-IDF
    unordered_map<int, unordered_map<string, double>> tfidf;
    for (auto& [docID, tfMap] : termFrequency) {
        for (auto& [term, tf] : tfMap) {
            tfidf[docID][term] = tf * inverseDocumentFrequency[term];
        }
    }

    // Step 4: Document Ranking (Cosine Similarity)
    // Assuming we have a query vector (queryTFIDF) to compare with
    unordered_map<string, double> queryTFIDF; // This should be computed similarly to tfidf for the query

    // Function to compute cosine similarity
    auto cosineSimilarity = [](unordered_map<string, double>& vec1, unordered_map<string, double>& vec2) {
        double dotProduct = 0.0, normA = 0.0, normB = 0.0;
        for (auto& [term, val] : vec1) {
            dotProduct += val * vec2[term];
            normA += val * val;
        }
        for (auto& [term, val] : vec2) {
            normB += val * val;
        }
        return dotProduct / (sqrt(normA) * sqrt(normB));
    };

    // Rank documents based on cosine similarity with the query
    vector<pair<int, double>> docScores;
    for (auto& [docID, docTFIDF] : tfidf) {
        double score = cosineSimilarity(docTFIDF, queryTFIDF);
        docScores.push_back({docID, score});
    }

    // Sort documents by score in descending order
    sort(docScores.begin(), docScores.end(), [](pair<int, double>& a, pair<int, double>& b) {
        return a.second > b.second;
    });

    // Output ranked documents
    for (auto& [docID, score] : docScores) {
        cout << "Document ID: " << docID << " Score: " << score << endl;
    }
}
