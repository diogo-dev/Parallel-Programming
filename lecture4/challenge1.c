/*

Desafio: Ao inves de criar 2 threads, crie uma quantidade de threads definida
pelo usuario (k), ateh um maximo permitido. Todas as k threads devem resolver
o mesmo problema, trabalhando juntas/cooperadas. Ajuste o codigo. Compile,
execute e analise.

*/

// Diogo Felipe Soares da Silva, RA: 124771

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define maxElem 50
#define MAX_TAM 10

void *fibonacci(void *ptr);

int fib[maxElem], n, num_threads;

int main()
{
    pthread_t threads[MAX_TAM];
    int threads_ids[MAX_TAM];

    // Entrada do usuário
    do {
        printf("\nDigite n > 1 (quantos termos da sequência calcular): ");
        scanf("%d", &n);
    } while (n < 2 || n > maxElem);

    do {
        printf("Digite o número de threads (1 a %d): ", MAX_TAM);
        scanf("%d", &num_threads);
    } while (num_threads < 1 || num_threads > MAX_TAM);

    // Inicializar o vetor com zeros
    for (int i = 0; i < n; i++)
        fib[i]=0;

    fib[0]=1;
    fib[1]=1;

    for (int i = 0; i <num_threads; i++) {
        threads_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, fibonacci, (void *) &threads_ids[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nSequencia de Fibonacci: \n");
    for (int i = 0; i < n; i++)
        printf("%d, ", fib[i]);
    
    printf("\n\n%d-esimo elemento de Fibonacci: %d\n",n, fib[n-1]);

    return 0;
}

void *fibonacci(void *ptr)
{    
    int *id, pos;

    id = (int *) ptr;

    printf("\nThread %d iniciada!\n", *id);

    pos = (*id) - 1;

    while (pos < n)
    {
        // Esperar até que os dois anteriores estejam calculados
        if (pos >= 2)
        {
            // Aguarda até que os dois valores anteriores estejam prontos
            while (fib[pos - 1] == 0 || fib[pos - 2] == 0);
            fib[pos] = fib[pos - 1] + fib[pos - 2];
        }
        else if (pos == 0 || pos == 1)
        {
            fib[pos] = 1;
        }

        pos += num_threads;
    }

}



