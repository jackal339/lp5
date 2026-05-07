#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void display(int nums[], int len){
    cout<<  "Array: [";
    for(int i=0; i<len; i++){
        cout << nums[i];
        if(i<len-1) cout<< ",";
    }
    cout << "]" << endl;
}

// finding smallest element seq and parallel
void min_seq (int nums[], int len){
    int minimum = nums[0];
    for(int i=0; i<len; i++){
        if(nums[i]<minimum) minimum = nums[i];
    }
    cout<< "Minimum Value: " << minimum << endl;
}

void min_parallel (int nums[], int len){
    int minimum = nums[0];
    #pragma omp parallel for reduction (min : minimum)
    for(int i=0; i<len; i++){
        if(nums[i]<minimum) minimum = nums[i];
    }
    cout<< "Minimum Value: " << minimum << endl;
}

//  finding largest element seq and parallel
void max_seq (int nums[], int len){
    int maximum = nums[0];
    for(int i=0; i<len; i++){
        if(nums[i]>maximum) maximum = nums[i];
    }
    cout<< "Maximum Value: " << maximum << endl;
}

void max_parallel (int nums[], int len){
    int maximum = nums[0];
    #pragma omp parallel for reduction (max : maximum)
    for(int i=0; i<len; i++){
        if(nums[i]>maximum) maximum = nums[i];
    }
    cout<< "Maximum Value: " << maximum << endl;
}

//  finding sum of the elements sequential and parallel
void sum_seq (int nums[], int len){
    int sum = 0;
    for(int i=0; i<len; i++){
        sum += nums[i];
    }
    cout<< "Sum: " << sum << endl;
}

void sum_parallel (int nums[], int len){
    int sum = 0;
    #pragma omp parallel for reduction (+ : sum)
    for(int i=0; i<len; i++){
        sum += nums[i];
    }
    cout<< "Sum: " << sum << endl;
}

// finding average of elements sequential and parallel
void avg_seq (int nums[], int len){
    int sum = 0;
    for(int i=0; i<len; i++){
        sum += nums[i];
    }
    cout<< "Avg: " << sum/len << endl;
}

void avg_parallel (int nums[], int len){
    int sum = 0;
    #pragma omp parallel for reduction (+ : sum)
    for(int i=0; i<len; i++){
        sum += nums[i];
    }
    cout<< "Avg: " << sum/len << endl;
}

int main(){
    int len; 
    cout<<"enter length of the array: "<<endl;
    cin>> len;

    int nums[len];
    for(int i=0; i<len; i++){
        nums[i]=i;
    }

    // display(nums, len);

    cout<<"Sequential Execution: "<<endl;

    auto st_seq = high_resolution_clock::now();
    min_seq(nums,len);
    max_seq(nums,len);
    sum_seq(nums,len);
    avg_seq(nums,len);
    auto end_seq = high_resolution_clock::now();

    auto duration_seq = duration_cast <microseconds> (end_seq-st_seq).count();

    cout<< "time reqd: " << duration_seq << "microseconds"<<endl;


    cout<<"Parallel Execution: "<<endl;

    auto st_para = high_resolution_clock::now();
    min_parallel(nums,len);
    max_parallel(nums,len);
    sum_parallel(nums,len);
    avg_parallel(nums,len);
    auto end_para = high_resolution_clock::now();

    auto duration_para = duration_cast <microseconds> (end_para-st_para).count();

    cout << "time reqd: " << duration_para << " microseconds" << endl;
}