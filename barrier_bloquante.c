#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NB_THREADS 4

pthread_barrier_t barrier;

void *thread_func(void *arg);

int main() {
    pthread_t threads[NB_THREADS];
    int thread_nums[NB_THREADS];

    // Initialiser la barrière
    pthread_barrier_init(&barrier, NULL, NB_THREADS);

    printf("Barrière initialisée...\n");

    // Créer les threads
    for(int i = 0; i < NB_THREADS; i++) {
        thread_nums[i] = i + 1;
        if(pthread_create(&threads[i], NULL, thread_func, &thread_nums[i]) != 0) {
            fprintf(stderr, "Erreur lors de la création du thread %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Attendre que tous les threads terminent
    for(int i = 0; i < NB_THREADS; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Erreur lors de l'attente du thread %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Détruire la barrière
    pthread_barrier_destroy(&barrier);

    printf("Barrière détruite...\n");

    return 0;
}

void *thread_func(void *arg) {
    int thread_num = *(int *)arg;
    printf("Thread %d lancée.\n", thread_num);

    // Attendre que tous les threads atteignent la barrière
    pthread_barrier_wait(&barrier);

    printf("Thread %d continue. :-) \n", thread_num);

    pthread_exit(NULL);
}