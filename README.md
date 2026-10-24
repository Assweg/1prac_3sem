# Сетевой сервер мини-СУБД · базовая версия

![Стек](https://img.shields.io/badge/C%2B%2B17%20%C2%B7%20POSIX%20sockets-334155?style=flat-square) ![Семестр](https://img.shields.io/badge/семестр-3-0f766e?style=flat-square) ![Статус](https://img.shields.io/badge/статус-%D0%A3%D1%87%D0%B5%D0%B1%D0%BD%D1%8B%D0%B9%20%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82-64748b?style=flat-square)

Практическая №1: TCP-сервер с SQL-подобными командами и хранением таблиц в файлах.

## Архитектура

| Файлы | Назначение |
|---|---|
| [main.cpp](main.cpp) | Сетевой сервер и работа с клиентами |
| [CommandParser.cpp](CommandParser.cpp) | Разбор команд |
| [FileHandler.cpp](FileHandler.cpp) | Чтение и запись файлов таблиц |
| [HashTable.cpp](HashTable.cpp), [Table.cpp](Table.cpp) | Структуры хранения |
| [schema.json](schema.json) | Схема данных |

## Сборка и запуск

Нужны GCC/Clang с C++17 и POSIX-сокеты (Linux или macOS). Запускать из корня проекта.

```bash
c++ -std=c++17 -pthread main.cpp Table.cpp CommandParser.cpp FileHandler.cpp HashTable.cpp -o database_server
./database_server
```

## Протокол и примеры

Поддерживаемые команды описаны в [CommandHelp.txt](CommandHelp.txt). Продолжение работы: [2prac_3sem](https://github.com/Assweg/2prac_3sem).

## Статус

Учебный сервер. Инструкция отражает состав исходников; сетевые сценарии и конкурентный доступ при публикации не проверялись.

---

[Все учебные работы](https://github.com/Assweg/nstu) · [Профиль Assweg](https://github.com/Assweg)
