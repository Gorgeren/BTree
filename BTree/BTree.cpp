#include <vector>
#include <iostream>
#include <queue>

#define T 2 // характерестическое число Б дерева
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
    Node() : leaf(1) {};
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
    int t;
public:
    void insert(unsigned long long key, std::string data);
    void print();
private:
    void insertPrivate(Node* ptr, Key* newKey);
    void insertPrivate(Key* newKey);
    Key* createKey(unsigned long long key, std::string data);
    Node* createNode(Key *key);
    Node* createNode();
    void splitChild(Node* ptr, int i);
public:
    BTree() : root(nullptr), t(T) {};
};

void BTree::splitChild(Node* parent, int i) {
    Node* child = parent->ptrs[i];
    Node* brother = new Node;
    brother->leaf = child->leaf;
    std::copy(child->keys.begin() + t, child->keys.end(), inserter(brother->keys, brother->keys.begin()));
    if (!child->leaf) {
        std::copy(child->ptrs.begin() + t, child->ptrs.end(), inserter(brother->ptrs, brother->ptrs.begin()));
        child->ptrs.erase(child->ptrs.begin() + t, child->ptrs.end());
    }
    parent->keys.insert(parent->keys.begin() + i, child->keys[t - 1]);
    parent->ptrs.insert(parent->ptrs.begin() + i + 1, brother);
    //parent->ptrs[i + 1] = brother;
    child->keys.erase(child->keys.begin() + t - 1, child->keys.end());
}
//Node* BTree::createNode(unsigned long long key, std::string data) {
//    return new Node(createKey(key, data));
//}
Node* BTree::createNode() {
    return new Node;
}
Node* BTree::createNode(Key* key) {
    return new Node(key);
}
Key* BTree::createKey(unsigned long long key, std::string data) {
    return new Key(key, data);
}
void BTree::insert(unsigned long long key, std::string data) {
    Key* newKey = createKey(key, data);
    if (root == nullptr) {
        root = createNode(newKey);
    } else {
        insertPrivate(newKey);
    }
}
void BTree::insertPrivate(Key* newKey) {
    if (root->keys.size() == 2 * t - 1) {
        Node* newRoot = createNode();
        newRoot->leaf = 0;
        newRoot->ptrs.push_back(root);
        root = newRoot;
        splitChild(root, 0);
        insertPrivate(root, newKey);
    } else {
        insertPrivate(root, newKey);
    }
}
void binarySearch(std::vector<Key*> arr, int& index, Key* key) {
    index = 0;
    while (index < arr.size() && key->key > arr[index]->key) {
        ++index;
    }
}
void BTree::insertPrivate(Node* ptr, Key* newKey) {
    int index;
    if (ptr->leaf) {
        binarySearch(ptr->keys, index, newKey);
        ptr->keys.insert(ptr->keys.begin() + index, newKey);
    }
    else {
        binarySearch(ptr->keys, index, newKey);
        if (ptr->ptrs[index]->keys.size() == 2 * t - 1) {
            splitChild(ptr, index);
        }
        binarySearch(ptr->keys, index, newKey);
        insertPrivate(ptr->ptrs[index], newKey);
    }
}
void printArr(std::vector<Key*> arr) {
    for (Key* i : arr) {
        std::cout << i->key << ' ';
    }
    std::cout << ' ';
}
void BTree::print() {
    if (root == nullptr) {
        return;
    }
    std::queue<Node*> queue;
    int arr[2] = { 1, 0 };
    int high = 0;
    Node* tmp = root;
    queue.push(tmp);
    std::cout << "High: " << high << "| ";
    while (!queue.empty()) {
        tmp = queue.front();
        if (arr[0] == 0) {
            arr[0] = arr[1];
            arr[1] = 0;
        }
        queue.pop();
        printArr(tmp->keys);
        arr[0]--;
        if (arr[0] == 0 && !tmp->leaf) {
            std::cout << std::endl;
            std::cout << "High: " << ++high << "| ";
            arr[0] = arr[1];
            arr[0] = 0;
        } else if (arr[0] == 0) {
            std::cout << std::endl;
            arr[0] = arr[1];
            arr[0] = 0;
        }
        if (!tmp->leaf) {
            arr[1] += tmp->ptrs.size();
            for (int i = 0; i < tmp->ptrs.size(); i++) {
                queue.push(tmp->ptrs[i]);
            }
        }
    }
}
int main()
{
    BTree tree;
    for (int i = 255; i >= 0; i--) {
        tree.insert(i, "abc");
        //std::cout << "------------------" << std::endl;
    }
    tree.print();
}

