#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include "Table.h"

using namespace std;

// Функция для удаления пробелов в начале и конце строки
string trim(const string& str);

// Функция для сохранения таблицы в формате JSON
void save_table_json(const Table& table);

// Функция для загрузки таблицы из файла JSON
void load_table_json(const string& table_name);

// Функция для загрузки таблицы из файла CSV
void load_table_csv(const string& table_name);

// Функция для сохранения таблицы в формате CSV
void save_table_csv(const Table& table);

// Функция для сохранения последовательности первичных ключей
void save_pk_sequence(const Table& table);

// Функция для загрузки последовательности первичных ключей
void load_pk_sequence(Table& table);

// Функция для сохранения состояния блокировки таблицы
void save_lock_state(const Table& table);

// Функция для загрузки состояния блокировки таблицы
void load_lock_state(Table& table);

// Функция для создания новой таблицы
void create_table(const string& table_name, const CustVector<string>& columns, const string& primary_key);

// Функция для вставки данных в таблицу
void insert_data(const string& table_name, const CustVector<string>& values);

// Функция для выборки данных из таблицы
void select_data(const CustVector<string>& table_names, const CustVector<string>& columns, const string& condition = "");

// Функция для удаления данных из таблицы
void delete_data(const string& table_name, const string& condition);

// Функция для создания таблиц из файла схемы
void create_tables_from_schema(const string& schema_file);

#endif