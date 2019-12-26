/*****************************************************************
Name    :leftist_heap
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191226 decrease-key, delete, find.
    20191226 reconstruct
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

    //for decrease-key and delete
    Node *parent = NULL;
    int s_value=1;
};

class LeftistHeap{
    //core operation
    Node *meld(Node *h1, Node *h2);

    //inorder and preorder traversal
    void inorderTraversal(Node *parent);
    void preorderTraversal(Node *parent);

    //inorder find
    Node *inorderFind(Node *parent, int key);
public:
    Node *root;
    int size;

    //five operations
    LeftistHeap(int *arr, int size);
    Node *insert(int input);
    int extract_min();
    int minimum();
    void merge(LeftistHeap &lh);

    //decrease-key and delete
    Node *decrease_key(Node *input, int new_val);
    void delete_key(Node *input);

    //find
    Node *find(int key);

    //dump elements
    void dump(void);
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
    meld_root->parent = NULL;
    meld_root->right->parent = meld_root;

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
Node* LeftistHeap::insert(int input)
{
    Node *newNode = new Node;
    newNode->data = input;
    root = meld(root, newNode);

    //update size
    size++;

    return newNode;
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

Node *LeftistHeap::decrease_key(Node *input, int new_val)
{
    if(NULL == input)
        return NULL;
    if(input->data <= new_val)
        return input;

    input->data = new_val;
    Node *current = input;
    Node *parent = input->parent;

    if(current == root)
        return root;

    //step 1 : cut
    if(current == parent->left){
        parent->left = NULL;
    }else{
        parent->right = NULL;
    }
    current->parent = NULL;

    //set 2 : update the s-value
    current = parent;
    while(current){
        //check the leftist
        if(NULL == current->left){
            current->left = current->right;
            current->right = NULL;

            //update the s-value
            current->s_value = 1;
        }else if(NULL == current->right){
            //update the s-value
            current->s_value = 1;
        }else{
            //check the s value
            if(current->left->s_value < current->right->s_value){
                Node *temp;
                temp = current->left;
                current->left = current->right;
                current->right = temp;

                //update the s-value
                current->s_value = current->right->s_value + 1;
            }else{
                break; //until there is no need to swap
            }
        }
        current = current->parent;
    }//while

    //set 3 : meld
    root = meld(root, input);
    return input;
}

void LeftistHeap::delete_key(Node *input)
{
    if(NULL == input)
        return;

    Node *current = input;
    Node *parent = input->parent;

    //step 1 : meld the two children
    //cout << "meld" << endl;
    if(current == root){
        root = meld(current->left, current->right);
    }else{
        if(current == parent->left){
            parent->left = meld(current->left, current->right);
            if(NULL != parent->left)
                parent->left->parent = parent;
        }else{
            parent->right = meld(current->left, current->right);
            if(NULL != parent->right)
                parent->right->parent = parent;
        }
    }

    //step 2 : bottom-up update the s-value
    //cout << "update the s-value" << endl;
    current = parent;
    while(current){
        //check the leftist
        if(NULL == current->left){
            current->left = current->right;
            current->right = NULL;

            //update the s-value
            current->s_value = 1;
        }else if(NULL == current->right){
            //update the s-value
            current->s_value = 1;
        }else{
            //check the s value
            if(current->left->s_value < current->right->s_value){
                Node *temp;
                temp = current->left;
                current->left = current->right;
                current->right = temp;

                //update the s-value
                current->s_value = current->right->s_value + 1;
            }else{
                break; //until there is no need to swap
            }
        }
        current = current->parent;
    }//while

    //setp 3 : delete the node and update size
    //cout << "delete" << endl;
    delete input;
    size--;
}

//binary tree inorder find
Node *LeftistHeap::inorderFind(Node *parent, int key)
{
    if(NULL == parent){
        return NULL;
    }

    Node *result = NULL;
    result = inorderFind(parent->left, key);
    if(result != NULL){ //find the target
        return result;
    }
    if(key == parent->data){
        result = parent;
    }else{
        result = inorderFind(parent->right, key);
    }

    return result;
}

//find
Node *LeftistHeap::find(int key)
{
    return inorderFind(root, key);
}

//binary tree preorder traversal
void LeftistHeap::preorderTraversal(Node *currentNode)
{
    if(NULL == currentNode){
        return;
    }

    cout << " " << currentNode->data;
    preorderTraversal(currentNode->left);
    preorderTraversal(currentNode->right);
}

//binary tree inorder traversal
void LeftistHeap::inorderTraversal(Node *currentNode)
{
    if(NULL == currentNode){
        return;
    }

    inorderTraversal(currentNode->left);
    cout << " " << currentNode->data;
    /*/check the parents
    if(NULL != currentNode->parent){
        cout << " ("<< currentNode->parent->data <<") ";
    }
    //*/
    inorderTraversal(currentNode->right);
}

void LeftistHeap::dump(void)
{
    cout << "Dump the heap : " << endl;
    cout << "inorder :";
    inorderTraversal(root);
    cout << endl;

    cout << "preorder :";
    preorderTraversal(root);
    cout << endl;
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
    LeftistHeap myHeap(random_data, n);
    cout << "myHeap size :" << myHeap.size << endl;
    cout << "The root is :" << myHeap.root->data <<
         ", s-value :" << myHeap.root->s_value <<endl;
    myHeap.dump();

    // Insert the new element
    cout << "\n\tInsert the new element" << endl;
    Node *temp = myHeap.insert(7);
    cout << "Heap size :" << myHeap.size << endl;
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
    cout << "decrease-key : " << temp->data << endl;
    myHeap.decrease_key(myHeap.find(5), 1);
    myHeap.dump();

    cout << "\nBefore delete" << endl;
    cout << "myHeap size :" << myHeap.size << endl;
    myHeap.dump();

    myHeap.delete_key(myHeap.find(7));
    myHeap.delete_key(myHeap.find(7));
    myHeap.delete_key(myHeap.find(6));
    myHeap.delete_key(myHeap.find(8));

    cout << "After delete" << endl;
    cout << "myHeap size :" << myHeap.size << endl;
    myHeap.dump();

    // Find the value
    cout << "\n\tFind the value" << endl;
    int find_value = 9;
    Node *find_node = myHeap.find(find_value);
    cout << "Find Node :";
    if(NULL != find_node){
        cout << find_node->data << endl;
    }else{
        cout << "NULL" << endl;
    }
    

    // Merge the two heap
    cout << "\n\tMerge the two heap" << endl;
    int *random_data2 = random_case(6, n);
#if DEBUG
    cout << "Generate Data2 :";
    for(int i=0; i<n; i++){
        cout << random_data2[i] << " ";
    }
    cout << endl;
#endif

    LeftistHeap myHeap2(random_data2, n);
    //merge
    myHeap.merge(myHeap2);
    cout << "myHeap size :" << myHeap.size << endl;
    myHeap.dump();

    // Heap sort
    cout << "\n\tHeap sort" << endl;
    cout << "Heap sort :";
    while(myHeap.root){
        cout << myHeap.extract_min() << " ";
    }
    cout << endl;

    return 0;
}
/*==============================================================*/