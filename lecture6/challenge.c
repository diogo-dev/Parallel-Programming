/*

Desafio: Faça a busca de um elemento em um vetor. O tamanho do vetor deve ser
fornecido pelo usuario e o vetor deve ser gerado aleatoriamente. Mostre o vetor
gerado e solicite um elemento a ser buscado. O usuario devera informar o numero
de threads que ira efetuar a busca em paralelo. Cada thread devera trabalhar em
uma parte diferente do vetor. No final, o programa devera mostrar em quais posicoes
do vetor o elemento foi encontrado.

*/

// Diogo Felipe Soares da Silva, RA:124771
// Abordagem por interpolação das threads

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define MAX_THREADS 10
#define MAX_TAM_ARRAY 100

int num_threads, size_array, target_element, amount_of_elements_found;
int array[MAX_TAM_ARRAY];
int target_indexes[MAX_TAM_ARRAY];

void print_array(int *array, int n);
void *search_array_element(void *ptr);

int main() {
    pthread_t threads[MAX_THREADS];
    int threads_ids[MAX_THREADS];
    amount_of_elements_found = 0;
    
    do {
        printf("Entre com o tamanho do vetor: ");
        scanf("%d", &size_array);
    } while(size_array < 1 || size_array > MAX_TAM_ARRAY);

    // Populando o array com numeros aleatorios
    srand(time(NULL)); 
    for (int i  = 0; i < size_array; i++) {
        array[i] = (rand() % 100) + 1;
    }

    print_array(array, size_array);

    printf("\nEntre com o elemento a ser buscado (dentre os elementos do vetor): ");
    scanf("%d", &target_element);

    do {
        printf("\nEntre com o numeros de threads desejada: ");
        scanf("%d", &num_threads);
    } while (num_threads < 1 || num_threads > MAX_THREADS);


    for (int i = 0; i < num_threads; i++) {
        threads_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, search_array_element, (void *) &threads_ids[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Mostrando o resultado na tela
    printf("\n\nAs posicoes dos valores encontrados sao: \n");
    print_array(target_indexes, amount_of_elements_found);


    return 0;
}

void print_array(int *array, int n) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%d, ", array[i]);

    }
    printf("\n");
}

void *search_array_element(void *ptr) {
    int *thread_id, array_index;
    
    thread_id = (int *) ptr;

    array_index  = *thread_id - 1;

    printf("\nThread %d iniciada!\n", *thread_id);

    /*Uma melhoria nesse código seria usar um mutex para preencher o vetor target_indexes,
    visto que do que jeito que foi implementado até o momento pode ser que ocorra race conditions*/

    while (array_index < size_array) {
        if (array[array_index] == target_element) {
            target_indexes[amount_of_elements_found] = (array_index + 1);
            amount_of_elements_found++;
        }
        array_index += num_threads;
    }
    
}

