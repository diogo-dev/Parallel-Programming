/*

Exemplo 04: implementa desafio 01 do exemplo 03: testar 2 threads calculando fa-
torial.


===========
Experimento
===========

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o se -
guinte nome:

exemplo04.c

Depois, compile o programa em linha de comando da seguinte forma:

cc -lpthread exemplo04.c -o exemplo4  {ou use -pthread ao inves de -lpthread}

Corrija os erros de compilacao. Quando a compilacao for bem sucedida, o arquivo
executavel "exemplo04" serah criado.

Entao, execute o executavel com a seguinte linha de comando:

./exemplo04

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *fatorial(void *ptr);

struct tipoFatData {
   char nome[10];
   int n, fat;
};

typedef struct tipoFatData tipoFatData;

void main(void)
{
     pthread_t thread1, thread2;

     int  iret1, iret2;

     tipoFatData fatData1, fatData2;

     strcpy(fatData1.nome,"Thread 1");
     fatData1.fat=1;

     strcpy(fatData2.nome,"Thread 2");
     fatData2.fat=1;

     printf("\n%s: Digite n > ", fatData1.nome);
     scanf("%d",&(fatData1.n));

     printf("\n%s: Digite n > ", fatData2.nome);
     scanf("%d",&(fatData2.n));

     iret1 = pthread_create(&thread1, NULL, fatorial, (void*) &fatData1);

     iret2 = pthread_create(&thread2, NULL, fatorial, (void*) &fatData2);

     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);

     printf("\nThread 1: Fat(%d) = %d\n",fatData1.n, fatData1.fat);
     printf("\nThread 2: Fat(%d) = %d\n",fatData2.n, fatData2.fat);
     exit(0);
}

void *fatorial(void *ptr)
{    int i;

     tipoFatData *fatData;

     fatData = (tipoFatData *) ptr;

     printf("\n%s: vou calcular uma fatorial", fatData->nome);

     for (i=fatData->n; i>1; i--)
	(fatData->fat) = (fatData->fat) * i;

}

/*

Desafio: Ao inves de criar 2 threads, crie uma quantidade (k) de threads definida
pelo usuário, ateh um maximo permitido. Ao inves de usar duas variaveis thread1
e thread2, use um vetor de threads. Considere que cada thread deve calcular uma
fatorial em particular. Compile, execute e analise. Entregue no Moodlep o arquivo
fonte.

*/