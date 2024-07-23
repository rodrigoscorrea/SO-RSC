#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

int **A;
int **B;
int **C; // Matriz resultante
int ordem; // Tamanho da matriz
int numThreads; // Quantidade de threads

struct ThreadArgs {
    int startRow;
    int endRow;
    int threadNumber; // Número da thread
};

void *multiplyRows(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    int startRow = args->startRow;
    int endRow = args->endRow;
    int threadNumber = args->threadNumber;

    for (int row = startRow; row <= endRow; row++) {
        for (int k = 0; k < ordem; k++) {
            for (int col = 0; col < ordem; col++) {
                C[row][col] += A[row][k] * B[k][col];
            }
        }
        //printf("Thread %d executando: Linha %d\n", threadNumber, row);
    }

    pthread_exit(NULL);
}

void multiplyMatrices(int **A, int **B, int **C) {
    for (int i = 0; i < ordem; i++) {
        for (int j = 0; j < ordem; j++) {
            C[i][j] = 0;
            for (int k = 0; k < ordem; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1e6;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <tamanho da matriz> <número de threads>\n", argv[0]);
        return 1;
    }

    ordem = atoi(argv[1]);
    numThreads = atoi(argv[2]);

    srand(time(NULL)); // Inicializa a semente para números aleatórios

    // Aloca memória para as matrizes A, B e C
    A = (int **)malloc(ordem * sizeof(int *));
    B = (int **)malloc(ordem * sizeof(int *));
    C = (int **)malloc(ordem * sizeof(int *));
    for (int i = 0; i < ordem; i++) {
        A[i] = (int *)malloc(ordem * sizeof(int));
        B[i] = (int *)malloc(ordem * sizeof(int));
        C[i] = (int *)malloc(ordem * sizeof(int));
    }

    // Preenche as matrizes A e B com valores aleatórios
    for (int i = 0; i < ordem; i++) {
        for (int j = 0; j < ordem; j++) {
            A[i][j] = rand() % 10; // Valores aleatórios entre 0 e 9
            B[i][j] = rand() % 10;
        }
    }

    double start_time_seq = get_time();
    multiplyMatrices(A, B, C);
    double end_time_seq = get_time();
    double execution_time = end_time_seq - start_time_seq;

    double start_time = get_time();
    pthread_t threads[numThreads];
    struct ThreadArgs threadArgs[numThreads];
    // Divide as linhas entre as threads
    int rowsPerThread = ordem / numThreads;
    for (int i = 0; i < numThreads; i++) {
        threadArgs[i].startRow = i * rowsPerThread;
        threadArgs[i].endRow = (i + 1) * rowsPerThread - 1;
        threadArgs[i].threadNumber = i; // Atribui o número da thread
        pthread_create(&threads[i], NULL, multiplyRows, &threadArgs[i]);
    }

    // Aguarda as threads terminarem
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    double end_time = get_time();
    double parallel_execution_time = end_time - start_time;
    printf("Tempo de execução da multiplicação paralela em %.6f segundos\n", parallel_execution_time);
    printf("Tempo de execução da multiplicação sequencial: %.6f segundos\n", execution_time);

    // Libera a memória
    for (int i = 0; i < ordem; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
