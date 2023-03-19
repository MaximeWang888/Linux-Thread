#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

void *jeEtMes();
void *synch();
sem_t sem1, sem2;

int main()
{
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    pthread_t JeEtMesThread, SynchThread;
    pthread_create(&JeEtMesThread, NULL, jeEtMes, NULL);
    pthread_create(&SynchThread, NULL, synch, NULL);
    pthread_join(JeEtMesThread, NULL);
    pthread_join(SynchThread, NULL);

    return 0;
}

void *jeEtMes(){
    printf("je ");
    sem_post(&sem1);
    sem_wait(&sem2);
    printf(" mes ");
    sem_post(&sem1);
}

void *synch(){
    sem_wait(&sem1);
    printf(" synchronise ");
    sem_post(&sem2);
    sem_wait(&sem1);
    printf(" threads \n");
}
