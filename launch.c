#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ipc_utils.h"
#include "passenger.h"

#define MAX_BOATS 10
#define MAX_PASSENGERS 100

int num_passengers;
int num_boats;
int seats_per_boat;
int passangers_saved;
pthread_mutex_t* savedLck;

void *boat_function(int id) {
    // Crear un nuevo barco
    Boat* boat = create_boat(seats_per_boat, id);
    add_boat_to_fleet(fleet, boat);

    while (passangers_saved < num_passengers) {
        // Si el barco está lleno
        if (boat->capacity == 0) {
            boat->available = 0;
            printf("Boat %d is full, heading to the island.\n", boat->id);

            // Usar el lock para actualizar la variable compartida `passengers_saved`
            pthread_mutex_lock(savedLck);
            passangers_saved += seats_per_boat;
            pthread_mutex_unlock(savedLck);

            // Aquí el barco regresa de la isla listo para recoger más pasajeros
            // Ajustar el semáforo al valor máximo (seats_per_boat)
            int current_value; // Variable para almacenar el valor actual del semáforo
            sem_getvalue(&boat->semaphore, &current_value); // Obtener el valor actual
            int value = current_value; // Usar el valor directamente

            // Incrementar el semáforo hasta que alcance `seats_per_boat`
            while (value < seats_per_boat) {
                sem_post(&boat->semaphore);
                value++;
            }

            // Reiniciar la capacidad del barco para el próximo viaje
            boat->capacity = seats_per_boat;
            printf("Boat %d has returned and is ready to board more passengers.\n", boat->id);
            boat->available = 1;
        }
    }

    // Cuando todos los pasajeros han sido salvados
    printf("Boat %d has helped save all passengers and is retiring.\n", boat->id);
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

    // I print the values of num_passengers, num_boats and seats_per_boat
    printf("Number of passengers: %d\n", num_passengers);
    printf("Number of boats: %d\n", num_boats);
    printf("Number of seats per boat: %d\n", seats_per_boat);
    printf("Passengers saved: %d\n", passangers_saved);

   pthread_t boat_threads[num_boats];
   pthread_t passenger_threads[num_passengers];


    printf("Launching %d boats with %d seats each to save %d passengers.\n", num_boats, seats_per_boat, num_passengers);
    // Create boat threads
    for (int i = 0; i < num_boats; i++) {
        if (pthread_create(&boat_threads[i], NULL, boat_function, i) != 0) {
            perror("Failed to create boat thread");
            return EXIT_FAILURE;
        }
    }

    printf("Launching %d passengers.\n", num_passengers);
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