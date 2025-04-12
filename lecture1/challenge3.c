#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define TAM_ARRAY 3000

// Diogo Felipe Soares da Silva RA:124771

// Declaring a global array
int array[TAM_ARRAY];

void *fillArrayWithValues(void * ptr) {
    char *message = (char *)ptr;
    // using the function atoi(char *) to convert a string to its respective integer number
    int integerValue = atoi(message);

    // this for loop will fill a portion of the array according to the integer value that is passed in
    for(int i = (integerValue - 1)* 1000; i < integerValue * 1000; i++){
        array[i] = integerValue;
    }
}

void printArray(int *array, int n){
    printf("[");
    for(int i = 0; i < n; i++) {
        printf("%d, ", array[i]);
    }
    printf("]\n");
}

int main() {

    // creating the threads
    pthread_t thread1, thread2, thread3;
    // creating the threads arguments (in this case the numbers that the thread are going to fill the array with)
    char *message1 = "1";
    char *message2 = "2";
    char *message3 = "3";
    // this is going to be the pthread_create return values
    int r1,r2,r3;

    r1 = pthread_create(&thread1, NULL, fillArrayWithValues, (void *) message1);
    r2 = pthread_create(&thread2, NULL, fillArrayWithValues, (void *) message2);
    r3 = pthread_create(&thread3, NULL, fillArrayWithValues, (void *) message3);

    // Checking if the return values are right (0)
    printf("\nCreation of thread1 retuned: %d", r1);
    printf("\nCreation of thread2 retuned: %d", r2);
    printf("\nCreation of thread3 retuned: %d", r3);

    // waiting for all of the threads to finish execution
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("\n");
    printArray(array, TAM_ARRAY);

    printf("\nThreads 1,2,3 has already finish and now the main thread has also finish its execution!\n");
    return 0;
}