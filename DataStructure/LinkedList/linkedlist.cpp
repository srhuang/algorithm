/*****************************************************************
Name    :linkedlist
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191009 Initial Version
*****************************************************************/
#include <iostream>
using namespace std;

/*==============================================================*/
//Global area
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class LinkedList{
public:
    ListNode *head;
    LinkedList(const int *input, int size);
    //data operations
    void insert_front(int x);
    void insert_back(int x);
    void delete_front();
    void delete_back();
    void delete_key(int x);
    ListNode *peek_back();

    //list operations
    bool search(int x);
    void display();
    void reverse();

};

LinkedList::LinkedList(const int *input, int size)
{
    head = NULL;
    for(int i=0; i<size; i++){
        insert_back(input[i]);
    }
}

void LinkedList::insert_front(int x)
{
    //new node
    ListNode *newNode = new ListNode(x);

    //the first node
    if(NULL == head){
        head = newNode;
        return;
    }

    //insert front
    newNode->next = head;
    head = newNode;
}

void LinkedList::insert_back(int x)
{
    //new node
    ListNode *newNode = new ListNode(x);

    //the first node
    if(NULL == head){
        head = newNode;
        return;
    }

    //insert back
    ListNode *current = head;
    while(current->next != NULL){
        current = current->next;
    }

    current->next = newNode;
}

void LinkedList::delete_front()
{
    if(NULL == head){
        return;
    }

    ListNode *temp = head;
    head = temp->next;
    delete temp;
}

void LinkedList::delete_back()
{
    if(NULL == head){
        return;
    }

    ListNode *current = head;
    if(NULL == current->next){
        head = NULL;
        delete current;
    }

    ListNode *pre = current;
    current = current->next;
    while(current->next != NULL){
        pre = current;
        current = current->next;
    }

    pre->next = NULL;
    delete current;
}

void LinkedList::delete_key(int x)
{
    if(NULL == head){
        return;
    }

    ListNode *pre = NULL;
    ListNode *current = head;
    while((current != NULL) && (current->val != x)){
        pre = current;
        current = current->next;
    }

    //not found
    if(current == NULL){
        return;
    }

    //delete the head
    if(current == head){
        ListNode *temp = head;
        head = temp->next;
        delete temp;
    }else{
        pre->next = current->next;
        delete current;
        current = NULL;
    }
}

ListNode *LinkedList::peek_back()
{
    if(NULL == head)
        return NULL;

    ListNode *current = head;
    while(current->next != NULL){
        current = current->next;
    }

    return current;
}

bool LinkedList::search(int x)
{
    if(NULL == head){
        return false;
    }

    ListNode *current = head;
    while(current != NULL){
        if(current->val == x)
            return true;
        current = current->next;
    }

    return false;
}

void LinkedList::display()
{
    if(NULL == head){
        return;
    }

    ListNode *current = head;
    while(current != NULL){
        cout << " " << current->val;
        current = current->next;
    }
    cout <<endl;
}

void LinkedList::reverse()
{
    if(NULL == head || NULL == head->next){
        return;
    }

    ListNode *pre = NULL;
    ListNode *current = head;
    ListNode *preceding = current->next;

    while(preceding != NULL){
        current->next = pre;

        pre = current;
        current = preceding;
        preceding = current->next;
    }
    current->next = pre;
    head = current;
}

/*==============================================================*/
//Function area

/*==============================================================*/
int main(int argc, char const *argv[]){
    const int test[] = {1,1,2,3,5,8,13,21,34};
    LinkedList ll(test, sizeof(test)/sizeof(test[0]));

    cout << "Display out all element:";
    ll.display();

    bool answer = ll.search(20);
    cout << "search : " << answer <<endl;

    ll.delete_front();

    cout << "Display out all element:";
    ll.display();

    ll.delete_key(5);

    cout << "Display out all element:";
    ll.display();

    ListNode *peek = ll.peek_back();
    cout << peek->val <<endl;

    cout << "Display out all element:";
    ll.display();

    ll.reverse();

    cout << "Display out all element:";
    ll.display();

    return 0;
}
/*==============================================================*/