#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>

using namespace std;


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

struct PriorityNode
{
    wstring inf;
    int priority;
    PriorityNode* next;
};

PriorityNode* priorityHead = nullptr;


PriorityNode* getPriorityStruct()
{
    PriorityNode* p = new PriorityNode;

    wcout << L"Введите название объекта: ";
    getline(wcin, p->inf);

    p->priority = readInt(L"Введите приоритет объекта: ");

    p->next = nullptr;

    return p;
}


void addPriority()
{
    PriorityNode* p = getPriorityStruct();

    if (priorityHead == nullptr)
    {
        priorityHead = p;
        return;
    }

    if (p->priority > priorityHead->priority)
    {
        p->next = priorityHead;
        priorityHead = p;
        return;
    }

    PriorityNode* current = priorityHead;

    while (current->next != nullptr &&
        current->next->priority >= p->priority)
    {
        current = current->next;
    }

    p->next = current->next;
    current->next = p;
}


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


void clearPriority()
{
    while (priorityHead != nullptr)
    {
        PriorityNode* temp = priorityHead;

        priorityHead = priorityHead->next;

        delete temp;
    }
}


struct QueueNode
{
    wstring inf;
    QueueNode* next;
};

QueueNode* queueHead = nullptr;
QueueNode* queueLast = nullptr;


QueueNode* getQueueStruct()
{
    QueueNode* p = new QueueNode;

    wcout << L"Введите название объекта: ";
    getline(wcin, p->inf);

    p->next = nullptr;

    return p;
}


void enqueue()
{
    QueueNode* p = getQueueStruct();

    // Если очередь пустая
    if (queueHead == nullptr)
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


struct StackNode
{
    wstring inf;
    StackNode* next;
};

StackNode* stackHead = nullptr;

StackNode* getStackStruct()
{
    StackNode* p = new StackNode;

    wcout << L"Введите название объекта: ";
    getline(wcin, p->inf);

    p->next = nullptr;

    return p;
}

void push()
{
    StackNode* p = getStackStruct();

    p->next = stackHead;

    stackHead = p;
}


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

void clearStack()
{
    while (stackHead != nullptr)
    {
        StackNode* temp = stackHead;

        stackHead = stackHead->next;

        delete temp;
    }
}


void priorityMenu()
{
    int choice;

    do
    {
        wcout << L"\n      ПРИОРИТЕТНАЯ ОЧЕРЕДЬ\n";
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

void queueMenu()
{
    int choice;

    do
    {
        wcout << L"\n      ОБЫЧНАЯ ОЧЕРЕДЬ\n";
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


void stackMenu()
{
    int choice;

    do
    {
        wcout << L"\n       СТЕК\n";
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


int main()
{
    // Настройка консоли Windows для нормальной работы
    // с русскими символами
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    int choice;

    do
    {
        wcout << L"       ДИНАМИЧЕСКИЕ СПИСКИ\n";
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

            wcout << L"\nПрограмма завершена.\n";
            break;

        default:
            wcout << L"Неверный выбор.\n";
        }

    } while (choice != 0);

    return 0;
}