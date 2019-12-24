/*****************************************************************
Name    :leftist_heap
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191218 change to min heap
    20191212 Initial Version
*****************************************************************/
#include <iostream>
#include <vector>
#include <queue>
#define DEBUG (1)
#define SCALE (10)
using namespace std;
using namespace std::chrono;

/*==============================================================*/
//Global area
struct Node{
    int data;
    Node *left=NULL;
    Node *right=NULL;
    int s_value=1;
};

class LeftistHeap{
    //core operation
    Node *meld(Node *h1, Node *h2);
public:
    Node *root;
    int size;

    //five operations
    LeftistHeap(int *arr, int size);
    void insert(int input);
    int extract_min();
    int minimum();
    void merge(LeftistHeap &lh);

    //for learning not necessary
    void preorderTraversal(Node *parent);
};

//core operation
Node *LeftistHeap::meld(Node *h1, Node *h2)
{
    Node *meld_root;
    Node *meld_child;

    //check the null head
    if(NULL == h1){
        return h2;
    }
    if(NULL == h2){
        return h1;
    }

    //pick the new root
    if(h2->data < h1->data){
        meld_root = h2;
        meld_child = h1;
    }else{
        meld_root = h1;
        meld_child = h2;
    }

    //recursive meld the right child
    meld_root->right = meld(meld_root->right, meld_child);

    //check the leftist
    if(NULL == meld_root->left){
        meld_root->left = meld_root->right;
        meld_root->right = NULL;
    }else{
        //check the s value
        if(meld_root->left->s_value < meld_root->right->s_value){
            Node *temp;
            temp = meld_root->left;
            meld_root->left = meld_root->right;
            meld_root->right = temp;
        }

        meld_root->s_value = meld_root->right->s_value + 1;
    }

    return meld_root;
}

//Initialize
LeftistHeap::LeftistHeap(int *arr, int size)
{
    //assign size
    this->size = size;

    //push into queue
    queue<Node*> q;
    for(int i=0; i<size; i++){
        Node *newNode = new Node;
        newNode->data = arr[i];
        q.push(newNode);
    }

    //extract_min two elements and then meld before push back into the queue
    while(q.size() >= 2){
        //extract_min two elements
        Node *h1 = q.front();
        q.pop();
        Node *h2 = q.front();
        q.pop();

        //meld
        Node *h_meld = meld(h1, h2);

        //push back into queue
        q.push(h_meld);
    }

    root = q.front();
    q.pop();
}

//insert
void LeftistHeap::insert(int input)
{
    Node *newNode = new Node;
    newNode->data = input;
    root = meld(root, newNode);

    //update size
    size++;
}

//extract_min
int LeftistHeap::extract_min()
{
    if(NULL == root)
        return -1;

    Node *pre = root;

    //meld two subtree
    root = meld(root->left, root->right);

    //delete
    delete pre;

    //update size
    size--;

    return pre->data;
}

//minimum
int LeftistHeap::minimum()
{
    if(NULL == root)
        return -1;

    return root->data;
}

//merge
void LeftistHeap::merge(LeftistHeap &lh)
{
    root = meld(root, lh.root);
    size = size + lh.size;
}

//for learning not necessary
void LeftistHeap::preorderTraversal(Node *parent)
{
    if(NULL == parent){
        return;
    }

    cout << " " << parent->data;
    preorderTraversal(parent->left);
    preorderTraversal(parent->right);
}

/*==============================================================*/
//Function area
int *random_case(int base, int number)
{
    int *result = new int[number];

    //generate index ordered arrary
    for(int i=0; i<number; i++){
        result[i] = base + i;
    }

    //swap each position
    srand(time(NULL));
    for(int i=0; i<number-1; i++){
        int j = i + rand() / (RAND_MAX / (number-i));
        //swap
        int t=result[i];
        result[i] = result[j];
        result[j]=t;
    }

    return result;
}

/*==============================================================*/
int main(int argc, char const *argv[]){
    int n=SCALE;

    //generate data
    int *random_data = random_case(1, n);

#if DEBUG
    cout << "Before build Heap :";
    for(int i=0; i<n; i++){
        cout << random_data[i] << " ";
    }
    cout << endl;
#endif

    //constructor
    LeftistHeap myHeap(random_data, n);
    cout << "myHeap size :" << myHeap.size << endl;

    cout << "The root is :" << myHeap.root->data <<
         ", s-value :" << myHeap.root->s_value <<endl;

    cout << "preorder Traversal :";
    myHeap.preorderTraversal(myHeap.root);
    cout << endl;

    //insertion
    int input = 7;
    cout << "Heap Insertion :" << input << endl;
    myHeap.insert(input);
    cout << "myHeap size :" << myHeap.size << endl;

    //merge
    int *random_data2 = random_case(6, n);
    LeftistHeap myHeap2(random_data2, n);
    myHeap.merge(myHeap2);
    cout << "myHeap size :" << myHeap.size << endl;

    //extract_min
    cout << "Heap sort :";
    int size = myHeap.size;
    for(int i=0; i<size; i++){
        cout << myHeap.extract_min() << " ";
    }
    cout << endl;

    return 0;
}
/*==============================================================*/