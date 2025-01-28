#ifndef PASSENGER_H
#define PASSENGER_H

#include <pthread.h>
#include "ipc_utils.h"

#define MAX_PASSENGERS 100


// Function to create a passenger
Passenger* create_passenger();
// Function to select a random boat
int random_boat_selector(int seed, int end);

// Function to search for an available boat
void search_available_boat(Fleet* fleet);

// Function for passenger process
void *passenger_process(void *arg);
#endif // PASSENGER_H