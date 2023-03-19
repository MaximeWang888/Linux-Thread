#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


void *jeEtMes();
void *synch();
pthread_mutex_t mutex1,mutex2;

int main()
{
    int pthread_mutex_init(pthread_mutex_t *mutex1, const pthread_mutexattr_t *mutexattr);
    int pthread_mutex_init(pthread_mutex_t *mutex2, const pthread_mutexattr_t *mutexattr);
    pthread_mutex_lock(&mutex2);
    pthread_mutex_lock(&mutex1);
    pthread_t JeEtMesThread, SynchThread;
    pthread_create(&JeEtMesThread, NULL, jeEtMes, NULL);
    pthread_create(&SynchThread, NULL, synch, NULL);
    pthread_join(JeEtMesThread, NULL);
    pthread_join(SynchThread, NULL);

    return 0;
}

void *jeEtMes(){
    printf("je ");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_lock(&mutex2);
    printf(" mes ");
    pthread_mutex_unlock(&mutex1);
}

void *synch(){
    pthread_mutex_lock(&mutex1);
    printf(" synchronise ");
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_lock(&mutex1);
    printf(" threads \n");
}

