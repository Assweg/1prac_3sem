# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./include -I./nlohmann

# Исходные файлы
SRCS = main.cpp Table.cpp CommandParser.cpp FileHandler.cpp HashTable.cpp

# Объектные файлы
OBJS = $(SRCS:src/%.cpp=build/%.o)

# Целевой исполняемый файл
TARGET = prac1

# Целевая цель по умолчанию
all: $(TARGET)

# Линковка исполняемого файла
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Компиляция исходных файлов
build/%.o: src/%.cpp
	@mkdir -p build  # Создание директории build, если она не существует
	$(CXX) $(CXXFLAGS) -c $< -o $@  # Компиляция исходного файла в объектный файл

# Очистка
clean:
	rm -rf build $(TARGET)  # Удаление директории build и исполняемого файла

.PHONY: all clean