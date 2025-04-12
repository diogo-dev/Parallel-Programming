#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *fatorial(void *ptr);

typedef struct {
   char mensagem[16];
   int tam, fat;
} tipoPack;

void main(void)
{
    pthread_t thread1, thread2;

    int  iret1, iret2;

    tipoPack pack1, pack2;

    strcpy(pack1.mensagem,"\nSou a thread 1");
    strcpy(pack2.mensagem,"\nSou a thread 2");

    printf("\nDigite o fatorial a ser calculado pela thread1: ");
    scanf("%d", &(pack1.tam));

    printf("\nDigite o fatorial a ser calculado pela thread2: ");
    scanf("%d", &(pack2.tam));

    iret1 = pthread_create(&thread1, NULL, fatorial, (void*) &pack1);

    iret2 = pthread_create(&thread2, NULL, fatorial, (void*) &pack2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("\nFatorial da Thread 1: %d\n",pack1.fat);
    printf("\nFatorial da Thread 2: %d\n",pack2.fat);
    exit(0);
}

void *fatorial(void *ptr) {
    tipoPack *pack = (tipoPack *) ptr;

    printf("%s: vou realizar o cálculo do fatorial de %d", pack->mensagem, pack->tam);

    pack->fat = 1;
    for (int i = 1; i <= pack->tam; i++) {
        pack->fat = i*pack->fat;
    }
}