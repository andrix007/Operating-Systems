#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int travelers_waiting = 0;
int drivers_waiting = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t driverCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t travelerCond = PTHREAD_COND_INITIALIZER;

void* traveler_function(void* id) {
    int tid = *((int*)id);
    while(1) {
        pthread_mutex_lock(&mutex);
        printf("taxi: [%d travelers, %d drivers] traveler %d entering\n", travelers_waiting, drivers_waiting, tid);
        if (drivers_waiting > 0) {
            drivers_waiting--;
            printf("taxi: [%d travelers, %d drivers] traveler %d picked by driver\n", travelers_waiting, drivers_waiting, tid);
            pthread_cond_signal(&driverCond);
        } else {
            travelers_waiting++;
            printf("taxi: [%d travelers, %d drivers] traveler %d waiting...\n", travelers_waiting, drivers_waiting, tid);
            pthread_cond_wait(&travelerCond, &mutex);
            printf("taxi: [%d travelers, %d drivers] ...traveler %d waking up\n", travelers_waiting, drivers_waiting, tid);
        }
        printf("taxi: [%d travelers, %d drivers] traveler %d leaving\n", travelers_waiting, drivers_waiting, tid);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3 + 1); // sleep for a random time
    }
    return NULL;
}

void* driver_function(void* id) {
    int did = *((int*)id);
    while(1) {
        pthread_mutex_lock(&mutex);
        printf("taxi: [%d travelers, %d drivers] driver %d entering\n", travelers_waiting, drivers_waiting, did);
        if (travelers_waiting > 0) {
            travelers_waiting--;
            printf("taxi: [%d travelers, %d drivers] driver %d picking traveler\n", travelers_waiting, drivers_waiting, did);
            pthread_cond_signal(&travelerCond);
        } else {
            drivers_waiting++;
            printf("taxi: [%d travelers, %d drivers] driver %d waiting...\n", travelers_waiting, drivers_waiting, did);
            pthread_cond_wait(&driverCond, &mutex);
            printf("taxi: [%d travelers, %d drivers] ...driver %d waking up\n", travelers_waiting, drivers_waiting, did);
        }
        printf("taxi: [%d travelers, %d drivers] driver %d leaving with traveler\n", travelers_waiting, drivers_waiting, did);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3 + 1); // sleep for a random time
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    int num_travelers = 1;
    int num_drivers = 1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            num_travelers = atoi(argv[i+1]);
            i++;
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            num_drivers = atoi(argv[i+1]);
            i++;
        }
    }

    pthread_t travelers[num_travelers];
    pthread_t drivers[num_drivers];
    int* traveler_ids[num_travelers];
    int* driver_ids[num_drivers];

    for (int i = 0; i < num_travelers; i++) {
        traveler_ids[i] = (int*) malloc(sizeof(int));
        *traveler_ids[i] = i;
        pthread_create(&travelers[i], NULL, traveler_function, traveler_ids[i]);
    }

    for (int i = 0; i < num_drivers; i++) {
        driver_ids[i] = (int*) malloc(sizeof(int));
        *driver_ids[i] = i;
        pthread_create(&drivers[ i], NULL, driver_function, driver_ids[i]);
    }

    for (int i = 0; i < num_travelers; i++) {
        pthread_join(travelers[i], NULL);
        free(traveler_ids[i]);  // Free the dynamically allocated memory for IDs
    }

    for (int i = 0; i < num_drivers; i++) {
        pthread_join(drivers[i], NULL);
        free(driver_ids[i]);  // Free the dynamically allocated memory for IDs
    }

    return 0;
}
