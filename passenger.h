ifndef PASSENGER_H
#define PASSENGER_H

#include <pthread.h>
#include "ipc_utils.h"

#define MAX_PASSENGERS 100

typedef struct {
    int id;
    int wants_to_board;
} Passenger;

// Function to create a passenger
Passenger* create_passenger();

// Function for passenger process
void *passenger_process(void *arg);

// Function to select a random boat
int random_boat_selector(int seed, int end);

// Function to search for an available boat
void search_available_boat(Fleet* fleet);

#endif // PASSENGER_H