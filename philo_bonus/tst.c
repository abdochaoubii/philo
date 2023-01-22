#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void child_function() {
	for(int i = 0; i < 5; i++) {
	printf("I am child process %d\n", getpid());
	usleep(505-i);
	}
	exit(0);
}

int main() {
	pid_t child_pid;
	int i;
	for(i = 0; i < 5; i++) {
		child_pid = fork();
		if (child_pid == 0) {
			// This is the child process
			child_function();
		} else {
            printf("Child Process %d with PID: %d\n", i+1, child_pid);

			// This is the parent press
		}
	}
	return 0;
}