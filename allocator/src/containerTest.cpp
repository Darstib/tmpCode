#include "Allocator.hpp"
#include "Test.hpp"
#include <bits/stdc++.h>

// vector test
template <class T, class Allocator, class StandardAllocator>
void vectorTest(const char* type_name) {
    std::cout << "Running vector test of " << type_name << std::endl;
    MyVector<T, Allocator> a;
    StdVector<T, StandardAllocator> b;
    for (int i = 0; i < OPERATIONS; i++) {
        int op = rng() % 7;
        switch (op) {
        case 0:
        {// clear
            if (rng()%16) { // to have less clear
                a.clear();
                b.clear();
                std::cerr << "clear vector" << std::endl;
                break;
            }
        }
        case 1:
        {// erase
            if (!a.empty()) {
                size_t pos = rng() % a.size();
                a.erase(a.begin() + pos);
                b.erase(b.begin() + pos);
            }
        }
        case 2:
        {// reserve
            size_t new_capacity = rng() % 1000;
            a.reserve(new_capacity);
            b.reserve(new_capacity);
            std::cerr << "reserve to " << new_capacity << std::endl;
            break;
        }
        case 3:
        {// resize 
            size_t new_size = rng() % 1000;
            a.resize(new_size);
            b.resize(new_size);
            std::cerr << "resize to " << new_size << std::endl;
            break;
        }
        case 4:
        {// pop_back
            if (!a.empty()) {
                a.pop_back();
                b.pop_back();
                std::cerr << "pop back" << std::endl;
                break;
            }
        }
        case 5:
        {// insert
            T val = generateValue<T>();
            std::cerr << "insert " << val << std::endl;
            if (!a.empty()) {
                size_t pos = rng() % a.size();
                a.insert(a.begin() + pos, val);
                b.insert(b.begin() + pos, val);
            } else {

                a.push_back(val);
                b.push_back(val);
            }
            break;
        }
        case 6:
        {// push_back
            T val = generateValue<T>();
            a.push_back(val);
            b.push_back(val);
            break;
        }
        default:
            break;
        }
        compare(a, b);
    }
    std::cout << "Vector test passed." << std::endl;
}
// Set test
template <class T, class Allocator>
void setTest(const char* type_name) {
    std::cout << "Running set test of " << type_name << std::endl;
    MySet<T, Allocator> a;
    StdSet<T, std::allocator<T>> b;
    for (int i = 0; i < OPERATIONS; i++) {
        int op = rng() % 4;
        switch (op) {
        case 0:
        {// clear
            if (rng()%16 == 0) { // to have less clear
                std::cerr << "clear set" << std::endl;
                a.clear();
                b.clear();
                break;
            }
        }
        case 1:
        {// erase
            if (!a.empty() && !b.empty()) {
                auto it = a.begin();
                std::cerr << "erase " << *it << std::endl;
                a.erase(it);
                b.erase(b.find(*it));
            }
        }
        case 2:
        {// insert
            T val_insert = generateValue<T>();
            std::cerr << "insert " << val_insert << std::endl;
            a.insert(val_insert);
            b.insert(val_insert);
            break;
        }
        case 3:
        {// find
            T val_find = generateValue<T>();
            std::cerr << "find " << val_find << std::endl;
            auto find_a = a.find(val_find);
            auto find_b = b.find(val_find);
            assert((find_a == a.end()) && (find_b == b.end()) || (*find_a == *find_b));
            break;
        }
        default:
            break;
        }

        compare(a, b);
    }
    std::cout << "Set test passed." << std::endl;
}

// Map test
template <class Key, class T, class Allocator>
void mapTest(const char* type_name) {
    std::cout << "Running map test of " << type_name << std::endl;
    MyMap<Key, T, Allocator> a;
    StdMap<Key, T, std::allocator<std::pair<const Key, T>>> b;
    for (int i = 0; i < OPERATIONS; i++) {
        int op = rng() % 4;
        switch (op) {
        case 0:
        {// clear
            if (rng()%16==0) { // to have less clear
                a.clear();
                b.clear();
                std::cerr << "clear map" << std::endl;
                break;
            }
        }
        case 1:
        {// erase
            if (!a.empty()) {
                auto it_a = a.begin();
                std::advance(it_a, rng() % a.size());
                std::cerr << "erase (" << it_a->first << " " << it_a->second << ")" << std::endl;
                a.erase(it_a);
                b.erase(b.find(it_a->first));
            }
        }
        case 2:
        {// insert
            Key key = generateValue<Key>();
            T value = generateValue<T>();
            std::cerr << "insert (" << key << " " << value << ")" << std::endl;
            a.emplace(key, value);
            b.emplace(key, value);
            break;
        }
        case 3:
        {// find
            Key key_find = generateValue<Key>();
            std::cerr << "find " << key_find << std::endl;
            auto find_a = a.find(key_find);
            auto find_b = b.find(key_find);
            assert((find_a == a.end() && find_b == b.end()) || *find_a == *find_b);
            break;
        }
        default:
            break;
        }

        compare_map(a, b);
    }
    std::cout << "Map test passed." << std::endl;
}

int main() {
    std::cout << "Running container tests..." << std::endl;
    // Vector test
    vectorTest<int, Allocator<int>, std::allocator<int>>("int");
    // Set test
    setTest<int, Allocator<int>>("int");
    // Map test
    mapTest<int, int, Allocator<std::pair<const int, int>>>("map<const int, int>");

    std::cout << "All container tests passed.\n" << std::endl;
    return 0;
}