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

// ===================== Генерация матриц =====================

// Неориентированный граф: симметричная матрица, без петель
Matrix generateAdjacencyUndirected(int n, double p, mt19937& gen) {
    Matrix a(n, vector<int>(n, 0));
    bernoulli_distribution edge(p);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            a[i][j] = a[j][i] = edge(gen) ? 1 : 0;
    return a;
}

// Ориентированный граф: несимметричная матрица, без петель
Matrix generateAdjacencyDirected(int n, double p, mt19937& gen) {
    Matrix a(n, vector<int>(n, 0));
    bernoulli_distribution edge(p);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j)
                a[i][j] = edge(gen) ? 1 : 0;
    return a;
}

// ===================== Печать матрицы =====================

void printMatrix(const Matrix& m, const string& title, bool columnsAreEdges = false) {
    cout << title << "\n";
    if (m.empty() || m[0].empty()) {
        cout << "(матрица пуста)\n";
        return;
    }
    cout << "     ";
    for (size_t j = 0; j < m[0].size(); j++) {
        string label = columnsAreEdges ? "e" + to_string(j + 1) : to_string(j + 1);
        cout << setw(4) << label;
    }
    cout << "\n";
    for (size_t i = 0; i < m.size(); i++) {
        cout << "v" << setw(2) << left << (i + 1) << right << "  ";
        for (size_t j = 0; j < m[i].size(); j++)
            cout << setw(4) << m[i][j];
        cout << "\n";
    }
    cout << "\n";
}

// ===================== Список рёбер =====================

struct Edge {
    int from;   // начальная вершина (1-based)
    int to;     // конечная вершина (1-based)
};

// Построение списка рёбер по матрице смежности
vector<Edge> buildEdgeList(const Matrix& a, bool directed) {
    vector<Edge> edges;
    int n = a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j]) {
                if (directed) {
                    edges.push_back({i + 1, j + 1});
                } else if (j > i) {          // для неориентированного — без дублей
                    edges.push_back({i + 1, j + 1});
                }
            }
        }
    }
    return edges;
}

void printEdgeList(const vector<Edge>& edges, bool directed) {
    cout << "Список рёбер (" << (directed ? "ориентированный" : "неориентированный") << "):\n";
    if (edges.empty()) {
        cout << "(рёбер нет)\n\n";
        return;
    }
    for (size_t k = 0; k < edges.size(); k++) {
        if (directed)
            cout << "e" << setw(2) << left << (k + 1) << right
                 << ":  v" << edges[k].from << " -> v" << edges[k].to << "\n";
        else
            cout << "e" << setw(2) << left << (k + 1) << right
                 << ":  v" << edges[k].from << " — v" << edges[k].to << "\n";
    }
    cout << "Всего рёбер: " << edges.size() << "\n\n";
}

// ===================== Степени и спецвершины =====================

// Степени вершин по матрице смежности
// directed:  true — полустепени исхода (out-degree); false — обычная степень
vector<int> degreesByAdjacency(const Matrix& a, bool directed) {
    int n = a.size();
    vector<int> deg(n, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (directed) {
                deg[i] += a[i][j];             // out-degree
            } else {
                if (a[i][j]) deg[i]++;          // обычная степень
            }
        }
    return deg;
}

// Для орграфа также считаем in-degree
vector<int> inDegreesByAdjacency(const Matrix& a) {
    int n = a.size();
    vector<int> deg(n, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            deg[j] += a[i][j];
    return deg;
}

void printSpecialVertices(const vector<int>& deg, int n) {
    vector<int> isolated, pendant, dominating;
    for (int i = 0; i < n; i++) {
        if (deg[i] == 0)     isolated.push_back(i + 1);
        if (deg[i] == 1)     pendant.push_back(i + 1);
        if (deg[i] == n - 1) dominating.push_back(i + 1);
    }
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

// ===================== Матрица инцидентности =====================

Matrix buildIncidence(const Matrix& a, bool directed) {
    int n = a.size();
    vector<Edge> edges = buildEdgeList(a, directed);
    Matrix inc(n, vector<int>(edges.size(), 0));
    for (size_t k = 0; k < edges.size(); k++) {
        int u = edges[k].from - 1;
        int v = edges[k].to   - 1;
        if (directed) {
            inc[u][k] =  1;   // исход
            inc[v][k] = -1;   // заход
        } else {
            inc[u][k] = 1;
            inc[v][k] = 1;
        }
    }
    return inc;
}

int sizeByIncidence(const Matrix& inc) {
    return inc.empty() ? 0 : (int)inc[0].size();
}

// Степени по матрице инцидентности
vector<int> degreesByIncidence(const Matrix& inc) {
    vector<int> deg(inc.size(), 0);
    for (size_t i = 0; i < inc.size(); i++)
        for (int x : inc[i])
            deg[i] += (x == 1) ? 1 : 0;   // считаем только +1 (out-degree для орграфа)
    return deg;
}

// ===================== Главная функция =====================

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

    double p;
    cout << "Введите вероятность появления ребра p (0..1): ";
    cin >> p;
    if (p < 0 || p > 1) {
        cout << "p должно быть в диапазоне [0, 1]\n";
        return 1;
    }

    int mode;
    cout << "Тип графа: 0 — неориентированный, 1 — ориентированный: ";
    cin >> mode;
    bool directed = (mode == 1);

    random_device rd;
    mt19937 gen(rd());

    // Генерация матрицы смежности
    Matrix adj = directed
        ? generateAdjacencyDirected(n, p, gen)
        : generateAdjacencyUndirected(n, p, gen);

    cout << "\n========================================\n";
    cout << (directed ? "ОРИЕНТИРОВАННЫЙ" : "НЕОРИЕНТИРОВАННЫЙ") << " ГРАФ\n";
    cout << "Вершины: " << n << ", вероятность ребра: " << p << "\n";
    cout << "========================================\n\n";

    // --- Задание 1: матрица смежности ---
    cout << "=== Задание 1: Матрица смежности ===\n\n";
    printMatrix(adj, "Матрица смежности:");

    vector<int> deg = degreesByAdjacency(adj, directed);
    cout << "Степени вершин:\n";
    if (directed) {
        vector<int> inDeg = inDegreesByAdjacency(adj);
        for (int i = 0; i < n; i++)
            cout << "v" << (i + 1) << ":  out = " << deg[i]
                 << ", in = " << inDeg[i] << "\n";
    } else {
        for (int i = 0; i < n; i++)
            cout << "deg(v" << (i + 1) << ") = " << deg[i] << "\n";
    }
    cout << "\n";

    printSpecialVertices(deg, n);

    // --- Список рёбер ---
    cout << "=== Список рёбер ===\n\n";
    vector<Edge> edges = buildEdgeList(adj, directed);
    printEdgeList(edges, directed);

    // --- Задание 2: матрица инцидентности ---
    cout << "=== Задание 2*: Матрица инцидентности ===\n\n";
    Matrix inc = buildIncidence(adj, directed);
    printMatrix(inc, "Матрица инцидентности:", true);
    cout << "Размер графа |E(G)| = " << sizeByIncidence(inc) << "\n\n";

    // Степени по матрице инцидентности
    vector<int> degInc = degreesByIncidence(inc);
    cout << "Степени вершин (по матрице инцидентности):\n";
    for (int i = 0; i < n; i++)
        cout << "deg(v" << (i + 1) << ") = " << degInc[i] << "\n";
    cout << "\n";

    return 0;
}
