#include "active_object.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>


#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold

pqueue pq1;
pqueue pq2;
pqueue pq3;

typedef struct ao_args {
    int fd;
    char *str;
    int stage;
    int bytes_recv;
} *p_args;

void free_args(p_args args) {
    free(args->str);
    free(args);
}

void send_to_client(p_args args){
    int fd = args->fd;
    if (send(fd, args->str, args->bytes_recv, 0) == -1) {
        perror("socket send at top");
    }
}

/**
 * This function take args struct and forward it to the next active object
 * @param args
 */
void to_next_ao(p_args args) {
    if (args->stage == 1) {
        enQ(pq1, args);
    }
    if (args->stage == 2) {
        enQ(pq2, args);
    }
    if (args->stage == 3) {
        enQ(pq3, args);
    }
}

/**
 * This function take a string and replace every caps letter with lower letter and vice versa
 * @param args
 */
void reverse_caps(p_args args) {
    char *str = args->str;
    for (int i = 0; i < args->bytes_recv; ++i) {
        if (isupper(str[i])) {
            str[i] = (char) tolower(str[i]);
        } else {
            str[i] = (char) toupper(str[i]);
        }
    }
    args->stage = 3;
}


/**
 * This function take a string and activate Caesar cipher on it as the question demand
 * @param str
 */
void caesar_cipher(p_args args) {
    char *str = args->str;
    for (int i = 0; i < args->bytes_recv; ++i) {
        if (isalpha(str[i])) {
            str[i] += 1;
        }

        if (str[i] > 90 && str[i] < 97) { // is passed Z
            str[i] -= 26;
        }

        if (str[i] > 122) // is passed z
            str[i] -= 26;
    }
    args->stage = 2;
}


void sigchld_handler(int s) {
    // waitpid might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get stockade, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void *client_handle(void *fd) {
    char buff[1024];
    int cli_fd = *(int *) fd;
    int bytes;
    while (1) {
        memset(buff, '\0', 1024);
        bytes = (int)recv(cli_fd, buff, sizeof(buff), 0);
        if (bytes < 0) {
            printf("%d ERROR client recv", errno);
            break;
        } else if (bytes == 0) {
            printf("seems like one of the clients has disconnect\n");
            break;
        }
        else{
            p_args args = (p_args) malloc(sizeof (struct ao_args));
            if (args == NULL){
                perror("malloc error");
            }
            char *str = (char *) malloc(bytes);
            memset(str, '\0', bytes);
            strcpy(str, buff);
            args->stage = 1;
            args->fd = cli_fd;
            args->str = str;
            args->bytes_recv = bytes;
            to_next_ao(args);
        }
    }
    free(fd);
    return NULL;
}


int main(void) {

    pq1 = createQ();
    pq2 = createQ();
    pq3 = createQ();

    p_active_object pao1 = newAO(pq1, (void (*)(void *)) caesar_cipher, (void (*)(void *)) to_next_ao);
    p_active_object pao2 = newAO(pq2, (void (*)(void *)) reverse_caps, (void (*)(void *)) to_next_ao);
    p_active_object pao3 = newAO(pq3, (void (*)(void *)) send_to_client, (void (*)(void *)) free_args);

    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction SIGCHLD");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        int *args = (int *) malloc(sizeof(int));
        if (args == NULL)
            perror("malloc");
        else *args = new_fd;

        pthread_t cli_threadID;
        pthread_attr_t attr; // thread's attribute

        // make the threads attribute in DETACHED mode for valgrind check
        // and because we don't join()
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        if (pthread_create(&cli_threadID, &attr, client_handle, args) != 0) {
            perror("create thread");
            free(args);
        }

    }

    return 0;
}