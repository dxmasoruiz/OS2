#include "ipc_utils.h"

// Define the fleet variable
Fleet* fleet = NULL;

pthread_mutex_t* create_mutex() {
    pthread_mutex_t *mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (mutex == NULL) {
        perror("Failed to allocate memory for mutex");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(mutex, NULL) != 0) {
        perror("Failed to initialize mutex");
        free(mutex);
        exit(EXIT_FAILURE);
    }

    return mutex;
}

Boat* create_boat(int seats_per_boat) {
    Boat *boat = (Boat *)malloc(sizeof(Boat));
    if (boat == NULL) {
        perror("Failed to allocate memory for boat");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&boat->semaphore, 0, seats_per_boat) != 0) {
        perror("Failed to initialize semaphore");
        free(boat);
        exit(EXIT_FAILURE);
    }

    boat->capacity = seats_per_boat;
    boat->available = 0;
    boat->id = (int) pthread_self();

    return boat;
}

Fleet* create_fleet(int num_boats, int seats_per_boat) {
    Fleet *fleet = (Fleet *)malloc(sizeof(Fleet));
    if (fleet == NULL) {
        perror("Failed to allocate memory for fleet");
        exit(EXIT_FAILURE);
    }

    fleet->boats = (Boat **)malloc(num_boats * sizeof(Boat *));
    if (fleet->boats == NULL) {
        perror("Failed to allocate memory for boats array");
        free(fleet);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_boats; i++) {
        fleet->boats[i] = create_boat(seats_per_boat);
    }

    fleet->num_boats = num_boats;
    fleet->mutex = create_mutex();

    return fleet;
}

void add_boat_to_fleet(Fleet* fleet, Boat* boat) {
    pthread_mutex_lock(fleet->mutex);
    for (int i = 0; i < fleet->num_boats; i++) {
        if (fleet->boats[i] == NULL) {
            fleet->boats[i] = boat;
            boat->in_fleet = 1;
            boat->available = 1;
            break;
        }
    }
    pthread_mutex_unlock(fleet->mutex);
}
