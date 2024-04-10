#include "atomic_counters.hpp"

atomic_counter_lock::atomic_counter_lock()
    : atomic_counter()
    , m_value(0)
    , m_lock() {
}

int atomic_counter_lock::increment() {
    // TODO: Add locks here
    std::lock_guard<std::mutex> guard(m_lock); // Or use: m_value.m_lock();
    int prev_value = m_value;
    m_value = m_value + 1;
    //m_value.m_unlock();
    return prev_value;
}

int atomic_counter_lock::decrement() {
    // TODO: Add locks here
    std::lock_guard<std::mutex> guard(m_lock); // Or use: m_value.m_lock();
    int prev_value = m_value;
    m_value = m_value - 1;
    // Or use: m_value.m_lock();
    return prev_value;
}

void atomic_counter_lock::set(int value) {
    // TODO: Add locks here
    std::lock_guard<std::mutex> guard(m_lock); //modern compiler helps if the data is not aligned in one cache line instead it is resided in two but to ensure since we don't know the compiler behavior use lock
    m_value = value; 
}

int atomic_counter_lock::get() {
    // TODO: Add locks here
    std::lock_guard<std::mutex> guard(m_lock);
    return m_value;
}
