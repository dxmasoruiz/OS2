#ifndef IPC_UTILS_H
#define IPC_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


// Boat structure   
typedef struct {
    sem_t semaphore;
    int capacity;
    int available;
    int id;
    int in_fleet;
} Boat;


// Fleet structure
typedef struct {
    Boat** boats;
    int num_boats;
    pthread_mutex_t* mutex;
} Fleet;

extern Fleet* fleet;


// Boat structure

typedef struct {
    int id;
    int wants_to_board;
} Passenger;




// Function to create and initialize a mutex
pthread_mutex_t* create_mutex();

// Function to create and initialize a boat
Boat* create_boat(int seats_per_boat);

// Function to create and initialize a fleet
Fleet* create_fleet(int num_boats, int seats_per_boat);


void add_boat_to_fleet(Fleet* fleet, Boat* boat) ;


#endif // IPC_UTILS_H