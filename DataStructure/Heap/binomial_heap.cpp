/*****************************************************************
Name    :binomial_heap
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191218 Initial Version
*****************************************************************/
#include <iostream>
#include <vector>
#define DEBUG (1)
#define SCALE (13)
using namespace std;
using namespace std::chrono;

/*==============================================================*/
//Global area
//Left-Child Right-sibling
struct Node{
    int data;
    Node *parent=NULL;
    Node *child=NULL;
    Node *sibling=NULL;

    // the height for the subtree
    // 1<<degree (2^degree)
    int degree=0;
};

class BinomialHeap{
    //core operation
    Node *treeUnion(Node *n1, Node *n2); //must be the same degree
    void heapUnion(bool isinsert); //handle with tree union
public:
    Node *minNode=NULL;
    Node *head=NULL;

    //five operations
    BinomialHeap();
    BinomialHeap(int *arr, int size);
    void insert(int input);
    int extract_min();
    int minimum();
    void merge(BinomialHeap &bh);

    //for learning not necessary
    void preorderTraversal(Node *parent);
    void dumpTree();
};

BinomialHeap::BinomialHeap()
{

}

//core operation
Node *BinomialHeap::treeUnion(Node *n1, Node *n2)
{
    //check if the same degree
    if(n1->degree != n2->degree)
        return NULL;

    //compare to find new root
    Node *less = (n1->data < n2->data)? n1 : n2;
    Node *greater = (n1 == less)? n2 : n1;

    //adjust the pointers and degree
    if(less->sibling == greater){
        less->sibling = greater->sibling;
    }
    greater->parent = less;
    greater->sibling = less->child;
    less->child = greater;
    less->degree++;

    return less;
}

//core operation
void BinomialHeap::heapUnion(bool isinsert)
{
    Node *pre = NULL;
    Node *current = head;
    Node *next = current->sibling;

    //keep tracking min
    minNode = head;

    //tree union for current and next if needed
    while(next){
        //update the min
        minNode = (next->data < minNode->data) ? next : minNode;

        if((current->degree != next->degree) || 
            (next->sibling && next->sibling->degree == current->degree))
        {
            //the degree of the first two trees is not the same
            //or
            //the degree of the first three trees are the same, 
            //we just deal with the last two.
            pre = current;
            current = next;

            //in the case of insertion , just break if no carry
            if(isinsert)
                break;
        }else{
            //tree union
            current = treeUnion(current, next);
            if(pre){
                pre->sibling = current;
            }else{
                head = current;
            }
            
        }

        next = current->sibling;
    }
}

//Initialize
BinomialHeap::BinomialHeap(int *arr, int size)
{
    for(int i=0; i<size; i++){
        insert(arr[i]);
    }
}

//insert
void BinomialHeap::insert(int input)
{
    Node *newNode = new Node();
    newNode->data = input;

    //insert into the head
    if(head){
        newNode->sibling = head;
    }
    head = newNode;

    //adjust the binomial heap
    heapUnion(true);
}

//extract_min
int BinomialHeap::extract_min()
{
    if(NULL==head)
        return -1;

    Node *target = minNode;
    int result = minNode->data;

    //find the previous binomial tree
    Node *pre = NULL;
    Node *current = head;
    while(current){
        if(current == target)
            break;

        pre = current;
        current = current->sibling;
    }

    //remove it
    if(minNode == head){
        head = minNode->sibling;
    }else{
        pre->sibling = minNode->sibling;
    }

    //update the minNode
    current = head;
    minNode = head;
    while(current){
        if(current->data < minNode->data)
            minNode = current;

        current = current->sibling;
    }

    //reverse the sibling list
    //and update min Node of new BH
    pre = NULL;
    current = target->child;
    Node *next = NULL;
    Node *tempMinNode = current;
    while(current){
        //update the minNode
        if(current->data < tempMinNode->data)
            tempMinNode = current;

        next = current->sibling;
        current->parent = NULL;
        current->sibling = pre;

        pre = current;
        current = next;
    }

    //create the new BH
    if(NULL != pre){
        BinomialHeap bh;
        bh.head = pre;
        bh.minNode = tempMinNode;
        //merge with updating the min Node
        merge(bh);
    }

    //delete the min Node
    delete target;
    target = NULL;

    return result;
}

//minimum
int BinomialHeap::minimum()
{
    if(NULL==head)
        return -1;
    
    return minNode->data;
}

//merge
void BinomialHeap::merge(BinomialHeap &bh)
{
    if(NULL == head){
        head = bh.head;
        minNode = bh.minNode;
        return;
    }

    if(NULL == bh.head){
        return;
    }

    //類似 merge sort 的作法
    Node *h1 = head;
    Node *h2 = bh.head;
    Node *current;

    //pick the new head and set current node
    if(h1->degree > h2->degree){
        current = head = h2;
        h2 = h2->sibling;
    }else{
        current = head = h1;
        h1 = h1->sibling;
    }

    //until one of them is running out.
    while(h1 && h2){
        if(h1->degree > h2->degree){
            current->sibling = h2;
            current = h2;
            h2 = h2->sibling;
        }else{
            current->sibling = h1;
            current = h1;
            h1 = h1->sibling;
        }
    }

    //if h1 has last elements
    if(h1){
        current->sibling = h1;
    }

    //if h2 has last elements
    if(h2){
        current->sibling = h2;
    }

    //adjust the binomial heap
    heapUnion(false);
}

//for learning not necessary
void BinomialHeap::preorderTraversal(Node *current)
{
    if(NULL == current){
        return;
    }

    cout << " " << current->data;
    preorderTraversal(current->child);
    if(current->parent){ //parent of root is NULL
        preorderTraversal(current->sibling);
    }
}

//for learning not necessary
void BinomialHeap::dumpTree()
{
    if(NULL == head)
        return;

    Node *current = head;
    while(current){
        cout << "B(" << current->degree << ") = ";
        //preorder traversal
        preorderTraversal(current);
        cout << endl;

        current = current->sibling;
    }
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
    int size;

    //generate data
    int *random_data = random_case(1, n);

#if DEBUG
    cout << "Before build Heap :";
    for(int i=0; i<n; i++){
        cout << random_data[i] << " ";
    }
    cout << endl;
#endif

    BinomialHeap bh(random_data, n);
    cout << "Dump the bh : " << endl;
    bh.dumpTree();

    //union
    n=13;
    int *random_data2 = random_case(20, n);

#if DEBUG
    cout << "Before build Heap2 :";
    for(int i=0; i<n; i++){
        cout << random_data2[i] << " ";
    }
    cout << endl;
#endif

    BinomialHeap bh2(random_data2, n);
    cout << "Dump the bh2 : " << endl;
    bh2.dumpTree();

    cout << "Union two BinomialHeap " << endl;
    bh.merge(bh2);
    cout << "Dump the bh : " << endl;
    bh.dumpTree();

    cout << "min : " << bh.minimum() << endl;

    //extract_min
    cout << "Heap sort :";
    while(bh.head){
        cout << bh.extract_min() << " ";
    }
    cout << endl;

    return 0;
}
/*==============================================================*/