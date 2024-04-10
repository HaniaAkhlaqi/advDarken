#include "user_locks.hpp"

user_lock_clh::user_lock_clh()
    : user_lock()
    , m_tail(&m_cells[2]) {
    m_local[0].local_cell = &m_cells[0];
    m_local[0].previous = nullptr;
    m_local[1].local_cell = &m_cells[1];
    m_local[1].previous = nullptr;

    m_cells[0] = false;
    m_cells[1] = false;
    m_cells[2] = false;
}

void user_lock_clh::lock(int thread_id) {
    local_l *l = &m_local[thread_id];
    
    l->local_cell->store(1);
    l->previous = m_tail.exchange(l->local_cell);
    while (l->previous->load() != 0)
    {
        /* busy waiting */
    } 
}

void user_lock_clh::unlock(int thread_id) {
    local_l *l = &m_local[thread_id];
    
    l->local_cell->store(0);

    l->local_cell = l->previous;
}


// void user_lock_clh::lock(int thread_id) {
//     local_l *l = &m_local[thread_id];
//     // we set the local node's flag to true, indicating that this thread wants to acquire the lock
//     l->local_cell->store(1, std::memory_order_release);

//     // then set the previous pointer to the tail of the queue atomically 
//     l->previous = m_tail.exchange(l->local_cell, std::memory_order_acq_rel);

//     // and spin until the previous node's flag becomes false, indicating that it has released the lock
//     while (l->previous->load(std::memory_order_acquire)) {
//         // Spin
//     }

// }

// void user_lock_clh::unlock(int thread_id) {
//     local_l *l = &m_local[thread_id];

//     // Set the local node's flag to false, indicating that this thread releases the lock
//     l->local_cell->store(0, std::memory_order_release);

//     // Move the tail to the previous node, removing the current node from the queue
//     l->local_cell = l->previous;
// }