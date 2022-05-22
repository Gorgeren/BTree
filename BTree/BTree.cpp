#include <vector>
#include <iostream>
#include <queue>
#include <string>

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
class NodeI {
public:
    Node* node;
    int index;
};
class BTree {
    Node* root;
    int t;
public:
    void insert(unsigned long long key, std::string data);
    void erase(unsigned long long key);
    void print();
    std::string search(unsigned long long key);
private:
    void insertPrivate(Node* ptr, Key* newKey);
    void insertPrivate(Key* newKey);
    Key* createKey(unsigned long long key, std::string data);
    Node* createNode(Key *key);
    Node* createNode();
    void splitChild(Node* ptr, int i);
    Key* getMin(Node* ptr);
    Key* getMax(Node* ptr);
    Node* searchPrivate(Node* currentNode, int& index, Node* parent, unsigned long long key);
    void mergeNodes(Node* left, Node* right, Node* parent, int index);

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

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i]->key;
        std::cout << ((i < arr.size() - 1) ?  ',' : ' ');
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
            arr[1] += (int)tmp->ptrs.size();
            for (int i = 0; i < tmp->ptrs.size(); i++) {
                queue.push(tmp->ptrs[i]);
            }
        }
    }
}
Key* BTree::getMin(Node* ptr) {
    if (!ptr->leaf) {
        return getMin(ptr->ptrs[0]);
    } else {
        Key* tmp = ptr->keys[0];
        ptr->keys.erase(ptr->keys.begin());
        return tmp;
    }
}
Key* BTree::getMax(Node* ptr) {
    if (!ptr->leaf) {
        return getMax(ptr->ptrs[ptr->ptrs.size() - 1]);
    }
    else {
        Key* tmp = ptr->keys[ptr->keys.size() - 1];
        ptr->keys.pop_back();
        return tmp;
    }
}
bool binarySearch(std::vector<Key*> arr, int& index, unsigned long long key) {
    index = 0;
    while (index < arr.size() && key > arr[index]->key) {
        ++index;
    }
    if (index == arr.size()) {
        return 0;
    } else {
        return ((key == arr[index]->key) ? 1 : 0);
    }
}
Node* BTree::searchPrivate(Node* currentNode, int &index, Node* parent, unsigned long long key) {
    if (currentNode->leaf) {
        return (binarySearch(currentNode->keys, index, key)) ? currentNode : nullptr;
    } else {
        bool exists = binarySearch(currentNode->keys, index, key);
        if (exists) return currentNode;
        else {
            parent = currentNode;
            return searchPrivate(currentNode->ptrs[index], index, parent, key);
        }
    }
}
std::string BTree::search(unsigned long long key) {
    int index;
    Node* parent = nullptr;
    Node* tmp = searchPrivate(root, index, parent, key);
    std::string data = (tmp == nullptr) ? "Not Exists" : tmp->keys[index]->data;
    return data;
}
void BTree::erase(unsigned long long key) {
    int index;
    Node* parent = nullptr;
    Node* tmp = searchPrivate(root, index, parent, key);
    if (tmp == nullptr) { // в дереве нет такого ключа
        std::cout << "NoSuchWord\n";
        return;
    }
    else if (tmp == root && tmp->leaf) { // если ключ в корне и он лист => в нем может хранится от 1 до 2*t-1 ключей
        delete tmp->keys[index];
        tmp->keys.erase(tmp->keys.begin() + index);
        if (tmp->keys.size() == 0) {
            delete root;
            root = nullptr;
        }
    } else if (tmp->leaf) { // если ключ находится в листе
        if (tmp->keys.size() > t - 1) {
            delete tmp->keys[index];
            tmp->keys.erase(tmp->keys.begin() + index);
        }
        else {
            int parentindex;
            binarySearch(parent->keys, parentindex, key);
            bool left = (parentindex > 0);
            bool right = (parentindex < parent->ptrs.size() - 1);
            int countleft = -1, countright = -1;
            if (left) {
                countleft = parent->ptrs[parentindex - 1]->keys.size();
            }
            if (right) {
                countright = parent->ptrs[parentindex + 1]->keys.size();
            }
            if (countleft > t - 1 && countleft >= countright) {
                tmp->keys.insert(tmp->keys.begin(), parent->keys[parentindex - 1]);
                parent->keys[parentindex - 1] = getMax(parent->ptrs[parentindex - 1]);
                delete tmp->keys[index + 1];
                tmp->keys.erase(tmp->keys.begin() + index + 1);
            }
            else if (countright > t - 1 && countright >= countleft) {
                tmp->keys.push_back(parent->keys[parentindex]);
                parent->keys[parentindex] = getMin(parent->ptrs[parentindex + 1]);
                delete tmp->keys[index];
                tmp->keys.erase(tmp->keys.begin() + index);
            }
            else if (left) {
                mergeNodes(parent->ptrs[parentindex - 1], tmp, parent, parentindex - 1);
            }
            else if (right) {
                mergeNodes(tmp,parent->ptrs[parentindex + 1], parent, parentindex);
            }
        }
        std::cout << "Ok\n";
    }
    else {
        std::cout << "It is not least\n";
    }
    
}
void BTree::mergeNodes(Node* left, Node* right, Node* parent, int index) {
    Node* newNode = createNode();
    newNode->leaf = left->leaf;
    newNode->keys = left->keys;
    newNode->keys.push_back(parent->keys[index]);
    std::copy(left->keys.begin(), left->keys.end(), inserter(newNode->keys, newNode->keys.end()));
    parent->keys.erase(parent->keys.begin() + index);
    parent->ptrs[index] = newNode;
    parent->ptrs.erase(parent->ptrs.begin() + index + 1);
    if (!left->leaf && !right->leaf) {
        newNode->ptrs = left->ptrs;
        std::copy(right->ptrs.begin(), right->ptrs.end(), inserter(newNode->ptrs, newNode->ptrs.end()));
    }
    free(left);
    free(right);
}
int main()
{
    BTree tree;
    int n = 23;            // 0       1          2          3      4        5         6        7      8       9
    std::string arr[10] = { "Key", "What", "Abracadabra", "Hmm", "John", "Artur", "Amazing", "Car", "Cow", "Apple" };
    for (int i = 0; i < n; i++) {
        unsigned long long key = 0;
        //std::cout << "Enter key" << std::endl;
        //std::cin >> key;
        tree.insert(i, arr[i % 10]);
    }
    tree.print();
    std::string todo;
    while ((std::cin >> todo)) {
        if (todo == "+") {
            unsigned long long key1;
            std::cin >> todo >> key1;
            tree.insert(key1, todo);
        }
        else if (todo == "-") {
            unsigned long long key1;
            std::cin >> key1;
            tree.erase(key1);
        }
        else if (todo == "p") {
            tree.print();
        }
        else {
            unsigned long long key = std::stoi(todo);
            std::cin >> key;
            todo = tree.search(key);
            std::cout << todo << '\n';
        }
        /*else {
            std::cout << "Wrong command\n";
        }*/
    }
}


