#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>

using namespace std;

// ============================================================
// ФУНКЦИЯ БЕЗОПАСНОГО ВВОДА ЦЕЛОГО ЧИСЛА
// ============================================================

int readInt(const wstring& message)
{
    int value;
    wstring input;

    while (true)
    {
        wcout << message;
        getline(wcin, input);

        try
        {
            size_t pos;
            value = stoi(input, &pos);

            // Проверяем, что введено только число
            if (pos != input.length())
            {
                throw invalid_argument("extra characters");
            }

            return value;
        }
        catch (...)
        {
            wcout << L"\n[Ошибка] Введите целое число.\n";
            wcout << L"Попробуйте ещё раз.\n\n";
        }
    }
}

// ============================================================
// ПУНКТ 1. ПРИОРИТЕТНАЯ ОЧЕРЕДЬ
// ============================================================

struct PriorityNode
{
    wstring inf;
    int priority;
    PriorityNode* next;
};

PriorityNode* priorityHead = nullptr;

// Создание элемента приоритетной очереди
PriorityNode* getPriorityStruct()
{
    PriorityNode* p = new PriorityNode;

    wcout << L"Введите название объекта: ";
    getline(wcin, p->inf);

    p->priority = readInt(L"Введите приоритет объекта: ");

    p->next = nullptr;

    return p;
}

// Добавление элемента в соответствии с приоритетом
void addPriority()
{
    PriorityNode* p = getPriorityStruct();

    // Если очередь пустая
    if (priorityHead == nullptr)
    {
        priorityHead = p;
        return;
    }

    // Если новый элемент имеет самый высокий приоритет
    if (p->priority > priorityHead->priority)
    {
        p->next = priorityHead;
        priorityHead = p;
        return;
    }

    // Поиск места для вставки
    PriorityNode* current = priorityHead;

    while (current->next != nullptr &&
        current->next->priority >= p->priority)
    {
        current = current->next;
    }

    p->next = current->next;
    current->next = p;
}

// Просмотр приоритетной очереди
void reviewPriority()
{
    if (priorityHead == nullptr)
    {
        wcout << L"Приоритетная очередь пуста.\n";
        return;
    }

    PriorityNode* current = priorityHead;

    wcout << L"\nПриоритетная очередь:\n";

    while (current != nullptr)
    {
        wcout << L"Объект: " << current->inf
            << L", приоритет: " << current->priority << endl;

        current = current->next;
    }
}

// Удаление элемента с наивысшим приоритетом
void removePriority()
{
    if (priorityHead == nullptr)
    {
        wcout << L"Приоритетная очередь пуста.\n";
        return;
    }

    PriorityNode* temp = priorityHead;

    priorityHead = priorityHead->next;

    wcout << L"Удален объект: " << temp->inf << endl;

    delete temp;
}

// Поиск элемента в приоритетной очереди
PriorityNode* findPriority(const wstring& name)
{
    PriorityNode* current = priorityHead;

    while (current != nullptr)
    {
        if (current->inf == name)
        {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

// Очистка приоритетной очереди
void clearPriority()
{
    while (priorityHead != nullptr)
    {
        PriorityNode* temp = priorityHead;

        priorityHead = priorityHead->next;

        delete temp;
    }
}

// ============================================================
// ПУНКТ 2. ОБЫЧНАЯ ОЧЕРЕДЬ
// ============================================================

struct QueueNode
{
    wstring inf;
    QueueNode* next;
};

QueueNode* queueHead = nullptr;
QueueNode* queueLast = nullptr;

// Создание элемента очереди
QueueNode* getQueueStruct()
{
    QueueNode* p = new QueueNode;

    wcout << L"Введите название объекта: ";
    getline(wcin, p->inf);

    p->next = nullptr;

    return p;
}

// Добавление элемента в конец очереди
void enqueue()
{
    QueueNode* p = getQueueStruct();

    // Если очередь пустая
    if (queueHead == nullptr)


wcout << L"Название: " << result->inf << endl;
                wcout << L"Приоритет: " << result->priority << endl;
            }
            else
            {
                wcout << L"\nЭлемент не найден.\n";
            }

            break;
        }

        case 0:
            break;

        default:
            wcout << L"Неверный выбор.\n";
        }

    } while (choice != 0);
}

