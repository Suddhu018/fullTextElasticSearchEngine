#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include<iostream>

using namespace std;

unordered_map<string, int> memo;

int levenshteinDistanceHelper(const string& s1, const string& s2, int m, int n) {
    string key = to_string(m) + "," + to_string(n);
    if (memo.find(key) != memo.end()) return memo[key];

    if (m == 0) return n;
    if (n == 0) return m;

    if (s1[m - 1] == s2[n - 1]) {
        memo[key] = levenshteinDistanceHelper(s1, s2, m - 1, n - 1);
    } else {
        memo[key] = 1 + min({levenshteinDistanceHelper(s1, s2, m - 1, n),    // deletion
                             levenshteinDistanceHelper(s1, s2, m, n - 1),    // insertion
                             levenshteinDistanceHelper(s1, s2, m - 1, n - 1) // substitution
                            });
    }

    return memo[key];
}

int levenshteinDistance(const string& s1, const string& s2) {
    memo.clear();
    return levenshteinDistanceHelper(s1, s2, s1.length(), s2.length());
}

// Find closest match from dictionaryOfTokens
string findClosestMatch(const string& word, unordered_map<string, unordered_map<int, int>> & dictionaryOfTokens) {
    if (dictionaryOfTokens.empty()) return "";
    
    auto it = dictionaryOfTokens.begin();
    string closest = it->first;
    int minDistance = levenshteinDistance(word, closest);
    
    for (const auto& dictWord : dictionaryOfTokens) {
        int distance = levenshteinDistance(word, dictWord.first);
        if (distance < minDistance) {
            minDistance = distance;
            closest = dictWord.first;
        }
    }
    
    return closest;
}