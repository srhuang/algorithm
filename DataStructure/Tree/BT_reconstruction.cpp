/*****************************************************************
Name    :BT_reconstruction
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20190927 Initial Version
*****************************************************************/
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
using namespace std;
/*==============================================================*/

struct TreeNode{
    char data;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : data(0), left(NULL), right(NULL) {}
    TreeNode(char x) : data(x), left(NULL), right(NULL) {}
};

class BinaryTree{
private:
    /*
        Search the target val int the array, and return index.
        return -1 if there is no element match the target.
    */
    int search(const char *arr, int start, int end, char val);

    /*
        This function extracts keys from level[] which are present in in[].
        The order of extracted keys must be maintained.
     */
    char *extractKeys(const char *in, int m, const char *level, int n);
public:
    TreeNode *root;

    //build the tree from inorder and preorder
    TreeNode *buildTreePre(
        const char *in, const char *pre, int inStart, int inEnd, int *preIndex);

    //build the tree from inorder and postorder
    TreeNode *buildTreePost(
        const char *in, const char *post, int inStart, int inEnd, int *postIndex);

    //build the tree from inorder and level order
    TreeNode *buildTreeLevel(
        const char *in, const char *level, int inStart, int inEnd, int leveln);
    //verify the tree
    void postorderTraversalRecursion(TreeNode *parent);
    void inorderTraversalRecursion(TreeNode *parent);
};

TreeNode *BinaryTree::buildTreePre(
    const char *in, const char *pre, int inStart, int inEnd, int *preIndex)
{
    if(inStart > inEnd){
        return NULL;
    }

    //pick node from preorder
    //cout << "preorder : " << pre[preIndex] << endl;
    TreeNode *newNode = new TreeNode(pre[*preIndex]);
    (*preIndex)++;

    if(inStart == inEnd)
        return newNode;

    int inIndex = search(in, inStart, inEnd, newNode->data);

    newNode->left = buildTreePre(in, pre, inStart, inIndex-1, preIndex);
    newNode->right = buildTreePre(in, pre, inIndex+1, inEnd, preIndex);

    return newNode;
}

TreeNode *BinaryTree::buildTreePost(
    const char *in, const char *post, int inStart, int inEnd, int *postIndex)
{
    if(inStart > inEnd){
        return NULL;
    }

    //pick node from preorder
    //cout << "preorder : " << pre[preIndex] << endl;
    TreeNode *newNode = new TreeNode(post[*postIndex]);
    (*postIndex)--;

    if(inStart == inEnd)
        return newNode;

    int inIndex = search(in, inStart, inEnd, newNode->data);

    newNode->right = buildTreePost(in, post, inIndex+1, inEnd, postIndex);
    newNode->left = buildTreePost(in, post, inStart, inIndex-1, postIndex);

    return newNode;
}

TreeNode *BinaryTree::buildTreeLevel(
        const char *in, const char *level, int inStart, int inEnd, int leveln)
{
    if(inStart > inEnd){
        return NULL;
    }

    TreeNode *newNode = new TreeNode(level[0]);

    if(inStart == inEnd)
        return newNode;

    //find the index of target int inorder array
    int inIndex = search(in, inStart, inEnd, newNode->data);

    //extract the level array
    vector<char> left_level;
    vector<char> right_level;
    int pos;
    for(int i=1; i<leveln; i++){
        if((pos=search(in, inStart, inEnd, level[i])) != -1){
            if(pos < inIndex){
                left_level.push_back(level[i]);
            }else{
                right_level.push_back(level[i]);
            }
        }
    }

    newNode->left = buildTreeLevel(in, &left_level[0], inStart, inIndex-1, left_level.size());
    newNode->right = buildTreeLevel(in, &right_level[0], inIndex+1, inEnd, right_level.size());

    return newNode;
}

//find the index of value
int BinaryTree::search(const char *arr, int start, int end, char val)
{
    for(int i=start; i<=end; i++){
        if(arr[i] == val)
            return i;
    }

    return -1;
}

void BinaryTree::postorderTraversalRecursion(TreeNode *parent)
{
    if(NULL == parent){
        return;
    }

    postorderTraversalRecursion(parent->left);
    postorderTraversalRecursion(parent->right);
    //do the right things
    cout << " " << parent->data;
}

void BinaryTree::inorderTraversalRecursion(TreeNode *parent)
{
    if(NULL == parent){
        return;
    }

    inorderTraversalRecursion(parent->left);
    //do the right things
    cout << " " << parent->data;
    inorderTraversalRecursion(parent->right);
}

int main(int argc, char const *argv[]){
    const char inorder[] = { 'D', 'B', 'E', 'A', 'F', 'C' };
    const char preorder[] = { 'A', 'B', 'D', 'E', 'C', 'F' };
    const char postorder[] = { 'D', 'E', 'B', 'F', 'C', 'A' };
    const char levelorder[] = { 'A', 'B', 'C', 'D', 'E', 'F'};
    int size_inorder = sizeof(inorder)/sizeof(char);
    int size_level = sizeof(levelorder)/sizeof(char);
    int preIndex = 0;
    int postIndex = size_inorder-1;
    //cout << "size : " << size << endl;
    BinaryTree BT;
    BT.root = BT.buildTreePre(inorder, preorder, 0, size_inorder-1, &preIndex);
    //BT.root = BT.buildTreePost(inorder, postorder, 0, size_inorder-1, &postIndex);
    //BT.root = BT.buildTreeLevel(inorder, levelorder, 0, size_inorder-1, size_level);

    cout << "check in postorder :";
    BT.postorderTraversalRecursion(BT.root);
    cout << endl;

    cout << "check in inorder :";
    BT.inorderTraversalRecursion(BT.root);
    cout << endl;

    return 0;
}
/*==============================================================*/