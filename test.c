#include <assert.h>
#include <unistd.h>
#include "queue.h"
#include "active_object.h"
#include "reactor.hpp"

#define DEFAULT_COLOR "\033[0m"
#define PURPLE "\033[1;35m"

void print_success(char *str) {
    printf("%s", PURPLE);
    printf("%s", str);
    printf("%s", DEFAULT_COLOR);
}

void print_sync_test(pqueue queue) { // for the queue sync tests
    int i = 0;
    int *data;
    while (i < 5) {
        data = (int *) deQ(queue);
        printf("%d\n", *data);
        ++i;
    }
}

void plus_AO_test(void *a) { // for the active object testing
    int *b = (int *) a;
    *b = *b + 2;
}

void print_AO_test(void *a) { // for the active object testing
    int *b = (int *) a;
    printf("%d\n", *b);
}

int main() {

    {/// basic queue operation testing
        int *a = (int *) malloc(sizeof(int) * 10);
        if (a == NULL) {
            perror("malloc ERROR");
            exit(1);
        }
        pqueue queue = (pqueue) createQ();

        for (int i = 0; i < 10; ++i) {
            a[i] = i;
        }

        for (int i = 9; i >= 0; --i) {
            enQ((void *) queue, (void *) (a + i));
        }

        int *b;
        for (int i = 9; i >= 0; --i) {
            b = (int *)((void *) deQ((void *) queue));
            assert(*b == i);
        }
        destoryQ((void *) queue);
        free(a);
        print_success("Basic queue operation tests end successfully!\n");
    }


    {/** queue sync with conditional variable testing
         This test check whether the thread wait to print what's inserted into the queue or not
         in successes it should print the numbers 0-4 with 1 second interval
         and a success message in purple color */

        pthread_t thread;
        pthread_attr_t attr; // thread's attribute
        // make the threads attribute in DETACHED mode for valgrind check not to yell
        // and because we don't join
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        int *a = (int *) malloc(sizeof(int) * 5);
        if (a == NULL) {
            perror("malloc ERROR");
            exit(1);
        }
        for (int i = 0; i < 5; ++i) {
            a[i] = i;
        }
        pqueue queue = (pqueue) createQ();
        pthread_create(&thread, &attr, (void *(*)(void *))(print_sync_test), queue);
        for (int i = 0; i < 5; ++i) {
            enQ((void *) queue, (void *) (a + i));
            sleep(1);
        }
        print_success("Queue sync with conditional variable tests end successfully!\n");
        free(a);
        destoryQ((void *) queue);
    }


    {/** active object testing, this test create active object With two functions:
        (1) Add tow to every integer in the queue (2) print
        the functions like before will wait until something will inserted into the queue
        In success this test should print the numbers 2-6 (instead 0-4 like before)
        1 second interval and a success massage in purple color */

        int *a = (int *) malloc(sizeof(int) * 5);
        if (a == NULL) {
            perror("malloc ERROR");
            exit(1);
        }
        for (int i = 0; i < 5; ++i) {
            a[i] = i;
        }

        pqueue queue = (pqueue) createQ();
        p_active_object pao = newAO((void *) queue, plus_AO_test, print_AO_test);
        for (int i = 0; i < 5; ++i) {
            enQ((void *) queue, (void *) (a + i));
            sleep(1);
        }
        destroyAO(pao);
        free(a);
        print_success("Active Object tests end successfully!\n");
    }

    return 0;
}