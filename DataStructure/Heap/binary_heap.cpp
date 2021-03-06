/*****************************************************************
Name    :binary_heap
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191226 decrease-key, delete, find.
    20191226 reconstruct
    20191225 decrease-key and delete
    20191225 return the node when insert
    20191218 add merge operation
    20191218 change to min heap
    20191129 Initial Version
*****************************************************************/
#include <iostream>
#include <vector>
#define DEBUG (1)
#define SCALE (10)
using namespace std;
using namespace std::chrono;

/*==============================================================*/
//Global area
class BinaryHeap{
    //core operation
    void Heapify(int root_index);
public:
    vector<int> data;

    //five operations
    BinaryHeap(int *arr, int size);
    int insert(int input);
    int extract_min();
    int minimum();
    void merge(BinaryHeap &bh);

    //decrease-key and delete
    int decrease_key(int index, int new_val);
    void delete_key(int index);

    //find
    int find(int key);

    //dump elements
    void dump(void);
};

//core operation
void BinaryHeap::Heapify(int parent_index)
{
    int left_index = 2*parent_index + 1;
    int right_index = 2*parent_index + 2;
    int heapify_index;
    int size = data.size();

    //check left child
    if((left_index < size) &&
        data[left_index] < data[parent_index])
    {
        heapify_index = left_index;
    }else{
        heapify_index = parent_index;
    }

    //check right child
    if((right_index < size) &&
        data[right_index] < data[heapify_index])
    {
        heapify_index = right_index;
    }

    //check if need heapify
    if(heapify_index != parent_index){
        //swap
        int temp = data[parent_index];
        data[parent_index] = data[heapify_index];
        data[heapify_index] = temp;

        //keep heapify
        Heapify(heapify_index);
    }

}

//initialize
BinaryHeap::BinaryHeap(int *arr, int arr_size)
{
    data = vector<int>(arr, arr+arr_size);
    int size = data.size();
    for(int i=(size-2)/2; i>=0; i--){
        Heapify(i);
    }
}

//insert
int BinaryHeap::insert(int input)
{
    data.push_back(input);
    int size = data.size();
    int current_index = size-1;
    int parent_index = (current_index-1)/2;
    while(current_index > 0 && (data[parent_index] > data[current_index]))
    {
        //swap
        int temp = data[current_index];
        data[current_index] = data[parent_index];
        data[parent_index] = temp;

        //bottom-up
        current_index = (current_index-1)/2;
        parent_index = (current_index-1)/2;
    }

    return current_index;
}

//extract min
int BinaryHeap::extract_min()
{
    if(data.empty())
        return -1;

    int result = data[0];

    data[0] = data.back();
    data.pop_back();

    //heapify from the root
    Heapify(0);

    return result;
}

//minimum
int BinaryHeap::minimum()
{
    if(data.empty())
        return -1;

    return data[0];
}

//merge
void BinaryHeap::merge(BinaryHeap &bh)
{
    data.insert(data.end(), bh.data.begin(), bh.data.end());
    int size = data.size();
    for(int i=(size-2)/2; i>=0; i--){
        Heapify(i);
    }
}

//decrease key
int BinaryHeap::decrease_key(int index, int new_val)
{
    if(index >= data.size())
        return -1;
    if(data[index] <= new_val)
        return index;

    data[index] = new_val;
    int current_index = index;
    int parent_index = (current_index-1)/2;

    while(current_index > 0 && (data[parent_index] > data[current_index]))
    {
        //swap
        int temp = data[current_index];
        data[current_index] = data[parent_index];
        data[parent_index] = temp;

        //bottom-up
        current_index = (current_index-1)/2;
        parent_index = (current_index-1)/2;
    }

    return current_index;
}

//delete
void BinaryHeap::delete_key(int index)
{
    if(index >= data.size())
        return;

    data[index] = data.back();
    data.pop_back();

    int current_index = index;
    int parent_index = (current_index-1)/2;
    if(data[parent_index] > data[current_index]){
        while(current_index > 0 && (data[parent_index] > data[current_index]))
        {
            //swap
            int temp = data[current_index];
            data[current_index] = data[parent_index];
            data[parent_index] = temp;

            //bottom-up
            current_index = (current_index-1)/2;
            parent_index = (current_index-1)/2;
        }
    }else{
        //heapify from the root
        Heapify(current_index);
    }
}

int BinaryHeap::find(int key)
{
    int size = data.size();
    for(int i=0; i<size; i++){
        if(key == data[i]){
            return i;
        }
    }
    
    return -1;
}

//dump
void BinaryHeap::dump(void)
{
    int size = data.size();
    cout << "Dump the heap : ";
    for(int i=0; i<size; i++){
        cout << data[i] << " ";
    }
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
    BinaryHeap myHeap(random_data, n);
    myHeap.dump();

    // Insert the new element
    cout << "\n\tInsert the new element" << endl;
    int index = myHeap.insert(7);
    cout << "insert :" << myHeap.data[index] << endl;
    myHeap.dump();

    // Test minimum and extract min
    cout << "\n\tTest minimum and extract min" << endl;
    cout << "minimum :" << myHeap.minimum() << endl;
    cout << "extract_min :" << myHeap.extract_min() << endl;
    myHeap.dump();
    cout << "minimum :" << myHeap.minimum() << endl;

    // Test decrease-key and delete
    cout << "\n\tTest decrease-key and delete" << endl;
    int index_temp = myHeap.insert(8);
    cout << "decrease-key : " << myHeap.data[index_temp] << endl;
    myHeap.decrease_key(index_temp, 1);
    myHeap.dump();
    int delete_index = 4;
    cout << "delete : " << delete_index << endl;
    myHeap.delete_key(delete_index);
    myHeap.dump();

    // Find the value
    cout << "\n\tFind the value" << endl;
    int find_value = 6;
    int find_index = myHeap.find(find_value);
    cout << "Find index :" << find_index << endl;

    // Merge the two heap
    cout << "\n\tMerge the two heap" << endl;
    //generate another random data
    int *random_data2 = random_case(6, n);
#if DEBUG
    cout << "Generate Data2 :";
    for(int i=0; i<n; i++){
        cout << random_data2[i] << " ";
    }
    cout << endl;
#endif

    BinaryHeap myHeap2(random_data2, n);
    //merge
    myHeap.merge(myHeap2);
    myHeap.dump();

    // Heap sort
    cout << "\n\tHeap sort" << endl;
    cout << "Heap sort :";
    size = myHeap.data.size();
    for(int i=0; i<size; i++){
        cout << myHeap.extract_min() << " ";
    }
    cout << endl;

    return 0;
}
/*==============================================================*/