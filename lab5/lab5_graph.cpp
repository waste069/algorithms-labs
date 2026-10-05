#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

using Matrix = vector<vector<int>>;

// Генерация матрицы смежности неориентированного графа (без петель)
Matrix generateAdjacency(int n, double p, mt19937& gen) {
    Matrix a(n, vector<int>(n, 0));
    bernoulli_distribution edge(p);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            a[i][j] = a[j][i] = edge(gen) ? 1 : 0;
        }
    }
    return a;
}

void printMatrix(const Matrix& m, const string& title, bool columnsAreEdges = false) {
    cout << title << "\n";
    if (m.empty() || m[0].empty()) {
        cout << "(матрица пуста)\n";
        return;
    }
    cout << "     ";
    for (size_t j = 0; j < m[0].size(); j++) {
        cout << setw(3) << (columnsAreEdges ? "e" + to_string(j + 1) : to_string(j + 1));
    }
    cout << "\n";
    for (size_t i = 0; i < m.size(); i++) {
        cout << "v" << setw(2) << left << (i + 1) << right << "  ";
        for (size_t j = 0; j < m[i].size(); j++) {
            cout << setw(3) << m[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
}

// Размер графа по матрице смежности: сумма элементов / 2
int sizeByAdjacency(const Matrix& a) {
    int sum = 0;
    for (const auto& row : a)
        for (int x : row) sum += x;
    return sum / 2;
}

// Степени вершин по матрице смежности: сумма элементов строки
vector<int> degreesByAdjacency(const Matrix& a) {
    vector<int> deg(a.size(), 0);
    for (size_t i = 0; i < a.size(); i++)
        for (int x : a[i]) deg[i] += x;
    return deg;
}

// Матрица инцидентности: строки - вершины, столбцы - рёбра
Matrix buildIncidence(const Matrix& a) {
    int n = a.size();
    vector<pair<int, int>> edges;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i][j]) edges.push_back({i, j});

    Matrix inc(n, vector<int>(edges.size(), 0));
    for (size_t k = 0; k < edges.size(); k++) {
        inc[edges[k].first][k] = 1;
        inc[edges[k].second][k] = 1;
    }
    return inc;
}

// Размер графа по матрице инцидентности: число столбцов
int sizeByIncidence(const Matrix& inc) {
    return inc.empty() ? 0 : inc[0].size();
}

// Степени вершин по матрице инцидентности: сумма элементов строки
vector<int> degreesByIncidence(const Matrix& inc) {
    vector<int> deg(inc.size(), 0);
    for (size_t i = 0; i < inc.size(); i++)
        for (int x : inc[i]) deg[i] += x;
    return deg;
}

// Поиск и вывод изолированных, концевых и доминирующих вершин
void printSpecialVertices(const vector<int>& deg) {
    int n = deg.size();
    vector<int> isolated, pendant, dominating;
    for (int i = 0; i < n; i++) {
        if (deg[i] == 0) isolated.push_back(i + 1);
        if (deg[i] == 1) pendant.push_back(i + 1);
        if (deg[i] == n - 1) dominating.push_back(i + 1);
    }

    cout << "Степени вершин:\n";
    for (int i = 0; i < n; i++)
        cout << "deg(v" << i + 1 << ") = " << deg[i] << "\n";

    auto show = [](const string& name, const vector<int>& v) {
        cout << name << ": ";
        if (v.empty()) cout << "нет";
        for (int x : v) cout << "v" << x << " ";
        cout << "\n";
    };
    show("Изолированные вершины", isolated);
    show("Концевые вершины", pendant);
    show("Доминирующие вершины", dominating);
    cout << "\n";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    int n;
    cout << "Введите количество вершин n: ";
    cin >> n;
    if (n <= 0) {
        cout << "n должно быть положительным\n";
        return 1;
    }

    random_device rd;
    mt19937 gen(rd());

    //  Задание 1
    cout << "\nЗадание 1 (матрица смежности)\n\n";
    Matrix adj = generateAdjacency(n, 0.4, gen);
    printMatrix(adj, "Матрица смежности:");

    cout << "Размер графа |E(G)| = " << sizeByAdjacency(adj) << "\n\n";
    printSpecialVertices(degreesByAdjacency(adj));

    // Задание 2
    cout << "Задание 2* (матрица инцидентности)\n\n";
    Matrix inc = buildIncidence(adj);
    printMatrix(inc, "Матрица инцидентности:", true);

    cout << "Размер графа |E(G)| = " << sizeByIncidence(inc) << "\n\n";
    printSpecialVertices(degreesByIncidence(inc));

    return 0;
}
