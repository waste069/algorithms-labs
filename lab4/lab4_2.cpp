#define NOMINMAX

#include <iostream>
#include <clocale>
#include <limits>
#include <windows.h>
#include <vector>
#include <string>

using namespace std;

struct Node
{
    int data;
    Node* left;
    Node* right;
};

Node* CreateTree(Node* root, int data)
{
    if (root == NULL)
    {
        Node* newNode = new Node;

        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;

        return newNode;
    }

    if (data < root->data)
    {
        root->left = CreateTree(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = CreateTree(root->right, data);
    }
    else
    {
        root->left = CreateTree(root->left, data);
    }

    return root;
}

struct TreeElement
{
    int data;
    int level;
    int position;
};

void GetPositions(Node* root, int level, int& position, vector<TreeElement>& elements)
{
    if (root == NULL)
    {
        return;
    }

    GetPositions(root->left, level + 1, position, elements);

    TreeElement element;

    element.data = root->data;
    element.level = level;
    element.position = position;

    elements.push_back(element);

    position++;

    GetPositions(root->right, level + 1, position, elements);
}

void print_tree(Node* root)
{
    if (root == NULL)
    {
        return;
    }

    vector<TreeElement> elements;

    int position = 0;

    GetPositions(root, 0, position, elements);

    int maxLevel = 0;
    int maxPosition = 0;

    for (const TreeElement& element : elements)
    {
        if (element.level > maxLevel)
        {
            maxLevel = element.level;
        }

        if (element.position > maxPosition)
        {
            maxPosition = element.position;
        }
    }

    const int STEP = 6;
    const int LEFT_MARGIN = 5;

    for (int level = 0; level <= maxLevel; level++)
    {
        int currentPosition = 0;

        for (const TreeElement& element : elements)
        {
            if (element.level != level)
            {
                continue;
            }

            string text = to_string(element.data) +
                          " (" + to_string(element.level) + ")";

            int targetPosition =
                LEFT_MARGIN +
                element.position * STEP -
                static_cast<int>(text.length()) / 2;

            if (targetPosition < currentPosition)
            {
                targetPosition = currentPosition;
            }

            for (int i = currentPosition; i < targetPosition; i++)
            {
                cout << " ";
            }

            cout << text;

            currentPosition = targetPosition + text.length();
        }

        cout << endl;
    }
}

bool Search(Node* root, int value)
{
    if (root == NULL)
    {
        return false;
    }

    if (root->data == value)
    {
        return true;
    }

    if (value < root->data)
    {
        return Search(root->left, value);
    }
    else
    {
        return Search(root->right, value);
    }
}

int CountOccurrences(Node* root, int value)
{
    if (root == NULL)
    {
        return 0;
    }

    int count = 0;

    if (root->data == value)
    {
        count = 1;
    }

    count += CountOccurrences(root->left, value);
    count += CountOccurrences(root->right, value);

    return count;
}

void DeleteTree(Node* root)
{
    if (root == NULL)
    {
        return;
    }

    DeleteTree(root->left);
    DeleteTree(root->right);

    delete root;
}

int InputNumber()
{
    int number;

    while (true)
    {
        cin >> number;

        if (!cin.fail())
        {
            return number;
        }

        cout << "Ошибка! Нужно ввести целое число: ";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
    }
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    Node* root = NULL;

    cout << "Введите целые числа для построения дерева." << endl;
    cout << "Для окончания ввода введите -1." << endl;
    cout << endl;

    while (true)
    {
        cout << "Введите число: ";

        int data = InputNumber();

        if (data == -1)
        {
            break;
        }

        root = CreateTree(root, data);
    }

    cout << endl;
    cout << "Построение дерева окончено." << endl;

    if (root == NULL)
    {
        cout << "Дерево пустое." << endl;
        return 0;
    }

    cout << endl;
    cout << "           ДЕРЕВО" << endl;
    cout << "-----------------------------" << endl;
    cout << endl;

    print_tree(root);

    while (true)
    {
        cout << endl;
        cout << "            МЕНЮ" << endl;
        cout << "-----------------------------" << endl;
        cout << "1 - Поиск элемента" << endl;
        cout << "2 - Подсчёт количества вхождений" << endl;
        cout << "3 - Показать дерево" << endl;
        cout << "4 - Завершить программу" << endl;
        cout << "Выберите действие: ";

        int choice = InputNumber();

        cout << endl;

        if (choice == 1)
        {
            cout << "Введите число для поиска: ";

            int value = InputNumber();

            if (Search(root, value))
            {
                cout << "Элемент " << value
                     << " найден в дереве." << endl;
            }
            else
            {
                cout << "Элемент " << value
                     << " не найден в дереве." << endl;
            }
        }
        else if (choice == 2)
        {
            cout << "Введите число: ";

            int value = InputNumber();

            int count = CountOccurrences(root, value);

            cout << "Количество вхождений элемента "
                 << value << ": "
                 << count << endl;
        }
        else if (choice == 3)
        {
            cout << "Бинарное дерево:" << endl;
            cout << endl;

            print_tree(root);
        }
        else if (choice == 4)
        {
            cout << "Программа завершена." << endl;
            break;
        }
        else
        {
            cout << "Ошибка! Такого пункта меню нет." << endl;
        }
    }

    DeleteTree(root);

    return 0;
}