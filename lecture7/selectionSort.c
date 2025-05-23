/* Desafio: Modifique o exemplo para realizar a ordenacao sobre o mesmo vetor,
ou seja, sem usar vetor auxiliar com a copia dos dados. OBS: eh permitido usar
estruturas auxiliares apenas para controlar a ordenacao */

//  Diogo Felipe Soares da Silva, RA: 124771


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define maxVetor 1000

typedef int tipoVetor[maxVetor];

tipoVetor vetorA, vetorAux;

int iret1, iret2, id1=0, id2=1, tamVet;

// variaveis para implementar o algoritmo de dekker para a secao critica
// algoritmo mais eficiente
// lembrando que ainda nao estamos usando a estrutura pthread_mutex para fazer a sincrinizacao
// por isso, vamos nos limitar a usar algoritmos de sincronizacao de software como o de dekker

volatile int interesse[2] = {0, 0}; // comeca com [falso, falso]
volatile int vez = 0;

pthread_t thread1, thread2;

void *selecaoDireta(void *ptr);

void geraVetor(void)
{   int i;

    for (i=0; i<tamVet; i++) 
        vetorA[i]=rand()%100;
       
}

void mostraVetor(char *nome, tipoVetor v)
{   int i;

    printf("\n\nVetor %s\n", nome);
    for (i=0; i<tamVet; i++)
        printf("%d, ", v[i]);
    printf("\n");

}

void trocaValores(int *vetor, int id_origem, int id_destino) {
    int aux = vetor[id_origem];
    vetor[id_origem] = vetor[id_destino];
    vetor[id_destino] = aux;
}

void entrada(int id)
{
    // Dekker: entrada na secao critica

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
}

void saida(int id)
{
    // Dekker: saida da secao critica

    // mudamos a vez para a outra thread
    vez = 1 - id;
    // tiramos nosso interesse de entrar na secao critica
    interesse[id] = 0;
}

void main(void)
{   
    int i;

    printf("\nDigite o tamanho do Vetor => ");
    scanf("%d", &tamVet);

    srand(time(NULL));
    geraVetor();

    mostraVetor("Vetor A", vetorA);

    iret1 = pthread_create(&thread1, NULL, selecaoDireta, (void *) &id1);
    iret2 = pthread_create(&thread2, NULL, selecaoDireta, (void *) &id2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    mostraVetor("Vetor A Ordenado", vetorA);

    printf("\n");

}

/*
    Problema que eh necessario solucionar quando usamos o mesmo vetor para fazer a ordenacao

    Exemplo do problema:
    1 - Thread 1 encontra o menor valor na posMenor = 4;
    2 - Antes de trocar, Thread 2 entra, troca valores em vetorA[4] e sai;
    3 - Agora posMenor não é mais o menor, mas Thread1 ainda troca posMenor com posDestino, corrompendo a ordenacao

    Solucao: a regiao critica deve conter busca e troca
*/


void *selecaoDireta(void *ptr)
{   
    int *id, aux, posDestino, posMenor, posVarredura, posMaior;

    id=(int *)ptr;

    if ((*id)==0)
    {
        printf("\n\nThread %d - entrei antes\n", *id);

        for (posDestino=0; posDestino<(tamVet/2); posDestino++)
        {   
            entrada(*id);

            posMenor=posDestino;
            for (posVarredura=posMenor+1; posVarredura<tamVet; posVarredura++)
            {
                if (vetorA[posMenor]>vetorA[posVarredura])
                    posMenor=posVarredura;
            }
            
            trocaValores(vetorA, posMenor, posDestino);
            saida(*id);
        }
        printf("\nThread %d finalizada\n", *id);

    }
    else
    {
        printf("\nThread %d - entrei depois\n", *id);

        for (posDestino=tamVet-1; posDestino>=(tamVet/2); posDestino--)
        {   
            entrada(*id);
            
            posMaior=posDestino;
            for (posVarredura=posMaior-1; posVarredura>=0; posVarredura--)
            {
                if (vetorA[posMaior]<vetorA[posVarredura])
                    posMaior=posVarredura;
            }
            
            trocaValores(vetorA, posMaior, posDestino);
            saida(*id);
        }
        printf("\nThread %d finalizada\n", *id);
    }

}


