//This is a simple porters stemming algorithms not that advanced but will work in most of the cases
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost::algorithm;
string step1b(string& word)
{
    // Common suffixes
    vector<string> suffixes = {
    "able", "al", "ed", "en", "er", "est", "ful", "ic", "ing", "ion", 
    "ity", "ive", "less", "ly", "ment", "ness", "or", "s", "se", 
    "tion", "y", "ward", "wise"
};
    
    for (const string &suffix : suffixes) {
        if (word.length() >= suffix.length() && 
            word.compare(word.length() - suffix.length(), suffix.length(), suffix) == 0) {
            return word.substr(0, word.length() - suffix.length()); // Remove the suffix
        }
    }
    
    return word; 

}
string step1a(string& word)//remove the last s 
{
    int n=word.length();
    if(n<=2)return word;
    if(word.size()>4&&word[n-1]=='s')
    {
        if(word[n-2]=='s') return step1b(word);//do nothing if there are 2 ss
        else word.pop_back();// else remove the last s
    }
    return step1b(word);
}
string stemmingWord(string& word)
{
    return step1a(word);
}