#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define maxVet 10000
#define maxNumero 10000

// Diogo Felipe Soares da Silva, RA:124771

typedef int tipoVetor[maxVet];

tipoVetor A;
int tamA;
int limite; // limite para parar de criar threads

typedef struct {
    tipoVetor *array;
    int inicio;
    int fim;
} ThreadData;

void mergeMetades(tipoVetor V, int ini, int meio, int fim)
{   tipoVetor auxV1, auxV2;
    int tam1=0, tam2=0, maior, i, i1, i2;

    /* copia primeira metade V[ini..meio] para auxV1 */
    for (i=ini; i<=meio; i++)
    {
        auxV1[tam1]=V[i];
        tam1++;
    }

    /* copia segunda metade V[meio+1..fim] para auxV2 */
    for (i=meio+1; i<=fim; i++)
    {
        auxV2[tam2]=V[i];
        tam2++;
    }

    /* descobre um valor maior que todos do segmento */

    if (auxV1[tam1-1] > auxV2[tam2-1])
        maior = abs(auxV1[tam1-1])+1;
    else maior = abs(auxV2[tam2-1])+1;

    /* insere o valor maior que todos logo apos o ultimo de
       cada metade, o qual servira como uma sentinela/barreira */

    auxV1[tam1]=maior;
    auxV2[tam2]=maior;

    /* avanca em cada metade retirando o menor elemento e
       colocando de volta no segmento, para que todo o segmento
       fique ordenado */

    i1=0;
    i2=0;

    for (i=ini; i<=fim; i++)
    {
        if (auxV1[i1]<auxV2[i2])
        {
            V[i]=auxV1[i1];
            i1++;
        }
        else {
            V[i]=auxV2[i2];
            i2++;
        }
    }

}

void *mergeSortThreaded(void *arg);

void mergeSort(tipoVetor *V, int ini, int fim)
{
    if (fim - ini + 1 <= limite) {
        if (fim > ini) {
            int meio = (ini + fim) / 2;
            mergeSort(V, ini, meio);
            mergeSort(V, meio + 1, fim);
            mergeMetades(*V, ini, meio, fim);
        }
    } else {
        pthread_t t1, t2;
        ThreadData *left = malloc(sizeof(ThreadData));
        ThreadData *right = malloc(sizeof(ThreadData));

        int meio = (ini + fim) / 2;

        left->array = V;
        left->inicio = ini;
        left->fim = meio;

        right->array = V;
        right->inicio = meio + 1;
        right->fim = fim;

        pthread_create(&t1, NULL, mergeSortThreaded, left);
        pthread_create(&t2, NULL, mergeSortThreaded, right);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        mergeMetades(*V, ini, meio, fim);
    }
}

void *mergeSortThreaded(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    mergeSort(data->array, data->inicio, data->fim);
    free(data);
    return NULL;
}

int leTam(void)
{
    int tam;
    do {
        printf("\nDigite o tamanho do Vetor = ");
        scanf("%d", &tam);
    } while ((tam < 1) || (tam > maxVet));
    return tam;
}

void geraVetor(tipoVetor V, int tam)
{
    int i, sinal;
    srand(time(NULL));
    printf("\nGeracao Aleatoria do Vetor\n");
    for (i = 0; i < tam; i++) {
        V[i] = rand() % maxNumero;
        sinal = (rand() % 2) * 2 - 1;
        V[i] *= sinal;
    }
}

void mostraVetor(char *id, tipoVetor V, int tam)
{
    int i;
    printf("\nElementos do Vetor %s:\n", id);
    printf("%d", V[0]);
    for (i = 1; i < tam; i++) printf(", %d", V[i]);
    printf("\n");
}

int main(void)
{
    int resp;

    do {
        tamA = leTam();

        printf("Digite o limite de elementos por thread: ");
        scanf("%d", &limite);

        geraVetor(A, tamA);
        mostraVetor("A Original", A, tamA);

        printf("\nOrdenando A por Merge Sort com threads...\n");

        ThreadData *t_data = malloc(sizeof(ThreadData));
        t_data->array = &A;
        t_data->inicio = 0;
        t_data->fim = tamA - 1;

        mergeSortThreaded(t_data); // primeira chamada sem criar nova thread

        mostraVetor("A Ordenado", A, tamA);

        printf("\nDeseja Nova Execucao, <1>Sim <2>Nao? => ");
        scanf("%d", &resp);
    } while (resp == 1);

    return 0;
}
