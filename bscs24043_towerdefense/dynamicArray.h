#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <iostream>
using namespace std;


template <class T>
class DynamicArray {
private:
    T* data;
    int capacity;
    int size;

    void regrow() {
        capacity = capacity * 2;
        T* newData = new T[capacity];
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    DynamicArray(int initialCapacity = 10) {
        size = 0;
        capacity = initialCapacity;
        data = new T[capacity];
    }

    DynamicArray(int count, const T& defaultValue)
        : capacity(count), size(count) {
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = defaultValue;
        }
    }

    DynamicArray(const DynamicArray<T>& other) {
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; ++i)
            data[i] = other.data[i];
    }

    ~DynamicArray() {
        delete[] data;
    }

    void push(const T& value) {
        if (size == capacity) {
            regrow();
        }
        data[size++] = value;
    }

    void pop() {
        if (size > 0) {
            size--;
        }
    }

    void remove(int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of bounds");
        }

        for (int i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }

    int getSize() const {
        return size;
    }



    T operator[](int index) const {
        return data[index];
    }

    T& operator[](int index) {
        return data[index];
    }


    void print() const {
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) {
            return *this;
        }

        delete[] data;

        capacity = other.capacity;
        size = other.size;

        data = new T[capacity];

        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }

        return *this;
    }

    void clear() {
        size = 0;
    }

};


#endif