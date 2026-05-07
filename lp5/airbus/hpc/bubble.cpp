#include<iostream>
#include<omp.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

void bubble(int array[], int n){
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            if (array[j] > array[j + 1]) swap(array[j], array[j + 1]);
        }
    }
}

void pBubble(int array[], int n){
    //Sort odd indexed numbers
    for(int i = 0; i < n; ++i){    
        #pragma omp for
        for (int j = 1; j < n; j += 2){
        if (array[j] < array[j-1])
        {
          swap(array[j], array[j - 1]);
        }
    }

    // Synchronize
    #pragma omp barrier

    //Sort even indexed numbers
    #pragma omp for
    for (int j = 2; j < n; j += 2){
      if (array[j] < array[j-1])
      {
        swap(array[j], array[j - 1]);
      }
    }
  }
}

void printArray(int arr[], int n){
    for(int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";
}

int main(){
    // Set up variables
    int n = 10;
    int arr[n];
    int brr[n];


    // Create an array with numbers starting from n to 1
    for(int i = 0, j = n; i < n; i++, j--) arr[i] = j;
    
    // Sequential time
    auto start_time = high_resolution_clock::now();
    bubble(arr, n);
    auto end_time = high_resolution_clock::now();     
    cout << "Sequential Bubble Sort took : " <<duration_cast<microseconds> (end_time - start_time).count() << " microseconds.\n";
    printArray(arr, n);
    
    // Reset the array
    for(int i = 0, j = n; i < n; i++, j--) brr[i] = j;
    
    // Parallel time
    auto start_time1 = high_resolution_clock::now();
    pBubble(brr, n);
    auto end_time1 = high_resolution_clock::now();     
    cout << "Parallel Bubble Sort took : " <<duration_cast<microseconds> (end_time1 - start_time1).count() << " microseconds.\n";
    printArray(brr, n);
}