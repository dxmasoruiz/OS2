#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include "ipc_utils.h"

#define MAX_PASSENGERS 100
pthread_mutex_t* boardMutex = create_Mutex();
pthread_mutex_t* capacityMutex = create_Mutex();




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

void *passenger_process(void *arg) {
    Passenger* passenger = create_passenger();
    search_available_boat(fleet);
    free(passenger);
    return NULL;   
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
                pthread_mutex_lock(&capacityMutex);
                selectedBoat->capacity--;
                pthread_mutex_unlock(&capacityMutex);   
                printf("Passenger is boarding boat %d. Remaining capacity: %d\n", boat->id, boat->capacity);
            }else{
            printf("Boat is occupied returning");
            selectedBoat = NULL;
            }
         
        }else {
            printf("Passenger %d decided not to board boat %d\n", pthread_self(), boat->id);
        }
        
        
      

    if (selected_boat == NULL) {
        printf("No available boats. Waiting...\n");
        sleep(1);
        search_available_boat(fleet);
    }
}

int main() {
    return 0;
}