#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 3

// barrière pour synchroniser les threads
pthread_barrier_t barriere;

// fonction exécutée par les threads
void *thread_func(void *arg);

// fonction exécutée par le thread barrier_thread
void *barrier_thread_func(void *arg);

int main() {
    // Initialisation de la barrière avec N+1 threads
    pthread_barrier_init(&barriere, NULL, N+1);

    // Création des threads
    pthread_t threads[N], barrier_thread;
    int ids[N];
    for (int i = 0; i < N; i++) {
        ids[i] = i+1;
        pthread_create(&threads[i], NULL, thread_func, (void *)&ids[i]);
    }
    pthread_create(&barrier_thread, NULL, barrier_thread_func, NULL);

    // Attente de la fin des threads
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_join(barrier_thread, NULL);

    // Destruction de la barrière
    pthread_barrier_destroy(&barriere);

    return 0;
}

void *thread_func(void *arg) {
    int id = *(int *)arg;
    // Simulation d'une attente aléatoire avant d'atteindre le point de synchronisation
    sleep(rand() % 5);
    printf("[thread %d] Point atteint\n", id);
    // Attente de tous les threads à la barrière
    pthread_barrier_wait(&barriere);
    // Attente de tous les threads à la barrière après l'affichage des points atteints
    pthread_barrier_wait(&barriere);
    pthread_exit(NULL);
}


void *barrier_thread_func(void *arg) {
    printf("[b_thread] Barrière atteinte, en attente...\n");
    // Attente de tous les threads à la barrière
    pthread_barrier_wait(&barriere);
    printf("[b_thread] Je peux continuer !\n");
    // Attente de tous les threads à la barrière après l'affichage de la confirmation de la barrière
    pthread_barrier_wait(&barriere);
    pthread_exit(NULL);
}