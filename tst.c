#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 10
#define NUM_MUTEXES 3

pthread_mutex_t *mutexes;
int shared_resources[NUM_MUTEXES] = {0, 0, 0};

void *increment_shared_resource(void *arg) {
    int id = *((int*)arg);
    int resource_id = id % NUM_MUTEXES;

    for (int i = 0; i < 10; i++) {
        // lock the mutex
        
        pthread_mutex_lock(&mutexes[resource_id]);

        // access the shared resource
        shared_resources[resource_id]++;
        printf("Thread %d: shared resource %d is now %d\n", id, resource_id, shared_resources[resource_id]);

        // release the mutex
        pthread_mutex_unlock(&mutexes[resource_id]);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    mutexes = malloc(sizeof(pthread_mutex_t) * NUM_MUTEXES);
    if(mutexes == NULL) {
        printf("malloc failed");
        return -1;
    }

    for (int i = 0; i < NUM_MUTEXES; i++) {
        // initialize the mutex
        
        pthread_mutex_init(&mutexes[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        // create a new thread and passing the thread id
        pthread_create(&threads[i], NULL, increment_shared_resource, &i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        // wait for the thread to finish
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_MUTEXES; i++) {
        // destroy the mutex
        pthread_mutex_destroy(&mutexes[i]);
    }

    free(mutexes);
}
