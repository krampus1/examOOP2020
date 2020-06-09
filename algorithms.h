#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QVector>
#include <QString>
#include <QObject>

using namespace std;

class adress{ //main class
public:
    QString country;
    QString region;
    QString city;
    QString street;
    int house;
    int room;

    adress(){
    }

    adress(QString cnt, QString reg, QString cit, QString str, int hos, int rom){
        country = cnt;
        region = reg;
        city = cit;
        street = str;
        house = hos;
        room = rom;
    }

    friend bool operator> (const adress &d1, const adress &d2);
    friend bool operator<= (const adress &d1, const adress &d2);
    friend bool operator< (const adress &d1, const adress &d2);
    friend bool operator>= (const adress &d1, const adress &d2);
};

bool operator> (const adress &d1, const adress &d2){
    return d1.room > d2.room;
}

bool operator>= (const adress &d1, const adress &d2){
    return d1.room >= d2.room;
}

bool operator< (const adress &d1, const adress &d2){
    return d1.room < d2.room;
}

bool operator<= (const adress &d1, const adress &d2){
    return d1.room <= d2.room;
}

class database{
public:
    QVector<adress> data;

    database(){
    }

    void add_rand(){
        QString cnt = QString::fromStdString("Country" + to_string(data.size()));
        QString reg = QString::fromStdString("Region" + to_string(data.size()));
        QString cit = QString::fromStdString("City" + to_string(data.size()));
        QString str = QString::fromStdString("Street" + to_string(data.size()));
        int hos = rand()%100 + 1;
        int rom = rand()%200 + 1;;
        data.push_back(adress(cnt, reg, cit, str, hos, rom));
    }

    void add(QString cnt, QString reg, QString cit, QString str, int hos, int rom){
        data.push_back(adress(cnt, reg, cit, str, hos, rom));
    }
};

/* list node */
struct Node{
    adress * a;
    Node * next;
};

class MyList{ //SL
public:
    Node * head;
    MyList(){
        head = nullptr;
    }
    ~MyList(){
        while (head != nullptr){
            Node *tmp = head->next;
            delete head;
            head = tmp;
        }
    }

    void add(adress * x){ //adding
        Node * tmp = new Node;
        tmp->a = x;
        tmp->next = head;
        head = tmp;
    }

    void del(adress * x){ //deleting
        if (head->a == x){
            Node *tmp = head->next;
            delete head;
            head = tmp;
            return;
        } else {
            for (auto i = head; i != nullptr; i = i->next){
                Node * k = i->next;
                if (k->a == x){
                    i->next = k->next;
                    delete k;
                    return;
                }
            }
        }
    }

    bool implement(adress * x){ //implemetion
        for (auto i = head; i != nullptr; i = i->next){
            if (i->a == x){
                return true;
            }
        }
        return false;
    }

    int size(){
        int k = 0;
        for (auto i = head; i != nullptr; i = i->next){
            k++;
        }
        return k;
    }
};

// An AVL tree node
class node{
    public:
    adress * key;
    node *left, *right;
};

/* Helper function that allocates a new node with the given key and NULL left and right pointers.*/
node* newNode(adress * key){
    node* Node = new node();
    Node->key = key;
    Node->left = Node->right = nullptr;
    return (Node);
}

