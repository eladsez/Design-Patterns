#ifndef DESIGN_PATTERNS_ACTIVE_OBJECT_H
#define DESIGN_PATTERNS_ACTIVE_OBJECT_H

#include "queue.h"

#define AO_SIZE sizeof(struct ActiveObject)

struct ActiveObject {
    pthread_t thread;
    pqueue queue;
    void (*func1)(void *); // the first function that taking a member void* from the queue
    void (*func2)(void *); // the second function
};

typedef struct ActiveObject *p_active_object;

p_active_object newAO(void *Q, void(func1)(void *), void(func2)(void *));

void activate(p_active_object pao);

void destroyAO(p_active_object pao);


#endif //DESIGN_PATTERNS_ACTIVE_OBJECT_H
