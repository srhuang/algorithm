/*****************************************************************
Name    :fibonacci_heap
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20200302 complete all seven functions
    20200222 consolidate and extract_min
    20200114 Initial Version
*****************************************************************/
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#define DEBUG (1)
#define SCALE (20)
using namespace std;
using namespace std::chrono;

/*==============================================================*/
//Global area
//Left-Child Right-sibling
struct Node{
    int data;
    Node *parent=NULL;
    Node *child=NULL;
    Node *left=NULL;
    Node *right=NULL;

    // the height for the subtree
    // 1<<degree (2^degree)
    int degree=0;
    bool mark=false;
};

class FibonacciHeap{
private:
    //core operation
    Node *treeUnion(Node *n1, Node *n2); //must be the same degree
    void consolidate(void); //handle with tree union

    //levelorder traversal
    void levelorderTraversal(Node *current);

    //preorder find
    Node *levelorderFind(Node *current, int key);
public:
    Node *minNode=NULL;
    int number=0;

    //five operations
    FibonacciHeap();
    FibonacciHeap(int *arr, int size);
    Node *insert(int input);
    int extract_min();
    int minimum();
    void merge(FibonacciHeap &fh);

    //cut and cascade cut
    void cut(Node *current, Node *parent);
    void cascade_cut(Node *current);

    //decrease-key and delete
    Node *decrease_key(Node *input, int new_val);
    void delete_key(Node *input);

    //find
    Node *find(int key);

    //dump elements
    void dump(void);
};

FibonacciHeap::FibonacciHeap()
{

}

//core operation
Node *FibonacciHeap::treeUnion(Node *n1, Node *n2)
{
    //check if the same degree
    if(n1->degree != n2->degree)
        return NULL;

    //compare to find new root
    Node *less = (n1->data < n2->data)? n1 : n2;
    Node *greater = (n1 == less)? n2 : n1;

    //adjust the pointers and degree
    //remove the greater from root list
    greater->left->right = greater->right;
    greater->right->left = greater->left;

    //add the greater node to the child of less node
    if(NULL == less->child){
        greater->left = greater;
        greater->right = greater;
        greater->parent = less;
        less->child = greater;
    }else{
        greater->right = less->child;
        greater->left = less->child->left;
        greater->parent = less;
        less->child->left->right = greater;
        less->child->left = greater;

        //child always point to the min node of child nodes
        if(greater->data < (less->child->data)){
            less->child = greater;
        }
    }
    less->degree++;

    return less;
}

//core operation
void FibonacciHeap::consolidate(void)
{
    int max_degree = log(number) / log(2);
    Node* arr[max_degree+1];

    //initialize the arr
    for(int i=0; i<=max_degree; i++){
        arr[i] = NULL;
    }

    //trace each node in the root list, from min back to min circularly.
    Node *current = minNode;
    Node *next_node;
    do{
        //min node just for marking the start node
        //consolidating until every root has a distinct degree value.
        int temp_degree = current->degree;
        next_node = current->right;

        //current might reassign, current->right might reassign, too.
        while(arr[temp_degree] != NULL && arr[temp_degree]!= current){
            Node *temp = arr[temp_degree];

            if(current==minNode){
                current = treeUnion(temp, current);
                minNode = current;
            }else{
                current = treeUnion(temp, current);
            }

            //keep the min node(start point) and next node in the root list
            if(temp==minNode)
                minNode = current;
            if(temp==next_node){
                next_node=current->right;
            }

            arr[temp_degree] = NULL;
            temp_degree++;
        }

        arr[temp_degree] = current;
        current = next_node;
    }while(current != minNode);

    //reconstruct the root list and find the min node    
    minNode = NULL;
    for(int i=0; i<=max_degree; i++){
        if(NULL==arr[i])
            continue;

        arr[i]->left = arr[i];
        arr[i]->right = arr[i];
        if(NULL==minNode){
            minNode = arr[i];
        }else{
            //add to the left of the min node
            arr[i]->right = minNode;
            arr[i]->left = minNode->left;
            minNode->left->right = arr[i];
            minNode->left = arr[i];

            //update the min node
            if(arr[i]->data < minNode->data){
                minNode = arr[i];
            }
        }
    }//for
}

//Initialize
FibonacciHeap::FibonacciHeap(int *arr, int size)
{
    for(int i=0; i<size; i++){
        insert(arr[i]);
        consolidate();
    }
}

