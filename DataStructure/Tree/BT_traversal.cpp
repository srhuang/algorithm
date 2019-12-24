/*****************************************************************
Name    :BT_traversal
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20190930 Initial Version
*****************************************************************/
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
using namespace std;
/*==============================================================*/

struct TreeNode{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(-1), left(NULL), right(NULL) {}
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class BinaryTree{
private:
    void LevelorderConstruct(stringstream &ss);
public:
    TreeNode *root;
    BinaryTree(const char *str);

    void preorderTraversalRecursion(TreeNode *parent);
    void inorderTraversalRecursion(TreeNode *parent);
    void postorderTraversalRecursion(TreeNode *parent);
    void levelorderTraversalIteration(TreeNode *parent);

    void preorderTraversalIteration(TreeNode *parent);
    void inorderTraversalIteration(TreeNode *parent);
    void postorderTraversalIteration(TreeNode *parent);
    void printGivenLevel(TreeNode *parent, int level);
    int height(TreeNode *parent);
    void levelorderTraversalRecursion(TreeNode *parent);

};

BinaryTree::BinaryTree(const char *str)
{
    stringstream ss;
    string temp;
    ss << str;

    getline(ss, temp, ',');
    root = new TreeNode(stoi(temp));

    LevelorderConstruct(ss);
}

void BinaryTree::LevelorderConstruct(stringstream &ss)
{
    queue<TreeNode*> q;
    TreeNode *current = root;
    string input;

    while(getline(ss, input, ',')){
        //handle the left child
        if(input.compare("x")){
            TreeNode *newNode = new TreeNode(stoi(input));
            current->left = newNode;
            q.push(newNode);
        }

        if(!(getline(ss, input, ','))){
            break;
        }
        
        if(input.compare("x")){
            TreeNode *newNode = new TreeNode(stoi(input));
            current->right = newNode;
            q.push(newNode);
        }

        current = q.front();
        q.pop();
    }
}

void BinaryTree::preorderTraversalRecursion(TreeNode *parent)
{
    if(NULL == parent){
        return;
    }

    //do the right things
    cout << " " << parent->val;
    preorderTraversalRecursion(parent->left);
    preorderTraversalRecursion(parent->right);
}

void BinaryTree::inorderTraversalRecursion(TreeNode *parent)
{
    if(NULL == parent){
        return;
    }

    inorderTraversalRecursion(parent->left);
    //do the right things
    cout << " " << parent->val;
    inorderTraversalRecursion(parent->right);
}

void BinaryTree::postorderTraversalRecursion(TreeNode *parent)
{
    if(NULL == parent){
        return;
    }

    postorderTraversalRecursion(parent->left);
    postorderTraversalRecursion(parent->right);
    //do the right things
    cout << " " << parent->val;
}

void BinaryTree::levelorderTraversalIteration(TreeNode *parent)
{
    queue<TreeNode*> q;
    q.push(parent);

    while(!q.empty()){
        TreeNode *current = q.front();
        q.pop();
        cout << " " << current->val;

        if(current->left)   q.push(current->left);
        if(current->right)  q.push(current->right);
    }//while
}

void BinaryTree::preorderTraversalIteration(TreeNode *parent)
{
    if(NULL == parent)
        return;

    stack<TreeNode*> st;
    st.push(parent);

    while(!st.empty()){
        TreeNode *current = st.top();
        st.pop();

        //do the right thing
        cout << " " << current->val;

        if(current->right)
            st.push(current->right);
        if(current->left)
            st.push(current->left);
    }
}

void BinaryTree::inorderTraversalIteration(TreeNode *parent)
{
    stack<TreeNode*> st;
    TreeNode *current = parent;

    while(current || !st.empty()){
        //step 1:reach the leftmost node
        while(current){
            st.push(current);
            current = current->left;
        }

        //step 2:pop the stack
        current = st.top();
        st.pop();

        //step 3:do the right thing
        cout << " " << current->val;

        //step 4:handle the right child if need
        current = current->right;
    }
}

void BinaryTree::postorderTraversalIteration(TreeNode *parent)
{
    if(NULL == parent)
        return;

    stack<TreeNode*> st;
    TreeNode *current = parent;

    do{
        //step 1:reach the leftmost node
        while(current){
            //push right child
            if(current->right)
                st.push(current->right);
            //push the current
            st.push(current);

            current = current->left;
        }

        //step 2:pop the stack
        current = st.top();
        st.pop();

        //step 3:handle the right child first
        if(current->right && !st.empty() && st.top() == current->right)
        {
            st.pop();
            st.push(current);
            current = current->right;
        }else{//step 4:do the right thing
            cout << " " << current->val;

            //for next pop
            current = NULL;
        }

    }while(!st.empty());
}

void BinaryTree::printGivenLevel(TreeNode *parent, int level)
{
    if(NULL == parent){
        return;
    }

    if(1 == level){
        cout << " " << parent->val;
    }else if(level > 1){
        printGivenLevel(parent->left, level-1);
        printGivenLevel(parent->right, level-1);
    }
}

int BinaryTree::height(TreeNode *parent)
{
    if(NULL == parent){
        return 0;
    }

    int left_height = height(parent->left);
    int right_height = height(parent->right);

    if(left_height > right_height)
        return (left_height+1);
    else
        return (right_height+1);
}

void BinaryTree::levelorderTraversalRecursion(TreeNode *parent)
{
    int h = height(parent);
    for(int i=1; i<=h; i++){
        printGivenLevel(parent, i);
    }
}

int main(int argc, char const *argv[]){
    const char *input = "1,2,3,x,x,4,5,6,7";
    BinaryTree BT(input);

    TreeNode *temp = BT.root->right;
    cout << "test : " << temp->right->val << endl;

    cout << "inorder :";
    BT.inorderTraversalRecursion(BT.root);
    cout << endl;

    cout << "preorder :";
    BT.preorderTraversalRecursion(BT.root);
    cout << endl;

    cout << "postorder :";
    BT.postorderTraversalRecursion(BT.root);
    cout << endl;

    cout << "levelorder :";
    BT.levelorderTraversalIteration(BT.root);
    cout << endl;

    cout << "inorder iteration :";
    BT.inorderTraversalIteration(BT.root);
    cout << endl;

    cout << "preorder iteration :";
    BT.preorderTraversalIteration(BT.root);
    cout << endl;

    cout << "postorder iteration :";
    BT.postorderTraversalIteration(BT.root);
    cout << endl;

    cout << "levelorder :";
    BT.levelorderTraversalRecursion(BT.root);
    cout << endl;

    return 0;
}
/*==============================================================*/