#ifndef CUSTVECTOR_H
#define CUSTVECTOR_H

#include <cstddef>  // для типа size_t

// Шаблонный класс CustVector для создания динамического массива
template<typename T>
struct CustVector {
    T* data;            // Указатель на массив данных
    size_t size;        // Текущий размер массива
    size_t capacity;    // Вместимость массива

    // Конструктор
    CustVector() : data(nullptr), size(0), capacity(0) {}

    // Конструктор копирования
    CustVector(const CustVector& other);

    // Оператор присваивания
    CustVector& operator=(const CustVector& other);

    // Деструктор
    ~CustVector();

    // Метод для добавления элемента в конец массива
    void push_back(const T& value);

    // Оператор доступа к элементу по индексу (неконстанта)
    T& operator[](size_t index);

    // Оператор доступа к элементу по индексу (константа)
    const T& operator[](size_t index) const;
};

// Реализация конструктора копирования
template<typename T>
CustVector<T>::CustVector(const CustVector& other) {
    size = other.size;              // Копируем размер
    capacity = other.capacity;      // Копируем вместимость
    data = new T[capacity];         // Выделяем память для нового массива
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];    // Копируем элементы из другого массива
    }
}

// Реализация оператора присваивания
template<typename T>
CustVector<T>& CustVector<T>::operator=(const CustVector& other) {
    if (this != &other) {            // Проверка на самоприсваивание
        delete[] data;               
        size = other.size;           // Копируем размер
        capacity = other.capacity;   // Копируем вместимость
        data = new T[capacity];      // Выделяем память для нового массива
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i]; // Копируем элементы из другого массива
        }
    }
    return *this;                    // Возвращаем текущий объект
}

// Реализация деструктора
template<typename T>
CustVector<T>::~CustVector() {
    delete[] data;
}

// Реализация метода push_back для добавления элемента в конец массива
template<typename T>
void CustVector<T>::push_back(const T& value) {
    if (size == capacity) {       // Если массив заполнен
        capacity = capacity == 0 ? 1 : capacity * 2;     // Увеличиваем вместимость
        T* new_data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];   // Копируем старые элементы в новый массив
        }
        delete[] data;
        data = new_data; 
    }
    data[size++] = value;            // Добавляем новый элемент и увеличиваем размер
}

// Реализация оператора доступа к элементу по индексу (неконстанта)
template<typename T>
T& CustVector<T>::operator[](size_t index) {
    return data[index];          // Возвращаем ссылку на элемент по индексу
}

// Реализация оператора доступа к элементу по индексу (константа)
template<typename T>
const T& CustVector<T>::operator[](size_t index) const {
    return data[index];          // Возвращаем константную ссылку на элемент по индексу
}

#endif