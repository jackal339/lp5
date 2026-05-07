#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>   // rand()
#include <ctime>     // time()

using namespace std;

// -------- SEQUENTIAL --------
void sequential(vector<int>& arr) {
    int n = arr.size();

    int sum = 0;
    int min_val = arr[0];
    int max_val = arr[0];

    double start = omp_get_wtime();

    for (int i = 0; i < n; i++) {
        sum += arr[i];

        if (arr[i] < min_val)
            min_val = arr[i];

        if (arr[i] > max_val)
            max_val = arr[i];
    }

    double avg = (double)sum / n;

    double end = omp_get_wtime();

    cout << "\n--- Sequential Results ---\n";
    cout << "Sum: " << sum << endl;
    cout << "Min: " << min_val << endl;
    cout << "Max: " << max_val << endl;
    cout << "Average: " << avg << endl;
    cout << "Time: " << end - start << " sec\n";
}

// -------- PARALLEL --------
void parallel(vector<int>& arr) {
    int n = arr.size();

    int sum = 0;
    int min_val = arr[0];
    int max_val = arr[0];

    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum) reduction(min:min_val) reduction(max:max_val)
    for (int i = 0; i < n; i++) {
        sum += arr[i];

        if (arr[i] < min_val)
            min_val = arr[i];

        if (arr[i] > max_val)
            max_val = arr[i];
    }

    double avg = (double)sum / n;

    double end = omp_get_wtime();

    cout << "\n--- Parallel Results ---\n";
    cout << "Sum: " << sum << endl;
    cout << "Min: " << min_val << endl;
    cout << "Max: " << max_val << endl;
    cout << "Average: " << avg << endl;
    cout << "Time: " << end - start << " sec\n";
}

// -------- MAIN --------
int main() {
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    // Set number of threads (optional but recommended)
    omp_set_num_threads(4);

    // Seed random generator
    srand(time(0));

    // Generate random data
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;   // values 0–999
    }

    cout << "\nRandom data generated successfully!\n";

    sequential(arr);
    parallel(arr);

    return 0;
}