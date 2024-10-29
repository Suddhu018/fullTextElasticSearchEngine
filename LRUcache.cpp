//This is the code for LRU cache to store the top k(here k=10)searches for fast retrivel of data.
//This works better if there are extremly large number of documents but with small number of documents its not that effective.
#include<bits/stdc++.h>
using namespace std;

class LRU_Node{// this is the node defination of the double linked list(which is used for the LRU implementation)
    public:
    string wordSearched;
    unordered_map<int, int> data;
    LRU_Node* next=NULL;
    LRU_Node* prev=NULL;
    LRU_Node(string wordSearched, unordered_map<int, int>&data)// ye jo hai bhaiya who constructor hai
    {
        this-> wordSearched=wordSearched;
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
    unordered_map<int,int> getStoredResult(string &wordSearched)
    {
        if (mp.find(wordSearched) == mp.end()) {
            return unordered_map<int,int> (); // key not found returning an empty unordered_map
        }
        LRU_Node* node=mp[wordSearched];
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

    void putSearchedWord(string wordSearched, unordered_map<int, int>&data)// insert into DLL if the size < capacity else remove the least recently used and then insert
    {
        if (mp.find(wordSearched) != mp.end()) {
            // Update the value and move to head
            LRU_Node* node = mp[wordSearched];
            node->data=data;
            getStoredResult(wordSearched); // Reuse get to move the node to the head
        }
        else {
            // Create a new node
            LRU_Node* node = new LRU_Node(wordSearched,data);
            if (!head) {
                head = tail = node;
            } else {
                node->next = head;
                head->prev = node;
                head = node;
            }
            mp[wordSearched] = node;

            // If the cache exceeds capacity, remove the LRU node
            if (mp.size() > capacity) {
                LRU_Node* lru = tail;
                if (tail->prev) {
                    tail->prev->next = NULL;
                }
                tail = tail->prev;
                mp.erase(lru->wordSearched);
                delete lru;
            }
        }

    }
} initial;

unordered_map<int,int> LRU_Query(string wordSearched)
{
    auto result=initial.getStoredResult(wordSearched);
    if(result.size()==0)cout<<"Not present in LRU"<<endl;
    else cout<<"Present in LRU"<<endl;
    return result;
}
void insertLRUQuery(string &wordSearched, unordered_map<int, int>&data)
{
    initial.putSearchedWord(wordSearched,data);
    return;
}