#include <iostream>
#include <iomanip>
#include <omp.h>
#include <cstdlib> // For rand()
#include <ctime>   // For time()

using namespace std;

// Sequential Bubble Sort
void sequentialBubbleSort(int a[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (a[j] > a[j+1]) {
                swap(a[j], a[j+1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(int a[], int n) {
    for (int i = 0; i < n; i++) {
        // Even phase
        if (i % 2 == 0) {
            #pragma omp parallel for
            for (int j = 0; j < n-1; j += 2) {
                if (a[j] > a[j+1]) {
                    swap(a[j], a[j+1]);
                }
            }
        }
        // Odd phase
        else {
            #pragma omp parallel for
            for (int j = 1; j < n-1; j += 2) {
                if (a[j] > a[j+1]) {
                    swap(a[j], a[j+1]);
                }
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter total number of elements: ";
    cin >> n;

    int* a = new int[n];
    int* b = new int[n];

    // Random array generation
    srand(time(0)); // Seed random number generator
    cout << "\nGenerating random array...\n";
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100000; // Random numbers between 0 and 99999
        b[i] = a[i];            // Copy for parallel sort
    }

    cout << "\nRandom array generated.\n";

    double start_time, end_time;

    // Sequential Bubble Sort
    start_time = omp_get_wtime();
    sequentialBubbleSort(a, n);
    end_time = omp_get_wtime();
    double seq_time = end_time - start_time;

    cout << "\nSequential Bubble Sort Time: " << fixed << setprecision(8) << seq_time << " seconds" << endl;

    // Parallel Bubble Sort
    start_time = omp_get_wtime();
    parallelBubbleSort(b, n);
    end_time = omp_get_wtime();
    double par_time = end_time - start_time;

    cout << "\nParallel Bubble Sort Time: " << fixed << setprecision(8) << par_time << " seconds" << endl;

    // Display sorted array (only if small size)
    if (n <= 100) {
        cout << "\nSorted array:\n";
        for (int i = 0; i < n; i++) {
            cout << b[i] << " ";
        }
        cout << endl;
    }

    delete[] a;
    delete[] b;

    return 0;
}
