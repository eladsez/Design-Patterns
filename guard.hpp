#ifndef DESIGN_PATTERNS_GUARD_HPP
#define DESIGN_PATTERNS_GUARD_HPP
#include <pthread.h>

/**
 * @brief This class is responsible for guarding a critical section, and implements the guard pattern.
 * @author Elad Sezanayev
 */

class Guard {
private:
    pthread_mutex_t *mutex;
public:
    Guard(pthread_mutex_t *mutex);

    ~Guard();
};


#endif //DESIGN_PATTERNS_GUARD_HPP
