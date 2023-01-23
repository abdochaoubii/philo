#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>

sem_t *sem;

void child_function() {
    sem_wait(sem); // wait for semaphore
    printf("I am child process %d\n", getpid());
    sem_post(sem); // release semaphore
    exit(0);
}

int main() {
    sem = sem_open("/mysem", O_CREAT, 0644, 1); // create/open semaphore
    pid_t child_pid;
    int i;
    for(i = 0; i < 5; i++) {
        child_pid = fork();
        if (child_pid == 0) {
            // This is the child process
            child_function();
        } else {
            // This is the parent process
        }
    }
    sem_close(sem); // close semaphore
    sem_unlink("/mysem"); //unlink/delete the semaphore
    return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void child_function() {
    printf("I am child process %d\n", getpid());
    exit(0);
}

int main() {
    pid_t child_pid;
    int i, status;
    for(i = 0; i < 5; i++) {
        child_pid = fork();
        if (child_pid == 0) {
            // This is the child process
            child_function();
        } else {
            // This is the parent process
        }
    }
    for (i = 0; i < 5; i++) {
        wait(&status);
    }
    printf("All child processes have completed\n");
    return 0;
}