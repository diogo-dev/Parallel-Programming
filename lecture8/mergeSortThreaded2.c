#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define maxVet 10000
#define maxNumero 10000

// Diogo Felipe Soares da Silva, RA:124771

// variaveis para implementar o algoritmo de dekker para a secao critica
volatile int interesse[2] = {0, 0}; // comeca com [falso, falso]
volatile int vez = 0;

typedef int tipoVetor[maxVet];

tipoVetor A;
int tamA;
int limite; // limite para parar de criar threads

typedef struct {
    tipoVetor *array;
    int inicio;
    int fim;
    int id; // 0 ou 1
} ThreadSelecaoData;

typedef struct {
    tipoVetor *array;
    int inicio;
    int fim;
} ThreadData;

void trocaValores(tipoVetor *vetor, int id_origem, int id_destino) {
    int aux = (*vetor)[id_origem];
    (*vetor)[id_origem] = (*vetor)[id_destino];
    (*vetor)[id_destino] = aux;
}

void *selecaoDiretaParcial(void *ptr)
{
    ThreadSelecaoData *data = (ThreadSelecaoData *)ptr;
    tipoVetor *vet = data->array;
    int ini = data->inicio;
    int fim = data->fim;
    int meio = (ini + fim) / 2;
    int id = data->id;

    if (id == 0) {
        for (int i = ini; i < meio; i++) {

            // Dekker: entrada na seção crítica

            // mostramos que temos interesse em entrar na secao critica
            interesse[id] = 1;
            // enquanto a outra thread tambem tem interrese na secao critica, fazemos:
            while (interesse[1 - id]) {
                // verificamos se eh ou nao nossa vez de entrar na secao critica
                // caso nao seja nossa vez, entramo no condicional
                if (vez != id) {
                    // tiramos nosso interesse na secao critica ate ser nossa vez de entrar na secao critica
                    interesse[id] = 0;
                    //espera ocupada
                    while (vez != id);
                    // como agora eh nossa vez, voltamos com o interesse
                    interesse[id] = 1;
                }
            }

            // Região crítica
            int min = i;
            for (int j = i + 1; j <= fim; j++) {
                if ((*vet)[j] < (*vet)[min])
                    min = j;
            }
            if (min != i)
                trocaValores(vet, i, min);

            // Dekker: saída

            // mudamos a vez para a outra thread
            vez = 1 - id;
            // tiramos nosso interesse de entrar na secao critica
            interesse[id] = 0;
        }
    } else {
        for (int i = fim; i >= meio; i--) {

            // Dekker: entrada na seção crítica
            interesse[id] = 1;
            while (interesse[1 - id]) {
                if (vez != id) {
                    interesse[id] = 0;
                    while (vez != id);
                    interesse[id] = 1;
                }
            }

            // Região crítica
            int max = i;
            for (int j = i - 1; j >= ini; j--) {
                if ((*vet)[j] > (*vet)[max])
                    max = j;
            }
            if (max != i)
                trocaValores(vet, i, max);

            // Dekker: saída
            vez = 1 - id;
            interesse[id] = 0;
        }
    }

    return NULL;
}

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
        pthread_t t1, t2;

        ThreadSelecaoData *left = malloc(sizeof(ThreadSelecaoData));
        ThreadSelecaoData *right = malloc(sizeof(ThreadSelecaoData));

        left->array = V;
        left->inicio = ini;
        left->fim = fim;
        left->id = 0;

        right->array = V;
        right->inicio = ini;
        right->fim = fim;
        right->id = 1;

        pthread_create(&t1, NULL, selecaoDiretaParcial, left);
        pthread_create(&t2, NULL, selecaoDiretaParcial, right);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

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
