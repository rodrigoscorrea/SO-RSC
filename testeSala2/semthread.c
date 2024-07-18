#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define N 10000000

int v1[N], v2[N], resultado;

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

void produto_escalar()
{  int i;
   resultado = 0;
   for(i=0;i<N;i++)
      resultado = resultado + sin(v1[i])*cos(v2[i]);
}

int main(int argc, char *argv[])
{
   int i;
   gera_vetores();

   struct timeval  tv1, tv2;
   gettimeofday(&tv1, NULL);

   produto_escalar();

   gettimeofday(&tv2, NULL);
   printf ("Tempo de execucao: %f segundos\n",
            (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec));
   printf("\n");

//   mostra_vetor(v1);
//   mostra_vetor(v2);

   printf("Produto Escalar = %d", resultado);
   return 0;
}