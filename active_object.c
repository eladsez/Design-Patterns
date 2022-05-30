#include "active_object.h"

void activate(p_active_object pao){
    while (1){
        void * data = deQ(pao->queue);
        if (pao->func1 != NULL){
            pao->func1(data);
        }
        if (pao->func2 != NULL){
            pao->func2(data);
        }
    }
}

p_active_object newAO(void *Q, void(func1)(void *), void(func2)(void *)){

    p_active_object pao = (p_active_object) malloc(AO_SIZE);
    if (pao == NULL){
        perror("malloc ERROR");
        exit(1);
    }

    pthread_t thread;
    pthread_attr_t attr; // thread's attribute
    // make the threads attribute in DETACHED mode for valgrind check not to yell
    // and because we don't join
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pao->queue = (pqueue) Q;
    pao->func1 = func1;
    pao->func2 = func2;
    pao->thread = thread;
    if (pthread_create(&(pao->thread), &attr, activate, pao) != 0){
        perror("thread create error");
    }
    return pao;
}

void destroyAO(p_active_object pao){
    pthread_cancel(pao->thread);
    destoryQ(pao->queue);
    free(pao);
}