/*****************************************************************
Name    :bubble_sort
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191123 Initial Version
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
        result[i]=number-i;
    }

    return result;
}

int *best_case(int number)
{
    int *result = new int[number];

    //generate index ordered arrary
    for(int i=0; i<number; i++){
        result[i]=i+1;
    }

    return result;
}

void bubble_sort(int *input, int n)
{
    cout << endl;
    int count = 0;
    bool exchanged = true;
    for(int i=0; exchanged && i<n; i++){
        //each run reset the flag
        exchanged = false;
        for(int j=0; j<n-i-1; j++){
            if(input[j] > input[j+1]){
                //swap
                count++;
                int temp = input[j];
                input[j] = input[j+1];
                input[j+1] = temp;

                //set flag
                exchanged = true;
            }
        }//for each run
    }//for i
    cout << "swap count :" << count << endl;
}

/*==============================================================*/
int main(int argc, char const *argv[]){
    int n=SCALE;

    //generate data
    int *best_data = best_case(n);
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
    bubble_sort(random_data, n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by random_data: "
         << duration.count() << " microseconds" << endl; 

    start = high_resolution_clock::now(); 
    bubble_sort(best_data, n);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by best_data: "
         << duration.count() << " microseconds" << endl; 

    start = high_resolution_clock::now(); 
    bubble_sort(worst_data, n);
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