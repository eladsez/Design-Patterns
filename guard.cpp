#include "guard.hpp"

/**
 * @brief The implementation of the Guard class.
 * @author Elad Sezanayev
 */

Guard::Guard(pthread_mutex_t *mutex){
    this->mutex = mutex;
    pthread_mutex_lock(mutex);
}

Guard::~Guard() {
    pthread_mutex_unlock(mutex);
}