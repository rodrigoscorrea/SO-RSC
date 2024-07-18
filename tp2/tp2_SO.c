#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 800
#define NUM_THREADS 4

struct thread_data {
    int row;
    int (*A)[N];
    int (*B)[N];
    int (*C)[N];
};

void fill_random_matrix(int matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

void multiply_matrices_sequential(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void *multiply_row(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int row = data->row;

    for (int j = 0; j < N; j++) {
        data->C[row][j] = 0;
        for (int k = 0; k < N; k++) {
            data->C[row][j] += data->A[row][k] * data->B[k][j];
        }
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    int A[N][N];
    int B[N][N];
    int C[N][N];

    fill_random_matrix(A);
    fill_random_matrix(B);

    clock_t start_time = clock();
    multiply_matrices_sequential(A, B, C);
    clock_t end_time = clock();

    double seq_execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Tempo de execução da multiplicação sequencial: %.6f segundos\n", seq_execution_time);

    pthread_t threads[NUM_THREADS];
    struct thread_data thread_data_array[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data_array[i].row = i;
        thread_data_array[i].A = A;
        thread_data_array[i].B = B;
        thread_data_array[i].C = C;
        pthread_create(&threads[i], NULL, multiply_row, (void *)&thread_data_array[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    start_time = clock();
    double par_execution_time = (double)(start_time - end_time) / CLOCKS_PER_SEC;

    printf("Tempo de execução da multiplicação paralela com %d threads: %.6f segundos\n", NUM_THREADS, par_execution_time);

    return 0;
}
