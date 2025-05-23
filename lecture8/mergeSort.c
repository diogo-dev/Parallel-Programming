/* Aplicacao de Base: Implementa o metodo de ordenacao Merge Sort recursivo na
modalidade sequencial

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o
seguinte nome:

mergeSort.c

Depois, compile o programa em linha de comando da seguinte forma:

cc mergeSort.c -o mergeSort

E por fim execute da seguinte forma:

./mergeSort

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define maxVet 10000
#define maxNumero 10000
#define maxThreads

typedef struct {
    tipoVetor *array;
    int inicio;
    int fim;
} ThreadData;

pthread_t threads[maxThreads];
ThreadData t_dados;
int cont = 0; //responsavel por ser o contador desse vetor de ThreadData
int limite;

typedef int tipoVetor[maxVet];

tipoVetor A;

int tamA;

/* efetua a leitura do tamanho, considerando as restricoes
para valores negativos ou acima do limite maximo, e retorna
no nome da funcao */

int leTam(void)
{   int tam;

    do
    {  printf("\nDigite o tamanho do Vetor = ");
       scanf("%d", &tam);
    } while ((tam<1)||(tam>maxVet));

    return tam;
}

/* gera o vetor aleatoriamente, de acordo com o tamanho passado
   como parametro. O valores gerados podem ser positivos ou negativos
   e estao limitados a maxNumero. */

void geraVetor(tipoVetor V, int tam)
{   int i, sinal;

    srand(time(NULL));

    printf("\nGeracao Aleatoria do Vetor\n");

    for (i=0; i<tam; i++)
    {
        V[i]=rand()%maxNumero;
        sinal=(rand()%2)*2-1; /* se rand%2==0, sinal=-1;
                                 se rand%2==1, sinal=+1 */

        V[i]=sinal*V[i];  /* impoe o sinal no numero */
    }
}

/* mostra o vetor no video como uma sequencia de elementos
   separados por virgula */

void mostraVetor(char *id, tipoVetor V, int tam)
{   int i;

    printf("\nElementos do Vetor %s:\n", id);
    printf("%d",V[0]);
    for (i=1; i<tam; i++)
    {
        printf(", %d",V[i]);
    }
    printf("\n");
}

/*  junta/funde duas metades ja ordenadas dentro do segmento do
    vetor que vai de ini ateh fim. A primeira metade vai de ini
    ate meio, a segunda vai de meio+1 ate fim. Usa-se 2 vetores
    auxiliares para copiar as metades, percorre-las, comparando
    os elementos e devolver para o segmento original, para que
    este fique totalmente ordenado. */

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

/* solucao recursiva do Merge Sort para o segmento do vetor que vai de
   ini ate fim. Na primeira chamada, esse intervalo alcanca todo o vetor.
   Em cada chamada, se houver mais que 1 elemento no segmento, o mesmo
   eh dividido em duas metades e para cada metade a funcao eh chamada de
   novo (recursivamente). A recursividade prossegue ate que surgem metades
   unitarias, quando entao ocorre o retorno da recursividade. No retorno,
   metades unitarias criam segementos duplos, metades duplas criam segmentos
   quadruplos, metades quadruplas criam segmentos octuplos e assim
   sucessivamente ate que todo o vetor esteja ordenado. */

void mergeSort2(tipoVetor V, int ini, int fim)
{   
    int meio;
   
    if (fim>ini)
    {
        meio = (ini+fim)/2;
        mergeSort(V, ini, meio);
        mergeSort(V, meio+1, fim);
        mergeMetades(V,ini,meio,fim);
    }
   
}

void * mergeSort(void *ptr)
{   
    ThreadData * thread_dados = (ThreadData *) ptr;
    int meio;
    int inicio = thread_dados->inicio;
    int fim =  thread_dados->fim;

    if (thread_dados->fim > thread_dados->inicio)
    {
        meio = (thread_dados->inicio + thread_dados->fim) / 2;
        if (limite < meio) {
            cont++;
            thread_dados->fim = meio;
            pthread_create(&threads[cont], NULL, mergeSort, (void *)&thread_dados);
            cont++;
            thread_dados->inicio = meio + 1;
            thread_dados->fim = fim;
            pthread_create(&threads[cont], NULL, mergeSort, (void *)&thread_dados);
            mergeMetades(thread_dados->array , thread_dados->inicio , meio, thread_dados->fim);
        } else {
            // seguir sem criar novas threads
            // chamar mergesort2()
            mergeSort2(thread_dados->array, thread_dados->inicio, meio);
            mergeSort2(thread_dados->array, meio + 1, fim);
            mergeMetades(thread_dados->array , thread_dados->inicio , meio, thread_dados->fim);
        }
    }

}

void main(void)
{ 
    int resp;

  do
  {
    printf("Entre com o valor limite da recursividade!");
    scanf("%d", &limite);

    // lendo o tamanho do vetor que sera ordenado
    tamA = leTam();

    geraVetor(A, tamA);

    mostraVetor("A Original", A, tamA);

    printf("\nOrdenando A por Merge Sort ...\n");

    t_dados.array = A;
    t_dados.inicio = 0;
    t_dados.fim = tamA;
    pthread_create(&threads[cont], NULL, mergeSort, (void *)&t_dados);

    mostraVetor("A Ordenado", t_dados.array, tamA);

    printf("\nDeseja Nova Execucao, <1>Sim <2>Nao? => ");
    scanf("%d",&resp);

  } while (resp==1);
}

/*

Desafio: Ajuste a implementacao para fazer a funcao MergeSort ser executada por
varias threads em conjunto, de acordo as chamadas recursivas, ou seja, cada
chamada recursiva deve disparar uma thread. Durante a recursividade do MergeSort, 
quando o intervalo de elementos atingir um valor menor do que este limite, a thread 
deve seguir sozinha com sua recursividade, sem criar novas threads. Peça esse
limite para o usuário.

Obs:

#include <pthread.h>

int pthread_create(pthread_t *restrict thread,
                   NULL,
                   void *(*start_routine)(void *),
                   void *restrict arg);

int pthread_join(pthread_t thread, NULL);

*/
