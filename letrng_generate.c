#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define N 10000000  // Loop count

volatile int coin = 0;
volatile bool active = true;

volatile unsigned long long x64 = 0;
volatile unsigned long long y64 = 0;

void* thread1_func(void* arg) {
    for (int n = 0; n < N; n++) {
        coin = n % 2;
    }
    active = false;  // Signal samplers to stop
    return NULL;
}

void* thread2_func(void* arg) {
    for (int n = N; n > 0; n--) {
        coin = n % 2;
    }
    active = false;  // Signal samplers to stop
    return NULL;
}

void* sampling_thread_x(void* arg) {
    while (active) {
        int loc = coin;
        x64 <<= 1;
        x64 |= loc;
    }
    return NULL;
}

void* sampling_thread_y(void* arg) {
    while (active) {
        int loc = coin;
        y64 <<= 1;
        y64 |= loc;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, tx, ty;

    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);
    pthread_create(&tx, NULL, sampling_thread_x, NULL);
    pthread_create(&ty, NULL, sampling_thread_y, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(tx, NULL);
    pthread_join(ty, NULL);

    FILE *f = fopen("letrng_simple2.bin", "ab");
    fwrite(&x64, sizeof x64, 1, f);
    fwrite(&y64, sizeof y64, 1, f);
    fclose(f);

    return 0;
}

