#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>

using namespace std;

// Структура, представляющая узел хэш-таблицы
struct HashNode {
    string key;          // Ключ узла
    void* value;         // Значение узла
    HashNode* next;      // Указатель на следующий узел в цепочке (для разрешения коллизий)

    // Конструктор для инициализации узла
    HashNode(const string& k, void* v) : key(k), value(v), next(nullptr) {}
};

// Класс, представляющий хэш-таблицу
class HashTable {
private:
    HashNode** table;   // Массив указателей на узлы хэш-таблицы
    int capacity;       // Вместимость хэш-таблицы

    // Метод для вычисления хэш-функции
    int hash(const string& key) const;

public:
    // Конструктор, с заданной начальной вместимостью
    HashTable(int initialCapacity);

    // Деструктор
    ~HashTable();

    // Метод для вставки пары ключ-значение
    void insert(const string& key, void* value);

    // Метод для вставки пары ключ-значение
    void put(const string& key, void* value);

    // Метод для получения значения по ключу
    void* get(const string& key) const;

    // Метод для удаления пары ключ-значение
    void remove(const string& key);

    // Метод для печати содержимого
    void print() const;

    // Метод для сохранения хэш-таблицы в файл
    void saveToFile(const string& filename) const;

    // Метод для загрузки хэш-таблицы из файла
    void loadFromFile(const string& filename);
};

#endif