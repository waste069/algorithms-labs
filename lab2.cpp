#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void shell(int *items, int count) {
    int i, j, gap, k;
    int x, a[5];

    a[0] = 9; a[1] = 5; a[2] = 3; a[3] = 2; a[4] = 1;

    for (k = 0; k < 5; k++) {
        gap = a[k];
        for (i = gap; i < count; ++i) {
            x = items[i];
            for (j = i - gap; (x < items[j]) && (j >= 0); j = j - gap)
                items[j + gap] = items[j];
            items[j + gap] = x;
        }
    }
}

void qs(int *items, int left, int right) {
    int i, j;
    int x, y;

    i = left; j = right;
    x = items[(left + right) / 2];

    do {
        while ((items[i] < x) && (i < right)) i++;
        while ((x < items[j]) && (j > left)) j--;

        if (i <= j) {
            y = items[i];
            items[i] = items[j];
            items[j] = y;
            i++; j--;
        }
    } while (i <= j);

    if (left < j) qs(items, left, j);
    if (i < right) qs(items, i, right);
}

int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

void test_matrix(int n) {
    int* a = new (nothrow) int[n * n];
    int* b = new (nothrow) int[n * n];
    int* c = new (nothrow) int[n * n];

    if (!a || !b || !c) {
        cout << "N = " << n << " | Memory allocation failed" << endl;
        delete[] a; delete[] b; delete[] c;
        return;
    }

    for (int i = 0; i < n * n; i++) {
        a[i] = rand() % 100 + 1;
        b[i] = rand() % 100 + 1;
    }

    clock_t start_prog = clock();

    clock_t start_mult = clock();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int elem_c = 0;
            for (int r = 0; r < n; r++) {
                elem_c += a[i * n + r] * b[r * n + j];
            }
            c[i * n + j] = elem_c;
        }
    }
    clock_t end_mult = clock();
    clock_t end_prog = clock();

    double t_mult = (double)(end_mult - start_mult) / CLOCKS_PER_SEC;
    double t_prog = (double)(end_prog - start_prog) / CLOCKS_PER_SEC;

    cout << "N = " << n 
         << " | Mult Time: " << t_mult << " s"
         << " | Total Prog Time: " << t_prog << " s" << endl;

    delete[] a;
    delete[] b;
    delete[] c;
}

void fill_random(int* arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 10000;
}

void fill_asc(int* arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = i;
}

void fill_desc(int* arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = n - i;
}

void fill_half(int* arr, int n) {
    int half = n / 2;
    for (int i = 0; i < half; i++) arr[i] = i;
    for (int i = half; i < n; i++) arr[i] = n - i;
}

void copy_arr(const int* src, int* dst, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

int main() {
    srand((unsigned int)time(NULL));

    cout << "TASK 1: MATRIX MULTIPLICATION" << endl;
    int matrix_sizes[] = {100, 200, 400, 1000, 2000, 4000, 10000};
    for (int i = 0; i < 7; i++) {
        if (matrix_sizes[i] >= 4000) {
            cout << "N = " << matrix_sizes[i] << " | Skipped (Execution time > 100s)" << endl;
            continue;
        }
        test_matrix(matrix_sizes[i]);
    }

    cout << "\nTASK 2: SORTING ALGORITHMS BENCHMARK" << endl;
    int sort_sizes[] = {10000, 25000, 50000};

    const char* names[] = {
        "Random Array",
        "Ascending Array",
        "Descending Array",
        "Half-Ascending / Half-Descending Array"
    };

    for (int s = 0; s < 3; s++) {
        int count = sort_sizes[s];
        int* base = new int[count];
        int* work = new int[count];

        cout << "BENCHMARK FOR N = " << count << endl;

        for (int mode = 0; mode < 4; mode++) {
            cout << "\n" << names[mode] << " " << endl;

            if (mode == 0) fill_random(base, count);
            else if (mode == 1) fill_asc(base, count);
            else if (mode == 2) fill_desc(base, count);
            else if (mode == 3) fill_half(base, count);

            copy_arr(base, work, count);
            clock_t t1 = clock();
            shell(work, count);
            clock_t t2 = clock();
            cout << "Shell Sort: " << (double)(t2 - t1) / CLOCKS_PER_SEC << " s" << endl;

            copy_arr(base, work, count);
            t1 = clock();
            qs(work, 0, count - 1);
            t2 = clock();
            cout << "Quick Sort (qs): " << (double)(t2 - t1) / CLOCKS_PER_SEC << " s" << endl;

            copy_arr(base, work, count);
            t1 = clock();
            qsort(work, count, sizeof(int), compare_ints);
            t2 = clock();
            cout << "qsort(): " << (double)(t2 - t1) / CLOCKS_PER_SEC << " s" << endl;
        }

        delete[] base;
        delete[] work;
    }

    return 0;
}