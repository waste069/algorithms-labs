#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

using Matrix = vector<vector<int>>;
using AdjList = vector<vector<int>>;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

Matrix generateGraph(int n) {
    Matrix M(n, vector<int>(n, 0));
    uniform_real_distribution<double> dist(0.0, 1.0);
    bool hasEdge = false;

    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (dist(rng) < 0.2) {
                M[i][j] = M[j][i] = 1;
                hasEdge = true;
            }

    if (!hasEdge && n > 1) {
        int i = rng() % n;
        int j = rng() % n;
        if (i == j) j = (j + 1) % n;
        M[i][j] = M[j][i] = 1;
    }
    return M;
}

void printMatrix(const Matrix& M, const string& title, int star = -1,
                 const vector<string>& labels = {}) {
    cout << title << "\n";
    int n = (int)M.size();

    auto label = [&](int i) {
        return labels.empty()
                   ? to_string(i) + (i == star ? "*" : "")
                   : labels[i];
    };

    int w = labels.empty() ? 4 : 6;

    cout << "      ";
    for (int j = 0; j < n; ++j) cout << setw(w) << label(j);
    cout << "\n";

    for (int i = 0; i < n; ++i) {
        cout << setw(w + 1) << label(i);
        for (int j = 0; j < n; ++j) cout << setw(w) << M[i][j];
        cout << "\n";
    }
    cout << "\n";
}

void printAdjList(const AdjList& L, const string& title, int star = -1) {
    cout << title << "\n";
    for (int i = 0; i < (int)L.size(); ++i) {
        cout << i << (i == star ? "*" : "") << ": ";
        for (int v : L[i]) cout << v << " ";
        cout << "\n";
    }
    cout << "\n";
}

AdjList matrixToAdjList(const Matrix& M) {
    int n = (int)M.size();
    AdjList L(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (M[i][j]) L[i].push_back(j);
    return L;
}

int readInt(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка. Повторите ввод: ";
    }
    return x;
}

int readVertex(const string& prompt, int n) {
    while (true) {
        int v = readInt(prompt);
        if (v >= 0 && v < n) return v;
        cout << "Недопустимый номер вершины. Диапазон: 0.." << n - 1 << "\n";
    }
}

int readAdjacentVertex(const string& prompt, const Matrix& M, int u) {
    while (true) {
        int v = readVertex(prompt, (int)M.size());
        if (v == u) {
            cout << "Вершина должна отличаться от первой.\n";
            continue;
        }
        if (M[u][v]) return v;
        cout << "Вершины " << u << " и " << v << " не смежны. Повторите.\n";
    }
}

Matrix readGraph(const string& name) {
    int n;
    while (true) {
        n = readInt("Число вершин " + name + " (1..20): ");
        if (n >= 1 && n <= 20) break;
        cout << "Допустимо от 1 до 20.\n";
    }

    while (true) {
        cout << "\nВведите матрицу смежности " << name << ".\n"
             << "В каждой строке введите " << n
             << " чисел (0 или 1) через пробел и нажмите Enter.\n"
             << "M[i][j] = 1 означает, что вершины i и j соединены ребром.\n"
             << "Матрица симметричная, на диагонали нули.\n";

        Matrix M(n, vector<int>(n, 0));
        bool ok = true;

        for (int i = 0; i < n; ++i) {
            cout << "Строка " << i << ": ";
            for (int j = 0; j < n; ++j) {
                if (!(cin >> M[i][j])) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    M[i][j] = -1;
                    ok = false;
                    break;
                }
                if (M[i][j] != 0 && M[i][j] != 1) ok = false;
            }
        }

        if (!ok) {
            cout << "Элементы должны быть числами 0 или 1. Введите матрицу заново.\n";
            continue;
        }
        for (int i = 0; i < n && ok; ++i) {
            if (M[i][i] != 0) ok = false;
            for (int j = 0; j < n && ok; ++j)
                if (M[i][j] != M[j][i]) ok = false;
        }
        if (!ok) {
            cout << "Матрица должна быть симметричной с нулевой диагональю. Введите заново.\n";
            continue;
        }
        return M;
    }
}

Matrix chooseGraph(const string& name) {
    cout << "Граф " << name << ": 1 - ввести вручную, 2 - сгенерировать случайно\n";
    int mode;
    while (true) {
        mode = readInt("Ваш выбор: ");
        if (mode == 1 || mode == 2) break;
        cout << "Введите 1 или 2.\n";
    }
    if (mode == 1) return readGraph(name);
    int n = 4 + rng() % 3;
    return generateGraph(n);
}

