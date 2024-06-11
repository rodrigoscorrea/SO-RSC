#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PRODUTORES 3 // Número de produtores
#define NUM_CONSUMIDORES 1 // Número de consumidores
#define TAM_BUFFER 10 // Tamanho do buffer
#define NUM_PRODUCOES 20 // Número de produções por produtor

int buffer[TAM_BUFFER];
int count = 0; // Contador de elementos no buffer
sem_t empty, full; // Semáforos para controle de concorrência
pthread_mutex_t mutex; // Mutex para exclusão mútua

void *produtor(void *arg) {
    int id = *(int*)arg;
    for (int i = 0; i < NUM_PRODUCOES; i++) {
        int item = rand() % 100; // Produz um item aleatório
        sem_wait(&empty); // Espera o buffer ter espaço
        pthread_mutex_lock(&mutex); // Entra na região crítica
        buffer[count++] = item; // Adiciona o item ao buffer
        printf("Produtor %d produziu: %d\n", id, item);
        pthread_mutex_unlock(&mutex); // Sai da região crítica
        sem_post(&full); // Sinaliza que o buffer não está mais vazio
        sleep(1); // Simula tempo de produção
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    int id = *(int*)arg;
    for (int i = 0; i < NUM_PRODUCOES; i++) {
        sem_wait(&full); // Espera o buffer ter um item
        pthread_mutex_lock(&mutex); // Entra na região crítica
        int item = buffer[--count]; // Remove o item do buffer
        printf("Consumidor %d consumiu: %d\n", id, item);
        pthread_mutex_unlock(&mutex); // Sai da região crítica
        sem_post(&empty); // Sinaliza que o buffer não está mais cheio
        sleep(0.1); // Simula tempo de consumo
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads_prod[NUM_PRODUTORES], threads_cons[NUM_CONSUMIDORES];
    int id_prod[NUM_PRODUTORES], id_cons[NUM_CONSUMIDORES];

    sem_init(&empty, 0, TAM_BUFFER); // Inicializa o semáforo empty com o tamanho do buffer
    sem_init(&full, 0, 0); // Inicializa o semáforo full com 0
    pthread_mutex_init(&mutex, NULL); // Inicializa o mutex

    // Cria as threads produtoras
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        id_prod[i] = i + 1;
        pthread_create(&threads_prod[i], NULL, produtor, &id_prod[i]);
    }

    // Cria as threads consumidoras
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        id_cons[i] = i + 1;
        pthread_create(&threads_cons[i], NULL, consumidor, &id_cons[i]);
    }

    // Espera as threads terminarem
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(threads_prod[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(threads_cons[i], NULL);
    }

    // Destrói os semáforos e o mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
