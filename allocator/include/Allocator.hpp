#pragma once

#include "MemoryPool.hpp"
#include <cstdlib>
#include <limits>
#include <memory>

template <class _Ty>
class Allocator {
    static MemoryPool mem_pool;
public:
    using __Not_user_specialized = void;
    using value_type = _Ty;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using is_always_equal = std::true_type;

    template <typename T>
    struct rebind { using other = Allocator<T>; };

    // https://en.cppreference.com/w/cpp/memory/allocator/address
    pointer address(reference x) const noexcept { return static_cast<pointer>(&x); }
    const_pointer address(const_reference x) const noexcept { return static_cast<const_pointer>(&x); }

    pointer allocate(size_type n, const void* hint = 0) {
        if (n > max_size()) throw std::bad_array_new_length();
        return static_cast<pointer>(mem_pool.alloc(n * sizeof(value_type)));
    }

    void deallocate(pointer p, size_type n) {
        mem_pool.free(p);
    }

    // https://en.cppreference.com/w/cpp/memory/allocator/destroy
    void destroy(pointer p) { p->~_Ty(); }

    // https://en.cppreference.com/w/cpp/memory/allocator/construct
    template <class T, class... Args>
    void construct(T* p, Args&&... args) {
        ::new ((void*)p) T(std::forward<Args>(args)...);
    }

    size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
};

template <class _Ty>
MemoryPool Allocator<_Ty>::mem_pool;

// https://en.cppreference.com/w/cpp/memory/allocator/operator_cmp
template< class T1, class T2 >
constexpr bool operator==(const Allocator<T1>& lhs, const Allocator<T2>& rhs) noexcept { return true; }

template< class T1, class T2 >
constexpr bool operator!=(const Allocator<T1>& lhs, const Allocator<T2>& rhs) noexcept { return false; }


