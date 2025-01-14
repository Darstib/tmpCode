#include "Allocator.hpp"
#include "Test.hpp"
#include <bits/stdc++.h>

template <class T, class Allocator, class StandardAllocator>
void vectorTest(const char* type_name) {
    std::cout << "Running test of " << type_name << std::endl;
    for (int i = 0; i < OPERATIONS; i++) {
        MyVector<T, Allocator> a;
        StdVector<T, StandardAllocator> b;
        int op = rng() % 6; // 6 different operations
        switch (op) {
        case 0:
        {// Clear operation
            if (rng() % 16 == 0) { // to have less clear
                a.clear();
                b.clear();
                std::cerr << "clear" << std::endl;
                break;
            }
        }
        case 1:
        {// pop_back operation
            if (!a.empty()) {
                a.pop_back();
                b.pop_back();
                std::cerr << "pop_back" << std::endl;
            }
        }
        case 2:
        {// push_back operation
            T val_insert = generateValue<T>();
            std::cerr << "insert " << val_insert << std::endl;
            a.push_back(val_insert);
            b.push_back(val_insert);
            break;
        }
        case 3:
        {// Insert at random position
            if (!a.empty()) {
                size_t pos = rng() % a.size();
                T val = generateValue<T>();
                std::cerr << "insert " << val << " at position " << pos << std::endl;
                a.insert(a.begin() + pos, val);
                b.insert(b.begin() + pos, val);
            }
            break;
        }
        case 4:
        {// Reserve operation
            size_t new_capacity = rng() % 1000;
            std::cerr << "reserve to " << new_capacity << std::endl;
            a.reserve(new_capacity);
            b.reserve(new_capacity);
        }
        break;
        case 5:
        {// Resize operation
            size_t new_size = rng() % 1000;
            std::cerr << "resize to " << new_size << std::endl;
            a.resize(new_size);
            b.resize(new_size);
            break;
        }
        default:
            break;
        }

        compare_all<T>(a, b);
    }
    std::cout << "Passed." << std::endl;
}

int main() {
    std::cout << "Running dataType tests..." << std::endl;
    // short int test
    vectorTest<short int, Allocator<short int>, std::allocator<short int>>("short int");
    // int test
    vectorTest<int, Allocator<int>, std::allocator<int>>("int");
    // long long test
    vectorTest<long long, Allocator<long long>, std::allocator<long long>>("long long");
    // pair<int, long long> test
    vectorTest<std::pair<int, long long>, Allocator<std::pair<int, long long>>, std::allocator<std::pair<int, long long>>>("pair<int, long long>");
    // tuple<bool, char, int, double>
    vectorTest<std::tuple<bool, char, int, double>, Allocator<std::tuple<bool, char, int, double>>, std::allocator<std::tuple<bool, char, int, double>>>("tuple<bool, char, int, double>");

    std::cout << "All dataType tests passed.\n" << std::endl;
    return 0;
}