#include "queue.h"
#include <pthread.h>

void *createQ() {
    pqueue newQ = (pqueue) malloc(QUEUE_SIZE);
    if (newQ == NULL) {
        perror("malloc ERROR");
        exit(1);
    }
    newQ->size = 0;
    newQ->first = NULL;
    newQ->last = NULL;
    pthread_cond_init(&newQ->cond, NULL);
    pthread_mutex_init(&newQ->lock, NULL);
    return (void *) newQ;
}

void destoryQ(void *Q) {

    pqueue queue = (pqueue) Q;
    pthread_mutex_lock(&queue->lock);
    pnode to_free;
    while (queue->first != NULL) {
        to_free = queue->first;
        queue->first = to_free->prev;
        free(to_free->data);
        free(to_free);
    }
    pthread_cond_destroy(&queue->cond);
    pthread_mutex_destroy(&queue->lock);
    pthread_mutex_unlock(&queue->lock);
    free(Q);
}

int isEmpty(void *Q) {
    return ((pqueue) Q)->size == 0;
}

void enQ(void *Q, void *n) {

    pqueue queue = (pqueue) Q;
    pthread_mutex_lock(&queue->lock);
    pnode node = (pnode) malloc(NODE_SIZE);
    if (node == NULL) {
        perror("malloc ERROR");
        exit(1);
    }
    node->data = n;
    node->prev = NULL;
    if (queue->size == 0) {
        queue->first = node;
        queue->last = node;
    } else {
        queue->last->prev = node;
        queue->last = node;
    }
    queue->size += 1;

    pthread_cond_signal(&queue->cond); // signal to the thread who want to deQ the queue is not empty anymore
    pthread_mutex_unlock(&queue->lock);
}

void *deQ(void *Q) {
    pqueue queue = (pqueue) Q;
    pthread_mutex_lock(&queue->lock);
    // the while loop is because the thread can wake up while another thread already consume the queue,
    // so he needs to wait again to the queue to be fill
    while (isEmpty(Q)) {
        pthread_cond_wait(&queue->cond, &queue->lock); // wait for someone to signal the queue is not empty
    }
    pnode to_de = queue->first;
    queue->first = to_de->prev;
    void *data = to_de->data;
    free(to_de);
    queue->size -= 1;

    pthread_mutex_unlock(&queue->lock);
    return data;
}