/*

Desafio: Faça a busca de um elemento em um vetor. O tamanho do vetor deve ser
fornecido pelo usuario e o vetor deve ser gerado aleatoriamente. Mostre o vetor
gerado e solicite um elemento a ser buscado. O usuario devera informar o numero
de threads que ira efetuar a busca em paralelo. Cada thread devera trabalhar em
uma parte diferente do vetor. No final, o programa devera mostrar em quais posicoes
do vetor o elemento foi encontrado.

*/

// Diogo Felipe Soares da Silva, RA:124771
// Abordagem por divisão do array para cada thread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define MAX_THREADS 10
#define MAX_TAM_ARRAY 1000

int num_threads, size_array, target_element;
int array[MAX_TAM_ARRAY];

typedef struct {
    int thread_id;
    int start_index;
    int end_index;
    int amount_found;
    int found_indexes[MAX_TAM_ARRAY];
} ThreadData;

void print_array(int *array, int n);
void *search_array_element(void *ptr);

int main() {
    pthread_t threads[MAX_THREADS];
    ThreadData threads_data[MAX_THREADS];
    int final_result_array[MAX_TAM_ARRAY];
    
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

    int base = size_array / num_threads;
    int remainder = size_array % num_threads;

    for (int i = 0; i < num_threads; i++) {
        threads_data[i].thread_id = i + 1;
        threads_data[i].amount_found = 0;
        threads_data[i].start_index = i * base + (i < remainder ? i : remainder);
        threads_data[i].end_index = threads_data[i].start_index + base - 1 + (i < remainder ? 1 : 0);
        pthread_create(&threads[i], NULL, search_array_element, (void *) &threads_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int counter = 0;
    for (int i = 0; i < num_threads; i++) {
        for (int j = 0; j < threads_data[i].amount_found; j++) {
            final_result_array[counter] = threads_data[i].found_indexes[j];
            counter++;
        }
    }

    // Mostrando o resultado na tela
    printf("\n\nAs posicoes dos valores encontrados sao: \n");
    print_array(final_result_array, counter);

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
    ThreadData* thread_data = (ThreadData *) ptr;
    
    int array_index = thread_data->start_index;

    while (array_index <= thread_data->end_index) {
        if(array[array_index] ==  target_element) {
            thread_data->found_indexes[thread_data->amount_found] = array_index + 1;
            thread_data->amount_found++;
        }
        array_index++;
    }
    
}

