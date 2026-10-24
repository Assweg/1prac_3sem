#include "HashTable.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Конструктор, с заданной начальной вместимостью
HashTable::HashTable(int initialCapacity) {
    capacity = initialCapacity;             // Устанавливаем вместимость
    table = new HashNode * [capacity]();    // Выделяем память под массив указателей на узлы
}

// Деструктор
HashTable::~HashTable() {
    for (int i = 0; i < capacity; i++) {    // Проходим по всем элементам массива
        HashNode* entry = table[i];         // Получаем указатель на текущий узел
        while (entry) {                     // Пока есть узлы в цепочке
            HashNode* prev = entry;         // Запоминаем текущий узел
            entry = entry->next;            // Переходим к следующему узлу
            delete prev; 
        }
        table[i] = nullptr;                 // Обнуляем указатель в массиве
    }
    delete[] table;
}

// Метод для вычисления хэш-функции
int HashTable::hash(const string& key) const {
    int hash = 0;           // Инициализируем хэш-код нулем
    for (char ch : key) {   // Проходим по всем символам ключа
        hash += ch;         // Добавляем код символа к хэш-коду
    }
    return hash % capacity; // Возвращаем хэш-код по модулю вместимости
}

// Метод для вставки пары ключ-значение в хэш-таблицу
void HashTable::insert(const string& key, void* value) {
    int index = hash(key);                // Вычисляем индекс в массиве
    HashNode* prev = nullptr;             // Указатель на предыдущий узел
    HashNode* entry = table[index];       // Указатель на текущий узел
    while (entry && entry->key != key) {  // Ищем узел с заданным ключом
        prev = entry;                     // Переходим к следующему узлу
        entry = entry->next;
    }
    if (!entry) {  // Если узел не найден
        entry = new HashNode(key, value); // Создаем новый узел
        if (prev) {  // Если есть предыдущий узел
            prev->next = entry;           // Вставляем новый узел после предыдущего
        }
        else {
            table[index] = entry;         // Вставляем новый узел в начало цепочки
        }
    }
    else {   // Если узел найден
        entry->value = value;             // Обновляем значение узла
    }
}

// Метод для вставки пары ключ-значение в хэш-таблицу
void HashTable::put(const string& key, void* value) {
    insert(key, value);  // Вызываем метод insert
}

// Метод для получения значения по ключу
void* HashTable::get(const string& key) const {
    int index = hash(key);           // Вычисляем индекс в массиве
    HashNode* entry = table[index];  // Указатель на текущий узел
    while (entry) {  // Ищем узел с заданным ключом
        if (entry->key == key) {     // Если ключ совпадает
            return entry->value;     // Возвращаем значение
        }
        entry = entry->next;         // Переходим к следующему узлу
    }
    cout << "Key not found!\n";
    return nullptr;
}

// Метод для удаления пары ключ-значение из хэш-таблицы
void HashTable::remove(const string& key) {
    int index = hash(key);  // Вычисляем индекс в массиве
    HashNode* prev = nullptr;  // Указатель на предыдущий узел
    HashNode* entry = table[index];  // Указатель на текущий узел
    while (entry && entry->key != key) {  // Ищем узел с заданным ключом
        prev = entry;  // Переходим к следующему узлу
        entry = entry->next;
    }
    if (!entry) {  // Если узел не найден
        cout << "Key not found!\n"; 
        return;
    }
    if (prev) {  // Если есть предыдущий узел
        prev->next = entry->next;  // Удаляем текущий узел из цепочки
    }
    else {
        table[index] = entry->next;  // Удаляем первый узел в цепочке
    }
    delete entry;  // Освобождаем память, выделенную под узел
}

// Метод для печати содержимого хэш-таблицы
void HashTable::print() const {
    for (int i = 0; i < capacity; i++) {  // Проходим по всем элементам массива
        HashNode* entry = table[i];       // Получаем указатель на текущий узел
        while (entry) {  // Пока есть узлы в цепочке
            cout << "Key: " << entry->key << ", Value: " << entry->value << "\n";  // Выводим ключ и значение
            entry = entry->next;          // Переходим к следующему узлу
        }
    }
}

// Метод для сохранения содержимого хэш-таблицы в файл
void HashTable::saveToFile(const string& filename) const {
    ofstream file(filename);  // Открываем файл для записи
    if (!file.is_open()) {    // Проверяем, удалось ли открыть файл
        cout << "Failed to open file for writing!\n";
        return;
    }
    file << "HashTable\n";  // Записываем заголовок файла
    for (int i = 0; i < capacity; i++) {  // Проходим по всем элементам массива
        HashNode* entry = table[i];       // Получаем указатель на текущий узел
        while (entry) {  // Пока есть узлы в цепочке
            file << entry->key << "," << reinterpret_cast<uintptr_t>(entry->value) << "\n";  // Записываем ключ и значение
            entry = entry->next;          // Переходим к следующему узлу
        }
    }
    file.close();
}

// Метод для загрузки содержимого хэш-таблицы из файла
void HashTable::loadFromFile(const string& filename) {
    ifstream file(filename);  // Открываем файл для чтения
    if (!file.is_open()) {    // Проверяем, удалось ли открыть файл
        cout << "Failed to open file for reading!\n";  // Выводим сообщение об ошибке
        return;
    }
    string header;
    getline(file, header);  // Читаем заголовок файла
    if (header != "HashTable") {  // Проверяем, соответствует ли заголовок ожидаемому
        cout << "Invalid file format!\n";  // Выводим сообщение об ошибке
        return;
    }
    string line;
    while (getline(file, line)) {  // Читаем строки из файла
        stringstream ss(line);     // Создаем поток для обработки строки
        string key, value;
        getline(ss, key, ',');     // Извлекаем ключ
        getline(ss, value, ',');   // Извлекаем значение
        uintptr_t ptrValue = stoull(value);  // Преобразуем строку в число
        insert(key, reinterpret_cast<void*>(ptrValue));  // Вставляем пару ключ-значение в хэш-таблицу
    }
    file.close();
}