// ============================================================
// МЕНЮ ОБЫЧНОЙ ОЧЕРЕДИ
// ============================================================

void queueMenu()
{
    int choice;

    do
    {
        wcout << L"\n===== ОБЫЧНАЯ ОЧЕРЕДЬ =====\n";
        wcout << L"1. Добавить элемент\n";
        wcout << L"2. Просмотреть очередь\n";
        wcout << L"3. Удалить элемент\n";
        wcout << L"4. Найти элемент\n";
        wcout << L"0. Назад\n";

        choice = readInt(L"Выберите действие: ");

        switch (choice)
        {
        case 1:
            enqueue();
            break;

        case 2:
            reviewQueue();
            break;

        case 3:
            dequeue();
            break;

        case 4:
        {
            wstring name;

            wcout << L"Введите название объекта: ";
            getline(wcin, name);

            QueueNode* result = findQueue(name);

            if (result != nullptr)
            {
                wcout << L"Элемент найден: "
                    << result->inf << endl;
            }
            else
            {
                wcout << L"Элемент не найден.\n";
            }

            break;
        }

        case 0:
            break;

        default:
            wcout << L"Неверный выбор.\n";
        }

    } while (choice != 0);
}

// ============================================================
// МЕНЮ СТЕКА
// ============================================================

void stackMenu()
{
    int choice;

    do
    {
        wcout << L"\n===== СТЕК =====\n";
        wcout << L"1. Добавить элемент (push)\n";
        wcout << L"2. Просмотреть стек\n";
        wcout << L"3. Удалить элемент (pop)\n";
        wcout << L"4. Найти элемент\n";
        wcout << L"0. Назад\n";

        choice = readInt(L"Выберите действие: ");

        switch (choice)
        {
        case 1:
            push();
            break;

        case 2:
            reviewStack();
            break;

        case 3:
            pop();
            break;

        case 4:
        {
            wstring name;

            wcout << L"Введите название объекта: ";
            getline(wcin, name);

            StackNode* result = findStack(name);

            if (result != nullptr)
            {
                wcout << L"Элемент найден: "
                    << result->inf << endl;
            }
            else
            {
                wcout << L"Элемент не найден.\n";
            }

            break;
        }

        case 0:
            break;

        default:
            wcout << L"Неверный выбор.\n";
        }

    } while (choice != 0);
}

// ============================================================
// ГЛАВНОЕ МЕНЮ
// ============================================================

