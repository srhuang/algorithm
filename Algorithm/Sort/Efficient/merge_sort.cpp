/*****************************************************************
Name    :merge_sort
Author  :srhuang
Email   :lukyandy3162@gmail.com
History :
    20191125 Initial Version
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

void merge(int *input, int front, int end, int mid)
{
    vector<int> left(input+front, input+mid+1);
    vector<int> right(input+mid+1, input+end+1);
    vector<int>::iterator it;

#if 0
    cout << "left :";
    for(it=left.begin(); it!=left.end(); it++){
        cout << *it << " ";
    }
    cout << endl;

    cout << "right :";
    for(it=right.begin(); it!=right.end(); it++){
        cout << *it << " ";
    }
    cout << endl;
#endif

    int left_index=0;
    int right_index=0;
    //cout << "front/end :" << front << "/" << end << endl;
    for(int i=front; i<=end; i++){
        if(left_index >= left.size()){ // left element is exhausted 
            input[i] = right[right_index++];
            //cout << "input right :" << input[i] << endl;
        }else if(right_index >= right.size()){ // right element is exhausted 
            input[i] = left[left_index++];
            //cout << "input left :" << input[i] << endl;
        }else if(left[left_index] <= right[right_index]){ //keep stable
            input[i] = left[left_index++];
            //cout << "input left :" << input[i] << endl;
        }else{
            input[i] = right[right_index++];
            //cout << "input right :" << input[i] << endl;
        }
    }//for
}

void merge_sort(int *input, int front, int end)
{
    //cout << "front :" << front << ", end :" << end << endl;
    if(front >= end)
        return;

    int mid = (front+end) / 2;
    //recursion
    merge_sort(input, front, mid);
    merge_sort(input, mid+1, end);
    merge(input, front, end, mid);
}

/*==============================================================*/
int main(int argc, char const *argv[]){
    int n=SCALE;

    //generate data
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
    merge_sort(random_data, 0, n-1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by random_data: "
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