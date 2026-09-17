#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

// Задание 5
struct Student {
    char famil[20];
    char name[20];
    char facult[20];
    int nomzach;
};

int main() {
    srand(time(0));

    // ЗАДАНИЕ 1
    cout << "TASK 1" << endl;
    int a[10] = {15, 3, 8, 24, 2, 19, 42, 7, 1, 11};
    int min_val = a[0];
    int max_val = a[0];

    for (int i = 1; i < 10; i++) {
        if (a[i] < min_val) min_val = a[i];
        if (a[i] > max_val) max_val = a[i];
    }

    cout << "Array: ";
    for (int i = 0; i < 10; i++) cout << a[i] << " ";
    cout << "\nMax: " << max_val << "\nMin: " << min_val << endl;
    cout << "Difference (max - min): " << max_val - min_val << endl;


// Задание 2
    cout << "TASK 2" << endl;
    int rand_arr[10];

    for (int i = 0; i < 10; i++) {
        rand_arr[i] = rand() % 100;
    }

    cout << "Random array: ";
    for (int i = 0; i < 10; i++) {
        cout << rand_arr[i] << " ";
    }
    cout << endl;


// Задание 3
    cout << "TASK 3" << endl;
    int n;
    cout << "Enter dynamic array size: ";
    cin >> n;

    if (n > 0) {
        int* dyn_arr = new int[n];

        cout << "Enter " << n << " array elements:" << endl;
        for (int i = 0; i < n; i++) {
            cout << "dyn_arr[" << i << "] = ";
            cin >> dyn_arr[i];
        }

        cout << "Entered dynamic array: ";
        for (int i = 0; i < n; i++) {
            cout << dyn_arr[i] << " ";
        }
        cout << endl;

        delete[] dyn_arr;
    } else {
        cout << "Error: size must be greater than 0!" << endl;
    }


// Задание 4
    cout << "TASK 4" << endl;
    int n_matrix;
    int m_matrix;
    cout << "Enter matrix size" << endl;
    cin >> n_matrix >> m_matrix;
    int** matrix = new int*[n_matrix];
    for (int i = 0; i < n_matrix; i++) {
        matrix[i] = new int[m_matrix];
    }
    for (int i = 0; i < n_matrix; i++) {
        for (int j = 0; j < m_matrix; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
    srand(time(NULL));
    for (int i = 0; i < n_matrix; i++) {
        for (int j = 0; j < m_matrix; j++) {
            cout << matrix[i][j] << " "; 
        }
        cout << endl;
    }

    cout << "Row sums" << endl;
    for (int i = 1; i < n_matrix; i += 2) {
        int row_sum_even = 0;
        for (int j = 0; j < m_matrix; j ++) {
            row_sum_even += matrix[i][j];
        }
        cout << "Row " << i + 1 << ": " << row_sum_even << endl;
    }

    cout << "Column sums" << endl;
    for (int j = 1; j < m_matrix; j += 2) {
        int col_sum = 0;
        for (int i = 0; i < n_matrix; i ++) {
            col_sum += matrix[i][j];
        }
        cout << "Column " << j + 1 << ": " << col_sum << endl;
    }


 // Задание 5
    cout << "TASK 5" << endl;
    int count = 3;
    Student stud[3];

    cout << "Enter data for " << count << " students:" << endl;
    for (int i = 0; i < count; i++) {
        cout << "\nStudent " << i + 1 << ":" << endl;
        cout << "  Last name: "; cin >> stud[i].famil;
        cout << "  First name: "; cin >> stud[i].name;
        cout << "  Faculty: "; cin >> stud[i].facult;
        cout << "  Record book number: "; cin >> stud[i].nomzach;
    }

    char search_famil[20];
    cout << "\nEnter last name to search: ";
    cin >> search_famil;

    bool found = false;
    for (int i = 0; i < count; i++) {
        if (strstr(stud[i].famil, search_famil) != NULL) {
            cout << "\n[Match found]:" << endl;
            cout << "  Student: " << stud[i].famil << " " << stud[i].name << endl;
            cout << "  Faculty: " << stud[i].facult << endl;
            cout << "  Record book number: " << stud[i].nomzach << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "Student with last name \"" << search_famil << "\" not found." << endl;
    }

    return 0;
}