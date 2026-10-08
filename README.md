# 1prac_3sem — Сетевой сервер БД на C++ (базовая версия)

Практическая №1, 3 семестр. Первая версия TCP-сервера мини-СУБД — однопоточная, основа для `2prac_3sem`.

## Что внутри
- `main.cpp` — обработка одного клиента, `SELECT`
- `CommandParser`, `FileHandler`, `HashTable`, `Table`, `CustVector`
- `Makefile`, `schema.json`, `CommandHelp.txt`

## Стек
C++17, POSIX sockets

## Запуск
```bash
make
./prac1
# или:
g++ *.cpp -o db_server -pthread && ./db_server
```

## Что изучено
Базовые сокеты, парсинг SQL-подобных команд, файловое хранение таблиц.

---
**Автор:** Assweg · Telegram: [@assweg](https://t.me/assweg) · Учебное портфолио, все работы — студенческие.
