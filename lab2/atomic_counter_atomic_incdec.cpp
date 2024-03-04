#include "atomic_counters.hpp"

// atomic_counter_atomic_incdec::atomic_counter_atomic_incdec()
//     : atomic_counter()
//     , m_value(0) {
// }

// int atomic_counter_atomic_incdec::increment() {
//     // TODO: Modify this code using atomic fetch_add operations
//     return m_value.fetch_add(1);
// }

// int atomic_counter_atomic_incdec::decrement() {
//     // TODO: Modify this code using atomic fetch_sub operations
//     return m_value.fetch_sub(1);
// }

// void atomic_counter_atomic_incdec::set(int value) {
//     // TODO: Modify this code using atomic store operations
//     m_value.store(value);
// }

// int atomic_counter_atomic_incdec::get() {
//     // TODO: Modify this code using atomic load operations
//     return m_value.load();
// }


/* Bonus part 2: Memory Orderings*/
/*Change your atomic counter implementation to use the most relaxed memory order from std::memory_order 
that is possible to use without introducing any bugs.*/

#include <atomic>

class atomic_counter_atomic_incdec {
public:
    atomic_counter_atomic_incdec();

    int increment();
    int decrement();
    void set(int value);
    int get();

private:
    std::atomic<int> m_value;
};

atomic_counter_atomic_incdec::atomic_counter_atomic_incdec()
    : m_value(0) {
}

int atomic_counter_atomic_incdec::increment() {
    return m_value.fetch_add(1, std::memory_order_relaxed);
}

int atomic_counter_atomic_incdec::decrement() {
    return m_value.fetch_sub(1, std::memory_order_relaxed);
}

void atomic_counter_atomic_incdec::set(int value) {
    m_value.store(value, std::memory_order_relaxed);
}

int atomic_counter_atomic_incdec::get() {
    return m_value.load(std::memory_order_relaxed);
}

