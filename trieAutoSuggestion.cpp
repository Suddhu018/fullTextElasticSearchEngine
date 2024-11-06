#ifndef TRIE_AUTO // Include guard
#define TRIE_AUTO
// Trie dataStructure for Auto-Suggestions(Prefix Tree)
#include<bits/stdc++.h>
using namespace std;
class TrieNode{
    public:
    bool isComplete=false;// to know that a particular word is completed or not
    unordered_map<char, TrieNode*> child;// to know that what is the following letter after this letter;
    //this is the modified trie node as the text may contain any character
};
class Trie{
    public:
    TrieNode* root;
    Trie()
    {
        root=new TrieNode();
    }
    void insert(string &wordToInsert)
    {
       TrieNode* node = root;
        for (auto &ch : wordToInsert) {
            if (node->child.find(ch) == node->child.end()) {
                node->child[ch] = new TrieNode();
            }
            node = node->child[ch];
        }
        node->isComplete = true;  // Mark the end of the word
    }
    void allWordsWithPrefixHelper(string &prefix, TrieNode* node, vector<string> &allWords)
    {
        if (node->isComplete) {
            allWords.push_back(prefix);
        }
        for (auto &it : node->child) {
            prefix.push_back(it.first);
            allWordsWithPrefixHelper(prefix, it.second, allWords);
            prefix.pop_back();
        }
    }
    vector<string> allWordsWithPrefixFinder(string &prefix)
    {
        vector<string> allWords;
        TrieNode* node = root;
        for (auto &ch : prefix) {
            if (node->child.find(ch) == node->child.end()) {
                cout<<"No words with this prefix"<<endl;
                return allWords;
            }
            node = node->child[ch];
        }
        allWordsWithPrefixHelper(prefix, node, allWords);
        return allWords;
    }

}rootBase;

void insertIntoTrie(vector<string> &words)
{

    for(auto &it: words)
    {
        rootBase.insert(it);
    }
    return;
}
vector<string> allWordsWithPrefix(string &prefix)
{
    cout<<"call received"<<endl;
     vector<string> tempRes=rootBase.allWordsWithPrefixFinder(prefix);
     return tempRes;
}
// Time Complexity: O(N) where N is the length of the word
#endif