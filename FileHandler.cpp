#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "HashTable.h"
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

extern HashTable tables;  // Объявление глобальной хеш-таблицы для хранения таблиц

// Функция для удаления пробелов в начале и конце строки
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Функция для сохранения таблицы в формате JSON
void save_table_json(const Table& table) {
    json j;
    j["name"] = table.name;         // Имя таблицы
    j["columns"] = json::array();   // Создание массива для столбцов
    for (size_t i = 0; i < table.columns.size; ++i) {
        j["columns"].push_back(table.columns[i]);  // Добавление столбцов в массив
    }
    j["rows"] = json::array();  // Создание массива для строк
    for (size_t i = 0; i < table.rows.size; ++i) {
        json row = json::array();
        for (size_t j = 0; j < table.rows[i].size; ++j) {
            row.push_back(table.rows[i][j]);  // Добавление значений строки в массив
        }
        j["rows"].push_back(row);          // Добавление строки в массив строк
    }
    j["primary_key"] = table.primary_key;  // Первичный ключ таблицы

    ofstream file(table.name + ".json");   // Открытие файла для записи
    file << j.dump(4);                     // Запись JSON в файл с отступами
}

// Функция для загрузки таблицы из JSON файла
void load_table_json(const string& table_name) {
    ifstream file(table_name + ".json");  // Открытие файла для чтения
    if (!file.is_open()) {
        cout << "File not found." << endl;
        return;
    }
    json j;
    file >> j;   // Чтение JSON из файла

    Table table(j["name"]);  // Создание таблицы с именем из JSON
    for (const auto& col : j["columns"]) {
        table.columns.push_back(col);  // Добавление столбцов в таблицу
    }
    for (const auto& row : j["rows"]) {
        CustVector<string> row_data;
        for (const auto& val : row) {
            row_data.push_back(val);  // Добавление значений строки в таблицу
        }
        table.rows.push_back(row_data);  // Добавление строки в таблицу
    }
    table.primary_key = j["primary_key"];  // Установка первичного ключа

    tables.put(table_name, reinterpret_cast<void*>(new Table(table)));  // Добавление таблицы в хеш-таблицу
}

// Функция для загрузки таблицы из CSV файла
void load_table_csv(const string& table_name) {
    string file_path = table_name + ".csv";
    cout << "Trying to open file: " << file_path << endl;

    ifstream file(file_path);  // Открытие файла для чтения
    if (!file.is_open()) {
        cout << "File not found." << endl;
        return;
    }

    Table table(table_name);  // Создание таблицы с указанным именем
    string line;
    bool first_line = true;

    while (getline(file, line)) {
        istringstream iss(line);
        string value;
        CustVector<string> row;

        while (getline(iss, value, ',')) {
            if (value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);  // Удаление кавычек вокруг значений
            }
            row.push_back(value);  // Добавление значения в строку
        }

        if (first_line) {
            table.columns = row;  // Первая строка содержит имена столбцов
            first_line = false;
        }
        else {
            table.rows.push_back(row);  // Остальные строки содержат данные
        }
    }

    for (size_t i = 0; i < table.rows.size; ++i) {
        table.rows[i][0] = to_string(i);  // Установка первичного ключа
    }

    tables.put(table_name, reinterpret_cast<void*>(new Table(table)));  // Добавление таблицы в хеш-таблицу
    save_table_json(table);  // Сохранение таблицы в JSON
    cout << "Table loaded from " << table_name << ".csv and saved to " << table_name << ".json" << endl;
}

// Функция для сохранения таблицы в CSV файл
void save_table_csv(const Table& table) {
    ofstream file(table.name + ".csv");  // Открытие файла для записи
    if (!file.is_open()) {
        cout << "Failed to open file for writing." << endl;
        return;
    }

    for (size_t i = 0; i < table.columns.size; ++i) {
        file << "\"" << table.columns[i] << "\"";  // Запись имен столбцов
        if (i < table.columns.size - 1) {
            file << ",";
        }
    }
    file << endl;

    for (size_t i = 0; i < table.rows.size; ++i) {
        for (size_t j = 0; j < table.rows[i].size; ++j) {
            file << "\"" << table.rows[i][j] << "\"";  // Запись значений строк
            if (j < table.rows[i].size - 1) {
                file << ",";
            }
        }
        file << endl;
    }

    cout << "Table saved to " << table.name << ".csv" << endl;
}

