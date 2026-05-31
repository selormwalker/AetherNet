#ifndef LOCK_FREE_QUEUE_HPP
#define LOCK_FREE_QUEUE_HPP

#include <atomic>
#include <vector>
#include <optional>

namespace AetherNet {

/**
 * A simple Single-Producer Single-Consumer (SPSC) Lock-Free Queue
 * Designed for low-latency message passing between threads.
 */
template<typename T>
class LockFreeQueue {
private:
    struct Node {
        T data;
        std::atomic<bool> has_data{false};
    };

    std::vector<Node> buffer;
    alignas(64) std::atomic<size_t> head{0};
    alignas(64) std::atomic<size_t> tail{0};
    size_t capacity;

public:
    explicit LockFreeQueue(size_t cap) : buffer(cap), capacity(cap) {}

    bool push(const T& value) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        size_t next_tail = (current_tail + 1) % capacity;

        if (next_tail == head.load(std::memory_order_acquire)) {
            return false; // Queue full
        }

        buffer[current_tail].data = value;
        buffer[current_tail].has_data.store(true, std::memory_order_release);
        tail.store(next_tail, std::memory_order_release);
        return true;
    }

    std::optional<T> pop() {
        size_t current_head = head.load(std::memory_order_relaxed);

        if (current_head == tail.load(std::memory_order_acquire)) {
            return std::nullopt; // Queue empty
        }

        if (!buffer[current_head].has_data.load(std::memory_order_acquire)) {
            return std::nullopt;
        }

        T value = std::move(buffer[current_head].data);
        buffer[current_head].has_data.store(false, std::memory_order_release);
        head.store((current_head + 1) % capacity, std::memory_order_release);
        return value;
    }
};

} // namespace AetherNet

#endif
