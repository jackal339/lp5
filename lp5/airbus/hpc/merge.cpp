#include <omp.h>
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace chrono;

// Print Array Function
void printArray(int arr[], int n)
{
    for(int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << "\n";
}

// Merge Function
void merge(int arr[], int l, int m, int r)
{
    int n = r - l + 1;

    int *temp = new int[n];

    int i = l;
    int j = m + 1;
    int k = 0;

    // Merge two sorted halves
    while(i <= m && j <= r)
    {
        if(arr[i] < arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }

    // Copy remaining left elements
    while(i <= m)
    {
        temp[k++] = arr[i++];
    }

    // Copy remaining right elements
    while(j <= r)
    {
        temp[k++] = arr[j++];
    }

    // Copy temp back to original array
    for(int i = 0; i < n; i++)
    {
        arr[l + i] = temp[i];
    }

    delete[] temp;
}

// Sequential Merge Sort
void seqMerge(int arr[], int l, int r)
{
    if(l < r)
    {
        int m = (l + r) / 2;

        seqMerge(arr, l, m);
        seqMerge(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void parMerge(int arr[], int l, int r)
{
    if(l < r)
    {
        int m = (l + r) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parMerge(arr, l, m);

            #pragma omp section
            parMerge(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

int main()
{
    int n = 10;

    int arr[n];
    int brr[n];

    // Initialize arrays
    for(int i = 0, j = n; i < n; i++, j--)
    {
        arr[i] = j;
        brr[i] = j;
    }

    // Sequential Merge Sort
    auto start_time = high_resolution_clock::now();

    seqMerge(arr, 0, n - 1);

    auto end_time = high_resolution_clock::now();

    cout << "Sequential Merge Sort Array:\n";
    printArray(arr, n);

    cout << "Time taken by Sequential Algorithm: "
         << duration_cast<microseconds>(end_time - start_time).count()
         << " microseconds\n\n";

    // Parallel Merge Sort
    auto start_time1 = high_resolution_clock::now();

    parMerge(brr, 0, n - 1);

    auto end_time1 = high_resolution_clock::now();

    cout << "Parallel Merge Sort Array:\n";
    printArray(brr, n);

    cout << "Time taken by Parallel Algorithm: "
         << duration_cast<microseconds>(end_time1 - start_time1).count()
         << " microseconds\n";

    return 0;
}