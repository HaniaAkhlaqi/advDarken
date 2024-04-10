// #include "user_locks.hpp"

// user_lock_dekker::user_lock_dekker()
//     : user_lock() {
//     m_flag[0] = m_flag[1] = false;
//     m_turn = false;
// }

// /* // Lock
// flag[i] ← true
// while flag[j] == true do
//     if turn != i then
//         flag[i] ← false
//         while turn != i do
//             // do nothing
//         end while
//         flag[i] ← true
//     end if
// end while */

// void user_lock_dekker::lock(int thread_id) {

//     int other_thread = 1 - thread_id;

//     m_flag[thread_id].store(true);
//     while (m_flag[other_thread].load()) {
//         if (m_turn.load() != thread_id) {
//             m_flag[thread_id].store(false);
//             while (m_turn.load() != thread_id) {
//                 // Busy wait
//             }
//             m_flag[thread_id].store(true);
//         }
//     }
// }

// /*// Unlock
// turn ← j
// flag[i] ← false
// */
// void user_lock_dekker::unlock(int thread_id) {
//     int other_thread = 1 - thread_id;

//     m_turn.store(other_thread);
//     m_flag[thread_id].store(false);
// }

// /*-------------------------------------------------------------------------------------*/
#include "user_locks.hpp"

user_lock_dekker::user_lock_dekker()
    : user_lock() {
    m_flag[0] = m_flag[1] = false;
    m_turn = false;
}

void user_lock_dekker::lock(int thread_id) {
    int other_thread = 1 - thread_id;

    
    m_flag[thread_id].store(true, std::memory_order_acquire); // Memory fence needed here to ensure proper visibility of m_flag[thread_id] modification
    while (m_flag[other_thread].load(std::memory_order_acquire)) {
        if (m_turn.load(std::memory_order_acquire) != thread_id) {
            
            m_flag[thread_id].store(false, std::memory_order_acquire);// Memory fence needed here 
            while (m_turn.load(std::memory_order_acquire) != thread_id) {
                // Busy wait
            }
           
            m_flag[thread_id].store(true, std::memory_order_acquire);  // Memory fence needed here 
        }
    }
}

void user_lock_dekker::unlock(int thread_id) {
    int other_thread = 1 - thread_id;

    
    m_turn.store(other_thread, std::memory_order_acquire); // Memory fence needed here 
    
    m_flag[thread_id].store(false, std::memory_order_acquire); // Memory fence needed here 
}



/*Notes for me*/
/*If we use memory_order_relaxed with no synchronization or ordering constraints on other reads or writes
it does the same as the implementation with load and store aromic operations now since it ensures atomicity and not what other threads see. Hence the need of memory fences where thety are commented.*/

/* memory_order_consume is a little bit stricter, which means that no reads or writes in the current thread dependent on the value currently loaded can be reordered before this load. It shouldn't pass the tests but it does! why?*/

/*For TSO memory model, memory_order_aquire should be used which ensures that no reads or writes in the current thread can be reordered before this load and garantees visibility of writes from other threads that release the same atomic variable whcih we needed in dekker algorithm */
