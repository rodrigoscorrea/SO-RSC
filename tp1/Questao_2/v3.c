#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_BUFFER 5
#define NUM_PRODUTORES 3
#define NUM_CONSUMIDORES 2
#define QTD_PROD 2

int buffer[MAX_BUFFER];
int count = 0;
int in = 0, out = 0;

void *produtor(void *arg) {
    int item, i;
    for (i = 0; i < QTD_PROD; i++) { 
        item = rand() % 100;
        printf("Produtor %d tentando inserir %d no buffer[%d]\n", *(int*)arg, item, in);

        
        usleep(rand() % 1000);

        buffer[in] = item;
        printf("Produtor %d inseriu %d no buffer[%d]\n", *(int*)arg, item, in);
        in = (in + 1) % MAX_BUFFER;
        count++;
        printf("\nBuffer: ");
        for (int j = 0; j < MAX_BUFFER; j++) {
            printf("%d ", buffer[j]);
        }
        printf("\n\n");
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    int item, i;
    int proporcaoConsumidor = QTD_PROD * NUM_PRODUTORES;
    for (i = 0; i < (proporcaoConsumidor / NUM_CONSUMIDORES); i++) {
        printf("Consumidor %d tentando retirar do buffer[%d]\n", *(int*)arg, out);

        
        usleep(rand() % 1000);

        item = buffer[out];
        buffer[out] = 0;
        printf("Consumidor %d retirou %d do buffer[%d]\n", *(int*)arg, item, out);
        out = (out + 1) % MAX_BUFFER;
        count--;
        printf("\nBuffer: ");
        for (int j = 0; j < MAX_BUFFER; j++) {
            printf("%d ", buffer[j]);
        }
        printf("\n");

        sleep(1);  // Simulando tempo de processamento
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod[NUM_PRODUTORES], cons[NUM_CONSUMIDORES];
    int i, rc, a[NUM_PRODUTORES + NUM_CONSUMIDORES];

    for (i = 0; i < NUM_PRODUTORES; i++) {
        a[i] = i + 1;
        rc = pthread_create(&prod[i], NULL, produtor, (void *)&a[i]);
        if (rc) {
            printf("Erro ao criar thread produtor %d\n", i + 1);
            exit(-1);
        }
    }

    for (i = 0; i < NUM_CONSUMIDORES; i++) {
        a[NUM_PRODUTORES + i] = i + 1;
        rc = pthread_create(&cons[i], NULL, consumidor, (void *)&a[NUM_PRODUTORES + i]);
        if (rc) {
            printf("Erro ao criar thread consumidor %d\n", i + 1);
            exit(-1);
        }
    }

    for (i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(prod[i], NULL);
    }
    for (i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(cons[i], NULL);
    }

    return 0;
}
