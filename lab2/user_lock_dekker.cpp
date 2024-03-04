#include "user_locks.hpp"

user_lock_dekker::user_lock_dekker()
    : user_lock() {
    m_flag[0] = m_flag[1] = false;
    m_turn = false;
}

/* // Lock
flag[i] ← true
while flag[j] == true do
    if turn != i then
        flag[i] ← false
        while turn != i do
            // do nothing
        end while
        flag[i] ← true
    end if
end while */

void user_lock_dekker::lock(int thread_id) {
    // TODO: Implement the lock acquire part of the Dekker algorithm here
    //(void)thread_id; // TODO: Delete this line

    int other_thread = 1 - thread_id;

    m_flag[thread_id] = true;
    while (m_flag[other_thread]) {
        if (m_turn != thread_id) {
            m_flag[thread_id] = false;
            while (m_turn != thread_id) {
                // Busy wait
            }
            m_flag[thread_id] = true;
        }
    }
}

/*// Unlock
turn ← j
flag[i] ← false
*/
void user_lock_dekker::unlock(int thread_id) {
    // TODO: Implement the lock release part of the Dekker algorithm here
    //(void)thread_id; // TODO: Delete this line
    int other_thread = 1 - thread_id;

    m_turn = other_thread;
    m_flag[thread_id] = false;
}
