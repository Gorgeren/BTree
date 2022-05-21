#include <vector>
#include <iostream>

class Key {
public:
    unsigned long long key;
    std::string data;
    Key(unsigned long long key, std::string data) : key(key), data(data) {};
};
class Node {
public:
    std::vector<Key*> keys;
    std::vector<Node*> ptrs;
    bool leaf;
    Node(Key* key) : leaf(1) { keys.push_back(key); };
    ~Node() {
        for (Key* itr : keys) {
            delete itr;
        }
        for (Node* itr : ptrs) {
            delete itr;
        }
    }
};
class BTree {
    Node* root;
public:
    void insert(unsigned long long key, std::string data);
private:
    void insertPrivate(unsigned long long key, std::string data);
    Key* createKey(unsigned long long key, std::string data);
    Node* createNode(unsigned long long key, std::string data);
};
Node* BTree::createNode(unsigned long long key, std::string data) {
    Node *tmp = new
}
Key* BTree::createKey(unsigned long long key, std::string data) {
    Key* tmp = new Key(key, data);
    return tmp;
}
void BTree::insert(unsigned long long key, std::string data) {

    if (root == nullptr) {
        
    }
}
void BTree::insertPrivate(unsigned long long key, std::string data) {

}
int main()
{

    std::cout << "Hello World!\n";
}

