#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>

using namespace std;

// ---------------- SEQUENTIAL BUBBLE SORT ----------------
void bubbleSortSeq(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ---------------- PARALLEL BUBBLE SORT ----------------
void bubbleSortPar(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        } 
        else {
            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

// ---------------- MERGE FUNCTION ----------------
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;
    int i = l, j = m + 1;

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) temp.push_back(arr[i++]);
        else temp.push_back(arr[j++]);
    }

    while (i <= m) temp.push_back(arr[i++]);
    while (j <= r) temp.push_back(arr[j++]);

    for (int k = 0; k < temp.size(); k++) {
        arr[l + k] = temp[k];
    }
}

// ---------------- SEQUENTIAL MERGE SORT ----------------
void mergeSortSeq(vector<int>& arr, int l, int r) {
    if (l >= r) return;

    int m = (l + r) / 2;
    mergeSortSeq(arr, l, m);
    mergeSortSeq(arr, m + 1, r);
    merge(arr, l, m, r);
}

// ---------------- PARALLEL MERGE SORT ----------------
void mergeSortPar(vector<int>& arr, int l, int r) {
    if (l >= r) return;

    int m = (l + r) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        mergeSortPar(arr, l, m);

        #pragma omp section
        mergeSortPar(arr, m + 1, r);
    }

    merge(arr, l, m, r);
}

// ---------------- PRINT FUNCTION ----------------
void printArray(vector<int>& arr, int limit = 20) {
    for (int i = 0; i < min((int)arr.size(), limit); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// ---------------- MAIN ----------------
int main() {
    int n = 10000;
    vector<int> arr(n);

    // Generate random data
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;
    vector<int> arr4 = arr;

    cout << "Original Array: ";
    printArray(arr);

    double start, end;

    // Sequential Bubble Sort
    start = omp_get_wtime();
    bubbleSortSeq(arr1);
    end = omp_get_wtime();
    cout << "Sequential Bubble Sorted: ";
    printArray(arr1);
    cout << "Sequential Bubble Sort Time: " << (end - start) << endl;

    // Parallel Bubble Sort
    start = omp_get_wtime();
    bubbleSortPar(arr2);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sorted: ";
    printArray(arr2);
    cout << "Parallel Bubble Sort Time: " << (end - start) << endl;

    // Sequential Merge Sort
    start = omp_get_wtime();
    mergeSortSeq(arr3, 0, n - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sorted: ";
    printArray(arr3);
    cout << "Sequential Merge Sort Time: " << (end - start) << endl;

    // Parallel Merge Sort
    start = omp_get_wtime();
    mergeSortPar(arr4, 0, n - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sorted: ";
    printArray(arr4);
    cout << "Parallel Merge Sort Time: " << (end - start) << endl;

    return 0;
}