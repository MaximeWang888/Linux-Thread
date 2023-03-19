#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10 // taille du tampon

sem_t mutex_sem; // sémaphore pour le mutex
sem_t empty_sem; // sémaphore pour la case vide
sem_t full_sem; // sémaphore pour la case pleine

int tampon[N]; // le tampon partagé
int tete = 0; // indice de la première case libre
int queue = 0; // indice de la première case occupée

#define P(s) sem_wait(&s)
#define V(s) sem_post(&s)

void ecrire_tampon(int data) {
    P(empty_sem); // attendre une case vide
    P(mutex_sem); // entrer en section critique
    tampon[tete] = data;
    tete = (tete + 1) % N; // avancer la tête circulairement
    V(mutex_sem); // sortir de la section critique
    V(full_sem); // signaler une case pleine
}

int lecture_tampon() {
    int data;
    P(full_sem); // attendre une case pleine
    P(mutex_sem); // entrer en section critique
    data = tampon[queue];
    queue = (queue + 1) % N; // avancer la queue circulairement
    V(mutex_sem); // sortir de la section critique
    V(empty_sem); // signaler une case vide
    return data;
}

void *ecrivain(void *arg) {
    int data;
    for (int i = 0; i < 20; i++) { // écrire 20 données
        data = i + 1;
        printf("[ecrivain] Ecriture de la donnée %d\n", data);
        ecrire_tampon(data);
    }
    pthread_exit(NULL);
}

void *lecteur(void *arg) {
    int data;
    for (int i = 0; i < 20; i++) { // lire 20 données
        data = lecture_tampon();
        printf("[lecteur] Lecture de la donnée %d\n", data);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[2];
    sem_init(&mutex_sem, 0, 1);
    sem_init(&empty_sem, 0, N);
    sem_init(&full_sem, 0, 0);
    pthread_create(&threads[0], NULL, ecrivain, NULL);
    pthread_create(&threads[1], NULL, lecteur, NULL);
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
    sem_destroy(&mutex_sem);
    sem_destroy(&empty_sem);
    sem_destroy(&full_sem);
    return 0;
}
