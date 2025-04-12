#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int n;
    int result;
} tipoPack;

void *calcula_fib(void *arg) {
    tipoPack *data = (tipoPack *)arg;
    int a = 0, b = 1, temp;

    if (data->n == 0) {
        data->result = 0;
        return;
    }

    // Logic to calculate fibonacci term iteratively
    for (int i = 2; i <= data->n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }

    data->result = b;
    return;
}

int main() {
    int n;
    printf("Digite o valor de n: ");
    scanf("%d", &n);

    if (n < 2) {
        printf("O termo %d da sequência de Fibonacci é %d\n", n, n);
        return 0;
    }

    // Declaring the threads
    pthread_t t1, t2;

    // Initializing the end index at both packs
    tipoPack pack1 = {.n = n - 1};
    tipoPack pack2 = {.n = n - 2};

    // Creating the threads
    pthread_create(&t1, NULL, calcula_fib, &pack1);
    pthread_create(&t2, NULL, calcula_fib, &pack2);

    // Waiting for them all finish execution
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Printing the result of both threads
    printf("Resultado do calculo da thread 1: %d\n", pack1.result);
    printf("Resultado do calculo da thread 2: %d\n", pack2.result);

    int result = pack1.result + pack2.result;

    // Now, printing the final result
    printf("O %dº termo da sequência de Fibonacci é: %d\n", n, result);

    return 0;
}