// Функция для сохранения последовательности первичных ключей
void save_pk_sequence(const Table& table) {
    ofstream file(table.name + "_pk_sequence.txt");  // Открытие файла для записи
    if (!file.is_open()) {
        cout << "Failed to open file for writing pk_sequence." << endl;
        return;
    }
    file << table.pk_sequence;  // Запись последовательности первичных ключей
    cout << "Primary key sequence saved to " << table.name << "_pk_sequence.txt" << endl;
}

// Функция для загрузки последовательности первичных ключей
void load_pk_sequence(Table& table) {
    ifstream file(table.name + "_pk_sequence.txt");  // Открытие файла для чтения
    if (!file.is_open()) {
        cout << "File not found for pk_sequence." << endl;
        return;
    }
    file >> table.pk_sequence;  // Чтение последовательности первичных ключей
    cout << "Primary key sequence loaded from " << table.name << "_pk_sequence.txt" << endl;
}

// Функция для сохранения состояния блокировки таблицы
void save_lock_state(const Table& table) {
    ofstream file(table.name + "_lock.txt");  // Открытие файла для записи
    if (!file.is_open()) {
        cout << "Failed to open file for writing lock state." << endl;
        return;
    }
    file << "locked";  // Запись состояния блокировки
    cout << "Lock state saved to " << table.name << "_lock.txt" << endl;
}

// Функция для загрузки состояния блокировки таблицы
void load_lock_state(Table& table) {
    ifstream file(table.name + "_lock.txt");  // Открытие файла для чтения
    if (!file.is_open()) {
        cout << "File not found for lock state." << endl;
        return;
    }
    string state;
    file >> state;  // Чтение состояния блокировки
    if (state == "locked") {
        cout << "Lock state loaded from " << table.name << "_lock.txt" << endl;
    }
}

// Функция для создания новой таблицы
void create_table(const string& table_name, const CustVector<string>& columns, const string& primary_key) {
    Table* existing_table = reinterpret_cast<Table*>(tables.get(table_name));  // Проверка наличия таблицы с таким именем
    if (existing_table) {
        cout << "Table already exists." << endl;
        return;
    }

    Table new_table(table_name);  // Создание новой таблицы
    new_table.columns.push_back(primary_key);  // Добавление первичного ключа в столбцы
    for (size_t i = 0; i < columns.size; ++i) {
        new_table.columns.push_back(columns[i]);  // Добавление остальных столбцов
    }
    new_table.primary_key = primary_key;  // Установка первичного ключа

    tables.put(table_name, reinterpret_cast<void*>(new Table(new_table)));  // Добавление таблицы в хеш-таблицу
    save_table_json(new_table);  // Сохранение таблицы в JSON
    save_pk_sequence(new_table);  // Сохранение последовательности первичных ключей
    save_lock_state(new_table);  // Сохранение состояния блокировки
    cout << "Table created successfully." << endl;
}

// Функция для вставки данных в таблицу
void insert_data(const string& table_name, const CustVector<string>& values) {
    Table* table = reinterpret_cast<Table*>(tables.get(table_name));  // Получение таблицы по имени
    if (!table) {
        cout << "Table not found." << endl;
        return;
    }
    lock_guard<mutex> guard(table->lock);  // Блокировка таблицы для безопасной вставки

    if (values.size != table->columns.size - 1) {
        cout << "Invalid number of values." << endl;
        return;
    }

    size_t last_pk = 0;
    if (table->rows.size > 0) {
        last_pk = stoi(table->rows[table->rows.size - 1][0]);  // Получение последнего первичного ключа
    }
    string pk_value = to_string(last_pk + 1);  // Генерация нового первичного ключа

    CustVector<string> new_row;
    new_row.push_back(pk_value);  // Добавление первичного ключа в новую строку
    for (size_t i = 0; i < values.size; ++i) {
        string value = values[i];
        if (value.front() == '(') value = value.substr(1);  // Удаление скобок вокруг значений
        if (value.back() == ')') value = value.substr(0, value.size() - 1);
        new_row.push_back(value);  // Добавление значений в новую строку
    }

    table->rows.push_back(new_row);  // Добавление новой строки в таблицу
    save_table_json(*table);         // Сохранение таблицы в JSON
    save_pk_sequence(*table);        // Сохранение последовательности первичных ключей
    cout << "Data inserted successfully." << endl;
}

