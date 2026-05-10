#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

#define SIZE 10000

void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {

        for (int j = 0; j < n - i - 1; j++) {

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void bubbleSortParallel(vector<int>& arr) {

    int n = arr.size();

    for (int i = 0; i < n; i++) {

        if (i % 2 == 0) {

            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2) {

                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }

        } else {

            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2) {

                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

void merge(vector<int>& arr, int low, int mid, int high) {

    vector<int> temp(high - low + 1);

    int i = low;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= high) {

        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= high) {
        temp[k++] = arr[j++];
    }

    for (int p = 0; p < k; p++) {
        arr[low + p] = temp[p];
    }
}

void mergeSortSequential(vector<int>& arr, int low, int high) {

    if (low < high) {

        int mid = (low + high) / 2;

        mergeSortSequential(arr, low, mid);
        mergeSortSequential(arr, mid + 1, high);

        merge(arr, low, mid, high);
    }
}

void mergeSortParallel(vector<int>& arr, int low, int high, int depth) {

    if (low < high) {

        int mid = (low + high) / 2;

        if (depth <= 0) {

            mergeSortSequential(arr, low, mid);
            mergeSortSequential(arr, mid + 1, high);

        } else {

            #pragma omp parallel sections
            {

                #pragma omp section
                mergeSortParallel(arr, low, mid, depth - 1);

                #pragma omp section
                mergeSortParallel(arr, mid + 1, high, depth - 1);
            }
        }

        merge(arr, low, mid, high);
    }
}

void generateRandomNumbers(vector<int>& arr) {

    for (int i = 0; i < arr.size(); i++) {
        arr[i] = rand() % 100000;
    }
}

int main() {

    vector<int> arr(SIZE), temp;

    srand(time(0));

    generateRandomNumbers(arr);

    double start, end;

    temp = arr;

    start = omp_get_wtime();

    bubbleSortSequential(temp);

    end = omp_get_wtime();

    cout << "Sequential Bubble Sort Time: "
         << end - start << " sec" << endl;

    temp = arr;

    start = omp_get_wtime();

    bubbleSortParallel(temp);

    end = omp_get_wtime();

    cout << "Parallel Bubble Sort Time: "
         << end - start << " sec" << endl;

    temp = arr;

    start = omp_get_wtime();

    mergeSortSequential(temp, 0, SIZE - 1);

    end = omp_get_wtime();

    cout << "Sequential Merge Sort Time: "
         << end - start << " sec" << endl;

    temp = arr;

    start = omp_get_wtime();

    mergeSortParallel(temp, 0, SIZE - 1, 4);

    end = omp_get_wtime();

    cout << "Parallel Merge Sort Time: "
         << end - start << " sec" << endl;

    return 0;
}