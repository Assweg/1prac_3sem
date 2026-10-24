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

[Каталог учебных репозиториев](https://github.com/Assweg/nstu) · [Профиль Assweg](https://github.com/Assweg)

## Материалы из локальной папки

Исходники и документы этой работы собраны в этом репозитории.

Локальный вариант отличается от ранее опубликованного кода и сохранён в [local-version](local-version/). Основная версия остаётся в корне. Для запуска локального варианта сначала перейдите в его каталог.

[local-version](local-version/) · [reports](reports/)

### Отчёты и документы

- [Отчет Практика 1 Бутаков НМ АБс-322.docx](reports/%D0%9E%D1%82%D1%87%D0%B5%D1%82%20%D0%9F%D1%80%D0%B0%D0%BA%D1%82%D0%B8%D0%BA%D0%B0%201%20%D0%91%D1%83%D1%82%D0%B0%D0%BA%D0%BE%D0%B2%20%D0%9D%D0%9C%20%D0%90%D0%91%D1%81-322.docx)
