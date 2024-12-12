#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "HashTable.h"
#include "Table.h"
#include "CommandParser.h"
#include "FileHandler.h"
#include "CustVector.h"

using namespace std;

// Глобальная хэш-таблица для хранения таблиц
HashTable tables(10);
// Мьютекс для синхронизации доступа к таблицам
mutex tables_mutex;

// Функция для обработки клиентского соединения
void handle_client(int client_socket) {
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, 1024);  // Читаем данные от клиента
    if (valread <= 0) {
        close(client_socket);
        return;
    }

    string command(buffer);
    CustVector<string> tokens = parse_command(command);  // Парсим команду на токены
    if (tokens.size == 0) {
        string response = "Invalid command.\n";
        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
        return;
    }

    // Обработка команды SELECT
    if (tokens[0] == "SELECT") {
        if (tokens.size < 4 || tokens[2] != "FROM") {
            string response = "Invalid SELECT command. Usage: SELECT ('column1', 'column2') FROM table_name1, table_name2\n";
            send(client_socket, response.c_str(), response.size(), 0);
            close(client_socket);
            return;
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

        // Блокируем доступ к таблицам
        lock_guard<mutex> guard(tables_mutex);
        stringstream result;
        select_data(table_names, columns, condition);  // Вызываем функцию для выборки данных
        string response = result.str();
        send(client_socket, response.c_str(), response.size(), 0);
    }
    // Обработка команды INSERT
    else if (tokens[0] == "INSERT") {
        if (tokens.size < 4 || tokens[1] != "INTO" || tokens[3] != "VALUES") {
            string response = "Invalid INSERT command. Usage: INSERT INTO table_name VALUES (value1, value2)\n";
            send(client_socket, response.c_str(), response.size(), 0);
            close(client_socket);
            return;
        }
        CustVector<string> values;
        string values_str = tokens[4];
        istringstream iss(values_str);
        string value;
        while (getline(iss, value, ',')) {
            values.push_back(trim(value));  // Добавляем значения в вектор
        }

        // Блокируем доступ к таблицам
        lock_guard<mutex> guard(tables_mutex);
        insert_data(tokens[2], values);  // Вызываем функцию для вставки данных
        string response = "Data inserted successfully.\n";
        send(client_socket, response.c_str(), response.size(), 0);
    }
    // Обработка команды DELETE
    else if (tokens[0] == "DELETE") {
        if (tokens.size < 4 || tokens[1] != "FROM" || tokens[3] != "WHERE") {
            string response = "Invalid DELETE command. Usage: DELETE FROM table_name WHERE column = value\n";
            send(client_socket, response.c_str(), response.size(), 0);
            close(client_socket);
            return;
        }

        // Блокируем доступ к таблицам
        lock_guard<mutex> guard(tables_mutex);
        delete_data(tokens[2], tokens[4]);  // Вызываем функцию для удаления данных
        string response = "Rows deleted successfully.\n";
        send(client_socket, response.c_str(), response.size(), 0);
    }
    // Обработка команды LOAD
    else if (tokens[0] == "LOAD") {
        if (tokens.size == 3 && tokens[1] == "TABLE") {
            // Блокируем доступ к таблицам
            lock_guard<mutex> guard(tables_mutex);
            load_table_json(tokens[2]);  // Загружаем таблицу из JSON файла
            string response = "Table loaded successfully.\n";
            send(client_socket, response.c_str(), response.size(), 0);
        }
        else if (tokens.size == 3 && tokens[1] == "CSV") {
            // Блокируем доступ к таблицам
            lock_guard<mutex> guard(tables_mutex);
            load_table_csv(tokens[2]);  // Загружаем таблицу из CSV файла
            string response = "Table loaded successfully.\n";
            send(client_socket, response.c_str(), response.size(), 0);
        }
        else {
            string response = "Invalid LOAD command. Usage: LOAD TABLE table_name or LOAD CSV table_name\n";
            send(client_socket, response.c_str(), response.size(), 0);
        }
    }
    // Обработка команды CREATE
    else if (tokens[0] == "CREATE") {
        if (tokens.size < 6 || tokens[1] != "TABLE" || tokens[3] != "(" || tokens[tokens.size - 2] != ")") {
            string response = "Invalid CREATE TABLE command. Usage: CREATE TABLE table_name (column1, column2) PRIMARY KEY (primary_key)\n";
            send(client_socket, response.c_str(), response.size(), 0);
            close(client_socket);
            return;
        }
        CustVector<string> columns;
        string columns_str = tokens[3];
        istringstream iss(columns_str);
        string column;
        while (getline(iss, column, ',')) {
            columns.push_back(trim(column));  // Добавляем столбцы в вектор
        }

        // Блокируем доступ к таблицам
        lock_guard<mutex> guard(tables_mutex);
        create_table(tokens[2], columns, tokens[tokens.size - 1]);  // Вызываем функцию для создания таблицы
        string response = "Table created successfully.\n";
        send(client_socket, response.c_str(), response.size(), 0);
    }
    // Обработка команды SAVE
    else if (tokens[0] == "SAVE") {
        if (tokens.size != 3 || tokens[1] != "TABLE") {
            string response = "Invalid SAVE command. Usage: SAVE TABLE table_name\n";
            send(client_socket, response.c_str(), response.size(), 0);
            close(client_socket);
            return;
        }

        // Блокируем доступ к таблицам
        lock_guard<mutex> guard(tables_mutex);
        Table* table = reinterpret_cast<Table*>(tables.get(tokens[2]));  // Получаем указатель на таблицу
        if (!table) {
            string response = "Table not found.\n";
            send(client_socket, response.c_str(), response.size(), 0);
            close(client_socket);
            return;
        }
        save_table_csv(*table);  // Сохраняем таблицу в CSV файл
        string response = "Table saved successfully.\n";
        send(client_socket, response.c_str(), response.size(), 0);
    }
    // Обработка команды EXIT
    else if (tokens[0] == "EXIT") {
        string response = "Goodbye!\n";
        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
        return;
    }
    // Обработка неизвестной команды
    else {
        string response = "Unknown command.\n";
        send(client_socket, response.c_str(), response.size(), 0);
    }

    close(client_socket);
}

int main() {
    // Загружаем таблицы из файла схемы
    create_tables_from_schema("schema.json");

    // Создаем TCP-сервер
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Создаем сокет
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязываем сокет к порту 7432
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7432);

    if (::bind(server_fd, (struct sockaddr *)&address, addrlen) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Начинаем прослушивание
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server is running on port 7432. Waiting for connections..." << endl;

    while (true) {
        // Принимаем новое соединение
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Создаем новый поток для обработки клиента
        thread client_thread(handle_client, new_socket);
        client_thread.detach();  // Отсоединяем поток
    }

    return 0;
}