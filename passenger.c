#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include "ipc_utils.h"

#define MAX_PASSENGERS 100


Passenger* create_passenger() {
    Passenger *passenger = (Passenger *)malloc(sizeof(Passenger));
    if (passenger == NULL) {
        perror("Failed to allocate memory for passenger");
        exit(EXIT_FAILURE);
    }
    passenger->id = pthread_self();
    passenger->wants_to_board = 1;
    return passenger;
}


int random_boat_selector(int seed, int end) {
    srand(seed);
    return rand() % end;
}

void search_available_boat(Fleet* fleet) {
   
    int randomBoat =random_boat_selector(pthread_self(), fleet->num_boats);
    Boat* selectedBoat = fleet->boats[randomBoat];
    if (selectedBoat == NULL)
    {
        printf("Boat not ready yet ");
        search_available_boat(fleet);
        
    }
    
        //Here with a random possibility of 90 yes and 10 no the passengers decdes to continue to fleet or not
        int decision= random_boat_selector(pthread_self(), 10); 
        if (decision<=9) {
            
            sem_wait(&selectedBoat->semaphore);
            if (selectedBoat->available==1)
            {
                pthread_mutex_lock(capacityMutex);
                selectedBoat->capacity--;
                pthread_mutex_unlock(capacityMutex);   
                printf("Passenger is boarding boat %d. Remaining capacity: %d\n", selectedBoat->id, selectedBoat->capacity);
            }else{
            printf("Boat is occupied returning");
            selectedBoat = NULL;
            }
         
        }else {
            printf("Passenger decided not to board boat %d\n", selectedBoat->id);
        }
        
        
      

    if (selectedBoat == NULL) {
        printf("No available boats. Waiting...\n");
        sleep(1);
        search_available_boat(fleet);
    }
}

void *passenger_process() {
    Passenger* passenger = create_passenger(); 
    if (capacityMutex == NULL) {
        capacityMutex = create_mutex();
    }
    search_available_boat(fleet);
    free(passenger);
    return NULL;   
}
