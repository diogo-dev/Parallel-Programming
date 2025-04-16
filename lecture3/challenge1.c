/*
Desafio: Ao inves de criar 2 threads, crie uma quantidade (k) de threads definida
pelo usuário, ateh um maximo permitido. Ao inves de usar duas variaveis thread1
e thread2, use um vetor de threads. Considere que cada thread deve calcular uma
fatorial em particular. Compile, execute e analise. Entregue no Moodlep o arquivo
fonte.
*/

// Diogo Felipe Soares da Silva, RA: 124771

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define MAX_TAM_ARRAY 100

void *fatorial(void *ptr);

typedef struct {
    char nome[10];
    int n, fat;
} tipoFatData;

int main()
{
    pthread_t thread_array[MAX_TAM_ARRAY];
    tipoFatData fatData[MAX_TAM_ARRAY];
    int tam_array;

    printf("Digite a quantidade de threads desejada (Maximo 100 threads):");
    scanf("%d", &tam_array);

    for (int i = 0; i < tam_array; i++) {
        // Inicializando o fatData para cada thread
        sprintf(fatData[i].nome, "Thread %d", i);
        fatData[i].fat = 1;
        // O valor 'n' do fatorial será preenchido com os valores de 0 ate tam_array
        fatData[i].n = i;
        pthread_create(&thread_array[i], NULL, fatorial, (void *) &fatData[i]);
    }

    for (int i = 0; i < tam_array; i++) {
        pthread_join(thread_array[i], NULL);
    }

    printf("\n");
    for (int i = 0; i < tam_array; i++) {
        printf("\n%s: Fat(%d) = %d\n", fatData[i].nome, fatData[i].n, fatData[i].fat);
    }

    return 0;
}

void *fatorial(void *ptr)
{    int i;

    tipoFatData *fatData;

    fatData = (tipoFatData *) ptr;

    printf("\n%s: vou calcular uma fatorial", fatData->nome);

    for (i=fatData->n; i>1; i--)
        (fatData->fat) = (fatData->fat) * i;

}