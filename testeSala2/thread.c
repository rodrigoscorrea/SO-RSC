#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define N 10000000
#define NUM_THREADS 12 // num_threads tem que ser divisor de N

int v1[N], v2[N], resultado, soma[NUM_THREADS];

void gera_vetores()
{  int i, x;
   srand(time(NULL));
   for(i=0;i<N;i++) 
   {   x = rand() % N;
       v1[i] = x;
       x = rand() % N;
       v2[i] = x;
   }
}

void mostra_vetor(int v[])
{  int i;
   for(i=0;i<N;i++)
       printf("v[%d]= %d  ",i,v[i]);
   printf("\n");
}

void *produto_escalar(void *t)
{  int i, fator, inicio, fim;
   long tid = (long)t;
   soma[tid] = 0;
   fator = N/NUM_THREADS;
   inicio = tid*fator;
   fim = tid*fator+fator;
   printf("tid: %ld  inicio= %d  fim = %d\n", tid, inicio, fim);
   for(i=inicio;i<fim;i++)
      soma[tid] = soma[tid] + sin(v1[i])*cos(v2[i]);
}

int main(int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   pthread_attr_t attr;
   int i, rc;
   long t;

   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   gera_vetores();


   struct timeval  tv1, tv2;
   gettimeofday(&tv1, NULL);

   for(t=0;t<NUM_THREADS;t++) 
   {  rc = pthread_create(&thread[t], &attr, produto_escalar, (void *)t); 
      if (rc) 
      {   printf("ERRO; código = %d\n", rc);
          exit(-1);
      }
   }
   for(t=0;t<NUM_THREADS;t++) 
      pthread_join(thread[t], NULL);
   
   for(t=0;t<NUM_THREADS;t++) 
      resultado = resultado + soma[t]; 

   gettimeofday(&tv2, NULL);
   printf ("Tempo de execucao: %f segundos\n",
            (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec));
   printf("\n");

//   mostra_vetor(v1);
//   mostra_vetor(v2);

   printf("Produto Escalar = %d", resultado);
   printf("\nMain terminando...\n");
   pthread_exit(NULL);
   return 0;
}