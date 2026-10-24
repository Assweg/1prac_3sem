#include "CommandParser.h"
#include <sstream>
#include <iostream>

using namespace std;

// Функция для парсинга команды на токены
CustVector<string> parse_command(const string& command) {
    CustVector<string> tokens;      // Создаем самописный вектор для хранения токенов
    istringstream iss(command);     // Создаем поток ввода из строки command
    string token;                   // Переменная для хранения текущего токена
    bool inside_quotes = false;     // Флаг, указывающий, находимся ли мы внутри скобок
    string current_token = "";      // Переменная для хранения текущего токена, если он внутри скобок

    // Цикл для обработки каждого токена в строке
    while (iss >> token) {
        // Если токен начинается и заканчивается скобками, добавляем его без скобок
        if (token.front() == '(' && token.back() == ')') {
            tokens.push_back(token.substr(1, token.size() - 2));
        }
        // Если токен начинается со скобки, но не заканчивается, начинаем собирать токен внутри скобок
        else if (token.front() == '(') {
            inside_quotes = true;  // Устанавливаем флаг, что мы внутри скобок
            current_token += token.substr(1) + " ";  // Добавляем токен без скобки и пробел
        }
        // Если токен заканчивается скобкой, заканчиваем собирать токен внутри скобок
        else if (token.back() == ')') {
            inside_quotes = false;  // Сбрасываем флаг, что мы внутри скобок
            current_token += token.substr(0, token.size() - 1);  // Добавляем токен без скобки
            tokens.push_back(current_token);  // Добавляем собранный токен в вектор
            current_token = "";  // Очищаем переменную для следующего токена
        }
        // Если мы внутри скобок, продолжаем собирать токен
        else if (inside_quotes) {
            current_token += token + " ";  // Добавляем токен и пробел
        }
        // Если токен не внутри скобок, добавляем его в вектор как есть
        else {
            tokens.push_back(token);
        }
    }

    // Если остался незавершенный токен внутри скобок, добавляем его в вектор
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }

    return tokens;  // Возвращаем вектор токенов
}

