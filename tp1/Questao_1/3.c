#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct carrinho {
    int itens;
    int valor;
};

struct carrinho car;

void *adiciona(void *i) {
    int aux;
    printf("[ESCRITORA] Antes de adicionar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    car.itens++;
    aux = car.valor;
    aux += *((int *)i);
    sleep(1); // Simula a perda de CPU
    car.valor += aux;
    printf("[ESCRITORA] Depois de adicionar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    return NULL;
}

void *retira(void *i) {
    int aux;
    printf("[ESCRITORA] Antes de retirar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    car.itens--;
    aux = car.valor;
    aux -= *((int *)i);
    sleep(1); 
    car.valor -= aux;
    printf("[ESCRITORA] Depois de retirar| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    return NULL;
}

void *consulta(void *arg) { 
    printf("[LEITORA] Carrinho| ITENS: %d VALOR: %d\n", car.itens, car.valor);
    return NULL;
}

int main() {
    car.itens = 0;
    car.valor = 0;
    int num_adiciona, num_retira, leitoras;

    printf("Insira a quantidade de itens que deseja adicionar, quantos deseja retirar e quantidade de threads leitoras: ");
    scanf("%d %d", &num_adiciona, &num_retira);

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
