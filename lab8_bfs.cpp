#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <string>
#include <sstream>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

using Matrix = vector<vector<int>>;
using AdjList = vector<vector<int>>;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());



Matrix generateGraph(int n, double p) {
    Matrix M(n, vector<int>(n, 0));
    uniform_real_distribution<double> dist(0.0, 1.0);
    bool hasEdge = false;

    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (dist(rng) < p) {
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

void printMatrix(const Matrix& M, const string& title) {
    cout << title << "\n";
    int n = (int)M.size();

    cout << "     ";
    for (int j = 0; j < n; ++j) cout << setw(3) << j;
    cout << "\n";

    for (int i = 0; i < n; ++i) {
        cout << setw(4) << i << " ";
        for (int j = 0; j < n; ++j) cout << setw(3) << M[i][j];
        cout << "\n";
    }
    cout << "\n";
}

void printAdjList(const AdjList& L, const string& title) {
    cout << title << "\n";
    for (int i = 0; i < (int)L.size(); ++i) {
        cout << "  " << i << ":";
        for (int v : L[i]) cout << " " << v;
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

int countEdges(const Matrix& M) {
    int cnt = 0;
    for (size_t i = 0; i < M.size(); ++i)
        for (size_t j = i + 1; j < M.size(); ++j)
            cnt += M[i][j];
    return cnt;
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

Matrix readGraph() {
    int n;
    while (true) {
        n = readInt("Число вершин (1..20): ");
        if (n >= 1 && n <= 20) break;
        cout << "Допустимо от 1 до 20.\n";
    }

    while (true) {
        cout << "\nВведите матрицу смежности.\n"
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

Matrix chooseGraph() {
    cout << "Граф: 1 - ввести матрицу вручную, 2 - сгенерировать случайно\n";
    int mode;
    while (true) {
        mode = readInt("Ваш выбор: ");
        if (mode == 1 || mode == 2) break;
        cout << "Введите 1 или 2.\n";
    }
    if (mode == 1) return readGraph();

    int n;
    while (true) {
        n = readInt("Число вершин (2..20): ");
        if (n >= 2 && n <= 20) break;
        cout << "Допустимо от 2 до 20.\n";
    }
    return generateGraph(n, 0.3);
}

class ListQueue {
    struct Node {
        int val;
        Node* next;
        explicit Node(int v) : val(v), next(nullptr) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t count = 0;

public:
    ListQueue() = default;
    ListQueue(const ListQueue&) = delete;
    ListQueue& operator=(const ListQueue&) = delete;
    ~ListQueue() {
        while (!empty()) pop();
    }

    bool empty() const { return head == nullptr; }
    size_t size() const { return count; }
    int front() const { return head->val; }

    void push(int x) {
        Node* node = new Node(x);
        if (tail) tail->next = node;
        else head = node;
        tail = node;
        ++count;
    }

    void pop() {
        Node* old = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete old;
        --count;
    }

    vector<int> toVector() const {
        vector<int> res;
        for (Node* p = head; p; p = p->next) res.push_back(p->val);
        return res;
    }
};


struct BfsResult {
    vector<int> order;
    vector<int> level;
    vector<int> compStart;
};


vector<int> snapshot(queue<int> q) {
    vector<int> res;
    while (!q.empty()) {
        res.push_back(q.front());
        q.pop();
    }
    return res;
}
vector<int> snapshot(const ListQueue& q) { return q.toVector(); }

string fmtList(const vector<int>& v, const string& open = "{", const string& close = "}") {
    string s = open;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) s += ", ";
        s += to_string(v[i]);
    }
    return s + close;
}

template <class Queue>
BfsResult bfsMatrix(const Matrix& G, int start, bool trace = false) {
    int n = (int)G.size();
    BfsResult R;
    R.order.reserve(n);
    R.level.assign(n, -1);
    vector<char> NUM(n, 0);
    int iter = 0;

    auto BFS = [&](int s) {
        Queue Q;
        Q.push(s);
        NUM[s] = 1;
        R.level[s] = 0;
        R.compStart.push_back((int)R.order.size());
        if (trace)
            cout << "Обход из вершины " << s << ": Q = " << fmtList(snapshot(Q)) << "\n";

        while (!Q.empty()) {
            int v = Q.front();
            Q.pop();
            R.order.push_back(v);
            vector<int> added;
            const int* row = G[v].data();
            for (int i = 0; i < n; ++i)
                if (row[i] == 1 && !NUM[i]) {
                    Q.push(i);
                    NUM[i] = 1;
                    R.level[i] = R.level[v] + 1;
                    if (trace) added.push_back(i);
                }
            if (trace) {
                cout << "  Итерация " << ++iter << ": текущая вершина " << v
                     << ", добавлены " << (added.empty() ? string("-") : fmtList(added, "", ""))
                     << ", Q = " << fmtList(snapshot(Q)) << "\n";
            }
        }
    };

    BFS(start);
    for (int v = 0; v < n; ++v)
        if (!NUM[v]) BFS(v);
    return R;
}

// Обход на списках смежности; тип очереди задаётся параметром шаблона
// (std::queue<int> или собственная ListQueue).
template <class Queue>
BfsResult bfsAdjList(const AdjList& L, int start, bool trace = false) {
    int n = (int)L.size();
    BfsResult R;
    R.order.reserve(n);
    R.level.assign(n, -1);
    vector<char> NUM(n, 0);
    int iter = 0;

    auto BFS = [&](int s) {
        Queue Q;
        Q.push(s);
        NUM[s] = 1;
        R.level[s] = 0;
        R.compStart.push_back((int)R.order.size());
        if (trace)
            cout << "Обход из вершины " << s << ": Q = " << fmtList(snapshot(Q)) << "\n";

        while (!Q.empty()) {
            int v = Q.front();
            Q.pop();
            R.order.push_back(v);
            vector<int> added;
            for (int w : L[v])
                if (!NUM[w]) {
                    Q.push(w);
                    NUM[w] = 1;
                    R.level[w] = R.level[v] + 1;
                    if (trace) added.push_back(w);
                }
            if (trace) {
                cout << "  Итерация " << ++iter << ": текущая вершина " << v
                     << ", добавлены " << (added.empty() ? string("-") : fmtList(added, "", ""))
                     << ", Q = " << fmtList(snapshot(Q)) << "\n";
            }
        }
    };

    BFS(start);
    for (int v = 0; v < n; ++v)
        if (!NUM[v]) BFS(v);
    return R;
}

void printBfsResult(const BfsResult& R) {
    int n = (int)R.level.size();
    cout << "Порядок обхода: ";
    for (int v : R.order) cout << v << " ";
    cout << "\n";
    cout << "Число обходов (компонент связности): " << R.compStart.size() << "\n";

    cout << "Вершина:";
    for (int i = 0; i < n; ++i) cout << setw(3) << i;
    cout << "\nУровень:";
    for (int i = 0; i < n; ++i) cout << setw(3) << R.level[i];
    cout << "\n\n";
}

volatile long long sink = 0;

vector<double> measureUs(const vector<function<long long()>>& fs, int reps) {
    using clk = chrono::steady_clock;
    vector<double> best(fs.size(), 1e300);
    for (int batch = 0; batch < 9; ++batch)
        for (size_t k = 0; k < fs.size(); ++k) {
            auto t0 = clk::now();
            for (int r = 0; r < reps; ++r) sink = sink + fs[k]();
            auto t1 = clk::now();
            double us = chrono::duration<double, micro>(t1 - t0).count() / reps;
            best[k] = min(best[k], us);
        }
    return best;
}

long long checksum(const BfsResult& R) {
    return (long long)R.order.size() * 1000003LL + (R.order.empty() ? 0 : R.order.back());
}

string padLeft(const string& s, int w) {
    int len = 0;
    for (unsigned char c : s)
        if ((c & 0xC0) != 0x80) ++len;
    return string(max(0, w - len), ' ') + s;
}

template <class Queue>
long long queueOnly(int n) {
    Queue Q;
    for (int i = 0; i < n; ++i) Q.push(i);
    long long s = 0;
    while (!Q.empty()) {
        s += Q.front();
        Q.pop();
    }
    return s;
}

string fmtNum(double x, int prec) {
    ostringstream os;
    os << fixed << setprecision(prec) << x;
    return os.str();
}

void timingTable() {
    const vector<int> sizes = {50, 100, 200, 500, 1000, 2000, 3000};
    const vector<double> densities = {0.25, 0.5, 0.75};

    cout << "а) Время одного полного обхода в ширину (мкс) для плотностей p = 0.25, 0.5, 0.75.\n";

    for (double p : densities) {
        cout << "\nПлотность графа p = " << p << "\n\n";
        cout << padLeft("n", 6) << padLeft("рёбер", 10)
             << padLeft("матр.+queue", 14) << padLeft("матр.+ListQ", 14)
             << padLeft("ListQ/queue", 13)
             << padLeft("спис.+queue", 14) << padLeft("спис.+ListQ", 14)
             << padLeft("ListQ/queue", 13) << "\n";
        cout << string(98, '-') << "\n";

        for (int n : sizes) {
            Matrix G = generateGraph(n, p);
            AdjList L = matrixToAdjList(G);
            int m = countEdges(G);

            int reps = max(3, (int)(50000000LL / ((long long)n * n)));

            vector<double> t = measureUs({
                [&] { return checksum(bfsMatrix<queue<int>>(G, 0)); },
                [&] { return checksum(bfsMatrix<ListQueue>(G, 0)); },
                [&] { return checksum(bfsAdjList<queue<int>>(L, 0)); },
                [&] { return checksum(bfsAdjList<ListQueue>(L, 0)); }}, reps);
            double tMatStd = t[0], tMatOwn = t[1], tLstStd = t[2], tLstOwn = t[3];

            cout << padLeft(to_string(n), 6) << padLeft(to_string(m), 10)
                 << padLeft(fmtNum(tMatStd, 1), 14) << padLeft(fmtNum(tMatOwn, 1), 14)
                 << padLeft(fmtNum(tMatOwn / tMatStd, 2), 13)
                 << padLeft(fmtNum(tLstStd, 1), 14) << padLeft(fmtNum(tLstOwn, 1), 14)
                 << padLeft(fmtNum(tLstOwn / tLstStd, 2), 13) << "\n";
        }
    }

    cout << "\nб) Только операции с очередью: n добавлений и n извлечений (мкс).\n\n";
    cout << padLeft("n", 6) << padLeft("std::queue", 14) << padLeft("ListQueue", 14)
         << padLeft("ListQ/queue", 14) << "\n";
    cout << string(48, '-') << "\n";

    for (int n : sizes) {
        int reps = max(10, 2000000 / n);
        vector<double> t = measureUs({
            [&] { return queueOnly<queue<int>>(n); },
            [&] { return queueOnly<ListQueue>(n); }}, reps);
        double tStd = t[0], tOwn = t[1];
        cout << padLeft(to_string(n), 6)
             << padLeft(fmtNum(tStd, 2), 14) << padLeft(fmtNum(tOwn, 2), 14)
             << padLeft(fmtNum(tOwn / tStd, 2), 14) << "\n";
    }
    cout << "\n";
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    if (argc > 1) rng.seed((unsigned)stoul(argv[1]));

    Matrix G = chooseGraph();
    int n = (int)G.size();
    AdjList L = matrixToAdjList(G);

    cout << "\n=== Граф ===\n";
    cout << "Число вершин: " << n << ", число рёбер: " << countEdges(G) << "\n\n";
    printMatrix(G, "Матрица смежности:");
    printAdjList(L, "Список смежности:");

    int start = readVertex("Введите вершину, с которой начать обход: ", n);

    cout << "\n=== Задание 1 ===\n";
    cout << "1-2. Обход в ширину (матрица смежности, std::queue)\n";
    BfsResult r1 = bfsMatrix<queue<int>>(G, start, true);
    cout << "\n";
    printBfsResult(r1);

    cout << "3*. Обход в ширину (списки смежности, std::queue)\n";
    BfsResult r2 = bfsAdjList<queue<int>>(L, start, true);
    cout << "\n";
    printBfsResult(r2);
    cout << "[проверка] порядок обхода на списках "
         << (r2.order == r1.order && r2.level == r1.level ? "совпадает" : "НЕ совпадает")
         << " с порядком на матрице\n";

    cout << "\n=== Задание 2* ===\n";
    cout << "1. Обход в ширину (матрица смежности, собственная очередь на списке)\n";
    BfsResult r3 = bfsMatrix<ListQueue>(G, start, true);
    cout << "\n";
    printBfsResult(r3);
    cout << "[проверка] порядок обхода с собственной очередью "
         << (r3.order == r1.order && r3.level == r1.level ? "совпадает" : "НЕ совпадает")
         << " с порядком со std::queue\n\n";

    cout << "Обход в ширину (списки смежности, собственная очередь на списке)\n";
    BfsResult r4 = bfsAdjList<ListQueue>(L, start, true);
    cout << "\n";
    printBfsResult(r4);
    cout << "[проверка] порядок обхода на списках с собственной очередью "
         << (r4.order == r1.order && r4.level == r1.level ? "совпадает" : "НЕ совпадает")
         << " с порядком со std::queue\n\n";

    cout << "2. Оценка времени работы\n";
    timingTable();

    return 0;
}
