#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define MAX_BUFFER 5
#define NUM_PRODUTORES 3
#define NUM_CONSUMIDORES 2
#define QTD_PROD 2


int buffer[MAX_BUFFER];
int count = 0;
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

void *produtor(void *arg) {
    int item, i;
    for (i = 0; i < QTD_PROD; i++) { 
        item = rand() % 100;
        sem_wait(&empty); 
        pthread_mutex_lock(&mutex); 
        buffer[in] = item;
        printf("Produtor %d produzindo %d\n", *(int*)arg, item);
        printf("Buffer: ");
        for (int j = 0; j < MAX_BUFFER; j++) {
            printf("%d ", buffer[j]);
        }
        sleep(2);
        in = (in + 1) % MAX_BUFFER;
        count++;
        pthread_mutex_unlock(&mutex); 
        sem_post(&full); 
        printf("\nProdutor %d dormindo\n\n", *(int*)arg);
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    int item, i;
    int proporcaoConsumidor = QTD_PROD * NUM_PRODUTORES;
    for (i = 0; i < (proporcaoConsumidor/ NUM_CONSUMIDORES); i++) { // Ajuste para consumir proporcionalmente
        sem_wait(&full); 
        pthread_mutex_lock(&mutex); 
        item = buffer[out];
        buffer[out] = 0;
        printf("Consumidor %d consumindo %d do buffer[%d]\n", *(int*)arg, item, out);
        buffer[out] = 0;
        printf("Buffer: ");
        for (int j = 0; j < MAX_BUFFER; j++) {
            printf("%d ", buffer[j]);
        }
        sleep(1);
        out = (out + 1) % MAX_BUFFER;
        count--;
        pthread_mutex_unlock(&mutex); 
        sem_post(&empty); 
        printf("\nConsumidor %d dormindo\n\n", *(int*)arg);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod[NUM_PRODUTORES], cons[NUM_CONSUMIDORES];
    int i, rc, a[NUM_PRODUTORES];

    sem_init(&empty, 0, MAX_BUFFER);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < NUM_PRODUTORES; i++) {
        a[i] = i + 1;
        rc = pthread_create(&prod[i], NULL, produtor, (void *)&a[i]);
    }

    for (i = 0; i < NUM_CONSUMIDORES; i++) {
        rc = pthread_create(&cons[i], NULL, consumidor, (void *)&a[i]);
    }

    for (i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(prod[i], NULL);
    }
    for (i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(cons[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