/* A utility function to right rotate subtree rooted with y.*/
node *rightRotate(node *x){
    node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

/* A utility function to left rotate subtree rooted with x.*/
node *leftRotate(node *x){
    node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

/* This function brings the key at root if key is present in tree. If key is not present, then it brings the last accessed item at
root. This function modifies the tree and returns the new root.*/
node *splay(node *root, adress * key)
{
    // Base cases: root is NULL or
    // key is present at root
    if (root == nullptr || root->key == key)
        return root;

    // Key lies in left subtree
    if (*root->key > *key){
        // Key is not in tree, we are done
        if (root->left == nullptr) return root;

        // Zig-Zig (Left Left)
        if (*root->left->key > *key){
            // First recursively bring the
            // key as root of left-left
            root->left->left = splay(root->left->left, key);

            // Do first rotation for root,
            // second rotation is done after else
            root = rightRotate(root);
        }
        else if (*root->left->key < *key){ // Zig-Zag (Left Right)
            // First recursively bring
            // the key as root of left-right
            root->left->right = splay(root->left->right, key);

            // Do first rotation for root->left
            if (root->left->right != nullptr)
                root->left = leftRotate(root->left);
        }

        // Do second rotation for root
        return (root->left == nullptr)? root: rightRotate(root);
    }
    else // Key lies in right subtree
    {
        // Key is not in tree, we are done
        if (root->right == nullptr) return root;

        // Zag-Zig (Right Left)
        if (*root->right->key > *key)
        {
            // Bring the key as root of right-left
            root->right->left = splay(root->right->left, key);

            // Do first rotation for root->right
            if (root->right->left != nullptr)
                root->right = rightRotate(root->right);
        }
        else if (*root->right->key < *key)// Zag-Zag (Right Right)
        {
            // Bring the key as root of
            // right-right and do first rotation
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        // Do second rotation for root
        return (root->right == nullptr)? root: leftRotate(root);
    }
}

// Function to insert a new key k in splay tree with given root
node *insert(node *root, adress * k){
    // Simple Case: If tree is empty
    if (root ==nullptr) return newNode(k);

    // Bring the closest leaf node to root
    root = splay(root, k);

    // If key is already present, then return
    if (root->key == k) return root;

    // Otherwise allocate memory for new node
    node *newnode = newNode(k);

    // If root's key is greater, make root as right child of newnode and copy the left child of root to newnode
    if (*root->key > *k){
        newnode->right = root;
        newnode->left = root->left;
        root->left = nullptr;
    }
    // If root's key is smaller, make root as left child of newnode and copy the right child of root to newnode
    else {
        newnode->left = root;
        newnode->right = root->right;
        root->right = nullptr;
    }

    return newnode; // newnode becomes new root
}

// The delete function for Splay tree. Note that this function returns the new root of Splay Tree after removing the key
node* delete_key(node * root, adress * key){
    node *temp;
    if (!root)
        return nullptr;

    // Splay the given key
    root = splay(root, key);

    // If key is not present, then
    // return root
    if (key != root->key)
        return root;

    // If key is present
    // If left child of root does not exist
    // make root->right as root
    if (!root->left){
        temp = root;
        root = root->right;
    }
    // Else if left child exits
    else {
        temp = root;
        /*Note: Since key == root->key,
        so after Splay(key, root->lchild),
        the tree we get will have no right child tree
        and maximum node in left subtree will get splayed*/
        // New root
        root = splay(root->left, key);

        // Make right child of previous root  as
        // new root's right child
        root->right = temp->right;
    }

    // free the previous root node, that is,
    // the node containing the key
    free(temp);

    // return root of the new Splay Tree
    return root;
}

/* The search function for Splay tree. Note that this function returns the new root of Splay Tree. If key is present in tree then, it is moved to root.*/
node *search(node *root, adress * key)
{
    return splay(root, key);
}

//Hash Table with Linear Probing
//template for generic type
template<typename K, typename V>

//Hashnode class
class hashnode{
    public:
    V value;
    K key;

    //Constructor of hashnode
    hashnode(K key, V value)
    {
        this->value = value;
        this->key = key;
    }
};

//template for generic type
template<typename K, typename V>

//Our own Hashmap class
class hashmap
{
public:
    //hash element array
    hashnode<K,V> **arr;
    int capacity;
    //current size
    int size;
    //dummy node
    hashnode<K,V> *dummy;
    hashmap()
    {
        //Initial capacity of hash array
        capacity = 128;
        size=0;
        arr = new hashnode<K,V>*[capacity];

        //Initialise all elements of array as NULL
        for(int i=0 ; i < capacity ; i++)
            arr[i] = NULL;

        //dummy node with value and key -1
        dummy = new hashnode<K,V>(-1, nullptr);
    }
    // This implements hash function to find index
    // for a key
    int hashCode(K key)
    {
        return key % capacity;
    }

    //Function to add key value pair
    void insertNode(K key, V value)
    {
        hashnode<K,V> *temp = new hashnode<K,V>(key, value);

        // Apply hash function to find index for given key
        int hashIndex = hashCode(key);

        //find next free space
        while(arr[hashIndex] != NULL && arr[hashIndex]->key != key
               && arr[hashIndex]->key != -1)
        {
            hashIndex++;
            hashIndex %= capacity;
        }

        //if new node to be inserted increase the current size
        if(arr[hashIndex] == NULL || arr[hashIndex]->key == -1)
            size++;
        arr[hashIndex] = temp;
    }

    //Function to delete a key value pair
    V deleteNode(int key)
    {
        // Apply hash function to find index for given key
        int hashIndex = hashCode(key);

        //finding the node with given key
        while(arr[hashIndex] != NULL)
        {
            //if node found
            if(arr[hashIndex]->key == key)
            {
                hashnode<K,V> *temp = arr[hashIndex];

                //Insert dummy node here for further use
                arr[hashIndex] = dummy;

                // Reduce size
                size--;
                return temp->value;
            }
            hashIndex++;
            hashIndex %= capacity;

        }

        //If not found return null
        return NULL;
    }

    //Function to search the value for a given key
    V get(int key)
    {
        // Apply hash function to find index for given key
        int hashIndex = hashCode(key);
        //finding the node with given key
        while(arr[hashIndex] != NULL)
        {    int counter =0;
             if(counter++>capacity)  //to avoid infinite loop
                return NULL;
            //if node found return its value
            if(arr[hashIndex]->key == key)
                return arr[hashIndex]->value;
            hashIndex++;
            hashIndex %= capacity;
        }

        //If not found return null
        return NULL;
    }

    //Return current size
    int sizeofMap()
    {
        return size;
    }

    //Return true if size is 0
    bool isEmpty()
    {
        return size == 0;
    }
};

//Hash Table chaining
// HashNode Class Declaration
const int TABLE_SIZE = 128;

class HashNode
{
    public:
    int key;
    adress * value;
    HashNode* next;
    HashNode(int key, adress * value){
        this->key = key;
        this->value = value;
        this->next = nullptr;
    }
};

//HashMap Class Declaration
class HashMap
{
    private:
        HashNode** htable;
    public:
        HashMap()
        {
            htable = new HashNode*[TABLE_SIZE];
            for (int i = 0; i < TABLE_SIZE; i++)
                htable[i] = nullptr;
        }
        ~HashMap()
        {
            delete[] htable;
        }
        //Hash Function
        int HashFunc(int key)
        {
            return key % TABLE_SIZE;
        }

        //Insert Element at a key
        void Insert(int key, adress * value)
        {
            int hash_val = HashFunc(key);
            HashNode* prev = nullptr;
            HashNode* entry = htable[hash_val];
            while (entry != nullptr)
            {
                prev = entry;
                entry = entry->next;
            }
            if (entry == nullptr)
            {
                entry = new HashNode(key, value);
                if (prev == nullptr)
            {
                    htable[hash_val] = entry;
                }
            else
            {
                    prev->next = entry;
                }
            }
            else
            {
                entry->value = value;
            }
        }

        //Remove Element at a key
        void Remove(int key)
        {
            int hash_val = HashFunc(key);
            HashNode* entry = htable[hash_val];
            HashNode* prev = nullptr;
            if (entry == nullptr || entry->key != key)
            {
                return;
            }
            while (entry->next != nullptr)
        {
                prev = entry;
                entry = entry->next;
            }
            if (prev != nullptr)
            {
                prev->next = entry->next;
            }
            delete entry;
        }
        //Search Element at a key
        adress * Search(int key)
        {
            int hash_val = HashFunc(key);
            HashNode* entry = htable[hash_val];
            while (entry != nullptr){
                if (entry->key == key){
                    return entry->value;
                }
                entry = entry->next;
            }
                return nullptr;
        }
};

//insertion sort
void insertionSort(QVector<int> &vec){
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        // Searching the upper bound, i.e., first
        // element greater than *it from beginning
        auto const insertion_point = upper_bound(vec.begin(), it, *it);

        // Shifting the unsorted part
        rotate(insertion_point, it, it+1);
    }
}

//quicksort
void swap(QVector<int>& v, int x, int y) {
    int temp = v[x];
    v[x] = v[y];
    v[y] = temp;
}

void quicksort(QVector<int> &vec, int L, int R) {
    int i, j, mid, piv;
    i = L;
    j = R;
    mid = L + (R - L) / 2;
    piv = vec[mid];

    while (i<R || j>L) {
        while (vec[i] < piv)
            i++;
        while (vec[j] > piv)
            j--;

        if (i <= j) {
            swap(vec, i, j); //error=swap function doesnt take 3 arguments
            i++;
            j--;
        }
        else {
            if (i < R)
                quicksort(vec, i, R);
            if (j > L)
                quicksort(vec, L, j);
            return;
        }
    }
}

//merge sort
QVector<int> merge(QVector<int> left, QVector<int> right)
{
    QVector<int> result;
    while (left.size() > 0 || right.size() > 0) {
        if (left.size() > 0 && right.size() > 0) {
            if (left.front() <= right.front()) {
                result.push_back(left.front());
                left.erase(left.begin());
            }
            else {
                result.push_back(right.front());
                right.erase(right.begin());
            }
        }  else if (left.size() > 0) {
            for (int i = 0; i < left.size(); i++)
                result.push_back(left[i]);
            break;
        }  else if (right.size() > 0) {
            for (int i = 0; i < right.size(); i++)
                result.push_back(right[i]);
            break;
        }
    }
    return result;
}

QVector<int> mSort(QVector<int> m){
    if (m.size() <= 1)
        return m;

    QVector<int> left, right, result;
    int middle = (m.size()+ 1) / 2;

    for (int i = 0; i < middle; i++) {
        left.push_back(m[i]);
    }

    for (int i = middle; i < m.size(); i++) {
        right.push_back(m[i]);
    }

    left = mSort(left);
    right = mSort(right);
    result = merge(left, right);

    return result;
}

void MergeSort(QVector<int> &m){
    m = mSort(m);
}

//radix sort
void radixSort(QVector<int> &a) {
    int d = 8, w = 32;
    for (int p = 0; p < w/d; p++) {
        QVector<int> c(1<<d, 0);
        // the next three for loops implement counting-sort
        QVector<int> b(a.size());
        for (int i = 0; i < a.size(); i++)
            c[(a[i] >> d*p)&((1<<d)-1)]++;
        for (int i = 1; i < 1<<d; i++)
            c[i] += c[i-1];
        for (int i = a.size()-1; i >= 0; i--)
            b[--c[(a[i] >> d*p)&((1<<d)-1)]] = a[i];
        a = b;
    }
}

#endif // ALGORITHMS_H
