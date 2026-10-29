#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

vector<vector<int>> G;
vector<vector<int>> adj;
vector<bool> NUM;
int size_G;

void generateGraph(int n, double p)
{
    G.assign(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            double r = (double)rand() / RAND_MAX;
            G[i][j] = G[j][i] = (r < p) ? 1 : 0;
        }
}

void printMatrix()
{
    cout << "Матрица смежности:" << endl << "    ";
    for (int j = 0; j < size_G; j++)
        cout << setw(3) << j + 1;
    cout << endl;
    for (int i = 0; i < size_G; i++)
    {
        cout << setw(3) << i + 1 << " ";
        for (int j = 0; j < size_G; j++)
            cout << setw(3) << G[i][j];
        cout << endl;
    }
}

void buildAdjacencyLists()
{
    adj.assign(size_G, vector<int>());
    for (int i = 0; i < size_G; i++)
        for (int j = 0; j < size_G; j++)
            if (G[i][j] == 1)
                adj[i].push_back(j);
}

void printAdjacencyLists()
{
    cout << "Списки смежности:" << endl;
    for (int i = 0; i < size_G; i++)
    {
        cout << setw(3) << i + 1 << ": ";
        if (adj[i].empty())
            cout << "-";
        for (size_t k = 0; k < adj[i].size(); k++)
            cout << adj[i][k] + 1 << " ";
        cout << endl;
    }
}

void DFS(int v)
{
    NUM[v] = true;
    cout << v + 1 << " ";
    for (int i = 0; i < size_G; i++)
        if (G[v][i] == 1 && !NUM[i])
            DFS(i);
}

void DFS_list(int v)
{
    NUM[v] = true;
    cout << v + 1 << " ";
    for (size_t k = 0; k < adj[v].size(); k++)
    {
        int u = adj[v][k];
        if (!NUM[u])
            DFS_list(u);
    }
}

void DFS_iterative(int start)
{
    stack<pair<int, int>> st;

    NUM[start] = true;
    cout << start + 1 << " ";
    st.push(make_pair(start, 0));

    while (!st.empty())
    {
        int v = st.top().first;
        int i = st.top().second;

        while (i < size_G && !(G[v][i] == 1 && !NUM[i]))
            i++;

        if (i < size_G)
        {
            int u = i;
            i++;
            st.top().second = i;
            NUM[u] = true;
            cout << u + 1 << " ";
            st.push(make_pair(u, 0));
        }
        else
        {
            st.pop();
        }
    }
}

template <typename Func>
void runTraversal(const char* title, Func dfs, int startVertex)
{
    NUM.assign(size_G, false);
    cout << title;

    auto t1 = chrono::high_resolution_clock::now();

    dfs(startVertex);
    for (int v = 0; v < size_G; v++)
        if (!NUM[v])
            dfs(v);

    auto t2 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t2 - t1).count();

    cout << endl;
    cout << "  Время выполнения: " << fixed << setprecision(4) << ms << " мс\n\n";
}

int main(int argc, char* argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    unsigned seed = (argc > 1) ? (unsigned)atoi(argv[1]) : (unsigned)time(nullptr);
    srand(seed);

    cout << "Введите количество вершин графа: ";
    if (!(cin >> size_G) || size_G <= 0)
    {
        cout << "Некорректное количество вершин" << endl;
        return 1;
    }

    double p;
    while (true)
    {
        cout << "Введите вероятность ребра (0..1): ";
        if (cin >> p && p >= 0.0 && p <= 1.0)
            break;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Некорректное значение. Введите число от 0 до 1." << endl;
    }

    generateGraph(size_G, p);
    printMatrix();
    buildAdjacencyLists();
    cout << endl;
    printAdjacencyLists();
    cout << endl;

    int startVertex;
    while (true)
    {
        cout << "Введите номер вершины, с которой начинать обход (1.."
             << size_G << "): ";
        if (cin >> startVertex && startVertex >= 1 && startVertex <= size_G)
        {
            startVertex--;
            break;
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Некорректный номер вершины." << endl;
    }
    cout << endl;

    runTraversal("Обход в глубину (матрица, рекурсия):       ", DFS, startVertex);
    runTraversal("Обход в глубину (списки смежности):         ", DFS_list, startVertex);
    runTraversal("Обход в глубину (матрица, без рекурсии):    ", DFS_iterative, startVertex);

    return 0;
}