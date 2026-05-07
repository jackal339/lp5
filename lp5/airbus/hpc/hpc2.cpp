#include <omp.h>
#include <iostream>
#include <chrono>
#include <cstdlib> // for rand()
using namespace std;
using namespace chrono;

// PRINT FUNCTION
void printArray(string msg, int arr[], int n)
{
    cout << msg << ": [ ";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << "]\n";
}

// SEQUENTIAL BUBBLE SORT
void seqBubble(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// PARALLEL BUBBLE SORT 
void parBubble(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int start = i % 2;

#pragma omp parallel for
        for (int j = start; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// MERGE FUNCTION
void merge(int arr[], int l, int m, int r)
{
    int n = r - l + 1;
    int *temp = new int[n];

    int i = l, j = m + 1, k = 0;

    while (i <= m && j <= r)
    {
        if (arr[i] < arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= m)
        temp[k++] = arr[i++];
    while (j <= r)
        temp[k++] = arr[j++];

    for (int i = 0; i < n; i++)
        arr[l + i] = temp[i];

    delete[] temp;
}

// SEQUENTIAL MERGE SORT
void seqMerge(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        seqMerge(arr, l, m);
        seqMerge(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// PARALLEL MERGE SORT 
void parMerge(int arr[], int l, int r)
{
    if (l < r)
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

// ---------------- MAIN FUNCTION ----------------
int main()
{
    int n = 80000;

    int *arr1 = new int[n];
    int *arr2 = new int[n];
    int *arr3 = new int[n];
    int *arr4 = new int[n];

    // Generate random numbers
    for (int i = 0; i < n; i++)
    {
        arr1[i] = rand() % 10000;
        arr2[i] = arr1[i];
        arr3[i] = arr1[i];
        arr4[i] = arr1[i];
    }

    // -------- BUBBLE SORT --------
    cout << "\n--- Bubble Sort ---\n";

    auto t1 = high_resolution_clock::now();
    seqBubble(arr1, n);
    auto t2 = high_resolution_clock::now();

    auto t3 = high_resolution_clock::now();
    parBubble(arr2, n);
    auto t4 = high_resolution_clock::now();

    cout << "Time Seq: "
         << duration_cast<microseconds>(t2 - t1).count() << " microseconds\n";

    cout << "Time Par: "
         << duration_cast<microseconds>(t4 - t3).count() << " microseconds\n";

    // -------- MERGE SORT --------
    cout << "\n--- Merge Sort ---\n";

    auto t5 = high_resolution_clock::now();
    seqMerge(arr3, 0, n - 1);
    auto t6 = high_resolution_clock::now();

    auto t7 = high_resolution_clock::now();
    parMerge(arr4, 0, n - 1);
    auto t8 = high_resolution_clock::now();

    cout << "Time Seq: "
         << duration_cast<microseconds>(t6 - t5).count() << " microseconds\n";

    cout << "Time Par: "
         << duration_cast<microseconds>(t8 - t7).count() << " microseconds\n";

    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
    delete[] arr4;

    return 0;
}

