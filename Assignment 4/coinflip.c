//Copyright Andrei Bancila 06.10.2023
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_COINS 20
#define NUM_PERSONS 100
#define NUM_FLIPS 10000

char coins[NUM_COINS];  // Global array to represent coins
pthread_mutex_t global_lock;  // Global lock for the first strategy

pthread_mutex_t coin_locks[NUM_COINS];  // Lock for each coin in the third strategy

// Function to flip a single coin
void flip_coin(char* coin) {
    if (*coin == 'X') {
        *coin = '0';
    } else {
        *coin = 'X';
    }
}

// Strategy 1: Global lock for all coins
void* strategy1(void* arg) {
    for (int i = 0; i < NUM_FLIPS; i++) {
        pthread_mutex_lock(&global_lock);
        for (int j = 0; j < NUM_COINS; j++) {
            flip_coin(&coins[j]);
        }
        pthread_mutex_unlock(&global_lock);
    }
    return NULL;
}

// Strategy 2: Global lock for each iteration
void* strategy2(void* arg) {
    for (int i = 0; i < NUM_FLIPS; i++) {
        for (int j = 0; j < NUM_COINS; j++) {
            pthread_mutex_lock(&global_lock);
            flip_coin(&coins[j]);
            pthread_mutex_unlock(&global_lock);
        }
    }
    return NULL;
}

// Strategy 3: Separate lock for each coin
void* strategy3(void* arg) {
    for (int i = 0; i < NUM_FLIPS; i++) {
        for (int j = 0; j < NUM_COINS; j++) {
            pthread_mutex_lock(&coin_locks[j]);
            flip_coin(&coins[j]);
            pthread_mutex_unlock(&coin_locks[j]);
        }
    }
    return NULL;
}

// Function to measure execution time
static double timeit(int n, void* (*proc)(void *)) {
    clock_t t1, t2;
    t1 = clock();
    pthread_t threads[n];
    
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, proc, NULL);
    }
    
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    t2 = clock();
    return ((double)t2 - (double)t1) / CLOCKS_PER_SEC * 1000;
}

int main(int argc, char* argv[]) {
    int num_persons = NUM_PERSONS;
    int num_flips = NUM_FLIPS;
    
    if (argc >= 2) {
        num_persons = atoi(argv[1]);
    }
    if (argc >= 3) {
        num_flips = atoi(argv[2]);
    }
    
    pthread_mutex_init(&global_lock, NULL);
    
    for (int i = 0; i < NUM_COINS; i++) {
        pthread_mutex_init(&coin_locks[i], NULL);
        coins[i] = (i >= NUM_COINS / 2) ? 'X' : '0';
    }
    coins[NUM_COINS] = '\0';

    printf("coins: %s (start - global lock)\n", coins);
    double time1 = timeit(num_persons, strategy1);
    printf("coins: %s (end - global lock)\n", coins);
    printf("%d threads x %d flips: %.3lf ms\n", num_persons, num_flips, time1);

    printf("coins: %s (start - iteration lock)\n", coins);
    double time2 = timeit(num_persons, strategy2);
    printf("coins: %s (end - iteration lock)\n", coins);
    printf("%d threads x %d flips: %.3lf ms\n", num_persons, num_flips, time2);

    printf("coins: %s (start - coin lock)\n", coins);
    double time3 = timeit(num_persons, strategy3);
    printf("coins: %s (end - coin lock)\n", coins);
    printf("%d threads x %d flips: %.3lf ms\n", num_persons, num_flips, time3);

    pthread_mutex_destroy(&global_lock);
    for (int i = 0; i < NUM_COINS; i++) {
        pthread_mutex_destroy(&coin_locks[i]);
    }

    return 0;
}
