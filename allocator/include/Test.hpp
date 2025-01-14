#pragma once

#ifndef _TEST_H_
#define _TEST_H_
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <random>
#include <cassert>

// number of operations
const int OPERATIONS = 100000;

// alias
template <class T, class Allocator>
using MyVector = std::vector<T, Allocator>;
template <class T, class Allocator>
using MySet = std::set<T, std::less<T>, Allocator>;
template <class Key, class T, class Allocator>
using MyMap = std::map<Key, T, std::less<Key>, Allocator>;
template <class T, class Allocator>
using StdVector = std::vector<T, Allocator>;
template <class T, class Allocator>
using StdSet = std::set<T, std::less<T>, std::allocator<T>>;
template <class Key, class T, class Allocator>
using StdMap = std::map<Key, T, std::less<Key>, std::allocator<std::pair<const Key, T>>>;

// random data generator
std::mt19937 rng(67656);
template <typename T>
T generateValue() {
    return static_cast<T>(rng());
}
template <>
std::pair<int, long long> generateValue<std::pair<int, long long>>() {
    return { static_cast<int>(rng()), static_cast<long long>(rng()) };
}
template <>
std::tuple<int, int, int> generateValue<std::tuple<int, int, int>>() {
    return std::make_tuple(static_cast<int>(rng()), static_cast<int>(rng()), static_cast<int>(rng()));
}
template <>
std::tuple<bool, char, int, double> generateValue<std::tuple<bool, char, int, double>>() {
    std::uniform_real_distribution<double> dist_double(0.0, 1000.0);
    std::uniform_int_distribution<int> dist_char(32, 126); // Printable ASCII
    std::uniform_int_distribution<int> dist_bool(0, 1);
    return
        std::make_tuple(static_cast<bool>(dist_bool(rng)), static_cast<char>(dist_char(rng)), rng(), dist_double(rng));
}

// linear container compare
template <typename ContainerA, typename ContainerB>
void compare(const ContainerA& a, const ContainerB& b) {
    assert(a.size() == b.size() && "Container sizes do not match.");
    auto it_a = a.begin();
    auto it_b = b.begin();
    size_t index = 0;
    while (it_a != a.end() && it_b != b.end()) {
        // debug info
        if (*it_a != *it_b) {
            std::cerr << a.size() << ' ' << b.size() << std::endl;
            std::cerr << "\taa\t\t\tbb" << std::endl;
            for (auto aa = a.begin(), bb = b.begin(); aa != a.end() && bb != b.end(); ++aa, ++bb) {
                std::cerr << *aa << "\t\t" << *bb << std::endl;
            }
            std::cerr << "Index: " << index << std::endl;
            std::cerr << "a: " << *it_a << std::endl;
            std::cerr << "b: " << *it_b << std::endl;
        }
        assert(*it_a == *it_b && "Container elements do not match.");
        ++it_a;
        ++it_b;
        ++index;
    }
}

// map container compare
template <typename Key, typename T, class AllocatorA, class AllocatorB = std::allocator<std::pair<const Key, T>>>
void compare_map(const MyMap<Key, T, AllocatorA>& a, const StdMap<Key, T, AllocatorB>& b) {
    assert(a.size() == b.size() && "Map sizes do not match.");
    auto it_a = a.begin();
    auto it_b = b.begin();
    size_t index = 0;
    while (it_a != a.end() && it_b != b.end()) {
        if (it_a->first != it_b->first || it_a->second != it_b->second) {
            std::cerr << "aa\t\t\t\tbb" << std::endl;
            for (auto aa = a.begin(), bb = b.begin(); aa != a.end() && bb != b.end(); ++aa, ++bb) {
                std::cerr << *aa << "\t\t" << *bb << std::endl;
            }
            std::cerr << "Difference found at index: " << index << std::endl;
            std::cerr << "MyMap: (" << it_a->first << ", " << it_a->second << ")" << std::endl;
            std::cerr << "StdMap: (" << it_b->first << ", " << it_b->second << ")" << std::endl;
        }
        assert(it_a->first == it_b->first && it_a->second == it_b->second && "Map elements do not match.");
        ++it_a;
        ++it_b;
        ++index;
    }
}

// different container compare
template <typename T, typename ContainerA, typename ContainerB>
void compare_all(const ContainerA& a, const ContainerB& b) {
    assert(a.size() == b.size());
    for (size_t idx = 0; idx < a.size(); idx++) {
        if constexpr (std::is_same<T, std::pair<int, long long>>::value) {
            assert(a[idx].first == b[idx].first);
            assert(a[idx].second == b[idx].second);
        } else if constexpr (std::is_same<T, std::tuple<bool, char, int, double>>::value) {
            assert(std::get<0>(a[idx]) == std::get<0>(b[idx]));
            assert(std::get<1>(a[idx]) == std::get<1>(b[idx]));
            assert(std::get<2>(a[idx]) == std::get<2>(b[idx]));
        } else {
            assert(a[idx] == b[idx]);
        }
    }
}

// overload the operator<< for std::pair and std::tuple
template<typename X, typename Y>
std::ostream& operator<<(std::ostream& os, const std::pair<X, Y>& pair) {
    os << "(" << pair.first << ", " << pair.second << ")";
    return os;
}

template<typename X, typename Y, typename Z, typename W>
std::ostream& operator<<(std::ostream& os, const std::tuple<X, Y, Z, W>& tuple) {
    os << "("
        << std::get<0>(tuple) << ", "
        << std::get<1>(tuple) << ", "
        << std::get<2>(tuple) << ", "
        << std::get<3>(tuple) << ")";
    return os;
}

#endif