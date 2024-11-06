//This is the code for LRU cache to store the top k(here k=10)searches for fast retrivel of data.
//This works better if there are extremly large number of documents but with small number of documents its not that effective.
#include<bits/stdc++.h>
using namespace std;

class LRU_Node{// this is the node defination of the double linked list(which is used for the LRU implementation)
    public:
    string sentenceSearched;
    vector<int> data;
    LRU_Node* next=NULL;
    LRU_Node* prev=NULL;
    LRU_Node(string sentenceSearched, vector<int>&data)// ye jo hai bhaiya who constructor hai
    {
        this-> sentenceSearched=sentenceSearched;
        this->data=data;
    }

};
class LRUcache{
    public:
    int capacity=5; // this can handle upto 5 recent searched and remove the least recently used if the size exceeds
    int currSize=0;// to note the crrent size of the caching doubly linked list
    LRU_Node* head=NULL;// to track the head of the DLL(double linked list) (to indicate the latest recently used)
    LRU_Node* tail=NULL;// to track the tail of the DLL(to indicate what least recently used)
    unordered_map<string, LRU_Node*> mp;// to store the node address to reach there in constant time otherwise it will take O(n) time
    vector<int> getStoredResult(string &sentenceSearched)
    {
        if (mp.find(sentenceSearched) == mp.end()) {
            return vector<int> (); // key not found returning an empty unordered_map
        }
        LRU_Node* node=mp[sentenceSearched];
        if (node != head) {
            // Remove the node from its current position
            if (node->prev) {
                node->prev->next = node->next;
            }
            if (node->next) {
                node->next->prev = node->prev;
            }
            if (node == tail) {
                tail = node->prev;
            }

            // Move the node to the head
            node->next = head;
            node->prev = NULL;
            if (head) {
                head->prev = node;
            }
            head = node;

            if (!tail) {
                tail = head;
            }
        }

        return node->data;
    }

    void putSearchedWord(string sentenceSearched, vector<int>& data)// insert into DLL if the size < capacity else remove the least recently used and then insert
    {
        if (mp.find(sentenceSearched) != mp.end()) {
            // Update the value and move to head
            LRU_Node* node = mp[sentenceSearched];
            node->data=data;
            getStoredResult(sentenceSearched); // Reuse get to move the node to the head
        }
        else {
            // Create a new node
            LRU_Node* node = new LRU_Node(sentenceSearched,data);
            if (!head) {
                head = tail = node;
            } else {
                node->next = head;
                head->prev = node;
                head = node;
            }
            mp[sentenceSearched] = node;

            // If the cache exceeds capacity, remove the LRU node
            if (mp.size() > capacity) {
                LRU_Node* lru = tail;
                if (tail->prev) {
                    tail->prev->next = NULL;
                }
                tail = tail->prev;
                mp.erase(lru->sentenceSearched);
                delete lru;
            }
        }

    }
} initial;

vector<int> LRU_Query(string sentenceSearched)
{
    auto result=initial.getStoredResult(sentenceSearched);
    if(result.size()==0)cout<<"Not present in LRU"<<endl;
    else cout<<"Present in LRU"<<endl;
    return result;
}
void insertLRUQuery(string &sentenceSearched,vector<int>& data)
{
    initial.putSearchedWord(sentenceSearched,data);
    return;
}