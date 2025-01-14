#pragma once
#include <memory>
#include <cstdlib>
class MemoryPool {
    static const size_t buffer_size = 0x1000; // 16 bytes
    struct BufferBlock {
        BufferBlock* next;
        char data[1]; // Flexible array member
    }*buffer_head;

public:
    MemoryPool() : buffer_head(nullptr) {}

    ~MemoryPool() {
        BufferBlock* current = buffer_head;
        while (current) {
            BufferBlock* next = current->next;
            std::free(current);
            current = next;
        }
    }

    void* alloc(size_t size) {
        size_t buffer_count = (size - 1) / buffer_size + 1;
        size_t total_size = sizeof(BufferBlock) + (buffer_count * buffer_size) - 1;
        BufferBlock* new_block = static_cast<BufferBlock*>(std::malloc(total_size));
        if (!new_block) throw std::bad_alloc();
        new_block->next = buffer_head;
        buffer_head = new_block;
        return static_cast<void*>(new_block->data);
    }

    void free(void* p) {
        BufferBlock* block = reinterpret_cast<BufferBlock*>(reinterpret_cast<char*>(p) - offsetof(BufferBlock, data));
        if (buffer_head == block) {
            buffer_head = buffer_head->next;
        } else {
            BufferBlock* current = buffer_head;
            while (current && current->next != block) {
                current = current->next;
            }
            if (current) {
                current->next = block->next;
            }
        }
        std::free(block);
    }
};