//insert
Node *FibonacciHeap::insert(int input)
{
    Node *newNode = new Node();
    newNode->data = input;

    //initial the node pointers
    newNode->parent = NULL;
    newNode->child = NULL;
    newNode->left = newNode;
    newNode->right = newNode;

    //initial the node internal data
    newNode->degree = 0;
    newNode->mark = false;

    //insert the new node at the left of the min node
    if(NULL != minNode){
        //handle the pointers
        minNode->left->right = newNode;
        newNode->right = minNode;
        newNode->left = minNode->left;
        minNode->left = newNode;

        //update the min node
        if(newNode->data < minNode->data){
            minNode = newNode;
        }
    }else{
        minNode = newNode;
    }

    //add the number of heap
    number++;

    return newNode;
}

//extract_min
int FibonacciHeap::extract_min()
{
    if(NULL==minNode)
        return -1;

    int result = minNode->data;

    if(minNode->child != NULL){
        Node *current = minNode->child;
        Node *next_node;
        do{
            next_node = current->right;
            //add into the root list
            minNode->left->right = current;
            current->right = minNode;
            current->left = minNode->left;
            minNode->left = current;

            current->parent = NULL;
            current = next_node;
        }while(current != minNode->child);
    }

    //modify the min node and the root list
    Node *target = minNode;
    if(minNode == minNode->right && NULL == minNode->child){
        minNode = NULL;
    }else{
        //remove the min node
        minNode->left->right = minNode->right;
        minNode->right->left = minNode->left;
        minNode = minNode->right;

        //update the min node
        consolidate();
    }
    
    //delete the min Node
    delete target;
    target = NULL;
    number--;

    return result;
}

//minimum
int FibonacciHeap::minimum()
{
    if(NULL==minNode)
        return -1;
    
    return minNode->data;
}

//merge
void FibonacciHeap::merge(FibonacciHeap &fh)
{
    if(NULL == minNode){
        minNode = fh.minNode;
        return;
    }

    if(NULL == fh.minNode){
        return;
    }

    //concatenate
    fh.minNode->left->right = minNode;
    Node *temp = fh.minNode->left;
    fh.minNode->left = minNode->left;
    minNode->left->right = fh.minNode;
    minNode->left = temp;

    //update the min node
    if(fh.minNode->data < minNode->data)
    {
        minNode = fh.minNode;
    }

    //update the numder
    number += fh.number;
}

//Cutting a node in the heap to be placed in the root list
void FibonacciHeap::cut(Node *current, Node *parent)
{
    //check if only one child of the parent
    if(current == current->right){
        parent->child = NULL;
    }
    //adjust the child of parent if need
    if(current == parent->child){
        parent->child = current->right;
    }
    //remove the current node from the parent
    current->left->right = current->right;
    current->right->left = current->left;

    //decrease the degree of the parent
    parent->degree--;

    //add into the root list
    current->left = minNode->left;
    current->right = minNode;
    minNode->left->right = current;
    minNode->left = current;

    //handle the meta data
    current->parent = NULL;
    current->mark = false;
}

void FibonacciHeap::cascade_cut(Node *current)
{
    Node *parent = current->parent;
    if(parent != NULL){
        if(!current->mark){
            current->mark=true;
        }else{
            cut(current, parent);
            cascade_cut(parent);
        }
    }
}

//decrease key
Node *FibonacciHeap::decrease_key(Node *input, int new_val)
{
    if(NULL == input)
        return NULL;
    if(input->data <= new_val)
        return input;

    input->data = new_val;
    Node *current = input;
    Node *parent = current->parent;
    if(parent != NULL && current->data < parent->data){
        cut(current, parent);
        cascade_cut(parent);
    }

    //check the min node
    if(current->data < minNode->data){
        minNode = current;
    }

    return current;
}

//delete
void FibonacciHeap::delete_key(Node *input)
{
    if(NULL == input)
        return;

    Node *current = input;
    Node *parent = current->parent;
    while(parent)
    {
        //swap
        int temp = current->data;
        current->data = parent->data;
        parent->data = temp;

        //bottom-up
        current = parent;
        parent = current->parent;
    }

    //extract min
    minNode = current;
    extract_min();
}

