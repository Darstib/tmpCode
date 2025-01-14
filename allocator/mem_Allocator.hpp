#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <utility>

class MemoryPool {
public:
    static const size_t buffer_size = 131072;

private:
    struct Buffer {           // store several small memory blocks
        Buffer* next = nullptr; // pointing to the next buffer
        void* start = nullptr;  // record the starting address of the this buffer
        void* endp = nullptr;   // record the address of the unallocated memory of the this buffer
        size_t count = 0;       // record how many small memory blocks having not been released from this buffer
    } *buffers;

    struct Block {           // store big memory blocks that larger than buffer_size
        Block* next = nullptr; // point to the next block
        void* start = nullptr; // record the starting address of the this block
        bool is_freed = false; // record whether this block having been released from the memory
    } *blocks;

    void free_buffer(Buffer* it) {
        if (it == nullptr) return;
        std::free(it->start);
        free_buffer(it->next);
        delete it;
    }

    void free_block(Block* it) {
        if (it == nullptr) return;
        if (!it->is_freed) { std::free(it->start); }
        free_block(it->next);
        delete it;
    }

public:
    MemoryPool() {
        buffers = nullptr;
        blocks = nullptr;
    }

    ~MemoryPool() {
        free_buffer(buffers);
        free_block(blocks);
    }

    // disable copy constructor and others to avoid the memory pool from being copied.
    MemoryPool(MemoryPool&& memoryPool) = delete;
    MemoryPool(const MemoryPool& memoryPool) = delete;
    MemoryPool operator=(MemoryPool&& memoryPool) = delete;
    MemoryPool operator=(const MemoryPool& memoryPool) = delete;

    void* malloc(size_t size) {
        if (size <= buffer_size) {
            // if the size of the requested memory is less than the buffer_size limit, try to allocate from the buffer pool
            for (Buffer* it = buffers; it != nullptr; it = it->next) {
                if ((size_t)(it->endp) + size <= (size_t)(it->start) + buffer_size) {
                    // found an existing buffer that can allocate the current memory
                    void* result = it->endp;
                    it->count++;
                    it->endp = (void*)((size_t)(it->endp) + size);
                    return result;
                }
            }
            // need to allocate a new buffer for the current memory
            Buffer* it = new Buffer();
            it->next = buffers;
            buffers = it;
            it->start = std::malloc(buffer_size);
            it->endp = (void*)((size_t)(it->start) + size);
            it->count = 1;
            return it->start;
        } else {
            // if the size of the requested memory is greater than the buffer_size limit, a whole block of memory is directly requested (no optimization)
            for (Block* it = blocks; it != nullptr; it = it->next) {
                if (it->is_freed) {
                    // found an empty node in the list, so that the length of the linked list can be saved
                    it->is_freed = false;
                    return it->start = std::malloc(size);
                }
            }
            // append a new node to the linked list
            Block* it = new Block();
            it->next = blocks;
            blocks = it;
            return it->start = std::malloc(size);
        }
    }

    void* cmalloc(size_t size) {
        void* pointer = malloc(size);
        memset(pointer, 0, size);
        return pointer;
    }

    void free(void* pointer) {
        for (Buffer* it = buffers; it != nullptr; it = it->next) {
            if (it->start <= pointer && pointer < (void*)((size_t)(it->start) + buffer_size)) { // 当前内存起始位置在buffer的内存区间中
                it->count--;
                if (it->count == 0) {
                    it->endp = it->start;
                    // if the memory in the current buffer has been completely released, the buffer can be reused from the beginning
                }
            }
        }
        for (Block* it = blocks; it != nullptr; it = it->next) {
            if (it->start == pointer) {
                ::free(it->start);
                it->is_freed = true;
                return;
            }
        }
    }
};
static MemoryPool _pool;

template <class _Ty>
class Allocator {

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

    template <typename U>
    struct rebind {
        typedef Allocator<U> other;
    };

    pointer address(reference x) noexcept {
        return static_cast<pointer>(&x);
    }

    const_pointer address(const_reference x) const noexcept {
        return static_cast<const_pointer>(&x);
    }

    pointer allocate(size_type n) {
        return static_cast<pointer>(_pool.malloc(n * sizeof(_Ty)));
    }

    void deallocate(pointer p, size_type n) {
        _pool.free(p);
    }

    size_type max_size() const {
        // since the memory pool can use new/delete to request memory from the system, the supported max_size can be considered as a hardware limit
        return std::numeric_limits<size_type>::max() / sizeof(_Ty);
    }

    void destroy(pointer p) {
        p->~_Ty(); // call the destructor of the element pointed to
    }

    // call the constructor to construct an object at the specified address (use std::forward to perfect forward parameters)
    template <class U, class... Args>
    void construct(U* p, Args&&... args) {
        new (p) U(std::forward<Args>(args)...);
    }
};

template <class _Ty>
bool operator==(const Allocator<_Ty>&, const Allocator<_Ty>&) { return true; }

template <class _Ty>
bool operator!=(const Allocator<_Ty>&, const Allocator<_Ty>&) { return false; }

