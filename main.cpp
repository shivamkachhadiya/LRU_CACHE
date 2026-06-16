#include<iostream>
#include<unordered_map>
using namespace std;

struct Node {
    int key;
    int value;
    Node *prev;
    Node *next;

    Node(int k, int v) : key(k), value(v), prev(NULL), next(NULL) {
    }
};

class LRUCache {
private:
    int capacity;
    unordered_map<int, Node *> mp;
    Node *head;
    Node *tail;

    void removeNode(Node *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void insertAfterHead(Node *node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

public:
    LRUCache(int cap) {
        capacity = cap;
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (mp.find(key) == mp.end()) {
            return -1;
        }
        Node *node = mp[key];
        removeNode(node);
        insertAfterHead(node);
        return node->value;
    }

    void put(int key, int value) {
        if (mp.find(key) != mp.end()) {
            removeNode(mp[key]);
            delete mp[key];
            mp.erase(key);
        }
        if ((int) mp.size() == capacity) {
            Node *lru = tail->prev;
            removeNode(lru);
            mp.erase(lru->key);
            delete lru;
        }
        Node *newNode = new Node(key, value);
        insertAfterHead(newNode);
        mp[key] = newNode;
    }
};

int main() {
    LRUCache cache(2);

    cache.put(1, 30);
    cache.put(2, 50);
    cout << cache.get(1) << "\n"; // 30
    cache.put(3, 70); // evicts key 2
    cout << cache.get(2) << "\n"; // -1 (evicted)
    cout << cache.get(3) << "\n"; // 70
    cache.put(2, 99); // key 2 wapas, evicts key 1
    cout << cache.get(1) << "\n"; // -1 (evicted)
    cout << cache.get(2) << "\n"; // 99


    return 0;
}

// LRUCache(2) call kiya:
// HEAD(dummy) <——————> TAIL(dummy)
//  key=0,val=0          key=0,val=0
//  prev=null            next=null
//  next=TAIL            prev=HEAD
//
// capacity = 2
// mp = {} (empty)


//==================GET======================
// capacity=2, DLL abhi aisi hai:
// HEAD <-> [1,30] <-> [2,50] <-> TAIL
// mp = {1->Node[1,30], 2->Node[2,50]}
// get(1) call kiya:
// Step 1: mp.find(1) → mila ✓
//         node = mp[1] → Node[1,30]
//
// Step 2: removeNode(Node[1,30])
//         HEAD <-> [2,50] <-> TAIL
//         [1,30] detached
//
// Step 3: insertAfterHead(Node[1,30])
//         HEAD <-> [1,30] <-> [2,50] <-> TAIL
//
// Step 4: return node->value → 30
// get(99) call kiya:
// mp.find(99) → nahi mila
// return -1
