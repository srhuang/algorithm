/*****************************************************************
Name    :quick_sort
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191124 Initial Version
*****************************************************************/
#include <iostream>
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

int *worst_case(int number)
{
    int *result = new int[number];

    //generate index ordered arrary
    for(int i=0; i<number; i++){
        result[i]=i+1;
    }

    return result;
}

int partition(int *input, int front, int end)
{
    //Using end as pivot
    int pivot = input[end];

    //partition by using pivot
    int left = front;
    int right = end-1;
    while(true){

        //find the value greater than pivot from left
        while((left<=end) && (input[left] < pivot)){
            left++;
        }

        //find the value less than pivot from right
        while((right>=front) && (input[right] >= pivot)){
            right--;
        }
        //cout << "left=" << left << ", right=" << right << endl;

        if(left >= right)
            break;

        //swap
        int temp = input[left];
        input[left] = input[right];
        input[right] = temp;
    }

    //left should be greater or equal than pivot
    return left;
}

void quick_sort(int *input, int front, int end)
{
    if(front >= end)
        return;

    //partition
    int pivot_index = partition(input, front, end);
    //cout << "pivot index :" << pivot_index << endl;

    //swap the pivot
    if(pivot_index!=end){
        int temp = input[pivot_index];
        input[pivot_index] = input[end];
        input[end] = temp;
    }

    //recursion
    quick_sort(input, front, pivot_index-1);
    quick_sort(input, pivot_index+1, end);
}

/*==============================================================*/
int main(int argc, char const *argv[]){
    int n=SCALE;

    //generate data
    int *worst_data = worst_case(n);
    int *random_data = random_case(n);

    #if DEBUG
    cout << "Before sorting :";
    for(int i=0; i<n; i++){
        cout << random_data[i] << " ";
    }
    cout << endl;
    #endif

    //sort
    auto start = high_resolution_clock::now(); 
    quick_sort(random_data, 0, n-1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by random_data: "
         << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now(); 
    quick_sort(worst_data, 0, n-1);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by worst_data: "
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