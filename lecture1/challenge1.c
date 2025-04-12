#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Diogo Felipe Soares da Silva RA:124771

void * printMessage(void *ptr) {
    char *message = (char *) ptr;
    printf("%s", message);
}

int main() {

    // creating the threads
    pthread_t thread1, thread2, thread3;
    // creating the threads arguments (in this case the messages they are going to print)
    char *message1 = "\nI'm thread number 1";
    char *message2 = "\nI'm thread number 2";
    char *message3 = "\nI'm thread number 3";
    // this is going to be the pthread_create return values
    int r1,r2,r3;

    r1 = pthread_create(&thread1, NULL, printMessage, (void *) message1);
    r2 = pthread_create(&thread2, NULL, printMessage, (void *) message2);
    r3 = pthread_create(&thread3, NULL, printMessage, (void *) message3);

    printf("\nCreation of thread1 retuned: %d", r1);
    printf("\nCreation of thread2 retuned: %d", r2);
    printf("\nCreation of thread3 retuned: %d", r3);

    // waiting for all of the threads to finish execution
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("\nThreads 1,2,3 has already finish and now the main thread has also finish its execution!\n");
    return 0;
}