int main()
{
    // Настройка консоли Windows для нормальной работы
    // с русскими символами
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    int choice;

    do
    {
        wcout << L"\n====================================\n";
        wcout << L"       ДИНАМИЧЕСКИЕ СПИСКИ\n";
        wcout << L"====================================\n";
        wcout << L"1. Приоритетная очередь\n";
        wcout << L"2. Обычная очередь\n";
        wcout << L"3. Стек\n";
        wcout << L"0. Выход\n";

        choice = readInt(L"Выберите пункт: ");

        switch (choice)
        {
        case 1:
            priorityMenu();
            break;

        case 2:
            queueMenu();
            break;

        case 3:
            stackMenu();
            break;

        case 0:
            clearPriority();
            clearQueue();
            clearStack();


{
        queueHead = p;
        queueLast = p;
    }
    else
    {
        queueLast->next = p;
        queueLast = p;
    }
}

// Просмотр очереди
void reviewQueue()
{
    if (queueHead == nullptr)
    {
        wcout << L"Очередь пуста.\n";
        return;
    }

    QueueNode* current = queueHead;

    wcout << L"\nОчередь:\n";

    while (current != nullptr)
    {
        wcout << L"Объект: " << current->inf << endl;

        current = current->next;
    }
}

// Удаление первого элемента очереди
void dequeue()
{
    if (queueHead == nullptr)
    {
        wcout << L"Очередь пуста.\n";
        return;
    }

    QueueNode* temp = queueHead;

    queueHead = queueHead->next;

    wcout << L"Удален объект: " << temp->inf << endl;

    delete temp;

    if (queueHead == nullptr)
    {
        queueLast = nullptr;
    }
}

// Поиск элемента в очереди
QueueNode* findQueue(const wstring& name)
{
    QueueNode* current = queueHead;

    while (current != nullptr)
    {
        if (current->inf == name)
        {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

// Очистка очереди
void clearQueue()
{
    while (queueHead != nullptr)
    {
        QueueNode* temp = queueHead;

        queueHead = queueHead->next;

        delete temp;
    }

    queueLast = nullptr;
}

// ============================================================
// ПУНКТ 3. СТЕК
// ============================================================

struct StackNode
{
    wstring inf;
    StackNode* next;
};

StackNode* stackHead = nullptr;

// Создание элемента стека
StackNode* getStackStruct()
{
    StackNode* p = new StackNode;

    wcout << L"Введите название объекта: ";
    getline(wcin, p->inf);

    p->next = nullptr;

    return p;
}

// Добавление элемента в стек
void push()
{
    StackNode* p = getStackStruct();

    // Новый элемент становится первым
    p->next = stackHead;

    stackHead = p;
}

// Просмотр стека
void reviewStack()
{
    if (stackHead == nullptr)
    {
        wcout << L"Стек пуст.\n";
        return;
    }

    StackNode* current = stackHead;

    wcout << L"\nСтек:\n";

    while (current != nullptr)
    {
        wcout << L"Объект: " << current->inf << endl;

        current = current->next;
    }
}

// Удаление верхнего элемента стека
void pop()
{
    if (stackHead == nullptr)
    {
        wcout << L"Стек пуст.\n";
        return;
    }

    StackNode* temp = stackHead;

    stackHead = stackHead->next;

    wcout << L"Удален объект: " << temp->inf << endl;

    delete temp;
}

// Поиск элемента в стеке
StackNode* findStack(const wstring& name)
{
    StackNode* current = stackHead;

    while (current != nullptr)
    {
        if (current->inf == name)
        {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

// Очистка стека
void clearStack()
{
    while (stackHead != nullptr)
    {
        StackNode* temp = stackHead;

        stackHead = stackHead->next;

        delete temp;
    }
}

// ============================================================
// МЕНЮ ПРИОРИТЕТНОЙ ОЧЕРЕДИ
// ============================================================

void priorityMenu()
{
    int choice;

    do
    {
        wcout << L"\n===== ПРИОРИТЕТНАЯ ОЧЕРЕДЬ =====\n";
        wcout << L"1. Добавить элемент\n";
        wcout << L"2. Просмотреть очередь\n";
        wcout << L"3. Удалить элемент\n";
        wcout << L"4. Найти элемент\n";
        wcout << L"0. Назад\n";

        choice = readInt(L"Выберите действие: ");

        switch (choice)
        {
        case 1:
            addPriority();
            break;

        case 2:
            reviewPriority();
            break;

        case 3:
            removePriority();
            break;

        case 4:
        {
            wstring name;

            wcout << L"Введите название объекта: ";
            getline(wcin, name);

            PriorityNode* result = findPriority(name);

            if (result != nullptr)
            {
                wcout << L"\nЭлемент найден.\n";


wcout << L"\nПрограмма завершена.\n";
            break;

        default:
            wcout << L"Неверный выбор.\n";
        }

    } while (choice != 0);

    return 0;
}