/*

Exemplo 06: implementacao do desafio do exemplo 04: k threads calculando uma fa-
torial diferente cada uma.

===========
Experimento
===========

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o se -
guinte nome:

exemplo06.c

Depois, compile o programa em linha de comando da seguinte forma:

cc -lpthread exemplo06.c -o exemplo06  {ou use -pthread ao inves de -lpthread}

Corrija os erros de compilacao. Quando a compilacao for bem sucedida, o arquivo
executavel "exemplo06" serah criado.

Entao, execute o executavel com a seguinte linha de comando:

./exemplo06

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *funcaoCalculaFatorial(void *ptr);

struct tipoPack {
   char mensagem[16];
   int n;
   double fat;
};

typedef struct tipoPack tipoPack;

void main(void)
{
     pthread_t thread[50];

     int  i, iret[50], nthreads;
     char str_nt[3];

     tipoPack pack[50];

     printf("\nEntre com o numero de threads => ");
     scanf("%d", &nthreads);

     for (i=0; i<nthreads; i++)
     {
            sprintf(str_nt,"%d", i);
            strcpy(pack[i].mensagem,"\nSou a thread ");
            strcat(pack[i].mensagem, str_nt);
            pack[i].fat= 1.0;
            printf("\n%s: Entre com o n para o calculo da fatorial => ", pack[i].mensagem);
            scanf("%d", &(pack[i].n));
     }

     for (i=0; i<nthreads; i++)
        iret[i] = pthread_create(&(thread[i]), NULL, funcaoCalculaFatorial, (void*) &pack[i]);

     for (i=0; i<nthreads; i++)
        pthread_join(thread[i], NULL);

     for (i=0; i<nthreads; i++)
        printf("\nThread %d fatorialou: %lf\n",i, pack[i].fat);

     exit(0);
}

void *funcaoCalculaFatorial(void *ptr)
{    int i;

     tipoPack *pack;

     pack= (tipoPack *) ptr;

     printf("%s: vou calcular a fatorial de %d\n", pack->mensagem, pack->n);

     for (i=1; i<=(pack->n); i++)
	     (pack->fat) = (pack->fat) * i;

}

/* Desafio: modifique o codigo para calcular uma mesma fatorial com k threads.

Cada thread calcula o seu intervalo de fatorial e no final, a
thread main() ira calcula a "fatorial" total por meio de uma multiplicacao de todas
as fatoriais individuais. Por exemplo, 10! (fatorial de 10) eh calculada da seguinte
forma:

10!=10*9*8*7*6*5*4*3*2*1

ou ainda

 = (10*9*8)*(7*6*5*4)*(3*2*1)

assim, nesse exemplo, poderiamos dividir o calculo da fatorial entre 3 threads,
cada uma calculando uma parte diferente da fatorial total, por exemplo, a thread 1
calcula o produto 10*9*8, a threads 2 calcula 7*6*5*4 e a thread 3 calcula 3*2*1 e
ao final multiplicamos todos os valores parciais em um �nico produto.

*/


