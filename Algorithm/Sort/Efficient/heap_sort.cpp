/*****************************************************************
Name    :heap_sort
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191128 Initial Version
*****************************************************************/
#include <iostream>
#include <vector>
#define DEBUG (0)
#define SCALE (10000)
using namespace std;
using namespace std::chrono;

/*==============================================================*/
//Global area

/*==============================================================*/
//Function area
int *random_case(int number)
{
    int *result = new int[number];

    //generate index ordered arrary
    for(int i=0; i<number; i++){
        result[i]=i+1;
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

int *descending_case(int number)
{
    int *result = new int[number];

    //generate index ordered arrary
    for(int i=0; i<number; i++){
        result[i]=number-i;
    }

    return result;
}

//from top to bottom
void MaxHeapify(int *input, int max_index, int root_index)
{
    int left_index = root_index * 2 + 1;
    int right_index = root_index * 2 + 2;
    int largest_index;

    //check left and root
    if((left_index <= max_index) && 
        (input[left_index] > input[root_index])){
        largest_index = left_index;
    }else{
        largest_index = root_index;
    }

    //check the right and largest
    if((right_index <= max_index) && 
        (input[right_index] > input[largest_index])){
        largest_index = right_index;
    }

    //check if the child need heapify
    if(root_index != largest_index){
        //swap
        int temp = input[root_index];
        input[root_index] = input[largest_index];
        input[largest_index] = temp;

        //keep heapify
        MaxHeapify(input, max_index, largest_index);
    }
}

//from bottom to top
void BuildMaxHeap(int *input, int max_index)
{
    for(int i=(max_index-1)/2; i>=0; i--){
        MaxHeapify(input, max_index, i);
    }
}

void heap_sort(int *input, int max_index)
{
    BuildMaxHeap(input, max_index);

    for(int i=max_index; i>0; i--){
        //swap
        int temp = input[0];
        input[0] = input[i];
        input[i] = temp;

        MaxHeapify(input, i-1, 0);
    }
}

/*==============================================================*/
int main(int argc, char const *argv[]){
    int n=SCALE;

    //generate data
    int *random_data = random_case(n);
    int *descending_data = descending_case(n);

#if DEBUG
    cout << "Before sorting :";
    for(int i=0; i<n; i++){
        cout << random_data[i] << " ";
    }
    cout << endl;
#endif

    //sort
    auto start = high_resolution_clock::now(); 
    heap_sort(random_data, n-1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by random_data: "
         << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now(); 
    heap_sort(descending_data, n-1);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by descending_data: "
         << duration.count() << " microseconds" << endl; 

#if DEBUG
    cout << "\nAfter sorting :";
    for(int i=0; i<n; i++){
        cout << random_data[i] << " ";
    }
    cout << endl;
#endif

    return 0;
}
/*==============================================================*/