Matrix mergeVertices(const Matrix& M, int u, int v, bool makeLoop,
                     vector<int>& oldToNew, int& z) {
    int n = (int)M.size();

    set<int> Q;
    for (int i = 0; i < n; ++i) if (M[u][i]) Q.insert(i);
    for (int i = 0; i < n; ++i) if (M[v][i]) Q.insert(i);
    Q.erase(u);
    Q.erase(v);

    oldToNew.assign(n, -1);
    int idx = 0;
    for (int i = 0; i < n; ++i)
        if (i != u && i != v) oldToNew[i] = idx++;

    z = idx;
    int m = n - 1;
    Matrix R(m, vector<int>(m, 0));

    for (int i = 0; i < n; ++i) {
        if (oldToNew[i] == -1) continue;
        for (int j = 0; j < n; ++j) {
            if (oldToNew[j] == -1) continue;
            R[oldToNew[i]][oldToNew[j]] = M[i][j];
        }
    }

    for (int w : Q)
        if (oldToNew[w] != -1)
            R[z][oldToNew[w]] = R[oldToNew[w]][z] = 1;

    if (M[u][u] || M[v][v]) R[z][z] = 1;
    if (makeLoop && M[u][v]) R[z][z] = 1;

    return R;
}

void printShift(const vector<int>& oldToNew, int u, int v, int z) {
    cout << "Как изменилась нумерация вершин:\n";
    for (int i = 0; i < (int)oldToNew.size(); ++i) {
        if (i == u || i == v) continue;
        cout << "  " << i << " -> " << oldToNew[i];
        if (oldToNew[i] != i) cout << "   (сдвиг)";
        cout << "\n";
    }
    cout << "  " << u << " и " << v << " -> " << z << "*   (новая вершина)\n\n";
}

