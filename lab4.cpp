#define NOMINMAX
#include <iostream>
#include <clocale>
#include <limits>
#include <windows.h>

using namespace std;

// Структура узла бинарного дерева
struct Node
{
    int data;
    Node* left;
    Node* right;
};

// Добавление элемента в дерево
// Дубликаты отправляются в левое поддерево
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
        // Дубликат отправляем в левое поддерево
        root->left = CreateTree(root->left, data);
    }

    return root;
}

// Вывод дерева
void print_tree(Node* r, int l)
{
    if (r == NULL)
    {
        return;
    }

    // Правое поддерево
    print_tree(r->right, l + 1);

    // Отступ
    for (int i = 0; i < l; i++)
    {
        cout << "    ";
    }

    cout << r->data << endl;

    // Левое поддерево
    print_tree(r->left, l + 1);
}

// Поиск элемента
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

// Подсчёт количества вхождений элемента
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

// Удаление дерева из памяти
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

// Безопасный ввод целого числа
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

        // Если введена буква или другой неправильный символ
        cout << "Ошибка! Нужно ввести целое число: ";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
    }
}

// Основная программа
int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    Node* root = NULL;
    cout << "Введите целые числа для построения дерева." << endl;
    cout << "Для окончания ввода введите -1." << endl;
    cout << endl;

    // Построение дерева
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

    // Проверяем, пустое ли дерево
    if (root == NULL)
    {
        cout << "Дерево пустое." << endl;
        return 0;
    }

    // Вывод дерева
    cout << endl;
    cout << "           ДЕРЕВО" << endl;
    cout << "-----------------------------" << endl;
    cout << endl;

    print_tree(root, 0);

    // Главное меню
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

        // Поиск
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

        // Подсчёт вхождений
        else if (choice == 2)
        {
            cout << "Введите число: ";

            int value = InputNumber();

            int count = CountOccurrences(root, value);

            cout << "Количество вхождений элемента "
                << value << ": "
                << count << endl;
        }

        // Повторный вывод дерева
        else if (choice == 3)
        {
            cout << "Бинарное дерево:" << endl;
            cout << endl;

            print_tree(root, 0);
        }

        // Выход
        else if (choice == 4)
        {
            cout << "Программа завершена." << endl;
            break;
        }

        // Неверный пункт меню
        else
        {
            cout << "Ошибка! Такого пункта меню нет." << endl;
        }
    }

    // Освобождение памяти
    DeleteTree(root);

    return 0;
}
