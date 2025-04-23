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
ao final multiplicamos todos os valores parciais em um unico produto.

*/

// Diogo Felipe Soares da Silva, RA:124771

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int fatorial, gap;

void *funcaoCalculaFatorial(void *ptr);

struct tipoPack {
   char mensagem[16];
   int n, start, end;
   double fat;
};

typedef struct tipoPack tipoPack;

void main(void)
{
    pthread_t thread[50];
    int  i, nthreads;

    tipoPack pack[50];

    printf("\nEntre com o numero de threads => ");
    scanf("%d", &nthreads);

    do {
        printf("Entre com o fatorial que deseja calcular (deve ser maior que o numero de threads criadas) => ");
        scanf("%d", &fatorial);
    } while (fatorial < nthreads);

    gap = fatorial / nthreads;

    for (i = 0; i < nthreads; i++) {
        // salvo os indices de início e fim de cada bloco que a thread vai executar
        pack[i].start = fatorial - i * gap;
        pack[i].end = (i == nthreads - 1) ? 1 : pack[i].start - gap + 1;
        pack[i].n = pack[i].start;
        pack[i].fat = 1.0;
        sprintf(pack[i].mensagem,"\nThread %d", i);
    }

    for (i = 0; i < nthreads; i++)
        pthread_create(&(thread[i]), NULL, funcaoCalculaFatorial, (void*) &pack[i]);

    for (i = 0; i < nthreads; i++)
        pthread_join(thread[i], NULL);

    for (i = 0; i < nthreads; i++)
        printf("\nThread %d calculou: %.1lf\n",i, pack[i].fat);
    
    double resultado_final = 1;
    for (i = 0; i < nthreads; i++)
        resultado_final *= pack[i].fat;
    printf("\nFatorial de %d eh: %.2lf\n\n", fatorial, resultado_final);

    exit(0);
}

void *funcaoCalculaFatorial(void *ptr)
{    
    tipoPack *pack = (tipoPack *) ptr;

    printf("%s - executando\n", pack->mensagem);

    for (int i = pack->start; i >= pack->end; i--)
        (pack->fat) *= i;

}