Matrix splitVertexFull(const Matrix& M, int v) {
    int n = (int)M.size();
    int nv = n;

    Matrix R(n + 1, vector<int>(n + 1, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            R[i][j] = M[i][j];

    for (int w = 0; w < n; ++w) {
        if (w == v) continue;
        if (M[v][w]) R[nv][w] = R[w][nv] = 1;
    }
    if (M[v][v]) R[nv][nv] = 1;
    R[v][nv] = R[nv][v] = 1;

    return R;
}

Matrix unionGraph(const Matrix& A, const Matrix& B) {
    int n = max((int)A.size(), (int)B.size());
    Matrix R(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            int a = (i < (int)A.size() && j < (int)A.size()) ? A[i][j] : 0;
            int b = (i < (int)B.size() && j < (int)B.size()) ? B[i][j] : 0;
            R[i][j] = (a || b);
        }
    return R;
}

Matrix intersectionGraph(const Matrix& A, const Matrix& B) {
    int n = min((int)A.size(), (int)B.size());
    Matrix R(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            R[i][j] = A[i][j] && B[i][j];
    return R;
}

Matrix ringSumGraph(const Matrix& A, const Matrix& B, vector<int>& keptOld) {
    int n = max((int)A.size(), (int)B.size());
    Matrix R(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            int a = (i < (int)A.size() && j < (int)A.size()) ? A[i][j] : 0;
            int b = (i < (int)B.size() && j < (int)B.size()) ? B[i][j] : 0;
            R[i][j] = (a != b);
        }

    keptOld.clear();
    for (int i = 0; i < n; ++i) {
        bool has = false;
        for (int j = 0; j < n; ++j)
            if (R[i][j]) { has = true; break; }
        if (has) keptOld.push_back(i);
    }

    int m = (int)keptOld.size();
    Matrix out(m, vector<int>(m, 0));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            out[i][j] = R[keptOld[i]][keptOld[j]];
    return out;
}

Matrix cartesianProduct(const Matrix& A, const Matrix& B) {
    int n1 = (int)A.size();
    int n2 = (int)B.size();
    int n = n1 * n2;

    Matrix R(n, vector<int>(n, 0));
    auto idx = [n2](int i, int j) { return i * n2 + j; };

    for (int i = 0; i < n1; ++i)
        for (int j1 = 0; j1 < n2; ++j1)
            for (int j2 = 0; j2 < n2; ++j2)
                if (B[j1][j2]) R[idx(i, j1)][idx(i, j2)] = 1;

    for (int j = 0; j < n2; ++j)
        for (int i1 = 0; i1 < n1; ++i1)
            for (int i2 = 0; i2 < n1; ++i2)
                if (A[i1][i2]) R[idx(i1, j)][idx(i2, j)] = 1;

    return R;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Matrix M1 = chooseGraph("G1");
    Matrix M2 = chooseGraph("G2");
    int n1 = (int)M1.size();
    int n2 = (int)M2.size();

    cout << "\nГрафы G1 (" << n1 << " вершин) и G2 (" << n2 << " вершин).\n\n";

    printMatrix(M1, "Матрица смежности G1:");
    printMatrix(M2, "Матрица смежности G2:");
    printAdjList(matrixToAdjList(M1), "Список смежности G1:");
    printAdjList(matrixToAdjList(M2), "Список смежности G2:");

    cout << "=== Задание 2 ===\n";

    if (n1 >= 2) {
        cout << "а) Отождествление вершин\n";
        int u = readVertex("Введите первую вершину u: ", n1);
        int v = readVertex("Введите вторую вершину v: ", n1);
        while (v == u) {
            cout << "Вершины должны различаться.\n";
            v = readVertex("Введите вторую вершину v: ", n1);
        }

        vector<int> map1;
        int z1;
        Matrix Rident = mergeVertices(M1, u, v, true, map1, z1);
        cout << "\n";
        printShift(map1, u, v, z1);
        printMatrix(Rident, "Результат отождествления (матрица), * - новая вершина:", z1);
        printAdjList(matrixToAdjList(Rident), "Результат отождествления (список):", z1);

        cout << "Проверка петли у новой вершины " << z1 << ":\n";
        if (Rident[z1][z1]) {
            cout << "  ПЕТЛЯ ОБРАЗОВАЛАСЬ. Причины:\n";
            if (M1[u][u]) cout << "    * у вершины " << u << " уже была петля;\n";
            if (M1[v][v]) cout << "    * у вершины " << v << " уже была петля;\n";
            if (M1[u][v]) cout << "    * вершины " << u << " и " << v
                               << " были смежны, их общее ребро стало петлёй;\n";
        } else {
            cout << "  Петли нет: исходные вершины не были смежны и не имели петель.\n";
        }
        cout << "\n";

        bool hasEdge = false;
        for (int i = 0; i < n1 && !hasEdge; ++i)
            for (int j = 0; j < n1; ++j)
                if (M1[i][j]) { hasEdge = true; break; }

        if (hasEdge) {
            cout << "б) Стягивание ребра\n";
            int ce_u = readVertex("Введите первую вершину ребра: ", n1);
            int ce_v = readAdjacentVertex("Введите вторую вершину ребра: ", M1, ce_u);

            vector<int> map2;
            int z2;
            Matrix Rcontr = mergeVertices(M1, ce_u, ce_v, false, map2, z2);
            cout << "\n";
            printShift(map2, ce_u, ce_v, z2);
            printMatrix(Rcontr, "Результат стягивания (матрица), * - новая вершина:", z2);
            printAdjList(matrixToAdjList(Rcontr), "Результат стягивания (список):", z2);

            cout << "Проверка петли у новой вершины " << z2 << ":\n";
            if (Rcontr[z2][z2]) {
                cout << "  ПЕТЛЯ ЕСТЬ (унаследована от петель исходных вершин).\n";
            } else {
                cout << "  Петли нет (при стягивании ребро u-v удаляется, петля не создаётся).\n";
            }
            cout << "\n";
        }
    }

    cout << "в) Расщепление вершины (полный дубль)\n";
    int sv = readVertex("Введите вершину для расщепления: ", n1);
    Matrix Rsplit = splitVertexFull(M1, sv);

    cout << "\nНовая вершина " << n1 << "* является полным дублёром вершины "
         << sv << " (имеет всех тех же соседей) и соединена с ней ребром.\n"
         << "Старые вершины 0.." << n1 - 1 << " сохраняют нумерацию.\n\n";
    printMatrix(Rsplit, "Результат расщепления (матрица), * - новая вершина:", n1);
    printAdjList(matrixToAdjList(Rsplit), "Результат расщепления (список):", n1);

    cout << "=== Задание 3 ===\n";
    printMatrix(unionGraph(M1, M2), "а) Объединение G1 и G2:");
    printMatrix(intersectionGraph(M1, M2), "б) Пересечение G1 и G2:");

    int nraw = max(n1, n2);
    vector<int> ringKept;
    Matrix Rring = ringSumGraph(M1, M2, ringKept);

    if (ringKept.empty()) {
        cout << "в) Кольцевая сумма G1 и G2: пустой граф.\n"
             << "   Все рёбра взаимно уничтожились, изолированных вершин не осталось.\n\n";
    } else {
        printMatrix(Rring, "в) Кольцевая сумма G1 и G2 (без изолированных вершин):");
        if ((int)ringKept.size() < nraw) {
            cout << "Изолированные вершины удалены. Перенумерация:\n";
            for (int i = 0; i < (int)ringKept.size(); ++i)
                cout << "  новая " << i << " <- старая " << ringKept[i] << "\n";
            cout << "Удалённые вершины:";
            for (int i = 0; i < nraw; ++i)
                if (find(ringKept.begin(), ringKept.end(), i) == ringKept.end())
                    cout << " " << i;
            cout << "\n\n";
        }
    }

    cout << "=== Задание 4 ===\n";

    vector<string> prodLabels;
    prodLabels.reserve(n1 * n2);
    for (int i = 0; i < n1; ++i)
        for (int j = 0; j < n2; ++j)
            prodLabels.push_back("(" + to_string(i) + "," + to_string(j) + ")");

    Matrix prod = cartesianProduct(M1, M2);
    printMatrix(prod, "Декартово произведение G1 x G2 "
                      "(вершина (i,j) - пара: i из G1, j из G2):",
                -1, prodLabels);

    return 0;
}