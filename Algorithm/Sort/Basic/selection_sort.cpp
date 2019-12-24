/*****************************************************************
Name    :selection_sort
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191123 Initial Version
*****************************************************************/
#include <iostream>
#define DEBUG (0)
#define SCALE (100)
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

int *ascending_case(int number)
{
    int *result = new int[number];

    //generate index ordered arrary
    for(int i=0; i<number; i++){
        result[i]=i+1;
    }

    return result;
}

void selection_sort(int *input, int n)
{
    // One by one move boundary of unsorted subarray
    for(int i=0; i<n; i++){

        // Find the minimum element in unsorted array
        int min_index = i;
        for(int j=i+1; j<n; j++){
            if(input[j] < input[min_index]){
                min_index = j;
            }
        }

        // Swap the found minimum element
        int t = input[i];
        input[i] = input[min_index];
        input[min_index] = t;
    }
}


/*==============================================================*/
int main(int argc, char const *argv[]){
    int n=SCALE;

    //generate data
    int *ascending_data = ascending_case(n);
    int *descending_data = descending_case(n);
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
    selection_sort(ascending_data, n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << "\nTime taken by ascending_data: "
         << duration.count() << " microseconds" << endl; 

    start = high_resolution_clock::now(); 
    selection_sort(descending_data, n);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by descending_data: "
         << duration.count() << " microseconds" << endl; 

    start = high_resolution_clock::now(); 
    selection_sort(random_data, n);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by random data: "
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