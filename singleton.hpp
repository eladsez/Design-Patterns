#include <iostream>
#include <pthread.h>

/**
 * @brief The Singleton class is a template class that can be used to create a singleton objects.
 * @details In this class, the template parameter T is the class that is used to create the singleton class.
 *          The class T must have a private default constructor.
 *          The class T must have a friend class declaration of this Singleton class.
 * @author Elad Sezanayev
 */

template<typename T>
class Singleton {
private:
    static T *instance;
    static pthread_mutex_t mutex;

    Singleton() {}

public:
    static T *Instance() {
        if (instance == nullptr) {
            pthread_mutex_lock(&mutex);
            if (instance == nullptr) {
                instance = new T();
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    static void destroy() {
        if (instance != nullptr) {
            pthread_mutex_lock(&mutex);
            delete instance;
            instance = nullptr;
            pthread_mutex_unlock(&mutex);
        }
    }
};

template<typename T>
T *Singleton<T>::instance = new T();
template<typename T>
pthread_mutex_t Singleton<T>::mutex = PTHREAD_MUTEX_INITIALIZER;