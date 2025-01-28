#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ipc_utils.h"

#define MAX_BOATS 10
#define MAX_PASSENGERS 100

int num_passengers;
int num_boats;
int seats_per_boat;
int passangers_saved;
pthread_mutex_t *savedLck;

void *boat_function() {
    create_boat(seats_per_boat);
    add_boat_to_fleet(fleet, boat);
    int* current_value; 

    while (passangers_saved < num_passengers && boat->in_fleet) {
        
        if (boat->capacity == 0) {
            boat->available = 0;
            printf("Boat is full, heading to the island.\n");
            //Here i use the lock to the shared variable passengers saved it also works as simultaing the way to the island 
            pthread_mutex_lock(savedLck);
            passangers_saved += seats_per_boat;
            pthread_mutex_unlock(savedLck);
            //Here the boat comes from th eisland ready to pick up more passengers
            // I adjust th esemafore to thr max value and althoug it should b 0 i in case check the current value for errors
            sem_getvalue(&boat->semaphore, &current_value);  
            while (current_value < seats_per_boat) {
            sem_post(&boat->semaphore);
            current_value++;
            } 
            boat->capacity = seats_per_boat; // Reset capacity for the next trip
            printf("Boat %d has returned and is ready to board more passengers.\n",boat->id);
            boat->available = 1;
        }
    }
    printf("Boat %d has helped saving  all passengers and is retiring.\n");
    free(boat);
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <num_passengers> <num_boats> <seats_per_boat>\n", argv[0]);
        return EXIT_FAILURE;
    }

    num_passengers = atoi(argv[1]);
    num_boats = atoi(argv[2]);
    seats_per_boat = atoi(argv[3]);
    passangers_saved=0;
    fleet = create_fleet(num_boats, seats_per_boat);
    pthread_mutex_t* savedLck = create_mutex();

    if (num_passengers > MAX_PASSENGERS || num_boats > MAX_BOATS || seats_per_boat <= 0) {
        fprintf(stderr, "Invalid input parameters too many boats or passengers.\n");
        return EXIT_FAILURE;

    }

   pthread_t boat_threads[num_boats];
   pthread_t passenger_threads[num_passengers];

    // Create boat threads
    for (int i = 0; i < num_boats; i++) {
        if (pthread_create(&boat_threads[i], NULL, boat_function, NULL) != 0) {
            perror("Failed to create boat thread");
            return EXIT_FAILURE;
        }
    }

    // Create passenger threads
    for (int i = 0; i < num_passengers; i++) {
        if (pthread_create(&passenger_threads[i], NULL, passenger_process, NULL) != 0) {
            perror("Failed to create passenger thread");
            return EXIT_FAILURE;
        }
    }

    // Wait for all passenger threads to finish
    for (int i = 0; i < num_passengers; i++) {
        pthread_join(passenger_threads[i], NULL);
    }

    // Wait for all boat threads to finish
    for (int i = 0; i < num_boats; i++) {
        pthread_join(boat_threads[i], NULL);
    }

    free(fleet);
    return EXIT_SUCCESS;
}