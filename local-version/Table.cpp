#include "Table.h"

using namespace std;

// Конструктор, инициализирующий таблицу с заданным именем
Table::Table(const string& n) : name(n), pk_sequence(0) {}

// Конструктор копирования
Table::Table(const Table& other)
    : name(other.name), columns(other.columns), rows(other.rows), primary_key(other.primary_key), pk_sequence(other.pk_sequence) {}

// Оператор присваивания
Table& Table::operator=(const Table& other) {
    if (this != &other) {  // Проверка на самоприсваивание
        name = other.name;  // Копируем имя таблицы
        columns = other.columns;  // Копируем столбцы таблицы
        rows = other.rows;  // Копируем строки таблицы
        primary_key = other.primary_key;  // Копируем первичный ключ
        pk_sequence = other.pk_sequence;  // Копируем последовательность первичных ключей
    }
    return *this;  // Возвращаем текущий объект
}
