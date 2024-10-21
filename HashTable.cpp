#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <cmath>

class HashTable {
private:
    std::vector<int> table;
    std::vector<bool> isDeleted; 
    int size;
    int numElements;
    float loadFactor;

    int hash(int key) {
        return key % size;
    }

    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    int nextPrime(int n) {
        while (!isPrime(n)) {
            ++n;
        }
        return n;
    }

    void resizeTable() {
        int newSize = nextPrime(size * 2);
        std::vector<int> newTable(newSize, -1);
        std::vector<bool> newIsDeleted(newSize, false);

        //rehashing existing keys
        for (int i = 0; i < size; i++) {
            if (table[i] != -1 && !isDeleted[i]) {
                int newHash = table[i] % newSize;
                int j = 0;
                while (newTable[(newHash + j * j) % newSize] != -1) {
                    j++;
                }
                newTable[(newHash + j * j) % newSize] = table[i];
            }
        }
        
        table = newTable;
        isDeleted = newIsDeleted;
        size = newSize;
    }

public:
    HashTable(int initialSize) : size(nextPrime(initialSize)), numElements(0), loadFactor(0.8f) {
        table.resize(size, -1); //-1 means empty
        isDeleted.resize(size, false);
    }

    void insert(int key) {
        if (float(numElements) / size >= loadFactor) {
            resizeTable();
        }
        int h = hash(key);
        int i = 0;
        while (table[(h + i * i) % size] != -1 && !isDeleted[(h + i * i) % size]) {
            i++;
        }
        table[(h + i * i) % size] = key;
        isDeleted[(h + i * i) % size] = false;
        numElements++;
    }

    int search(int key) {
        int h = hash(key);
        int i = 0;
        while (table[(h + i * i) % size] != -1) {
            if (table[(h + i * i) % size] == key && !isDeleted[(h + i * i) % size]) {
                return (h + i * i) % size;
            }
            i++;
        }
        return -1; //return -1 if the key is not found
    }

    void remove(int key) {
        int idx = search(key);
        if (idx != -1) {
            isDeleted[idx] = true;
            numElements--;
        }
    }

    void printTable() {
        for (int i = 0; i < size; i++) {
            if (table[i] != -1 && !isDeleted[i]) {
                std::cout << "[" << i << "] -> " << table[i] << std::endl;
            } else {
                std::cout << "[" << i << "] -> " << "empty" << std::endl;
            }
        }
    }
}