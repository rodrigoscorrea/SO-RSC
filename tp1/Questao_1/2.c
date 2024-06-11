#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

int dinheiro = 0;
int countWriter;
sem_t semaforoL; // Semaphore for readers
sem_t semaforoE; // Semaphore for writers

typedef struct {
    int itens;
    int valor;
} carrinho;

carrinho car;

void *adiciona(void *arg) { // Writer
    int *valor = (int *)arg;
    sem_wait(&semaforoE);
    printf("(THREAD ESCRITORA) Antes de adicionar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    car.itens++;
    sleep(0.7);
    car.valor += *valor;
    printf("(THREAD ESCRITORA) Depois de adicionar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    countWriter--;
    sem_post(&semaforoE);
    sem_post(&semaforoL);
    return NULL;
}

void *retira(void *arg) { // Writer
    int *valor = (int *)arg;
    sem_wait(&semaforoE);
    printf("(THREAD ESCRITORA) Antes de retirar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    car.itens--;
    sleep(0.7);
    car.valor -= *valor;
    printf("(THREAD ESCRITORA) Depois de retirar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    countWriter--;
    sem_post(&semaforoE);
    sem_post(&semaforoL);
    return NULL;
}

void *consulta(void *arg) { // Reader
    while (countWriter != 0) {
        sem_wait(&semaforoL);
    }
    printf("(THREAD LEITORA) Carrinho| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    sem_post(&semaforoL);
    return NULL;
}

int main() {
    sem_init(&semaforoL, 0, 1);
    sem_init(&semaforoE, 0, 1);
    int leitoras, num_adiciona, num_retira;
    printf("Insira a quantidade de adiciona, retira e leitoras: ");
    scanf("%d %d %d", &num_adiciona, &num_retira, &leitoras);
    countWriter = num_adiciona + num_retira;

    pthread_t threads[num_adiciona + num_retira + leitoras];
    int add, rm;
    printf("Qual valor de adicionar e retirar: ");
    scanf("%d %d", &add, &rm);

    for (int i = 0; i < num_adiciona; i++) {
        pthread_create(&threads[i], NULL, adiciona, &add);
    }

    for (int i = num_adiciona; i < num_adiciona + num_retira; i++) {
        pthread_create(&threads[i], NULL, retira, &rm);
    }

    for (int i = num_adiciona + num_retira; i < num_adiciona + num_retira + leitoras; i++) {
        pthread_create(&threads[i], NULL, consulta, NULL); 
    }

    for (int i = 0; i < num_adiciona + num_retira + leitoras; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("VALORES FINAIS| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    return 0;
}
