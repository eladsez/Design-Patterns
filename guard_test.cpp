#include "guard.hpp"
#include <pthread.h>
#include <iostream>


/**
 * @details This file contains the test for the Guard class.
 */

int a = 0;
pthread_mutex_t mutex;

void *thread_func1(void *arg) {
    Guard guard(&mutex);
    for (int i = 0; i < 10000; i++) {
        a++;
    }
    std::cout << "a = " << a << std::endl;
    return nullptr;
}

void *thread_func2(void * arg) {
    Guard guard(&mutex);
    for (int i = 0; i < 10000; i++) {
        a--;
    }
    std::cout << "a = " << a << std::endl;
    return nullptr;
}


/**
 * @brief spouse to print a = 1000\n a = 0 if the test passed.
 */
int main() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_t t1, t2;
    pthread_create(&t1, nullptr, thread_func1, nullptr);
    pthread_create(&t2, nullptr, thread_func2, nullptr);

    pthread_join(t2, nullptr);

    printf("\033[1;35m"); // change text color
    if (a == 0) {
        printf("guard test end successfully!");
    }else {
        printf("guard test failed!");
    }
    printf("\n");
    return 0;
}