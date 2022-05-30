/**
 * This file represent the header of our implementation to safe-thread queue with mutex and condition variable
 * using pthread_cond and pthread_mutex
 */

#ifndef DESIGN_PATTERNS_QUEUE_H
#define DESIGN_PATTERNS_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NODE_SIZE sizeof(struct Node)
#define QUEUE_SIZE sizeof(struct Queue)

struct Node {
    void *data;
    struct Node *prev;
};
typedef struct Node *pnode;


struct Queue {
    pnode first; // the first element inserted into the queue
    pnode last; // the last element inserted into the queue
    size_t size;
    pthread_cond_t cond;
    pthread_mutex_t lock;
};
typedef struct Queue *pqueue;

void *createQ();

void destoryQ(void *Q);

int isEmpty(void *Q);

void enQ(void *Q, void *n);

void *deQ(void *Q);

#endif //DESIGN_PATTERNS_QUEUE_H
