/*

Exemplo 05: implementa desafio 02 do exemplo 03: testar 2 threads calculando o
n-ésimo termo da sequencia de Fibonacci em conjunto (cooperada).


===========
Experimento
===========

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o se -
guinte nome:

exemplo05.c

Depois, compile o programa em linha de comando da seguinte forma:

cc -lpthread exemplo05.c -o exemplo05  {ou use -pthread ao inves de -lpthread}

Corrija os erros de compilacao. Quando a compilacao for bem sucedida, o arquivo
executavel "exemplo05" serah criado.

Entao, execute o executavel com a seguinte linha de comando:

./exemplo05

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define maxElem 50

void *fibonacci(void *ptr);

int fib[maxElem], n;

void main(void)
{
     pthread_t thread1, thread2;
     int  iret1, iret2, id1=1, id2=2, i;

     printf("\nDigite n > 0 = ");
     scanf("%d",&n);
     while (n<1)
     {  printf("\nDigite n > 0 = ");
        scanf("%d",&n);
     }

     for (i=0; i<n; i++)
        fib[i]=0;

     fib[0]=1;
     fib[1]=1;
     fib[2]=2;

     if (n>3)
     {
        iret1 = pthread_create(&thread1, NULL, fibonacci, (void*) &id1);
        iret2 = pthread_create(&thread2, NULL, fibonacci, (void*) &id2);
     }

     if (n>3)
     {
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
     }

     printf("\nSequencia de Fibonacci: \n");
     for (i=0; i<n; i++)
        printf("%d, ", fib[i]);
     printf("\n\n%d-esimo elemento de Fibonacci: %d\n",n, fib[n-1]);
     exit(0);
}

void *fibonacci(void *ptr)
{    int *id, pos;

     id = (int *) ptr;

     printf("\nThread %d iniciada!\n", *id);

     pos=(*id)-1;

     while (pos<n-2)
     {
        pos=pos+2;
        while (fib[pos-1]==0);
        fib[pos]=fib[pos-1]+fib[pos-2];
     }

}

/*

Desafio: Ao inves de criar 2 threads, crie uma quantidade de threads definida
pelo usuario (k), ateh um maximo permitido. Todas as k threads devem resolver
o mesmo problema, trabalhando juntas/cooperadas. Ajuste o codigo. Compile,
execute e analise.

*/

