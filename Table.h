#ifndef TABLE_H
#define TABLE_H

#include <string> 
#include <mutex>
#include "CustVector.h"  

using namespace std;

// Структура, представляющая таблицу
struct Table {
    string name;  // Имя таблицы
    CustVector<string> columns;  // Вектор столбцов таблицы
    CustVector<CustVector<string>> rows;  // Вектор строк таблицы
    string primary_key;  // Имя столбца, являющегося первичным ключом
    size_t pk_sequence;  // Последовательность первичных ключей
    mutex lock;  // Мьютекс для синхронизации доступа к таблице

    // Конструктор, инициализирующий таблицу с заданным именем
    Table(const string& n);

    // Конструктор копирования
    Table(const Table& other);

    // Оператор присваивания
    Table& operator=(const Table& other);
};

#endif