Node *FibonacciHeap::levelorderFind(Node *current, int key)
{
    if(NULL == current){
        return NULL;
    }

    queue<Node*> q;
    Node *result = NULL;
    if(current->child)  q.push(current->child);

    while(!q.empty()){
        Node *temp = q.front();
        q.pop();

        Node *terminal = temp;
        do{
            if(temp->data == key){
                return temp;
            }
            if(temp->child)   
                q.push(temp->child);
            temp = temp->right;
        }while(temp != terminal);
    }//while
    return result;
}

//find
Node *FibonacciHeap::find(int key)
{
    if(NULL == minNode)
        return NULL;

    Node *current = minNode;
    Node *result = NULL;
    do{
        //levelorder traversal
        
        if(current->data == key){
            result = current;
        }else{
            result = levelorderFind(current, key);
            if(result != NULL)
                break;
        }
        current = current->right;
    }while(current!=minNode);

    return result;
}

//binomial tree levelorder traversal
void FibonacciHeap::levelorderTraversal(Node *current)
{
    if(NULL == current){
        return;
    }
    cout << " " << current->data;
    queue<Node*> q;

    if(current->child)  q.push(current->child);

    while(!q.empty()){
        Node *temp = q.front();
        q.pop();

        Node *terminal = temp;
        cout << "(";
        do{
            cout << " " << temp->data;
            if(temp->parent)cout << "[" << temp->parent->data <<  "]";
            if(temp->child)   q.push(temp->child);

            temp = temp->right;
        }while(temp != terminal);
        cout << ")";
    }//while
}

//dump elements
void FibonacciHeap::dump(void)
{
    if(NULL == minNode)
        return;

    cout << "Dump the heap : " << endl;
    Node *current = minNode;
    do{
        cout << "B(" << current->degree << ") = ";
        //preorder traversal
        levelorderTraversal(current);
        cout << endl;

        current = current->right;
    }while(current!=minNode);
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
    cout << "Generate Data :";
    for(int i=0; i<n; i++){
        cout << random_data[i] << " ";
    }
    cout << endl;
#endif

    // Initialize from the array
    cout << "\n\tInitialize from the array" << endl;
    FibonacciHeap myHeap(random_data, n);
    myHeap.dump();
    cout << "minimum :" << myHeap.minimum() << endl;

    // Insert the new element
    cout << "\n\tInsert the new element" << endl;
    Node *temp = myHeap.insert(8);
    myHeap.insert(15);
    myHeap.insert(4);
    myHeap.dump();

    // Test minimum and extract min
    cout << "\n\tTest minimum and extract min" << endl;
    cout << "minimum :" << myHeap.minimum() << endl;
    cout << "extract_min :" << myHeap.extract_min() << endl;
    myHeap.dump();
    cout << "minimum :" << myHeap.minimum() << endl;

    // Test decrease-key and delete
    cout << "\n\tTest decrease-key and delete" << endl;
    myHeap.dump();
    int decrease_value = 7;

    Node *decrease_node = myHeap.decrease_key(myHeap.find(7), 1);
    cout << "decrease-key from " << decrease_value << " to " 
        << decrease_node->data << endl;
    myHeap.dump();

    cout << "\nBefore delete" << endl;
    myHeap.dump();

    myHeap.delete_key(myHeap.find(6));
    //myHeap.delete_key(myHeap.find(4));

    cout << "After delete" << endl;
    myHeap.dump();

    // Find the value
    int find_value = 9;
    cout << "\n\tFind the value : " << find_value << endl;
    Node *find_node = myHeap.find(find_value);
    cout << "Find Node :";
    if(NULL != find_node){
        cout << find_node->data << endl;
    }else{
        cout << "NULL" << endl;
    }

    // Merge the two heap
    cout << "\n\tMerge the two heap" << endl;
    n=13;
    int *random_data2 = random_case(50, n);
#if DEBUG
    cout << "Generate Data2 :";
    for(int i=0; i<n; i++){
        cout << random_data2[i] << " ";
    }
    cout << endl;
#endif

    FibonacciHeap myHeap2(random_data2, n);
    myHeap2.dump();

    myHeap.merge(myHeap2);
    myHeap.dump();

    // Heap sort
    cout << "\n\tHeap sort" << endl;
    cout << "Heap sort :";
    while(myHeap.minNode){
        cout << myHeap.extract_min() << " ";
    }
    cout << endl;

    return 0;
}
/*==============================================================*/