#include <iostream>
#include <string> 
#include <sstream>  // для istringstream
#include "HashTable.h"
#include "Table.h"
#include "CommandParser.h"
#include "FileHandler.h"
#include "CustVector.h"

using namespace std;

// Создаем глобальную хэш-таблицу для хранения таблиц
HashTable tables(10);

// // Объявление функции trim
// string trim(const string& str);

int main() {
    // Загружаем таблицы из файла схемы
    create_tables_from_schema("schema.json");

    string temp_command;
    string command;
    while (true) {
        cout << "Enter command: ";
        getline(cin, temp_command);  // Считываем команду с консоли
        command = temp_command;
        CustVector<string> tokens = parse_command(command);  // Парсим команду на токены
        if (tokens.size == 0) continue;  // Если токенов нет, пропускаем итерацию

        // Обработка команды SELECT
        if (tokens[0] == "SELECT") {
            if (tokens.size < 4 || tokens[2] != "FROM") {
                cout << "Invalid SELECT command. Usage: SELECT ('column1', 'column2') FROM table_name1, table_name2" << endl;
                continue;
            }
            CustVector<string> columns;
            string columns_str = tokens[1];
            istringstream iss_columns(columns_str);
            string column;
            while (getline(iss_columns, column, ',')) {
                columns.push_back(trim(column));  // Добавляем столбцы в вектор
            }
            CustVector<string> table_names;
            string table_names_str = tokens[3];
            istringstream iss_tables(table_names_str);
            string table_name;
            while (getline(iss_tables, table_name, ',')) {
                table_names.push_back(trim(table_name));  // Добавляем имена таблиц в вектор
            }
            string condition = "";
            if (tokens.size > 4 && tokens[4] == "WHERE") {
                condition = tokens[5];  // Если есть условие, добавляем его
            }
            select_data(table_names, columns, condition);  // Вызываем функцию для выборки данных
        }
        // Обработка команды INSERT
        else if (tokens[0] == "INSERT") {
            if (tokens.size < 4 || tokens[1] != "INTO" || tokens[3] != "VALUES") {
                cout << "Invalid INSERT command. Usage: INSERT INTO table_name VALUES (value1, value2)" << endl;
                continue;
            }
            CustVector<string> values;
            string values_str = tokens[4];
            istringstream iss(values_str);
            string value;
            while (getline(iss, value, ',')) {
                values.push_back(trim(value));  // Добавляем значения в вектор
            }
            insert_data(tokens[2], values);  // Вызываем функцию для вставки данных
        }
        // Обработка команды DELETE
        else if (tokens[0] == "DELETE") {
            if (tokens.size < 4 || tokens[1] != "FROM" || tokens[3] != "WHERE") {
                cout << "Invalid DELETE command. Usage: DELETE FROM table_name WHERE column = value" << endl;
                continue;
            }
            delete_data(tokens[2], tokens[4]);  // Вызываем функцию для удаления данных
        }
        // Обработка команды LOAD
        else if (tokens[0] == "LOAD") {
            if (tokens.size == 3 && tokens[1] == "TABLE") {
                load_table_json(tokens[2]);  // Загружаем таблицу из JSON файла
            }
            else if (tokens.size == 3 && tokens[1] == "CSV") {
                load_table_csv(tokens[2]);  // Загружаем таблицу из CSV файла
            }
            else {
                cout << "Invalid LOAD command. Usage: LOAD TABLE table_name or LOAD CSV table_name" << endl;
            }
        }
        // Обработка команды CREATE
        else if (tokens[0] == "CREATE") {
            if (tokens.size < 6 || tokens[1] != "TABLE" || tokens[3] != "(" || tokens[tokens.size - 2] != ")") {
                cout << "Invalid CREATE TABLE command. Usage: CREATE TABLE table_name (column1, column2) PRIMARY KEY (primary_key)" << endl;
                continue;
            }
            CustVector<string> columns;
            string columns_str = tokens[3];
            istringstream iss(columns_str);
            string column;
            while (getline(iss, column, ',')) {
                columns.push_back(trim(column));  // Добавляем столбцы в вектор
            }
            create_table(tokens[2], columns, tokens[tokens.size - 1]);  // Вызываем функцию для создания таблицы
        }
        // Обработка команды SAVE
        else if (tokens[0] == "SAVE") {
            if (tokens.size != 3 || tokens[1] != "TABLE") {
                cout << "Invalid SAVE command. Usage: SAVE TABLE table_name" << endl;
                continue;
            }
            Table* table = reinterpret_cast<Table*>(tables.get(tokens[2]));  // Получаем указатель на таблицу
            if (!table) {
                cout << "Table not found." << endl;
                continue;
            }
            save_table_csv(*table);  // Сохраняем таблицу в CSV файл
        }
        // Обработка команды EXIT
        else if (tokens[0] == "EXIT") {
            break;  // Выходим из цикла
        }
        // Обработка неизвестной команды
        else {
            cout << "Unknown command." << endl;
        }
    }

    return 0;
}

// // Определение функции trim
// string trim(const string& str) {
//     size_t first = str.find_first_not_of(' ');
//     if (string::npos == first) {
//         return str;
//     }
//     size_t last = str.find_last_not_of(' ');
//     return str.substr(first, last - first + 1);
// }