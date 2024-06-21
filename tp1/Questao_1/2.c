#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

int dinheiro = 0;
int countWriter;
sem_t semaforoLeitura; 
sem_t semaroEscrita;

typedef struct {
    int itens;
    int valor;
} carrinho;

carrinho car;

void *adiciona(void *arg) { 
    int *valor = (int *)arg;
    sem_wait(&semaroEscrita);
    printf("[ESCRITORA] Antes de adicionar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    car.itens++;
    sleep(1);
    car.valor += *valor;
    printf("[ESCRITORA] Depois de adicionar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    countWriter--;
    sem_post(&semaroEscrita);
    sem_post(&semaforoLeitura);
    return NULL;
}

void *retira(void *arg) { 
    int *valor = (int *)arg;
    sem_wait(&semaroEscrita);
    printf("[ESCRITORA] Antes de retirar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    car.itens--;
    sleep(1);
    car.valor -= *valor;
    printf("[ESCRITORA] Depois de retirar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    countWriter--;
    sem_post(&semaroEscrita);
    sem_post(&semaforoLeitura);
    return NULL;
}

void *consulta(void *arg) { 
    while (countWriter != 0) {
        sem_wait(&semaforoLeitura);
    }
    sleep(1);
    printf("[LEITORA] Carrinho| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    sem_post(&semaforoLeitura);
    return NULL;
}

int main() {
    sem_init(&semaforoLeitura, 0, 1);
    sem_init(&semaroEscrita, 0, 1);
    int leitoras, num_adiciona, num_retira;
    printf("Insira a quantidade de itens que deseja adicionar, quantos deseja retirar e quantidade de threads leitoras: ");
    scanf("%d %d %d", &num_adiciona, &num_retira, &leitoras);
    countWriter = num_adiciona + num_retira;

    pthread_t threads[num_adiciona + num_retira + leitoras];
    int add, rm;
    printf("Qual valor deseja adicionar e retirar?: ");
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