// Функция для выборки данных из таблицы
void select_data(const CustVector<string>& table_names, const CustVector<string>& columns, const string& condition) {
    if (table_names.size == 0) {
        cout << "No tables specified." << endl;
        return;
    }

    Table* first_table = reinterpret_cast<Table*>(tables.get(table_names[0]));  // Получение первой таблицы
    if (!first_table) {
        cout << "Table not found: " << table_names[0] << endl;
        return;
    }

    CustVector<string> selected_columns = columns;
    if (columns.size == 1 && columns[0] == "*") {
        selected_columns = first_table->columns;  // Если выбраны все столбцы, используем все столбцы первой таблицы
    }

    for (size_t i = 0; i < selected_columns.size; ++i) {
        bool found = false;
        for (size_t j = 0; j < table_names.size; ++j) {
            Table* table = reinterpret_cast<Table*>(tables.get(table_names[j]));  // Получение таблицы по имени
            if (!table) {
                cout << "Table not found: " << table_names[j] << endl;
                return;
            }
            for (size_t k = 0; k < table->columns.size; ++k) {
                if (table->columns[k] == selected_columns[i]) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (!found) {
            cout << "Column not found: " << selected_columns[i] << endl;
            return;
        }
    }

    for (size_t i = 0; i < first_table->rows.size; ++i) {
        bool match = true;
        if (!condition.empty()) {
            istringstream iss(condition);
            string col, op, val;
            iss >> col >> op >> val;  // Разбор условия

            if (val.front() == '(') val = val.substr(1);  // Удаление скобок вокруг значения
            if (val.back() == ')') val = val.substr(0, val.size() - 1);

            for (size_t j = 0; j < first_table->columns.size; ++j) {
                if (first_table->columns[j] == col) {
                    if (op == "=" && first_table->rows[i][j] != val) match = false;  // Проверка условия
                    if (op == "!=" && first_table->rows[i][j] == val) match = false;
                    break;
                }
            }
        }
        if (match) {
            for (size_t j = 0; j < selected_columns.size; ++j) {
                for (size_t k = 0; k < first_table->columns.size; ++k) {
                    if (first_table->columns[k] == selected_columns[j]) {
                        cout << first_table->rows[i][k] << " ";  // Вывод соответствующих значений
                        break;
                    }
                }
            }
            cout << endl;
        }
    }

    if (table_names.size > 1) {
        Table* second_table = reinterpret_cast<Table*>(tables.get(table_names[1]));  // Получение второй таблицы
        if (!second_table) {
            cout << "Table not found: " << table_names[1] << endl;
            return;
        }

        for (size_t i = 0; i < first_table->rows.size; ++i) {
            for (size_t j = 0; j < second_table->rows.size; ++j) {
                bool match = true;
                if (!condition.empty()) {
                    istringstream iss(condition);
                    string col, op, val;
                    iss >> col >> op >> val;  // Разбор условия

                    if (val.front() == '(') val = val.substr(1);  // Удаление скобок вокруг значения
                    if (val.back() == ')') val = val.substr(0, val.size() - 1);

                    for (size_t k = 0; k < first_table->columns.size; ++k) {
                        if (first_table->columns[k] == col) {
                            if (op == "=" && first_table->rows[i][k] != val) match = false;  // Проверка условия
                            if (op == "!=" && first_table->rows[i][k] == val) match = false;
                            break;
                        }
                    }
                    for (size_t k = 0; k < second_table->columns.size; ++k) {
                        if (second_table->columns[k] == col) {
                            if (op == "=" && second_table->rows[j][k] != val) match = false;  // Проверка условия
                            if (op == "!=" && second_table->rows[j][k] == val) match = false;
                            break;
                        }
                    }
                }
                if (match) {
                    for (size_t k = 0; k < selected_columns.size; ++k) {
                        for (size_t l = 0; l < first_table->columns.size; ++l) {
                            if (first_table->columns[l] == selected_columns[k]) {
                                cout << first_table->rows[i][l] << " ";  // Вывод соответствующих значений
                                break;
                            }
                        }
                        for (size_t l = 0; l < second_table->columns.size; ++l) {
                            if (second_table->columns[l] == selected_columns[k]) {
                                cout << second_table->rows[j][l] << " ";  // Вывод соответствующих значений
                                break;
                            }
                        }
                    }
                    cout << endl;
                }
            }
        }
    }
}

// Функция для удаления данных из таблицы
void delete_data(const string& table_name, const string& condition) {
    Table* table = reinterpret_cast<Table*>(tables.get(table_name));  // Получение таблицы по имени
    if (!table) {
        cout << "Table not found." << endl;
        return;
    }
    lock_guard<mutex> guard(table->lock);  // Блокировка таблицы для безопасного удаления

    if (table->rows.size == 0) {
        cout << "Table is empty. Nothing to delete." << endl;
        return;
    }

    regex condition_regex(R"((\w+)\s*(=|<|>|<=|>=|!=)\s*('[^']*'|"[^"]*"|\d+))");  // Регулярное выражение для разбора условия
    smatch match;
    if (!regex_search(condition, match, condition_regex)) {
        cout << "Invalid condition format." << endl;
        return;
    }

    string col = match[1];
    string op = match[2];
    string val = match[3];

    if (val.front() == '\'' || val.front() == '"') {
        val = val.substr(1, val.size() - 2);  // Удаление кавычек вокруг значения
    }

    cout << "Parsed condition: col=" << col << ", op=" << op << ", val=" << val << endl;

    CustVector<CustVector<string>> new_rows;
    bool any_match = false;

    for (size_t i = 0; i < table->rows.size; ++i) {
        bool match = false;
        for (size_t j = 0; j < table->columns.size; ++j) {
            if (table->columns[j] == col) {
                if (op == "=" && table->rows[i][j] == val) match = true;  // Проверка условия
                if (op == "!=" && !(table->rows[i][j] == val)) match = true;
                break;
            }
        }
        if (!match) {
            new_rows.push_back(table->rows[i]);  // Добавление строки в новый список, если она не соответствует условию
        }
        else {
            any_match = true;
        }
    }

    if (!any_match) {
        cout << "No rows matched the condition. Nothing to delete." << endl;
        return;
    }

    for (size_t i = 0; i < new_rows.size; ++i) {
        new_rows[i][0] = to_string(i);  // Обновление первичных ключей
    }

    table->rows = new_rows;      // Обновление строк таблицы
    save_table_json(*table);     // Сохранение таблицы в JSON
    save_pk_sequence(*table);    // Сохранение последовательности первичных ключей
    cout << "Rows deleted successfully." << endl;
}

// Функция для создания таблиц из схемы
void create_tables_from_schema(const string& schema_file) {
    ifstream file(schema_file);  // Открытие файла схемы для чтения
    if (!file.is_open()) {
        cout << "Schema file not found." << endl;
        return;
    }
    json j;
    file >> j;  // Чтение JSON из файла

    for (const auto& table_json : j["tables"]) {
        string table_name = table_json["name"];
        CustVector<string> columns;
        for (const auto& col : table_json["columns"]) {
            columns.push_back(col);  // Добавление столбцов в таблицу
        }
        string primary_key = table_json["primary_key"];

        Table new_table(table_name);  // Создание новой таблицы
        new_table.columns = columns;
        new_table.primary_key = primary_key;

        tables.put(table_name, reinterpret_cast<void*>(new Table(new_table)));  // Добавление таблицы в хеш-таблицу
        save_table_json(new_table);  // Сохранение таблицы в JSON
        save_pk_sequence(new_table);  // Сохранение последовательности первичных ключей
        save_lock_state(new_table);  // Сохранение состояния блокировки
        cout << "Table " << table_name << " created successfully." << endl;
    }
}