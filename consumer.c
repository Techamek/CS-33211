#include "shared.h"

int main() {
    // Open shared memory with read and write permissions
    int fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);

    // Set the size of the shared memory object
    ftruncate(fd, SIZE);

    // Map the shared memory object into the address space
    struct table *share = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    
    // Check if the memory mapping failed
    if(share == MAP_FAILED){
        perror("Error: Failed to map shared memory");
        return EXIT_FAILURE;
    }
    
    // Open the semaphores with appropriate permissions and initial values
    sem_t *full = sem_open(semFullName, O_CREAT, 0666, 0); // Semaphore for full items
    sem_t *empty = sem_open(semEmptyName, O_CREAT, 0666, bufferSize); // Semaphore for empty slots
    sem_t  *mutex = sem_open(semMutexName, O_CREAT, 0666, 1); // Mutex semaphore to ensure mutual exclusion
    
    // Initialize random number generator
    time_t t;
    srand((unsigned) time(&t));

    // Set the number of iterations (consumptions)
    int loop = 10;

    // Print a message indicating that the consumer process has started
    printf("\n[Consumer] Process has started\n");

    // Consumer loop to consume items from the shared buffer
    while(loop--) {
        // Wait for an item to be available in the buffer
        sem_wait(full);

        // Simulate some processing delay with sleep
        sleep(rand() % 3);

        // Wait for the mutex to ensure no other process is modifying the buffer
        sem_wait(mutex);

        // Simulate some processing delay
        sleep(rand() % 3);

        // Print the consumed item and its position
        printf("item consumed: %d in pos: %d\n", share->buffer[share->out], share->out);

        // Update the position for the next consumption
        share->out = (share->out + 1) % bufferSize; 

        // Release the mutex after modifying the buffer
        sem_post(mutex);

        // Signal that an empty slot is now available
        sem_post(empty);
    }

    // Print a message indicating that the consumer process has completed
    printf("[Consumer] Process completed\n");

    // Close and unlink the semaphores
    sem_close(full);
    sem_close(empty);
    sem_close(mutex);
    sem_unlink(semFullName);
    sem_unlink(semEmptyName);
    sem_unlink(semMutexName);

    // Unmap the shared memory and close the file descriptor
    munmap(share, SIZE);
    close(fd);

    // Unlink the shared memory object
    shm_unlink(NAME);

    return EXIT_SUCCESS;
}
