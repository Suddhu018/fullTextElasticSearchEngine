// Trie dataStructure for Auto-Suggestions(Prefix Tree)
#include<bits/stdc++.h>
using namespace std;
class TrieNode{
    public:
    bool isComplete=false;// to know that a particular word is completed or not
    unordered_map<char, TrieNode*> child;// to know that what is the following letter after this letter;
    //this is the modified trie node as the text may contain any character
    TrieNode()
    {}
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
    bool isPresent(string &word)
    {
        TrieNode* node = root;
        for (auto &ch : word) {
            if (node->child.find(ch) == node->child.end()) {
                return false;
            }
            node = node->child[ch];
        }
        return node->isComplete;
    }

};

void insertIntoTrie(vector<string> &words)
{
    Trie root;//the process to insert into a trie is triggerd using this root;
    for(auto it: words)
    {
        root.insert(it);
    }
    return;
}